#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080
#define FILENAME "sample.txt" // in my quest to add file upload

int download_file(int sock, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }

    char buffer[1024];
    ssize_t bytes_received;
    

}

int send_file(int sock, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("File not found");
        return -1;
    }

    char buffer[1024];
    size_t n;

    // want to send file data to the server
    while ((n fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(sock, buffer, n, 0) == -1) {
            perror("Send failed");
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    printf("File sent successfully.\n");
    return 0;
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char message[1024];

    // will create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    // to connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at %s:%d\n", SERVER_ADDR, SERVER_PORT);

    // will send  a message to the server
    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);

    send(sock, message, strlen(message), 0);
    
    // to send file to the server
    if (send_file(sock, FILENAME) == -1) {
        printf("File upload failed\n");
    }

    // making sure to receiving response from the server
    // int bytes_received = recv(sock, message, sizeof(message) - 1, 0);
    // message[bytes_received] = '\0';
    // printf("Server response: %s\n", message);

    close(sock);
    return 0;
}
