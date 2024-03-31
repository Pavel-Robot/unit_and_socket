// opt/webapp/app.bin
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8888
#define IP_ADR "127.0.0.1"
#define LOCALHOST inet_addr(IP_ADR);


int main(int argc, char const* argv[])
{
    int server_fd;
    int client_sc; 

	ssize_t valread;
	struct sockaddr_in address;
	int opt = 1;
	socklen_t addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	char* status = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nStatus is running\n";
    char* status_notfing = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nNothing!\n";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8888
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = LOCALHOST;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

    printf("Server is running on ip: %s on port: %d\n", IP_ADR, PORT);
	while(1){
	if ((client_sc = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	valread = read(client_sc, buffer,
				1024 - 1); // subtract 1 for the null
							// terminator at the end
    
	printf("%s\n", buffer);

    char buf[9]; // Ищем подстроку в строке
    for(int i=0; i < 9; i++) buf[i] = buffer[i+5]; buf[8]='\0';
    if(strcmp(buf, "status H") == 0) send(client_sc, status, strlen(status), 0);
	else send(client_sc, status_notfing, strlen(status_notfing), 0);
	printf("Status message be send\n");

	// closing the connected socket
	close(client_sc);
	}
	// closing the listening socket
	close(server_fd);
	return 0;
}
