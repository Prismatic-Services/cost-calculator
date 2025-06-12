from controllers.db import Database

"""
Make a new user
Make a new contractor
Delete files

"""


if __name__ == '__main__':
    db = Database()

    print("-----------")
    print("Capitalization, spacing, and chars MUST be EXACT\n")

    print("[1] Make a new user")
    print("[2] Make a new contractor")
    print("[3] Delete files")
    print("[4] Delete user")
    req = int(input("Enter Numeric Code: "))

    if req == 1:
        try:
            email = input("Email: ")
            password = input("Password: ")
            contractor = input("Contractor: ")
            isAdmin = input("Is Admin? (True/False): ")

            db.create_new_user(email, password, contractor, isAdmin)
        except Exception as e:
            print("Something went wrong trying to create user: " + str(e))

    elif req == 2:
        try:
            contractor = input("Contractor Name: ")
            email = input("Email: ")
            address = input("Address: ")
            phone = input("Phone: ")

            db.create_contractor(contractor, email, address, phone)
        except Exception as e:
            print("Something went wrong trying to create contractor: " + str(e))

    elif req == 3:
        pass

    elif req == 4:
        try:
            email = input("Email: ")
            db.delete_user(email)
        except Exception as e:
            print("Something went wrong trying to delete user: " + str(e))