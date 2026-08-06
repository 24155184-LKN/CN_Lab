/*
1. Write a C program
- Create an UDP socket
- Client - Send an integer number to the server
- Server - Reverse the number
	     - Send it back to the Client
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
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

    my_addr.sin_family = AF_INET; // host byte order
    my_addr.sin_port = htons(PORT); // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero); // zero the rest of the struct

    // Bind the socket to the port
    if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    addr_len = sizeof their_addr;

    // Receive data from a client
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Received packet from %s\n", inet_ntoa(their_addr.sin_addr));
    printf("Packet is %d bytes long\n", numbytes);

    // Null-terminate the received data and print it
    buf[numbytes] = '\0';
    printf("Packet contains \"%s\"\n", buf);

    // Reverse the number
    int num = atoi(buf);
    int reversed = 0;
    while (num != 0) {
        reversed = reversed * 10 + num % 10;
        num /= 10;
    }
    printf("Reversed number is %d\n", reversed);

    // Sending the reversed number back to the client
    if (sendto(sockfd, &reversed, sizeof(reversed), 0, (struct sockaddr *)&their_addr, addr_len) == -1) {
        perror("sendto");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    return 0;
}
