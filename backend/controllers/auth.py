import time
from controllers.utils import generate_random_string, sha256_hash
from controllers.db import Database


def create_timestamp():
    return time.time()


class Auth:
    def __init__(self, uad_timeout=43200, UAD_length=16):
        self.uad_timeout = uad_timeout  # stored as seconds (default is 12 hours)
        self.UAD_length = UAD_length
        self.database = Database()

    def check_hash(self, email, password):
        row = self.database.pull_user_by_email(email)
        if not row:
            print("Invalid Email Provided... ", email)
            return False

        hashed_psw = row[1]
        salt = row[2]

        if sha256_hash(password + salt) != hashed_psw:
            print("Invalid Password Provided... ", password)
            return False

        return True

    def set_uad_and_timestamp(self, email):
        uad = generate_random_string(self.UAD_length)
        self.database.update_uad(
            uad=uad,
            timestamp=create_timestamp(),
            email=email
        )
        return uad

    def login(self, email, password):
        if not self.check_hash(email, password): return False
        return self.set_uad_and_timestamp(email)

    def verify_uad_and_timestamp(self, email, uad):
        db_uad, db_timestamp = self.database.get_uad_and_timestamp(email)
        if db_uad != uad:
            return False
        if time.time() - float(db_timestamp) > self.uad_timeout:
            return False
        return True
