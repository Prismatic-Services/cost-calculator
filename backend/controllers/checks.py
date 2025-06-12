# this class is designed to check and evaluate all data uploaded to the /storage directory
# to minimize runtime issues and errors - it's pre-checks essentially
import os
import csv
import dateinfer


class DataEvaluations:
    def __init__(self, contractor_name, storage_directory="./storage/"):
        self.storage_directory = storage_directory
        self.contractor_name = contractor_name
        self.errors = []

    def confirm_data(self) -> bool:
        if os.path.isdir(self.storage_directory + self.contractor_name):
            files = ["Buses and Drivers.csv", "Daily Bus Routes.csv", "Driver Payment Scale.csv",
                     "Maintenance Payment Scale.csv", "Management Payment Scale.csv", "PVA Table.csv"]

            for file in files:
                if not os.path.isfile(os.path.join(self.storage_directory + self.contractor_name, file)):
                    self.errors.append("File [" + file + "] does not exist...")
                    return False
        else:
            self.errors.append("Contractor directory does not exist")
            return False

    def buses_and_drivers_eval(self):
        with open(os.path.join(self.storage_directory + self.contractor_name, "Buses and Drivers.csv")) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            for row in csv_reader:
                if len(row) != 9: self.errors.append("Missing or extra values in Buses and Drivers CSV file: " + str(row))

                if dateinfer.infer(row[2]) != "%Y-%m-%d":
                    self.errors.append("Incorrect date format in Buses and Drivers CSV file: " + str(row))
                if dateinfer.infer(row[3]) != "%Y-%m-%d":
                    self.errors.append("Incorrect date format in Buses and Drivers CSV file: " + str(row))
                if not row[4].isdigit():
                    self.errors.append("Incorrect row count format in Buses and Drivers CSV file: " + str(row))
                if "Gas" not in row[5] or "Diesel" not in row[5] or "Electric" not in row[5]:
                    self.errors.append("Incorrect fuel type in Buses and Drivers CSV file: " + str(row))

                if len(self.errors) > 0:
                    return False

    def driver_payment_scale_eval(self):
        with open(os.path.join(self.storage_directory + self.contractor_name, "Driver Payment Scale.csv")) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            for row in csv_reader:
                if len(row) != 2: self.errors.append("Missing or extra values in Driver Payment Scale CSV file: " + str(row))

                valid_values = ["1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ",", "-", ".", "+"]
                checker = str(row[0] + row[1])

                for char in checker:
                    if char not in valid_values:
                        self.errors.append("Incorrect value format in Driver Payment Scale CSV file: " + str(row))

        if len(self.errors) > 0:
            return False

    def pva_table_eval(self):  # TODO: better checks
        with open(os.path.join(self.storage_directory + self.contractor_name, "PVA Table.csv")) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            master_len = -1

            for row in csv_reader:
                if master_len == -1: master_len = len(row)

                if len(row) != master_len:
                    self.errors.append("Missing or extra values in PVA Table CSV file: " + str(row))
                    return False

    def maintenance_payment_scale_eval(self):
        with open(os.path.join(self.storage_directory + self.contractor_name, "Maintenance Payment Scale.csv")) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            for row in csv_reader:
                if len(row) != 2: self.errors.append(
                    "Missing or extra values in Maintenance Payment Scale CSV file: " + str(row))

                valid_values = ["1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ",", "-", ".", "+"]
                checker = str(row[0] + row[1])

                for char in checker:
                    if char not in valid_values:
                        self.errors.append("Incorrect value format in Maintenance Payment Scale CSV file: " + str(row))

    def management_payment_scale(self):
        with open(os.path.join(self.storage_directory + self.contractor_name,
                               "Management Payment Scale.csv")) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            for row in csv_reader:
                if len(row) != 2: self.errors.append(
                    "Missing or extra values in Management Payment Scale CSV file: " + str(row))

                valid_values = ["1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ",", "-", ".", "+", ">", "<", "r", "s"]
                checker = str(row[0] + row[1])

                for char in checker:
                    if char not in valid_values:
                        self.errors.append(
                            "Incorrect value format in Management Payment Scale CSV file: " + str(row))

        if len(self.errors) > 0:
            return False

    def check(self):  # TODO: "Daily Bus Routes.csv"
        if self.confirm_data():
            if self.buses_and_drivers_eval():
                if self.driver_payment_scale_eval():
                    if self.pva_table_eval():
                        if self.maintenance_payment_scale_eval():
                            if self.management_payment_scale():
                                return True
        return self.errors
