CC=gcc
CFLAGS=-g
CLIBS=
OBJ_DIR=obj/$(shell uname -m)
OBJS=$(OBJ_DIR)/dijkstra.o $(OBJ_DIR)/graph.o $(OBJ_DIR)/linked_list.o
PORT=4242

all: server

# libraries

server: server.c  
	$(CC) -o server $^ $(CFLAGS) $(OBJS) $(CLIBS)

server_memcheck: server 
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./server $(PORT)

clean:
	rm -f server *.o
