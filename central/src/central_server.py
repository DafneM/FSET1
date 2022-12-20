import socket 
import threading
from parse import json_to_object
from parse import object_to_json
import json
import sys
import time

# IP = "164.41.98.26"
# PORT = 10733
# ADDR = (IP, PORT)
IP = ""
PORT = 0
ADDR = (IP, PORT)
data_payload = 2048
FORMAT = "utf-8"
json_message =  {}

connections = {}

salas = dict()

def receive_client(connection, addr):
    global json_message, salas

    print("Conexao nova")
    flag_conn = 1

    response = init_json(connection, addr)
    salas[response["nome"]] = response
    
    while flag_conn:
        message = connection.recv(data_payload)

        if not message: 
            break

        salas[response["nome"]] = json.loads(message)
        salas[response["nome"]]["ip"] = connection
        # print(f"Mensagem recebida aqui {salas}")

def send_message(connection, message):
    len_message = len(message)

    connection.send(bytes(message, encoding=FORMAT))
    

def manage_user_interface():
    global json_message, salas
    instruction = -1
    while(instruction != 0):
        
        if not salas:
            time.sleep(5)
            continue

        for chave, sala in salas.items():
            estados_print = (chave +
            '\n\nLampada 1: ' +  str(sala["L_01_state"]) +
            '\nLampada 2: ' + str(sala["L_02_state"]) +
            '\nProjetor: ' + str(sala["PR_state"]) +
            '\nAr condicionado: ' + str(sala["AC_state"]) +
            '\nAlarme: ' + str(sala["AL_BZ_state"]) +
            '\nSensor de presença: ' + str(sala["SPres_state"]) +
            '\nSensor de fumaça: ' + str(sala["SFum_state"]) +
            '\nSensor de janela: ' + str(sala["SJan_state"]) +
            '\nSensor de porta: ' + str(sala["SPor_state"]) +
            '\nQuantidade de pessoas: ' + str(sala["SC_qtd"]) +
            '\nTemperatura: ' + str(round(sala["DHT_temp"], 2)) + 'ºC'
            '\nUmidade: ' + str(round(sala["DHT_humidity"], 2)) + '%')

            print(estados_print)

        # for sala in salas.keys():
        #     print(f"{sala}")

        choice = input("\n\nEscolha a sala que deseja acessar: ")
        # if choice <= len(self.salas.values()):
        #     print('Essa escolha nao é valida!') 

        print("\n\nO que você deseja fazer?")
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
        instruction = int(input("\nQual é a opção que você deseja? "))

        # print(salas[choice])
        # breakpoint()
        if instruction == 0:
            return

        if instruction == 1:
            salas[choice]["L_01_state"] = 1

        if instruction == 2:
            salas[choice]["L_01_state"] = 0
        
        if instruction == 3:
            salas[choice]["L_02_state"] = 1

        if instruction == 4:
            salas[choice]["L_02_state"] = 0

        if instruction == 5:
            salas[choice]["L_01_state"] = 1
            salas[choice]["L_02_state"] = 1

        if instruction == 6:
            salas[choice]["L_01_state"] = 0
            salas[choice]["L_02_state"] = 0

        if instruction == 7:
            salas[choice]["AC_state"] = 1

        if instruction == 8:
            salas[choice]["AC_state"] = 0

        if instruction == 9:
            salas[choice]["PR_state"] = 1

        if instruction == 10:
            salas[choice]["PR_state"] = 0

        if instruction == 11:
            salas[choice]["AL_BZ_state"] = 1

        if instruction == 12:
            salas[choice]["AL_BZ_state"] = 0

        if instruction == 13:
            print(f'{salas[choice]}\n')

        ip = salas[choice].pop("ip")
        json_send_message = object_to_json(salas[choice])
        send_message(ip, json_send_message)
        salas[choice]["ip"] = ip

def init_json(connection, addr):
    global json_message

    message = connection.recv(data_payload)
    json_message = json.loads(message)
    
    return json_message

def main():
    global IP, PORT, ADDR, salas

    IP = sys.argv[1]
    PORT = int(sys.argv[2])
    ADDR = (IP, PORT)

    print("Server is starting")
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(ADDR)
    server.listen()
    print(f"ip e porta {IP}:{PORT}")

    # manage_user_interface(connection, addr)
    thread = threading.Thread(target=manage_user_interface)
    thread.start()

    while True:
        connection, addr = server.accept()

        connections[connection.getpeername()[0]] = connection

        threading.Thread(target=receive_client, args=(connection, addr)).start()

    while True:
        time.sleep(100)

if __name__ == '__main__':
    main()