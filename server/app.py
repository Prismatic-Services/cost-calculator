from flask import Flask, render_template, send_file

"""
TODO's
- [ ] Add a new endpoint to serve the downloaded file
- [ ] Add an authorization system with methods to differentiate users contractor affiliations
- [ ] Add a general admin select (see above)
- [ ] Display of current files client has
- [ ] Contractor data input fields
- [ ] Database system/management? (local or hosted?)
- [ ] Error toast message system
- [ ] Error detection prior to execution
- [ ] Python execution of C++ executable...?
"""

app = Flask(__name__)

@app.route('/')
def index():
    return "Hello World"


@app.route('/download')
def download():
    path = "../output.csv"
    return send_file(path, as_attachment=True)

app.run(port=8080, host='0.0.0.0', debug=True)