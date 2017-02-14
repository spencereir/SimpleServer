CC=g++
CFLAGS=-I.
DEPS = server.h socket.h
LIBS=-lws2_32

all: server client
	
server: server.o main.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

client: client.o socket.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
