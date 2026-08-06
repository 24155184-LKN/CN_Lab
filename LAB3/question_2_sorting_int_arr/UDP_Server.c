/* 2. Write a C program
- Create an UDP socket
- Client - Sends and integer array to the server
- Server - Sort the array in ascending order
	 - Send it back to the Client
*/

// Server Side

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
#define MAXBUFLEN 1024*5

int main(){
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

    while(1) {
        printf("Server: waiting to recvfrom...\n");
        if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
            (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        buf[numbytes] = '\0';
        printf("Server: got packet from %s\n", inet_ntoa(their_addr.sin_addr));
        printf("Server: packet is %d bytes long\n", numbytes);
        printf("Server: packet contains \"%s\"\n", buf);

        // Convert received string to integer array
        int arr[100];
        int n = 0;
        char *token = strtok(buf, " ");
        if (token != NULL) {
            n = atoi(token);
            for (int i = 0; i < n; i++) {
                token = strtok(NULL, " ");
                if (token == NULL) {
                    break;
                }
                arr[i] = atoi(token);
            }
        }

        // Sort the array in ascending order
        for(int i = 0; i < n-1; i++) {
            for(int j = 0; j < n-i-1; j++) {
                if(arr[j] > arr[j+1]) {
                    int temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }

        // Convert sorted array back to string
        char sorted_buf[MAXBUFLEN];
        int offset = snprintf(sorted_buf, sizeof(sorted_buf), "%d", n);
        for(int i = 0; i < n; i++) {
            offset += snprintf(sorted_buf + offset, sizeof(sorted_buf) - offset, " %d", arr[i]);
        }

        // Send the sorted array back to the client
        if ((numbytes = sendto(sockfd, sorted_buf, strlen(sorted_buf), 0,
             (struct sockaddr *)&their_addr, addr_len)) == -1) {
            perror("sendto");
            exit(EXIT_FAILURE);
        }

        printf("Server: sent %d bytes back to client\n", numbytes);
    }
    
    close(sockfd);
    return 0;
}
