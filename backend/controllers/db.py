import sqlite3
from controllers.utils import generate_random_string, sha256_hash


class Database:
    def __init__(self, filename="store.db"):
        self.filename = filename

    def setup(self):
        connection = sqlite3.connect(self.filename)
        cursor = connection.cursor()

        cursor.execute("CREATE TABLE IF NOT EXISTS Users (Email TEXT PRIMARY KEY, Password TEXT, Salt TEXT, ContractorIdentifier TEXT, IsAdmin BOOL, UAD TEXT, UADTimestamp TEXT)")
        cursor.execute("CREATE TABLE IF NOT EXISTS Contractors (ContractorIdentifier TEXT PRIMARY KEY, Address TEXT, Email TEXT, Phone TEXT)")

        connection.commit()
        connection.close()

    def create_new_user(self, Email, Password, ContractorIdentifier, IsAdmin):
        connection = sqlite3.connect(self.filename)
        cursor = connection.cursor()

        salt = generate_random_string(16)

        cursor.execute("INSERT INTO Users (Email, Password, Salt, ContractorIdentifier, IsAdmin, UAD, UADTimestamp) VALUES (?, ?, ?, ?, ?, ?, ?)", (
            Email,
            sha256_hash(Password + salt),
            salt,
            ContractorIdentifier,
            IsAdmin,
            "null",
            "null"
        ))

        connection.commit()
        connection.close()

    def pull_user_by_email(self, Email):
        connection = sqlite3.connect(self.filename)
        cursor = connection.cursor()
        return (cursor.execute("SELECT * FROM Users WHERE Email = ?", (Email,)).fetchone(), connection.close())[0]

    def update_uad(self, uad, timestamp, email):
        connection = sqlite3.connect(self.filename)
        cursor = connection.cursor()

        cursor.execute("UPDATE Users SET UAD = ?, UADTimestamp = ? WHERE Email = ?", (uad, timestamp, email))
        connection.commit()
        connection.close()

    def get_uad_and_timestamp(self, email):
        connection = sqlite3.connect(self.filename)
        cursor = connection.cursor()

        result = cursor.execute("SELECT * FROM Users WHERE Email = ?", (email,)).fetchone()
        connection.close()
        return [result[-2], result[-1]]

    def create_contractor(self, contractor_name, address, email, phone):
        connection = sqlite3.connect(self.filename)
        cursor = connection.cursor()
        cursor.execute("INSERT INTO Contractors (ContractorIdentifier, Address, Email, Phone) VALUES (?, ?, ?, ?, ?)",
                       (contractor_name, address, email, phone))

        connection.commit()
        connection.close()

    def delete_user(self, email):
        connection = sqlite3.connect(self.filename)
        cursor = connection.cursor()
        cursor.execute("DELETE FROM Users WHERE Email = ?", (email,))
        connection.commit()
        connection.close()
