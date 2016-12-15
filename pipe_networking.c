#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "pipe_networking.h"

int server_handshake(int *from_client) {
    mkfifo(WKP, 0644);
    printf("[*] Waiting for client connection\n");

    int wkp = open(WKP, O_RDONLY);
    char buf[MESSAGE_BUFFER_SIZE];
    read(wkp, buf, sizeof(buf));
    printf("[*] Received: %s\n", buf);

    int private = open(buf, O_WRONLY);
    char *message = "syn-ack";
    write(private, message, sizeof(message));
    close(private);

    private = open(buf, O_RDONLY);
    read(private, buf, sizeof(buf));
    printf("[*] Processing: %s\n", buf);

    mkfifo("p1", 0644);
    mkfifo("p2", 0644);
    int to_client = open("p1", O_WRONLY);
    *from_client = open("p2", O_RDONLY);

    return to_client;
}

int client_handshake(int *to_server) {
    int pid = getpid();
    char buf[64];
    sprintf(buf, "%d", pid);
    mkfifo(buf, 0644);
    printf("[*] Connecting to server\n");

    int wkp = open(WKP, O_WRONLY);
    *to_server = wkp;
    write(wkp, buf, sizeof(buf));

    int private = open(buf, O_RDONLY);
    char received[MESSAGE_BUFFER_SIZE];
    read(private, received, sizeof(received));
    printf("[*] Received: %s\n", received);
    close(private);

    char *message = "ack";
    private = open(buf, O_WRONLY);
    write(private, message, sizeof(message));

    remove(buf);
    remove(WKP);

    close(wkp);

    mkfifo("p1", 0644);
    mkfifo("p2", 0644);
    int from_server = open("p1", O_RDONLY);
    *to_server = open("p2", O_WRONLY);

    return from_server;
}
