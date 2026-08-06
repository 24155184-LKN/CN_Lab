/*
1. Write a C program
- Create an UDP socket
- Client - Send an integer number to the server
- Server - Reverse the number
	 - Send it back to the Client
*/

//Client_part

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8081
#define MAXBUFLEN 200

int main() {
    int sockfd;
    struct sockaddr_in my_addr, their_addr;
    socklen_t addr_len;
    int numbytes;
    char buf[MAXBUFLEN];

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);
    addr_len = sizeof their_addr;

    // Send an integer number to the server from user input
    int number;
    printf("Enter an integer number to send to the server: ");
    scanf("%d", &number);

    char msg[32];
    snprintf(msg, sizeof msg, "%d", number);

    if ((numbytes = sendto(sockfd, msg, strlen(msg), 0,
        (struct sockaddr *)&their_addr, addr_len)) == -1) {
        perror("sendto");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Sent %d bytes to server\n", numbytes);

    // Receive the reversed number from the server
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Null-terminate the received data and print it
    buf[numbytes] = '\0';
    int reversed_number;
    memcpy(&reversed_number, buf, sizeof(reversed_number));
    printf("Received reversed number: %d\n", reversed_number);

    // Close the socket
    close(sockfd);
    return 0;
}
