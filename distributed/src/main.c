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

cont_pessoa_in = 0;
cont_pessoa_out = 0;

#define WAIT_TIME 10000

void handleSPres(void) {
  	if (states.SPres_state == 1) {
      states.SPres_state = 0;
		}
		else {
      states.SPres_state = 1;
		}
}

void handleSFum(void) {
  	if (states.SFum_state == 1) {
      states.SFum_state = 0;
		}
		else {
      states.SFum_state = 1;
		}
}

void handleSJan(void) {
  	if (states.SJan_state == 1) {
      states.SJan_state = 0;
		}
		else {
      states.SJan_state = 1;
		}
}

void handleSPor(void) {
  	if (states.SPor_state == 1) {
      states.SPor_state = 0;
		}
		else {
      states.SPor_state = 1;
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
  int result = 0;
  double *DHTtemp_humidity = malloc(2);
  while(1){
		result = capture_dht22(gpio_temp.gpio, DHTtemp_humidity);
    states.DHT_temp = DHTtemp_humidity[0];
    states.DHT_humidity = DHTtemp_humidity[1];
		// delay(WAIT_TIME); 
  }
}

void init_gpio(){
  L_01 = gpio_outputs[0].gpio;
  L_02 = gpio_outputs[1].gpio;
  PR = gpio_outputs[2].gpio;
  AC = gpio_outputs[3].gpio;
  AL_BZ = gpio_outputs[4].gpio;
  SPres = gpio_inputs[0].gpio;
  SFum = gpio_inputs[1].gpio;
  SJan = gpio_inputs[2].gpio;
  SPor = gpio_inputs[3].gpio;
  SC_IN = gpio_inputs[4].gpio;
  SC_OUT = gpio_inputs[5].gpio;
  DHT22 = gpio_temp.gpio;
}

void init_states(){
  states.L_01_state =  digitalRead(L_01);
  states.L_02_state = digitalRead(L_02);
  states.AC_state = digitalRead(AC);
  states.PR_state = digitalRead(PR);
  states.AL_BZ_state = digitalRead(AL_BZ);
  states.SPres_state = digitalRead(SPres);
  states.SFum_state = digitalRead(SFum);
  states.SJan_state = digitalRead(SJan);
  states.SPor_state = digitalRead(SPor);
  states.SC_IN_state = digitalRead(SC_IN);
  states.SC_OUT_state = digitalRead(SC_OUT);
  states.SC_qtd = 0;
  states.DHT_temp = 0;
  states.DHT_humidity = 0;
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
  pullUpDnControl(SJan, PUD_DOWN);
  pullUpDnControl(SPor, PUD_DOWN);  
  pullUpDnControl(SJan, PUD_DOWN);
  pullUpDnControl(SPor, PUD_DOWN);  
  pullUpDnControl(SC_IN, PUD_DOWN);
  pullUpDnControl(SC_OUT, PUD_DOWN);  
}

void open_distributed_client_socket() {

	IP_Servidor = ip_dist;
	servidorPorta = porta_dist;

  printf("servidor ip %s\n", IP_Servidor);

  pthread_t read_central_thread;

	// Criar Socket
	if((clienteSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		printf("Erro no socket()\n");
    fflush(stdin);

	// Construir struct sockaddr_in
	memset(&servidorAddr, 0, sizeof(servidorAddr)); // Zerando a estrutura de dados
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = inet_addr(IP_Servidor);
	servidorAddr.sin_port = htons(servidorPorta);

  while(1){
    if (connect(clienteSocket, (struct sockaddr *) &servidorAddr, 
                sizeof(servidorAddr)) < 0) {
      printf("Ainda não estou conectado! Por favor, tente novamente.\n");
      fflush(stdin);
    }
    else{
      fflush(stdin);
      break;
    }
    sleep(1);
  }
  fflush(stdin);
}

void *read_central(void *arg){

	char *mensagem = malloc(2048);
	char buffer[2048];
	unsigned int tamanhoMensagem;

  while(1){
    memset(buffer, 0, 2048);
    if(recv(clienteSocket, buffer, 2048 - 1, 0) < 0) {
      printf("erro\n");
      fflush(stdin);
    }
    else{
      read_json_message(buffer);
      verify_states();
      fflush(stdin);
    }
  }

	close(clienteSocket);
}

void verify_states(){
  if(states.L_01_state == 0){
    turnOff(L_01);
  }

  if(states.L_01_state == 1){
    turnOn(L_01);
  }

  if(states.L_02_state == 0){
    turnOff(L_02);
  }

  if(states.L_02_state == 1){
    turnOn(L_02);
  }

  if(states.AC_state == 1){
    turnOn(AC);
  }

  if(states.AC_state == 0){
    turnOff(AC);
  }

  if(states.PR_state == 1){
    turnOn(PR);
  }

  if(states.PR_state == 0){
    turnOn(PR);
  }

  if(states.AL_BZ_state == 1){
    turnOn(AL_BZ);
  }

  if(states.AL_BZ_state == 0){
    turnOff(AL_BZ);
  }
}

void* conta_pessoas(void *arg){
  while (1) {
    // printf("ENTRADAS E SAIDAS %d %d", gpio_inputs[4].gpio, gpio_inputs[5].gpio);
    // printf("IN E OUT: %d %d", SC_IN, SC_OUT);
    int in = digitalRead(SC_IN);
    int out = digitalRead(SC_OUT);
    // printf("IN E OUT: %d %d\n", in, out);
    // TA PRINTANDO 0
    // printf("IN E OUT: %d %d", in, out);
    if(in == 0 || in != cont_pessoa_in){
      cont_pessoa_in = in;
      states.SC_qtd = states.SC_qtd + cont_pessoa_in;
    }

    if(out == 0 || out != cont_pessoa_out ){
      cont_pessoa_out = out;
      states.SC_qtd = states.SC_qtd - cont_pessoa_out;
    }
    // printf("thread conta pessoas: %d", states.SC_qtd);
    usleep(200000);
  }
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
  char *path = malloc(200);
  char *string = malloc(1000);

  if (wiringPiSetup () == -1)
    return 1 ;

  pthread_t dht22_thread;
  pthread_t cont_pessoa_thread;
  pthread_t read_central_thread;

  printf("Coloque o nome do arquivo de configuracao da sala que deseja configurar (exemplo -> sala.json):\n");
  scanf("%s", path);

  read_jsonconfig(path);

  init_gpio();
  init_states();

  open_distributed_client_socket();

  pthread_create(&dht22_thread, NULL, read_dht22, NULL);
  pthread_create(&cont_pessoa_thread, NULL, conta_pessoas, NULL);
  pthread_create(&read_central_thread, NULL, read_central, NULL);

  wiringPiISR(SPres, INT_EDGE_BOTH, &handleSPres);
  wiringPiISR(SFum, INT_EDGE_BOTH, &handleSFum);
  wiringPiISR(SJan, INT_EDGE_BOTH, &handleSJan);
  wiringPiISR(SPor, INT_EDGE_BOTH, &handleSPor);

  while(1){
    send_central_data();
    sleep(1);
  }

  pthread_join(dht22_thread, NULL);

  // turnOnAll();
  // delay(500);
  // turnOffAll();

  return 0 ;
}