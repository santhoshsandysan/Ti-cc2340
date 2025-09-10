import socket

HOST = '0.0.0.0'
PORT = 6000

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen(1)
    print(f"Listening on port {PORT}...")

    while True:
        try:
            conn, addr = s.accept()
            print(f"Connected by {addr}")
            with conn:
                while True:
                    try:
                        data = conn.recv(1024)
                        if not data:
                            print("Client disconnected")
                            break
                        print("Received:", data.decode('utf-8', errors='ignore'))
                    except ConnectionResetError:
                        print("Connection reset by remote host")
                        break
        except KeyboardInterrupt:
            print("Server stopped by user")
            break
