#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include "echo_io.h"

#define ECHO_PORT 7
#define MAX_INPUT_LENGTH 1024
#define LISTEN_BACKLOG_SIZE 1024

int port_number(int argc, char *argv[]) {
	short int port = -1;
	char      *endptr;

    if ( argc == 2 ) {
		port = strtol(argv[1], &endptr, 0);
		if ( *endptr ) { /* strtol stops at the first non-digit */
			port = 0;
		}
    }
    else if ( argc < 2 ) {
		port = ECHO_PORT;
    }

	return port;
}

void log_message(char *str) {
	printf("[DEBUG]: %s\n", str);
	fflush(stdout);
}

int main(int argc, char *argv[]) {
	short int port;
	int       listening_socket;
	int       client_socket;
	struct    sockaddr_in server_address;
	char      buffer[MAX_INPUT_LENGTH];

	/* 
		Allow default echo port to be overridden at the command line 
	*/
	port = port_number(argc, argv);
	if (port == 0) {
		fprintf(stderr, "ECHO SERVER: Invalid port number.\n");
		exit(EXIT_FAILURE);
	}
	else if (port < 0) {
		fprintf(stderr, "ECHO SERVER: Invalid arguments.\n");
		exit(EXIT_FAILURE);
	}

	/* Yeah, I got lazy here. */
	printf("[DEBUG]: I will listen on port %d\n", port);
	fflush(stdout);

	/*  Create the listening socket  */
	listening_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (listening_socket < 0) {
		fprintf(stderr, "ECHO SERVER: Failed to create socket.\n");
		exit(EXIT_FAILURE);
	}

	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family      = PF_INET;
	server_address.sin_port        = htons(port);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listening_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0 ) {
		fprintf(stderr, "ECHO SERVER: Failed to bind socket (%d).\n", errno);
		exit(EXIT_FAILURE);
	}

	if (listen(listening_socket, LISTEN_BACKLOG_SIZE) < 0) {
		fprintf(stderr, "ECHO SERVER: Listen failed\n");
		exit(EXIT_FAILURE);
	}

	while ( 1 ) { 
		client_socket = accept(listening_socket, NULL, NULL);
		if (client_socket < 0) {
			fprintf(stderr, "ECHO SERVER: Failed to accept client.\n");
			exit(EXIT_FAILURE);
		}

		log_message("Client connected!");


		read_client_input(client_socket, buffer, MAX_INPUT_LENGTH-1);

		log_message("Client says,");
		log_message(buffer);

		echo_to_client(client_socket, buffer, strlen(buffer));

		if (close(client_socket) < 0) {
			fprintf(stderr, "ECHO SERVER: Failed to close client socket.\n");
			exit(EXIT_FAILURE);
		}

		log_message("Client disconnected.");
	}
}
