#ifndef ECHO_IO

#define ECHO_IO
#include <unistd.h>
#include <errno.h>

ssize_t read_client_input(int socket, void *buffer_ptr, size_t max_length);
ssize_t echo_to_client(int socket, const void *buffer_ptr, size_t max_length);


#endif
