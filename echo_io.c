#include "echo_io.h"

ssize_t read_client_input(int socket, void *buffer, size_t max_length) {
    ssize_t n, read_character;
    char    c, *buffer_ptr;

    buffer_ptr = buffer;

    for ( n = 1; n < max_length; n++ ) {
		read_character = read(socket, &c, 1);
		if (read_character == 1) {
			*buffer_ptr = c;
			buffer_ptr++;
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
	
	*buffer_ptr = 0;
	return 0;
}

ssize_t echo_to_client(int socket, const void *buffer, size_t max_length) {
	const char *buffer_ptr;
	size_t n;
	ssize_t n_written;

	buffer_ptr = buffer;
	n = max_length;

	while (n > 0) {
		n_written = write(socket, buffer_ptr, n);
		if (n_written <= 0) {
			/* If we were interrupted, continue.  Otherwise, return an error. */
			if ( errno == EINTR ) {
				n_written = 0;
			}
			else {
				return -1;
			}
		}
		/* Account for what we've written, and shift the pointer. */
		n -= n_written;
		buffer_ptr += n_written;
	}

	return n_written;
}
