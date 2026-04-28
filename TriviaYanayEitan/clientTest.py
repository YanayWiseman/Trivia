import socket
import json
import random
import string


HOST = '127.0.0.1'
PORT = 9000

LOGIN_CODE = 1
SIGNUP_CODE = 3


def generate_random_string(length=10):
    # characters = string.ascii_letters + string.digits
    characters = string.ascii_letters + string.digits
    return ''.join(random.choices(characters, k=length))


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


def test_1_and_2(s):
    print("1 - Logging in a user that does not exist:")
    data = {
        "username": generate_random_string(10),
        "password": generate_random_string(10)
    }
    send_packet(s, data, LOGIN_CODE)

    print("2 - signing in a user with the same name TWICE!:")
    username = generate_random_string(10)
    data = {
        "username": username,
        "password": generate_random_string(10),
        "email": "mpgmpg.gmail.com"
    }
    print("first time: ")
    send_packet(s, data, SIGNUP_CODE)
    print("second time: ")
    send_packet(s, data, SIGNUP_CODE)


def test_3(s):
    print("3 - a user can't log in when he's already logged in")

    data = {
        "username": generate_random_string(10),
        "password": generate_random_string(10),
        "email": "mpgmpg.gmail.com"
    }
    # first sign the user up
    send_packet(s, data, SIGNUP_CODE)

    # now log him in twice
    send_packet(s, data, LOGIN_CODE)
    send_packet(s, data, LOGIN_CODE)


def main():
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
            choice = input("Select one of the following:\n1. test requirements 1 & 2\n2. test requirement 3\n")

            if choice == "1":
                test_1_and_2(s)

            elif choice == "2":
                test_3(s)

            else:
                print("Invalid choice")

    except socket.error:
        print("Connection error")


if __name__ == "__main__":
    main()
