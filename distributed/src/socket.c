// #include <stdio.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>

// void open_socket_distributed () {
// 	int distributedSocket;
// 	struct sockaddr_in centralAddr;
// 	unsigned short porta_servidor_central;
// 	char *IP_Central;
// 	char *mensagem;
// 	char buffer[16];
// 	unsigned int tamanhoMensagem;

//     IP_Central = argv[1];
// 	porta_servidor_central = atoi(argv[2]);
// 	mensagem = argv[3];

//     if((distributedSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
// 		printf("Erro no socket()\n");

//     memset(&centralAddr, 0, sizeof(centralAddr)); // Zerando a estrutura de dados
// 	centralAddr.sin_family = AF_INET;
// 	centralAddr.sin_addr.s_addr = inet_addr(IP_Central);
// 	centralAddr.sin_port = htons(porta_servidor_central);

//     	// Connect
// 	if(connect(clienteSocket, (struct sockaddr *) &servidorAddr, 
// 							sizeof(servidorAddr)) < 0)
// 		printf("Erro no connect()\n");

// 	tamanhoMensagem = strlen(mensagem);

// 	if(send(clienteSocket, mensagem, tamanhoMensagem, 0) != tamanhoMensagem)
// 		printf("Erro no envio: numero de bytes enviados diferente do esperado\n");

// 	totalBytesRecebidos = 0;
// 	while(totalBytesRecebidos < tamanhoMensagem) {
// 		if((bytesRecebidos = recv(clienteSocket, buffer, 16-1, 0)) <= 0)
// 			printf("Não recebeu o total de bytes enviados\n");
// 		totalBytesRecebidos += bytesRecebidos;
// 		buffer[bytesRecebidos] = '\0';
// 		printf("%s\n", buffer);
// 	}
//     close(clienteSocket);
// 	exit(0);
// }




