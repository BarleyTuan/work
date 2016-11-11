OBJ=$(wildcard *.o)

all:$(PWD_DIR)/client $(PWD_DIR)/server

$(PWD_DIR)/client:$(OBJ) $(PWD_DIR)/echo_client.o
	$(CC) -o $@ $^ $(CFLAG)

$(PWD_DIR)/server:$(OBJ) $(PWD_DIR)/echo_server.o
	$(CC) -o $@ $^ $(CFLAG)

##$(PWD_DIR)/main:$(OBJ) $(PWD_DIR)/echo_server.o
##	$(CC) -o $@ $^ $(CFLAG)

