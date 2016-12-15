#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pipe_networking.h"

void process(char *s) {
    int len = strlen(s);
    int i, j;
    char tmp;
    for (i = 0, j = len-1; i < len/2; ++i, --j) {
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }
}

int main() {

    int to_client, from_client;
    char buffer[MESSAGE_BUFFER_SIZE];

    to_client = server_handshake(&from_client);

    while (1) {
        read(from_client, buffer, sizeof(buffer));
        process(buffer);
        write(to_client, buffer, sizeof(buffer));
    }

    return 0;
}
