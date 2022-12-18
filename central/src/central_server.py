import socket 
import threading
from parse import json_to_object
from parse import object_to_json
import json
import sys

# IP = "164.41.98.26"
# PORT = 10733
# ADDR = (IP, PORT)
IP = ""
PORT = 0
ADDR = (IP, PORT)
data_payload = 2048
FORMAT = "utf-8"
json_message =  {}

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

    instruction = -1
    while(instruction != 0):
        print("Esses são os estados da sua gpio:\n")

        estados_print = ('Lampada 1: ' +  str(json_message["L_01_state"]) +
        '\nLampada 2: ' + str(json_message["L_02_state"]) +
        '\nProjetor:' + str(json_message["PR_state"]) +
        '\nAr condicionado:' + str(json_message["AC_state"]) +
        '\nAlarme:' + str(json_message["AL_BZ_state"]) +
        '\nSensor de presença' + str(json_message["SPres_state"]) +
        '\nSensor de fumaça:' + str(json_message["SFum_state"]) +
        '\nSensor de janela' + str(json_message["SJan_state"]) +
        '\nSensor de porta' + str(json_message["SPor_state"]) +
        '\nQuantidade de pessoas' + str(json_message["SC_qtd"]))
        print(estados_print)

        print("O que você deseja fazer?")
        print('''   [1] Ligar lampada 01
        [2] Desligar lampada 01
        [3] Ligar lampada 02
        [4] Desligar lampada 02
        [5] Ligar as duas lampadas
        [6] Desligar as duas lampadas
        [7] Ligar ar condicionado
        [8] Desligar ar condicionado
        [9] Ligar projetor
        [10] Desligar projetor
        [11] Ligar alarme
        [12] Desligar alarme
        [13] Ver os estados 
        [0] Sair
        ''')
        instruction = int(input("Qual é a opção que você deseja? "))

        # breakpoint()
        if instruction == 0:
            break

        if instruction == 1:
            json_message["L_01_state"] = 1

        if instruction == 2:
            json_message["L_01_state"] = 0
        
        if instruction == 3:
            json_message["L_02_state"] = 1

        if instruction == 4:
            json_message["L_02_state"] = 0

        if instruction == 5:
            json_message["L_01_state"] = 1
            json_message["L_02_state"] = 1

        if instruction == 6:
            json_message["L_01_state"] = 0
            json_message["L_02_state"] = 0

        if instruction == 7:
            json_message["AC_state"] = 1

        if instruction == 8:
            json_message["AC_state"] = 0

        if instruction == 9:
            json_message["PR_state"] = 1

        if instruction == 10:
            json_message["PR_state"] = 0

        if instruction == 11:
            json_message["AL_BZ_state"] = 1

        if instruction == 12:
            json_message["AL_BZ_state"] = 0

        if instruction == 13:
            print(f'{json_message}\n')

        json_send_message = object_to_json(json_message)
        send_message(connection, json_send_message)

def init_json(connection, addr):
    global json_message

    message = connection.recv(data_payload)
    json_message = json.loads(message)

def main():
    global IP, PORT, ADDR

    IP = sys.argv[1]
    PORT = int(sys.argv[2])
    ADDR = (IP, PORT)

    print("Server is starting")
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(ADDR)
    server.listen()
    print(f"ip e porta {IP}:{PORT}")

    connection, addr = server.accept()

    init_json(connection, addr)

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