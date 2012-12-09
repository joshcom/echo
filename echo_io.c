#include "echo_io.h"

ssize_t read_client_input(int socket, void *buffer_ptr, size_t max_length) {
    ssize_t n, read_character;
    char    c, *buffer;

    buffer = buffer_ptr;

    for ( n = 1; n < max_length; n++ ) {
		read_character = read(socket, &c, 1);
		if (read_character == 1) {
			*buffer = c;
			buffer++;
			if (c == '\n') {
				break;
			}
		}
		else if (read_character == 0) {
			/* If n == 1, we haven't ready anything. 
			   Return a size of 0. */
			if (n == 1) {
				return 0;
			}
			else {
				break;
			}
		}
		else {
			/* If we were interrupted, continue.  Otherwise, return an error. */
			if ( errno == EINTR )
				continue;
			return -1;
		}
	}
	
	*buffer = 0;
	return 0;
}

ssize_t echo_to_client(int socket, const void *buffer_ptr, size_t max_length) {
	return 0;
}
