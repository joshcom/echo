P=server
CFLAGS= -g -Wall -03
CC=gcc
LIB_DIR=lib
BIN_DIR=bin
SRC_DIR=src
INCLUDE_FLAGS= -I include
LIB_FLAGS= -L lib

OBJS=echo_io.o
OBJECTS = $(patsubst %,$(LIB_DIR)/%,$(OBJS))

$(P): $(OBJECTS) directories
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $(LIB_FLAGS) -o $(BIN_DIR)/$@ $(SRC_DIR)/server.c $(OBJECTS)

lib/echo_io.o: src/echo_io.c
	$(CC) $(INCLUDE_FLAGS) -c $< -o $@

$(LIB_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(INCLUDE_FLAGS) -c $< -o $@

directories:
	mkdir -p $(BIN_DIR) $(LIB_DIR)

clean:
	rm $(BIN_DIR)/$(P) $(OBJECTS)
