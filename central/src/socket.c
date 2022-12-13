#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "server_config.c"

int socketCentral;
int socketDistributed;
struct sockaddr_in centralAddr;
struct sockaddr_in distributedAddr;

void TrataClienteTCP(int socketDistributed) {
	char buffer[16];
	int tamanhoRecebido;

	if((tamanhoRecebido = recv(socketDistributed, buffer, 16, 0)) < 0)
		printf("Erro no recv()\n");

	while (tamanhoRecebido > 0) {
		if(send(socketDistributed, buffer, tamanhoRecebido, 0) != tamanhoRecebido)
			printf("Erro no envio - send()\n");
		
		if((tamanhoRecebido = recv(socketDistributed, buffer, 16, 0)) < 0)
			printf("Erro no recv()\n");
	}
}

void function(){
  while(1) {
    distributedLength = sizeof(distributedAddr);
    if((socketDistributed = accept(socketCentral, 
			                      (struct sockaddr *) &distributedAddr, 
			                      &distributedLength)) < 0)
			    printf("Falha no Accept\n");
    printf("Conexão do Cliente %s\n", inet_ntoa(clienteAddr.sin_addr));
		
		TrataClienteTCP(socketDistributed);
		close(socketDistributed);
  }
}

void open_socket_central (int argc, char *argv[]) {
    unsigned short porta_servidor_central;
    unsigned int distributedLength;

    pthread_t thread1_id;
    pthread_t thread2_id;

    pthread_create (&thread1_id, NULL, &function, &thread1_args);
    pthread_create (&thread2_id, NULL, &function, &thread2_args);

	  porta_servidor_central = atoi(argv[1]);

    if((socketCentral = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		  printf("O socket do servidor central falhou\n");

    memset(&centralAddr, 0, sizeof(centralAddr)); 
	  centralAddr.sin_family = AF_INET;
	  centralAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	  centralAddr.sin_port = htons(porta_servidor_central);

    if(bind(socketCentral, (struct sockaddr *) &centralAddr, sizeof(centralAddr)) < 0)
		printf("Falha no Bind\n");

    if(listen(servidorSocket, 1000) < 0)
		printf("Falha no Listen\n");	

    close(socketCentral);
}