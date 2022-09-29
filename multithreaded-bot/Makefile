SRC=./src
MAIN=./src/main/
HEADER=./src/headers/
TARGET=./src/target/
SQLITE=./src/sqlite/
GCC = gcc
ARG1 = 6070
ARG2 = 127.0.0.1
RM = rm -rf

CFLAGS = -o $(TARGET)sqlitefinal.out
OFLAGS = -o $(TARGET)servermain.out
SOURCE= Sqlite/sqlite3.c
LDFLAGS=-lpthread -ldl
#INCLUDES=/Sqlite
PROGRAM=sqlite.out

all: clean default

default: build

test-server:
	$(TARGET)servermain.out $(ARG1) 

test-client:
	$(TARGET)client.o $(ARG2) $(ARG1)

build: sqlitedb.o server.o client.o servermain 

build_db: sqlitedb.o servermain

test-db:
	$(TARGET)sqlitefinal.out 

clean: 
	$(RM) $(TARGET)*.o $(TARGET)*.out
	
server.o: $(MAIN)server.c
	$(GCC) -pthread -lnsl -c $(MAIN)server.c -o $(TARGET)server.o -g


client.o: $(MAIN)client.c
	gcc -lpthread -lnsl $(MAIN)client.c -o $(TARGET)client.o -g

sqlitedb.o: $(MAIN)sqlite.c $(SQLITE)sqlite3.h
	$(GCC) -lpthread -ldl -c $(MAIN)sqlite.c -o $(TARGET)sqlite.o -g

sqlitefinal: sqlite.o $(SQLITE)sqlite3.o 
	$(GCC) -lpthread -ldl sqlite.o $(SQLITE)sqlite3.o $(CFLAGS) -g

servermain: $(TARGET)server.o $(SQLITE)sqlite3.o $(TARGET)sqlite.o
	$(GCC) -lpthread -ldl $(TARGET)server.o $(TARGET)sqlite.o $(SQLITE)sqlite3.o  $(OFLAGS) -g