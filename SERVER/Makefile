PORT=40002
CFLAGS = -DSERVER_PORT=$(PORT) -g -Wall -Werror -fsanitize=address

all: server

server: main.o socket.o client.o
	gcc ${CFLAGS} -o $@ $^

%.o: %.c
	gcc ${CFLAGS} -c $<

clean:
	rm -f *.o server
