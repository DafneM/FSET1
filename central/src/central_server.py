import socket 
import threading
from parse import json_to_object
from parse import object_to_json
import json

IP = "164.41.98.26"
PORT = 10732
ADDR = (IP, PORT)
data_payload = 2048
FORMAT = "utf-8"

json_states = ''' 
    {  
        ip_servidor_central": "192.168.0.53",
        "porta_servidor_central": 10000,
        "ip_servidor_distribuido": "192.168.0.52",
        "porta_servidor_distribuido": 10200,
        "nome": "Sala 01",
        "id_servidor_distribuido": 1,
        "L_01_state": 1,
        "L_02_state": 1,
        "PR_state": 1,
        "AC_state": 1,
        "AL_BZ_state": 1,
        "SPres_state": 1,
        "SFum_state": 1,
        "SJan_state": 1,
        "SPor_state": 1,
        "SC_IN_state": 1,
        "SC_OUT_state": 1,
        "DHT22_state": 1
    }
'''

def interface():
       ...

def receive_client(connection, addr):
    print("Conexao nova")
    flag_conn = 1
    
    while flag_conn:
        message = connection.recv(data_payload)

        if not message: 
            break
            
        print(message)

        json_message = json.loads(message)

        print(f"Mensagem recebida: {json_message}")
        # connection.sendall(len(message).to_bytes(4, 'little'), encoding=FORMAT)
        # connection.send(bytes(message, encoding=FORMAT))

    connection.close()

def send_message(connection, message):
    len_message = len(message)

    connection.sendall(len(message).to_bytes(4, 'little'), encoding=FORMAT)
    connection.send(bytes(message, encoding=FORMAT))

def manage_user_interface(connection, addr):
    send_message(connection, addr)

def main():
    print("Server is starting")
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(ADDR)
    server.listen()
    print(f"ip e porta {IP}:{PORT}")

    while 1:
        connection, addr = server.accept()
        thread = threading.Thread(target=receive_client, args=(connection, addr))
        thread.start()
        # connection.sendall(b"ola mundo")
        # connection.sendall(b'json_states')
    
    thread = threading.Thread(target=manage_user_interface(connection, addr))
    thread.start()

if __name__ == '__main__':
    main()