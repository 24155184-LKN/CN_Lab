#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MYPORT 4952 // the port users will be connecting to
#define MAXBUFLEN 200

int main() {
    int sockfd;
    struct sockaddr_in my_addr; // my address information
    struct sockaddr_in their_addr; // connector's address information
    socklen_t addr_len;
    int numbytes;
    char buf[MAXBUFLEN];

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the address structure
    my_addr.sin_family = AF_INET; // host byte order
    my_addr.sin_port = htons(MYPORT); // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero); // zero the rest of the struct

    // Bind the socket to the port
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    addr_len = sizeof their_addr;

    // Receive data from a client
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("got packet from %s\n", inet_ntoa(their_addr.sin_addr));
    printf("packet is %d bytes long\n", numbytes);

    // Null-terminate the received data and print it
    buf[numbytes] = '\0';
    printf("packet contains \"%s\"\n", buf);

    // Close the socket
    close(sockfd);
    return 0;
}
