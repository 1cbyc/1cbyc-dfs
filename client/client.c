#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080

int upload_file(int sock, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("File not found");
        return -1;
    }

    char buffer[1024];
    size_t n;

    // to send the "UPLOAD" command to the server
    send(sock, "UPLOAD", 6, 0);

    // to send file data to the server
    while ((n = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(sock, buffer, n, 0) == -1) {
            perror("Send failed");
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    printf("File uploaded successfully.\n");
    return 0;
}

int download_file(int sock, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }

    char buffer[1024];
    ssize_t bytes_received;

    // to send the "DOWNLOAD" command to the server
    send(sock, "DOWNLOAD", 8, 0);

    // then to receive the file data from the server
    while ((bytes_received = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, bytes_received, file);
    }

    if (bytes_received == 0) {
        printf("File download complete.\n");
    } else {
        perror("Recv failed");
    }

    fclose(file);
    return 0;
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char message[1024];

    // creating socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    // connecting to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at %s:%d\n", SERVER_ADDR, SERVER_PORT);

    // choosing between upload or download
    int choice;
    printf("Enter 1 to upload a file, 2 to download a file: ");
    scanf("%d", &choice);
    getchar(); // consuming newline

    if (choice == 1) {
        if (upload_file(sock, "sample.txt") == -1) {
            printf("File upload failed\n");
        }
    } else if (choice == 2) {
        if (download_file(sock, "downloaded_sample.txt") == -1) {
            printf("File download failed\n");
        }
    } else {
        printf("Invalid choice\n");
    }

    close(sock);
    return 0;
}
