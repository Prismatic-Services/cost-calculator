import json
import os

from flask import Flask, request, jsonify, send_file, send_from_directory
from controllers.auth import Auth
from controllers.utils import map_to_csv
from flask_cors import CORS
import subprocess  # is this secure???

app = Flask(__name__)
CORS(app, origins="http://192.168.1.71:3000", supports_credentials=True)


@app.route('/api/v1/ping', methods=["POST"])
def ping():
    return 'pong'


@app.route('/api/v1/verify_token', methods=["POST"])
def verify_token():
    auth = Auth()
    client_uad = request.headers.get('Authorization')
    client_email = request.headers.get('X-Client-Email')

    if auth.verify_uad_and_timestamp(client_email, client_uad):
        return jsonify(True)
    return jsonify(False)


@app.route('/api/v1/login', methods=["POST"])
def login():
    auth: Auth = Auth()

    req = request.get_json()

    usr = req.get('username')
    psw = req.get('password')

    try:
        uad = auth.login(usr, psw)

        return jsonify({
            "uad": uad
        })
    except TypeError:
        return jsonify({
            False
        })

@app.route('/api/v1/recalculate', methods=["POST"])  # TODO: Better api error responses
def recalculate():
    auth: Auth = Auth()

    client_uad = request.headers.get('Authorization')
    client_email = request.headers.get('X-Client-Email')

    if auth.verify_uad_and_timestamp(client_email, client_uad):
        contractor = auth.database.pull_user_by_email(client_email)[3]

        file_path = f"./calculated/{contractor}output.csv"
        if os.path.exists(file_path):
            os.remove(file_path)
            print(f"File '{file_path}' deleted successfully.")

        result = subprocess.run(["./contractor-calculator-instance.exe", contractor], capture_output=True, text=True)
        print(result.stdout)
        return str(result.stdout)

    return jsonify(False)


@app.route('/api/v1/get_file', methods=["POST"])
def get_file():
    auth: Auth = Auth()

    valid_files = ["Buses and Drivers.csv", "Daily Bus Routes.csv", "Driver Payment Scale.csv",
                   "Maintenance Payment Scale.csv", "Management Payment Scale.csv", "PVA Table.csv"]
    file = request.args.get('file')

    if file not in valid_files:
        return "Invalid file extension supplied"

    client_uad = request.headers.get('Authorization')
    client_email = request.headers.get('X-Client-Email')
    if auth.verify_uad_and_timestamp(client_email, client_uad):
        contractor = auth.database.pull_user_by_email(client_email)[3]

        try:
            path = "./storage/" + contractor + "/" + file
            with open(path, "r+") as f:
                return [f.read()]
        except FileNotFoundError:
            return "File not found: /" + contractor + "/" + file


@app.route('/api/v1/update_file', methods=["POST"])
def update_file():
    auth: Auth = Auth()

    valid_files = ["Buses and Drivers.csv", "Daily Bus Routes.csv", "Driver Payment Scale.csv",
                   "Maintenance Payment Scale.csv", "Management Payment Scale.csv", "PVA Table.csv"]
    file = request.args.get('file')

    data = request.get_json()
    if file not in valid_files:
        return "Invalid file extension supplied"

    client_uad = request.headers.get('Authorization')
    client_email = request.headers.get('X-Client-Email')

    if auth.verify_uad_and_timestamp(client_email, client_uad):
        contractor = auth.database.pull_user_by_email(client_email)[3]

        with open('./storage/' + contractor + "/" + file, 'w') as f:
            if request.args.get('type') == "csv":
                f.write(data)
            else:
                f.write(map_to_csv(data))
    return "200"


@app.route('/api/v1/get-output-file', methods=["POST"])
def get_output_file():
    auth: Auth = Auth()

    client_token = request.headers.get('Authorization')
    client_email = request.headers.get('X-Client-Email')

    print(client_token, client_email)

    if auth.verify_uad_and_timestamp(client_email, client_token):
        contractor = auth.database.pull_user_by_email(client_email)[3]

        return send_file("./calculated/" + contractor + "output.csv", as_attachment=True, download_name="output.csv")
    return jsonify(False)


if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)
