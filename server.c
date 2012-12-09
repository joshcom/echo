#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

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

int main(int argc, char *argv[]) {

    short int port;
	int       listening_socket;
	struct    sockaddr_in server_address;

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

	printf("[DEBUG]: PORT IS: %d", port);

    /*  Create the listening socket  */
	if (listening_socket = socket(PF_INET6, SOCK_STREAM, 0) == -1) {
		fprintf(stderr, "ECHO SERVER: Invalid arguments.\n");
		exit(EXIT_FAILURE);
	}

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family      = PF_INET6;
    server_address.sin_port        = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	/*  Bind our socket addresss to the 
	    listening socket, and call listen()  */
	if (bind(listening_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0 ) {
		fprintf(stderr, "ECHO SERVER: Failed to bind socket (%d).\n", errno);
		exit(EXIT_FAILURE);
	}

    if (listen(listening_socket, LISTEN_BACKLOG_SIZE) < 0) {
		fprintf(stderr, "ECHO SERVER: Listen failed\n");
		exit(EXIT_FAILURE);
    }

/*    while ( 1 ) { */
		/*  Wait for a connection, then accept() it  */

		/*  Retrieve an input line from the connected socket
			then simply write it back to the same socket.     */

		/*  Close the connected socket  */
/*    } */

}
