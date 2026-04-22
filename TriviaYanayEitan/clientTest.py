import socket
import json

HOST = '127.0.0.1'
PORT = 9000

LOGIN_CODE = 1
SIGNUP_CODE = 3


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


def main():
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
            choice = input("Login (1) or Signup (3)? ")
            if choice == "1":
                data = {
                    "username": "EitanRadinsky",
                    "password": "thisIsAUniquePassword"
                }
                code = LOGIN_CODE
            elif choice == "3":
                data = {
                    "username": "EitanRadinsky ",
                    "password": "thisIsAUniquePassword",
                    "email": "mpgmpg.gmail.com"
                }
                code = SIGNUP_CODE
            else:
                print("Invalid choice")
                return

            packet = build_packet(code, data)
            s.sendall(packet)

            response = s.recv(4096)
            print("Server response:", response.decode())

    except socket.error:
        print("Connection error")


if __name__ == "__main__":
    main()
