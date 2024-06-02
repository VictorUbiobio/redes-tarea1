CC = g++
CFLAGS = -std=c++11

all: servidor cliente

servidor: servidor.cpp
    $(CC) $(CFLAGS) -o servidor servidor.cpp

cliente: cliente.cpp
    $(CC) $(CFLAGS) -o cliente cliente.cpp

clean:
    rm -f servidor cliente
