#define MESSAGE_BUFFER_SIZE 1024
#define HANDSHAKE_BUFFER_SIZE 1024
#define WKP "wkp"

int client_handshake(int *to_client);
int server_handshake(int *from_client);

int server_handshake1(char *buf);
int server_handshake2(char *buf, int from_client);
