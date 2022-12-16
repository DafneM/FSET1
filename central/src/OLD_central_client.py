import socket
from parse import object_to_json

IP = "164.41.98.26"
PORT = 10731
ADDR = (IP, PORT)
data_payload = 2048
FORMAT = "utf-8"

json_states = ''' 
    {  
        "nome": "Dafne" 
    }
'''

def main():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(ADDR)
    print("cliente conectado")

    flag_conn = 1
    while flag_conn:

        message = object_to_json(json_states)

        client.send(bytes(message, encoding=FORMAT))

        message = client.recv(data_payload).decode(FORMAT)
        print(f"SERVER {message}")

        if not message:
            break

if __name__ == '__main__':
    main()