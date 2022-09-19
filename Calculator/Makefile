SRC=./src
MAIN=./src/main/
HEADER=./src/headers/
TARGET=./src/target/
GCC = gcc
GDB = gdb
CFLAGS = -o $(TARGET)main.out
GFLAGS = -g
INCLUDEFLAG = -I ./headers
ARG1 = 50
ARG2 = -
ARG3 = 20
RM = rm -rf

all: clean default test

default: build calculator

test: calculator
	$(TARGET)main.out $(ARG1) $(ARG2) $(ARG3)

build: Add.o Multiply.o Devide.o Substract.o Main.o

clean: 
	$(RM) $(TARGET)*.o $(TARGET)*.out
	
Add.o: $(MAIN)Add.c $(HEADER)add.h
	$(GCC) ${INCLUDEFLAG} -c $(MAIN)Add.c -o $(TARGET)Add.o -g

Multiply.o: $(MAIN)Multiply.c $(HEADER)multiply.h
	$(GCC) ${INCLUDEFLAG} -c $(MAIN)Multiply.c -o $(TARGET)Multiply.o -g

Substract.o: $(MAIN)Substract.c $(HEADER)substract.h
	$(GCC) ${INCLUDEFLAG} -c $(MAIN)Substract.c -o $(TARGET)Substract.o -g

Devide.o: $(MAIN)Devide.c $(HEADER)devide.h
	$(GCC) ${INCLUDEFLAG} -c $(MAIN)Devide.c -o $(TARGET)Devide.o -g

Main.o: $(MAIN)Main.c $(HEADER)add.h $(HEADER)multiply.h $(HEADER)devide.h $(HEADER)substract.h
	$(GCC) ${INCLUDEFLAG} -c $(MAIN)Main.c -o $(TARGET)Main.o -g

calculator: $(TARGET)Main.o $(TARGET)Add.o $(TARGET)Multiply.o $(TARGET)Substract.o $(TARGET)Devide.o
	$(GCC) $(TARGET)Main.o $(TARGET)Add.o $(TARGET)Multiply.o $(TARGET)Substract.o $(TARGET)Devide.o $(CFLAGS) -g