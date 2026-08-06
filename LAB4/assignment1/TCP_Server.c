#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUF_SIZE];

    // 1. Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Allow quick reuse of address (avoids "Address already in use")
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. Bind
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Listen
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // 4. Accept a connection
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Get client IP
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

    // 5. Receive message from client
    int bytes_received = recv(client_fd, buffer, BUF_SIZE - 1, 0);
    if (bytes_received < 0) {
        perror("Receive failed");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';

    // Display message and client IP
    printf("\nMessage from client: %s\n", buffer);
    printf("Client IP address  : %s\n", client_ip);
    printf("Client port         : %d\n", ntohs(client_addr.sin_port));

    // 6. Send the same message back to client
    send(client_fd, buffer, bytes_received, 0);
    printf("Message echoed back to client.\n");

    // 7. Cleanup
    close(client_fd);
    close(server_fd);

    return 0;
}
