#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include "jsonParser/read_jsonconfig.c"
// #include "io.h"
#include "dht22.h"
#include "distributed_client.h"

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

int main (int argc, char *argv[])
{
  int done;

  if (wiringPiSetup () == -1)
    return 1 ;

  pthread_t dht22_thread;

  //chama arquivo 1
  // read_jsonconfig("/home/dafnemoreira/distributed/configuracao_sala_01.json");
  // chama arquivo 2
  read_jsonconfig("/home/dafnemoreira/distributed/configuracao_sala_02.json");

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

  open_distributed_client_socket();

  pthread_create(&dht22_thread, NULL, read_dht22, NULL);

  // Define pinos OUTPUT
  pinMode (L_01, OUTPUT) ; //L01 1
  pinMode (L_02, OUTPUT) ; //L02 2
  pinMode (AC, OUTPUT) ; //Ar
  pinMode (PR, OUTPUT) ; //Projetor   
  pinMode (AL_BZ, OUTPUT) ; //Alarme       
 
  //Define sensores INPUT
  pullUpDnControl(SPres, PUD_DOWN);
  pullUpDnControl(SFum, PUD_DOWN);

    stateSPres = digitalRead(stateSPres);

    wiringPiISR(SPres, INT_EDGE_BOTH, &handle);

    turnOnLamps();
    delay(500);
    turnOffLamps();

    turnOnAll();
    delay(500);
    turnOffAll();

  return 0 ;
}