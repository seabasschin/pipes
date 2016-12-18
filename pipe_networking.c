#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

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

    char buf[MESSAGE_BUFFER_SIZE];
    read(wkp, buf, sizeof(buf));
    printf("[*] Received: %s\n", buf);

    *from_client = wkp;
    remove(pp);

    return private;
}

int client_handshake(int *to_server) {
    // Create the private FIFO
    int pid = getpid();
    char buf[64];
    sprintf(buf, "%d", pid);
    mkfifo(buf, 0644);
    printf("[*] Connecting to server\n");

    // Open the well-known-pipe and send the name of the private fifo over
    int wkp = open(WKP, O_WRONLY);
    write(wkp, buf, sizeof(buf));

    // Read the acknowledgement message from the client and remove the private fifo
    int private = open(buf, O_RDONLY);
    char received[MESSAGE_BUFFER_SIZE];
    read(private, received, sizeof(received));
    printf("[*] Received: %s\n", received);
    if (remove(buf) == -1) {
        printf("Error: %s\n", strerror(errno));
    }

    // Send back an acknowledgement message to the client
    char *message = "ack";
    write(wkp, message, sizeof(message));
    printf("[*] Connection established\n");

    *to_server = wkp;
    return private;
}

int server_handshake1(char *buf) {
    // Create the well-known-pipe
    mkfifo(WKP, 0644);
    printf("[*] Waiting for client connection\n");

    // Read the name of the private fifo and remove it
    int wkp = open(WKP, O_RDONLY);
    read(wkp, buf, sizeof(buf));
    printf("[*] Received: %s\n", buf);
    if (remove(WKP) == -1) {
        printf("Error: %s\n", strerror(errno));
    }

    return wkp;
}

int server_handshake2(char *buf, int from_client) {
    // Open the private fifo and send a synchronization message
    int private = open(buf, O_WRONLY);
    char *message = "syn-ack";
    write(private, message, sizeof(message));

    // Read the acknowledgement message from the client
    char received[MESSAGE_BUFFER_SIZE];
    /* read(from_client, received, sizeof(received)); // hack */
    read(from_client, received, sizeof(received));
    printf("[*] Receieved: %s\n", received);

    return private;
}
