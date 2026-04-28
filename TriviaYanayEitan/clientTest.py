import socket
import json
import random
import string


first_names = ["Noam", "Lior", "Eden", "Yuval", "Daniel", "Omer", "Shira"]
last_names = ["Cohen", "Levi", "Mizrahi", "Peretz", "Biton", "Dahan"]

streets = ["Herzl", "Ben Gurion", "Rothschild", "Weizmann", "Jabotinsky"]
cities = ["Tel Aviv", "Haifa", "Jerusalem", "Ramat Gan", "Netanya"]

domains = ["gmail.com", "yahoo.com", "outlook.com"]

HOST = '127.0.0.1'
PORT = 12334

LOGIN_CODE = 1
SIGNUP_CODE = 3


def generate_username():
    return random.choice(first_names) + str(random.randint(10, 999))


def generate_valid_password():
    # always valid, but slightly random
    specials = "*&^%$#@!"
    return (
        random.choice(string.ascii_uppercase) +
        random.choice(string.ascii_lowercase) +
        str(random.randint(0, 9)) +
        random.choice(specials) +
        ''.join(random.choices(string.ascii_letters + string.digits, k=4))
    )


def generate_valid_email(username):
    return f"{username.lower()}@{random.choice(domains)}"


def generate_valid_address():
    return f"{random.choice(streets)}, {random.randint(1, 200)}, {random.choice(cities)}"


def generate_valid_phone():
    prefixes = ["02", "03", "04", "050", "052", "054"]
    return f"{random.choice(prefixes)}-{random.randint(1000000, 9999999)}"


def generate_valid_birthdate():
    day = random.randint(1, 28)
    month = random.randint(1, 12)
    year = random.randint(1980, 2010)
    return f"{day:02}/{month:02}/{year}"


def int_to_4bytes(n):
    return bytes([
        (n >> 24) & 0xFF,
        (n >> 16) & 0xFF,
        (n >> 8) & 0xFF,
        n & 0xFF
    ])


def build_packet(code, data_dict):
    json_data = json.dumps(data_dict).encode()
    length_bytes = int_to_4bytes(len(json_data))

    return bytes([code]) + length_bytes + json_data


def send_packet(s, data, code):
    packet = build_packet(code, data)
    s.sendall(packet)
    response = s.recv(4096)
    print("Server response:", response.decode())


def build_valid_user():
    username = generate_username()

    return {
        "username": username,
        "password": generate_valid_password(),
        "email": generate_valid_email(username),
        "address": generate_valid_address(),
        "phone": generate_valid_phone(),
        "birthdate": generate_valid_birthdate()
    }


# INVALID TESTS (TO BREAK REGEX)

def test_invalid_email(sock):
    print("\n[TEST] Invalid Email")

    user = build_valid_user()
    user["email"] = "not-an-email"

    send_packet(sock, user, SIGNUP_CODE)


def test_invalid_password(sock):
    print("\n[TEST] Invalid Password")

    user = build_valid_user()
    user["password"] = "abc"

    send_packet(sock, user, SIGNUP_CODE)


def test_invalid_address(sock):
    print("\n[TEST] Invalid Address")

    user = build_valid_user()
    user["address"] = "bad address format"

    send_packet(sock, user, SIGNUP_CODE)


def test_invalid_phone(sock):
    print("\n[TEST] Invalid Phone")

    user = build_valid_user()
    user["phone"] = "123456"

    send_packet(sock, user, SIGNUP_CODE)


def test_invalid_birthdate(sock):
    print("\n[TEST] Invalid Birthdate")

    user = build_valid_user()
    user["birthdate"] = "99-99-9999"

    send_packet(sock, user, SIGNUP_CODE)

# SIGNUP TESTER


def test_valid_signup(sock):
    print("\n[TEST] Valid Signup")

    user = build_valid_user()
    send_packet(sock, user, SIGNUP_CODE)

# LOGIN TESTER


def test_login(sock):
    print("\n[TEST] Login Flow")

    user = build_valid_user()

    # signup first
    send_packet(sock, user, SIGNUP_CODE)

    # login
    login_data = {
        "username": user["username"],
        "password": user["password"]
    }

    send_packet(sock, login_data, LOGIN_CODE)


def menu():
    print("""
    TESTER MENU:
    1. Valid Signup
    2. Invalid Email
    3. Invalid Password
    4. Invalid Address
    5. Invalid Phone
    6. Invalid Birthdate
    7. Full Login Flow
    8. Exit
    """)


def main():
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))

            while True:
                menu()
                choice = input("Select option: ")

                if choice == "1":
                    test_valid_signup(s)

                elif choice == "2":
                    test_invalid_email(s)

                elif choice == "3":
                    test_invalid_password(s)

                elif choice == "4":
                    test_invalid_address(s)

                elif choice == "5":
                    test_invalid_phone(s)

                elif choice == "6":
                    test_invalid_birthdate(s)

                elif choice == "7":
                    test_login(s)

                elif choice == "8":
                    print("Exiting tester.")
                    break

                else:
                    print("Invalid option.")

    except socket.error:
        print("Connection error")


if __name__ == "__main__":
    main()
