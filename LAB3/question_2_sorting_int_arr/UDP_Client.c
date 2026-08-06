/* 2. Write a C program
- Create an UDP socket
- Client - Sends and integer array to the server
- Server - Sort the array in ascending order
	 - Send it back to the Client
*/

// Client Side

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
    struct sockaddr_in their_addr;
    socklen_t addr_len;
    int numbytes;
    char buf[MAXBUFLEN];

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    their_addr.sin_family = AF_INET; // host byte order
    their_addr.sin_port = htons(PORT); // short, network byte order
    their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero); // zero the rest of the struct

    // Send an integer array to the server that is entered by the user
    int n;
    printf("Enter the number of elements in the array: ");
    scanf("%d", &n);
    int arr[n];
    
    printf("Enter the elements of the array:\n");
    for(int i=0; i<n; i++){
        scanf("%d", &arr[i]);
    }

    // Convert the array to a text message and send it to the server
    char send_buf[MAXBUFLEN];
    int offset = snprintf(send_buf, sizeof(send_buf), "%d", n);
    for(int i=0; i<n; i++) {
        offset += snprintf(send_buf + offset, sizeof(send_buf) - offset, " %d", arr[i]);
    }

    if ((numbytes = sendto(sockfd, send_buf, strlen(send_buf), 0, (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }

    printf("Client: sent %d bytes to %s\n", numbytes, inet_ntoa(their_addr.sin_addr));

    // Receive the sorted array from the server
    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }
    buf[numbytes] = '\0';

    // Print the sorted array
    int sorted_arr[100];
    int received_n = 0;
    char *token = strtok(buf, " ");
    if (token != NULL) {
        received_n = atoi(token);
        for(int i=0; i<received_n; i++) {
            token = strtok(NULL, " ");
            if (token == NULL) {
                break;
            }
            sorted_arr[i] = atoi(token);
        }
    }

    printf("Client: received sorted array: ");
    for(int i=0; i<received_n; i++){
        printf("%d ", sorted_arr[i]);
    }
    printf("\n");

    close(sockfd);
    return 0;
}
