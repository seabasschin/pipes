all:
	gcc client.c pipe_networking.c -o client
	gcc fserver.c pipe_networking.c -o server

clean:
	rm client server
