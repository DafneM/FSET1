#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include "jsonParser/read_jsonconfig.c"
// #include "io.h"
#include "dht22.h"
#include <states_io.h>
#include <create_message.h>
#include <read_message.h>
#include <states_io.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int L_01;
int L_02;
int AC;
int PR;
int AL_BZ;
int SPres;
int SFum;
int SJan;
int SPor;
int SC_IN;
int SC_OUT;
int DHT22;  

int clienteSocket;
struct sockaddr_in servidorAddr;
unsigned short servidorPorta;
char *IP_Servidor;

#define WAIT_TIME 10000

static volatile int stateSPres;

void handle(void) {
  	if (stateSPres) {
      stateSPres = 0;
		}
		else {
      stateSPres = 1;
		}
}

void turnOn(int X){
    digitalWrite (X, 1) ;  
}

void turnOff(int Y){
    digitalWrite (Y, 0) ;  
}

void turnOnLamps(){
  turnOn(L_01);
  turnOn(L_02);
  send_central_data();
}

void turnOffLamps(){
  turnOff(L_01);
  turnOff(L_02);
}

void turnOnAll(){
    turnOn(L_01);
    turnOn(L_02);
    turnOn(AC);
    turnOn(PR);
    turnOn(AL_BZ);
}

void turnOffAll(){
    turnOff(L_01);
    turnOff(L_02);
    turnOff(AC);
    turnOff(PR);
    turnOff(AL_BZ);
}

void *read_dht22 (void *arg){
  int done = 0;
  while(!done){
		done = read_dht_data(DHT22);
		delay(WAIT_TIME); 
  }
}

void init_gpio(){
  L_01 = gpio_outputs[0].gpio;
  L_02 = gpio_outputs[1].gpio;
  AC = gpio_outputs[2].gpio;
  PR = gpio_outputs[3].gpio;
  AL_BZ = gpio_outputs[4].gpio;
  SPres = gpio_inputs[1].gpio;
  SFum = gpio_inputs[2].gpio;
  SJan = gpio_inputs[3].gpio;
  SPor = gpio_inputs[4].gpio;
  SC_IN = gpio_inputs[5].gpio;
  SC_OUT = gpio_inputs[6].gpio;
  DHT22 = gpio_temp.gpio;
}

void init_states(){
  states.L_01_state = 0;
  states.L_02_state = 0;
  states.AC_state = 0;
  states.PR_state = 0;
  states.AL_BZ_state = 0;
  states.SPres_state = 0;
  states.SJan_state = 0;
  states.SPor_state = 0;
  states.SC_IN_state = 0;
  states.SC_OUT_state = 0;
  states.DHT22_state = 0;
}

void def_pins(){
  // Define pinos OUTPUT
  pinMode (L_01, OUTPUT) ; //L01 1
  pinMode (L_02, OUTPUT) ; //L02 2
  pinMode (AC, OUTPUT) ; //Ar
  pinMode (PR, OUTPUT) ; //Projetor   
  pinMode (AL_BZ, OUTPUT) ; //Alarme  

  //Define sensores INPUT
  pullUpDnControl(SPres, PUD_DOWN);
  pullUpDnControl(SFum, PUD_DOWN);  
}

void open_distributed_client_socket() {
	IP_Servidor = "164.41.98.26";
	servidorPorta = 10732;

  pthread_t read_central_thread;

	// Criar Socket
	if((clienteSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		printf("Erro no socket()\n");

	// Construir struct sockaddr_in
	memset(&servidorAddr, 0, sizeof(servidorAddr)); // Zerando a estrutura de dados
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = inet_addr(IP_Servidor);
	servidorAddr.sin_port = htons(servidorPorta);

  while(1){
    // Connect
    if(connect(clienteSocket, (struct sockaddr *) &servidorAddr, 
                sizeof(servidorAddr)) < 0)
      printf("Ainda não estou conectado! Por favor, tente novamente.\n");
    else{
      break;
    }
    sleep(1);
  }
}

void *read_central(void *arg){

	char *mensagem = malloc(2048);
	char buffer[2048];
	unsigned int tamanhoMensagem;

  memset(buffer, 0, 2048);

  while(1){
    if(recv(clienteSocket, tamanhoMensagem, 4, MSG_WAITALL) < 0) {
      printf("erro\n");
    }
    if(recv(clienteSocket, buffer, 2048, MSG_WAITALL) < 0) {
      printf("erro\n");
    }
    else{
      read_json_message(buffer);
      printf("%d\n", states.SPres_state);
    }
  }


	close(clienteSocket);
	exit(0);
}

void send_central_data(){
  	char *string;

    string = create_json_message();

    int len = strlen(string);

  	send(clienteSocket, string, len, 0);
}

int main (int argc, char *argv[])
{
  int done;
  char *string = malloc(1000);

  if (wiringPiSetup () == -1)
    return 1 ;

  pthread_t dht22_thread;
  pthread_t read_central_thread;

  //chama arquivo 1
  // read_jsonconfig("/home/dafnemoreira/distributed/configuracao_sala_01.json");

  // chama arquivo 2
  read_jsonconfig("/home/dafnemoreira/distributed/configuracao_sala_02.json");

  init_gpio();
  init_states();

  // string = create_json_message();
  // read_json_message(string);

  open_distributed_client_socket();

  pthread_create(&dht22_thread, NULL, read_dht22, NULL);
  pthread_create(&read_central_thread, NULL, read_central, NULL);

    // stateSPres = digitalRead(stateSPres);

    // wiringPiISR(SPres, INT_EDGE_BOTH, &handle);

    while(1){
      send_central_data();
      sleep(1);
    }

    turnOnLamps();
    delay(500);
    turnOffLamps();

    turnOnAll();
    delay(500);
    turnOffAll();

  return 0 ;
}