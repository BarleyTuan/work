.PHONY:all clean
##
PWD_DIR=$(shell pwd)
OBJ_DIR=$(PWD_DIR)/obj
##
CC=g++
CFLAG=-g -fno-inline-functions -rdynamic -D_DEBUG -pipe -Werror -pthread -lprotobuf -lpthread
## CFLAG=-Wall -g `pkg-config --cflags --libs protobuf`
##
export PWD_DIR BASE_DIR OBJ_DIR
export CC CFLAG

all:$(OBJ_DIR)/echo.pb.o \
	$(OBJ_DIR)/easy_epoll.o \
	$(OBJ_DIR)/protobuf_service.o \
	$(OBJ_DIR)/protobuf_channel.o \
	echo_client.o \
	echo_server.o

$(OBJ_DIR)/echo.pb.o:echo.pb.cc
	$(CC) -c $^ -o $@ $(CFLAG)
$(OBJ_DIR)/easy_epoll.o:easy_epoll.cc
	$(CC) -c $^ -o $@ $(CFLAG)
$(OBJ_DIR)/protobuf_service.o:protobuf_service.cc
	$(CC) -c $^ -o $@ $(CFLAG)
$(OBJ_DIR)/protobuf_channel.o:protobuf_channel.cc
	$(CC) -c $^ -o $@ $(CFLAG)
echo_client.o:echo_client.cc
	$(CC) -c $^ -o $@ $(CFLAG)
echo_server.o:echo_server.cc
	$(CC) -c $^ -o $@ $(CFLAG)

all:
	make -C $(OBJ_DIR)

##
clean:
	$(RM) -rf $(OBJ_DIR)/*o
	$(RM) -rf *.o
	$(RM) client server main
