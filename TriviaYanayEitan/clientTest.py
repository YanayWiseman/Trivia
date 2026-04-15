import sockets

HOST = '127.0.0.1'
PORT = 9000


def main():
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
            s.sendall(b"Hello")

    except socket.error:
        print("Connection error")