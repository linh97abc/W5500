# echo-client.py

import socket

HOST = "169.254.151.138"  # The server's hostname or IP address
PORT = 65432  # The port used by the server

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    print("connect")

    s.sendall(b"Hello, world from client")

    while True:
        data = input()
        if data == "xx":
            break
        s.sendall(data.encode())


