#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "jsonParser/read_message.c"

int servidorSocket;
int socketCliente;
struct sockaddr_in servidorAddr;
struct sockaddr_in clienteAddr;
unsigned short servidorPorta;
unsigned int clienteLength;

void *TrataClienteTCP(void *arg) {
	char *buffer = malloc(2048);;
	int tamanhoRecebido;

	// while(1){
		tamanhoRecebido = recv(socketCliente, buffer, 2048, 0);
	// 		printf("Erro no recv()\n");
		// sleep(1);
	// }

	// buffer = read_json_message();
	// printf("%s", buffer);

	while (tamanhoRecebido > 0) {
	if(send(socketCliente, buffer, tamanhoRecebido, 0) != tamanhoRecebido)
		printf("Erro no envio - send()\n");
	
	if((tamanhoRecebido = recv(socketCliente, buffer, 2048, 0)) < 0)
		printf("2 Erro no recv()\n");
	}
}

void open_distributed_server_socket() {
	pthread_t a_thread;

	printf("aqui");

	servidorPorta = 10731;

	printf("servidor rodando na porta %d", servidorPorta);

	// Abrir Socket
	if((servidorSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		printf("falha no socker do Servidor\n");

	// Montar a estrutura sockaddr_in
	memset(&servidorAddr, 0, sizeof(servidorAddr)); // Zerando a estrutura de dados
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servidorAddr.sin_port = htons(servidorPorta);

	// Bind
	if(bind(servidorSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0)
		printf("Falha no Bind\n");

	// Listen
	if(listen(servidorSocket, 10) < 0)
		printf("Falha no Listen\n");		

	while(1) {
		clienteLength = sizeof(clienteAddr);
		if((socketCliente = accept(servidorSocket, 
			                      (struct sockaddr *) &clienteAddr, 
			                      &clienteLength)) < 0)
			printf("Falha no Accept\n");
		
		printf("Conexão do Cliente %s\n", inet_ntoa(clienteAddr.sin_addr));
		
		pthread_create(&a_thread, NULL, TrataClienteTCP, NULL);

	}
	close(servidorSocket);

}
