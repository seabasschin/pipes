#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int server_handshake(int *from_client) {
	mkfifo("mario", 0644);
	int wkp = open("mario", O_RDONLY);
	char buf[128];
	read(wkp, buf, sizeof(buf));
	int private = open(buf, O_WRONLY);	
	char *message = "Hello";
	write(private, message, strlen(message));

	return wkp;
}

int client_handshake(int *to_server) {
	int pid = getpid();
	char buf[64];
	sprintf(buf, "%d", pid);
	mkfifo(buf, 0644);
	
	int wkp = open("mario", O_WRONLY);
	*to_server = wkp;
	write(wkp, buf, sizeof(buf));
	int private = open(buf, O_RDONLY);
	char received[MESSAGE_BUFFER_SIZE];
	read(private, received, sizeof(received));
	remove(buf);`
}
