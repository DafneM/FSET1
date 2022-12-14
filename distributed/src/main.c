#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>
#include "jsonParser/read_jsonconfig.c"
// #include "io.h"
#include "dht22.h"

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
int SC_TEMP;

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

// PI_THREAD (turnOnAlarm)
//     turnOn(AL_BZ);

int main (int argc, char *argv[])
{
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
  SC_TEMP = gpio_temp.gpio;


  // printf("gpio: %d", gpio_inputs[1].gpio);
  // printf("L02: %d", L_02);

  if (wiringPiSetup () == -1)
    return 1 ;

  // Define pinos OUTPUT
  pinMode (L_01, OUTPUT) ; //L01 1
  pinMode (L_02, OUTPUT) ; //L02 2
  pinMode (AC, OUTPUT) ; //Ar
  pinMode (PR, OUTPUT) ; //Projetor   
  pinMode (AL_BZ, OUTPUT) ; //Alarme       
 
  //Define sensores INPUT
  pullUpDnControl(SPres, PUD_DOWN);
  pullUpDnControl(SFum, PUD_DOWN);

  //int a = piThreadCreate(turnOnAll); 

    stateSPres = digitalRead(stateSPres);

    wiringPiISR(SPres, INT_EDGE_BOTH, &handle);
    // printf("%d", stateSPres);

    turnOnLamps();
    delay(500);
    turnOffLamps();

    turnOnAll();
    delay(500);
    turnOffAll();

    // for (;;) {
    //   printf("%d", stateSPres);
		//   sleep(0.5);
	  // }
  // printf("ola");

  // init_socket_teste(argc, &argv[]);

  
    // read_dht_data(SC_TEMP);

  return 0 ;
}