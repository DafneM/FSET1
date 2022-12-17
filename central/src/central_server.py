import socket 
import threading
from parse import json_to_object
from parse import object_to_json
import json

IP = "164.41.98.26"
PORT = 10733
ADDR = (IP, PORT)
data_payload = 2048
FORMAT = "utf-8"
json_message = {}

def receive_client(connection, addr):
    global json_message

    print("Conexao nova")
    flag_conn = 1
    
    while flag_conn:
        message = connection.recv(data_payload)

        if not message: 
            break

        json_message = json.loads(message)

        # print(f"Mensagem recebida aqui {json_message}")

    connection.close()

def send_message(connection, message):
    len_message = len(message)

    connection.send(bytes(message, encoding=FORMAT))
    

def manage_user_interface(connection, addr):
    global json_message

    print("Seja bem vindo, esses são os estados da sua gpio:")

    print("O que você deseja fazer?")
    
    instruction = int(input("Digite 1 para apagar a lampada: "))
    json_message["L_01_state"] = 1

    json_send_message = object_to_json(json_message)

    # breakpoint()
    if instruction == 1:

        send_message(connection, json_send_message)


def main():
    print("Server is starting")
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(ADDR)
    server.listen()
    print(f"ip e porta {IP}:{PORT}")

    connection, addr = server.accept()

    thread = threading.Thread(target=receive_client, args=(connection, addr))
    thread.start()

    thread = threading.Thread(target=manage_user_interface(connection, addr))
    thread.start()
    # manage_user_interface(connection, addr)

    import time
    while True:
        time.sleep(100)

if __name__ == '__main__':
    main()