// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
 
#define PORT     8080
#define MAXLINE 1024
 
// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    const char *hello = "Hello from client";
    struct sockaddr_in6     servaddr;
 
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
 
    memset(&servaddr, 0, sizeof(servaddr));
     
    // Filling server information
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_port = htons(PORT);

    inet_pton(AF_INET6, "::ffff:127.0.0.1", servaddr.sin6_addr.s6_addr);

#pragma region DualStackBind
    /** 
     * Bind socket to IPv6 address will make the mapped IPv4 address unreachable, but I do not get an error.
     * Uncomment bellow code to see the result, your server should be in IPv4 version as this commit is in.
     * */
    // struct sockaddr_in6 localAddr;
    // memset(&localAddr, 0, sizeof(localAddr));
    // localAddr.sin6_family = AF_INET6;
    // localAddr.sin6_addr = IN6ADDR_LOOPBACK_INIT;
    // if ( bind(sockfd, (const struct sockaddr *)&localAddr, 
    //         sizeof(localAddr)) < 0 )
    // {
    //     perror("bind to local failed");
    //     exit(EXIT_FAILURE);
    // }
#pragma endregion

    int n;
    socklen_t len;
     
    sendto(sockfd, (const char *)hello, strlen(hello),
        MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
            sizeof(servaddr));
    printf("Hello message sent.\n");
         
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, (struct sockaddr *) &servaddr,
                &len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);
 
    close(sockfd);
    return 0;
}