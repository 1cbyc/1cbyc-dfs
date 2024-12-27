#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 10

void send_file(int client_sock, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("File not found");
        return;
    }

    char buffer[1024];
    size_t bytes_read;

    // to read file and send to the client
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(client_sock, buffer, bytes_read, 0) == -1) {
            perror("Send failed");
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("File sent successfully.\n");
}

void receive_file(int client_sock) {
    char filename[1024];
    FILE *file;

    // to receive the filename from the client
    ssize_t bytes_received = recv(client_sock, filename, sizeof(filename), 0);
    if (bytes_received <= 0) {
        perror("Failed to receive filename");
        close(client_sock);
        return;
    }

    filename[bytes_received] = '\0'; // this will null-terminate the filename
    printf("Client requested file: %s\n", filename);

    // then send the requested file to the client
    send_file(client_sock, filename);
}

void handle_client(int client_sock) {
    char buffer[1024];
    ssize_t bytes_received;

    // to receive command from client
    bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
    if (bytes_received <= 0) {
        perror("Failed to receive command");
        close(client_sock);
        return;
    }

    buffer[bytes_received] = '\0';

    // now handle upload or download request
    if (strcmp(buffer, "UPLOAD") == 0) {
        receive_file(client_sock);  // this will receive and save the file from the client
    } else if (strcmp(buffer, "DOWNLOAD") == 0) {
        send_file(client_sock, "sample.txt");  // will be able to send a specific file to the client
    } else {
        printf("Invalid command received.\n");
    }

    close(client_sock);
}

int main() {
    int server_fd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // creating server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // binding the socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // then listening for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // accepting incoming connections and handle clients
    while ((client_sock = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) >= 0) {
        printf("New connection accepted\n");
        handle_client(client_sock);
    }

    if (client_sock < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    close(server_fd);
    return 0;
}
