GCC = gcc
ARG1 = 7080
ARG2 = 127.0.0.1
RM = rm -rf

all: clean default

default: build

test-server: server
	./server.out $(ARG1) 

test-client: client
	./client.out $(ARG2) $(ARG1)

build: server client

clean: 
	$(RM) *.out
	
server: server.c
	$(GCC) -pthread -lnsl server.c -o server.out -g


client: $(MAIN)client.c
	gcc -lnsl client.c -o client.out -g