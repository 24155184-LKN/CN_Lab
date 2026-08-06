#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUF_SIZE 1024
#define SERVER_IP "127.0.0.1"   // Change to server's IP if running on another machine

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    char message[BUF_SIZE];

    // 1. Create TCP socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Specify server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Connect to server
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // 4. Get message from user and send it
    printf("Enter message to send to server: ");
    fgets(message, BUF_SIZE, stdin);
    message[strcspn(message, "\n")] = '\0';  // strip newline

    send(sock_fd, message, strlen(message), 0);
    printf("Message sent to server.\n");

    // 5. Receive echoed message from server
    int bytes_received = recv(sock_fd, buffer, BUF_SIZE - 1, 0);
    if (bytes_received < 0) {
        perror("Receive failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';

    // Display echoed message
    printf("Message received back from server: %s\n", buffer);

    // 6. Cleanup
    close(sock_fd);

    return 0;
}
