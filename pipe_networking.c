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
    char pp[64];
    read(wkp, pp, sizeof(pp));
    printf("[*] Received: %s\n", pp);

    int private = open(pp, O_WRONLY);
    char *message = "syn-ack";
    write(private, message, sizeof(message));
    close(private);

    char buf[MESSAGE_BUFFER_SIZE];
    private = open(pp, O_RDONLY);
    read(private, buf, sizeof(buf));
    printf("[*] Received: %s\n", buf);
    close(private);

    printf("Opening %s\n", pp);
    int to_client = open(pp, O_WRONLY);

    *from_client = wkp;
    remove(pp);

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

    printf("Opening %s\n", buf);
    int from_server = open(buf, O_RDONLY);
    *to_server = wkp;
    remove(WKP);

    return from_server;
}
