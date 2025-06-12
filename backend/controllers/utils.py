import random
import string
import hashlib


def generate_random_string(length):
    letters = string.ascii_letters + string.digits
    return ''.join(random.choice(letters) for _ in range(length))


def sha256_hash(input_string):
    """Computes the SHA-256 hash of a string."""
    sha256_hash = hashlib.sha256()
    sha256_hash.update(input_string.encode('utf-8'))
    return sha256_hash.hexdigest()


def map_to_csv(data):
    master_string = ""
    print(data)
    previous = "0"
    for k, v in data.items():
        if k.split('-')[0] != previous:
            previous = k.split('-')[0]
            master_string = master_string[:-1]
            master_string += "\n"

        master_string += f"{str(v)},"

    return master_string
