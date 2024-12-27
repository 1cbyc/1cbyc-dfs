#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define FILENAME "uploaded_file.txt"

void handle_client(int client_sock) {
    char buffer[1024];
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        perror("Failed to open file");
        close(client_sock);
        return;
    }
    // int bytes_received;

    ssize_t bytes_received

    // to receive data from client side
    while ((bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("Received: %s\n", buffer);
        
        // easily respond to the client (for now, echoing the message back)
        send(client_sock, buffer, bytes_received, 0);
    }

    if (bytes_received == 0) {
        printf("Client disconnected\n");
    } else {
        perror("recv failed");
    }

    close(client_sock);
}

int main() {
    int server_fd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // will create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // to bind the socket to the port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // to listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // to accept incoming connections and handle clients
    while ((client_sock = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) >= 0) {
        printf("New connection accepted\n");
        handle_client(client_sock);
    }

    if (client_sock < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    close(server_fd);
    return 0;
}
