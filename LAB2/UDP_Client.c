#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 4952
#define MAXBUFLEN 200

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len = sizeof(server_addr);
    const char *message = "Hello, UDP Server!";

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET; // host byte order
    server_addr.sin_port = htons(SERVER_PORT); // short, network byte order
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr); // convert IP address

    // Send a message to the server
    if (sendto(sockfd, message, strlen(message), 0,
               (struct sockaddr *)&server_addr, addr_len) == -1) {
        perror("sendto");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Sent message: \"%s\"\n", message);

    // Optionally receive a response from the server
    ssize_t num_bytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0,
                  (struct sockaddr *)&server_addr, &addr_len);
    if (num_bytes == -1) {
        perror("recvfrom");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    buf[num_bytes] = '\0'; // Null-terminate the received data
    printf("Received response: \"%s\"\n", buf);

    // Close the socket
    close(sockfd);
    return 0;
}
