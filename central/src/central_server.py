import socket 
import threading
from parse import json_to_object
from parse import object_to_json
import json
import sys
import time
from datetime import datetime


# IP = "164.41.98.26"
# PORT = 10733
# ADDR = (IP, PORT)
IP = ""
PORT = 0
ADDR = (IP, PORT)
data_payload = 2048
FORMAT = "utf-8"
json_message =  {}
cont_pessoas = 0

connections = {}

salas = dict()

def receive_client(connection, addr):
    global json_message, salas

    print("\nOlha! Você teve uma nova conexão!\n")
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


def print_states():
    for chave, sala in salas.items():
        print( 
        '\n\n-------------------------------------------------------\n  ' +
        chave +
        '\n\n  Temperatura: ' + str(round(sala["DHT_temp"], 2)) + 'ºC'
        '\n  Umidade: ' + str(round(sala["DHT_humidity"], 2)) + '%'
        '\n\n  Lampada 1: ' +  str(sala["L_01_state"]) +
        '\n  Lampada 2: ' + str(sala["L_02_state"]) +
        '\n  Projetor: ' + str(sala["PR_state"]) +
        '\n  Ar condicionado: ' + str(sala["AC_state"]) +
        '\n  Alarme: ' + str(sala["AL_BZ_state"]) +
        '\n\n  Sensor de presença: ' + str(sala["SPres_state"]) +
        '\n  Sensor de fumaça: ' + str(sala["SFum_state"]) +
        '\n  Sensor de janela: ' + str(sala["SJan_state"]) +
        '\n  Sensor de porta: ' + str(sala["SPor_state"]) +
        '\n  Quantidade de pessoas: ' + str(sala["SC_qtd"]) + '\n'
        '--------------------------------------------------------')

def create_log(comando):
    log = open('log.csv', 'a')
    log.write(f'{datetime.now()}, {comando}\n')
    log.close()

def manage_user_interface():
    global json_message, salas
    instruction = -1
    while(instruction != 0):
        
        if not salas:
            time.sleep(5)
            continue

        choice = input("\n\n  Escolha a sala que deseja acessar: (Utilize o seguinte formato -> Sala 01) Para mandar um comando para as duas digite Duas\n")

        if choice == "Duas":
            print("\n\n  O que você deseja fazer?")
            print('''   
            [1] Ligar todas as lampadas
            [2] Desligar todas as lampadas
            [3] Ligar todos os aparelhos
            [4] Desligar todos os aparelhos
            [5] Ligar sistema de alarme
            [6] Desligar sistema de alarme
            [7] Ver os estados das salas
            ''')
            instruction = int(input("\n  Qual é a opção que você deseja? "))

            if instruction == 1:
                create_log(f'Ligar todas as lampadas')
                for s in salas.values():
                    s["L_01_state"] = 1
                    s["L_02_state"] = 1

            if instruction == 2:
                create_log(f'Desligar todas as lampadas')
                for s in salas.values():
                    s["L_01_state"] = 0
                    s["L_02_state"] = 0

            if instruction == 3:
                create_log(f'Ligar todos os aparelhos')
                for s in salas.values():
                    s["L_01_state"] = 1
                    s["L_02_state"] = 1
                    s["AC_state"] = 1
                    s["L_PR_state"] = 1
            
            if instruction == 4:
                create_log(f'Desligar todos os aparelhos')
                for s in salas.values():
                    s["L_01_state"] = 0
                    s["L_02_state"] = 0
                    s["AC_state"] = 0
                    s["L_PR_state"] = 0

            if instruction == 5:
                create_log(f'Ligar sistema de alarme')
                for s in salas.values():
                    if (s["SPres_state"] == 1 and s["SPor_state"] == 1 and s["SJan_state"] == 1):
                        s["sistema_alarme_state"] = 1
                    else:
                        print("  Você não pode ligar o sistema de alarme, pois os sensores que o ativam estão desligados\n")

            if instruction == 6:
                create_log(f'Desligar sistema de alarme')
                for s in salas.values():
                    s["sistema_alarme_state"] = 0
            
            if instruction == 7:
                create_log(f'Ver todos os estados')
                print_states()
            
            for s in salas.values():
                ip = s.pop("ip")
                json_send_message = object_to_json(s)
                send_message(ip, json_send_message)
                s["ip"] = ip

        else: 
            print("\n\nO que você deseja fazer?")
            print('''   
            [1] Ligar lampada 01
            [2] Desligar lampada 01
            [3] Ligar lampada 02
            [4] Desligar lampada 02
            [5] Ligar as duas lampadas
            [6] Desligar as duas lampadas
            [7] Ligar ar condicionado
            [8] Desligar ar condicionado
            [9] Ligar projetor
            [10] Desligar projetor
            [11] Ligar todos os aparelhos 
            [12] Desligar todos os aparelhos
            [13] Ver os estados das salas
            [0] Sair
            ''')
            instruction = int(input("\n  Qual é a opção que você deseja? "))

            # print(salas[choice])
            # breakpoint()
            if instruction == 0:
                return

            if instruction == 1:
                salas[choice]["L_01_state"] = 1
                create_log(f'Ligar lâmpada 01 da {choice}')

            if instruction == 2:
                salas[choice]["L_01_state"] = 0
                create_log(f'Desligar lâmpada 01 da {choice}')
            
            if instruction == 3:
                salas[choice]["L_02_state"] = 1
                create_log(f'Ligar lâmpada 02 da {choice}')

            if instruction == 4:
                salas[choice]["L_02_state"] = 0
                create_log(f'Desligar lâmpada 02 da {choice}')


            if instruction == 5:
                salas[choice]["L_01_state"] = 1
                salas[choice]["L_02_state"] = 1
                create_log(f'Ligar as duas lampadas da {choice}')

            if instruction == 6:
                salas[choice]["L_01_state"] = 0
                salas[choice]["L_02_state"] = 0
                create_log(f'Desligar as duas lampadas da {choice}')

            if instruction == 7:
                salas[choice]["AC_state"] = 1
                create_log(f'Ligar ar condicionado da {choice}')

            if instruction == 8:
                salas[choice]["AC_state"] = 0
                create_log(f'Desligar ar condicionado da {choice}')

            if instruction == 9:
                salas[choice]["PR_state"] = 1
                create_log(f'Ligar projetor da {choice}')

            if instruction == 10:
                salas[choice]["PR_state"] = 0
                create_log(f'Desligar projetor da {choice}')

            if instruction == 11:
                salas[choice]["L_01_state"] = 1
                salas[choice]["L_02_state"] = 1
                salas[choice]["L_AC_state"] = 1
                salas[choice]["PR_state"] = 1
                create_log(f'Ligar todos os aparelhos da {choice}')

            if instruction == 12:
                salas[choice]["L_01_state"] = 0
                salas[choice]["L_02_state"] = 0
                salas[choice]["L_AC_state"] = 0
                salas[choice]["PR_state"] = 0
                create_log(f'Desligar todos os aparelhos da {choice}')

            if instruction == 13:
                print_states()

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
