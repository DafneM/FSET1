#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void open_distributed_client_socket() {
	int clienteSocket;
	struct sockaddr_in servidorAddr;
	unsigned short servidorPorta;
	char *IP_Servidor;
	char *mensagem;
	char buffer[2048];
	unsigned int tamanhoMensagem;

	int bytesRecebidos;
	int totalBytesRecebidos;

	IP_Servidor = "164.41.98.26";
	servidorPorta = 10732;
	mensagem = "oi";

	// Criar Socket
	if((clienteSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		printf("Erro no socket()\n");

	// Construir struct sockaddr_in
	memset(&servidorAddr, 0, sizeof(servidorAddr)); // Zerando a estrutura de dados
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = inet_addr(IP_Servidor);
	servidorAddr.sin_port = htons(servidorPorta);

	// Connect
	if(connect(clienteSocket, (struct sockaddr *) &servidorAddr, 
							sizeof(servidorAddr)) < 0)
		printf("Erro no connect()\n");

	memset(buffer, 0, 2048);

	if(recv(clienteSocket, buffer, 9, MSG_WAITALL) < 0) {
		printf("erro");
	}
	else{
		printf("%s", buffer);
	}

	send(clienteSocket, buffer, 9, 0);

	close(clienteSocket);
	exit(0);
}
