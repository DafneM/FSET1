import socket 
import threading
from parse import json_to_object

IP = "164.41.98.26"
PORT = 10732
ADDR = (IP, PORT)
data_payload = 2048
FORMAT = "utf-8"

def interface():
       ...

def receive_client(connection, addr):
    print("Conexao nova")
    flag_conn = 1
    
    while flag_conn:
        message = connection.recv(data_payload).decode(FORMAT)

        # json_message = json_to_object(message)

        print(f"Mensagem recebida: {message}")
        # connection.sendall(len(message).to_bytes(4, 'little'), encoding=FORMAT)
        connection.sendall(bytes(message, encoding=FORMAT))

    connection.close()

def main():
    print("Server is starting");
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(ADDR)
    server.listen()
    print(f"ip e porta {IP}:{PORT}")

    thread = threading.Thread(target=interface)
    thread.start()

    while 1:
        connection, addr = server.accept()
        thread = threading.Thread(target=receive_client, args=(connection, addr))
        thread.start()
        connection.sendall(b"ola mundo")

if __name__ == '__main__':
    main()