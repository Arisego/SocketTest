// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
 
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
        perror("socket creation failed \n");
        exit(EXIT_FAILURE);
    }
 
    memset(&servaddr, 0, sizeof(servaddr));
     
    // Filling server information
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_port = htons(PORT);

    inet_pton(AF_INET6, "::ffff:127.0.0.1", servaddr.sin6_addr.s6_addr);

#pragma region DualStackBind
    /** 
     * Bind socket to IPv6 address will make the mapped IPv4 address unreachable
     * ErrNo: 101 | Network is unreachable
     **/
    // struct sockaddr_in6 localAddr;
    // memset(&localAddr, 0, sizeof(localAddr));
    // localAddr.sin6_family = AF_INET6;
    // localAddr.sin6_addr = IN6ADDR_LOOPBACK_INIT;
    // if ( bind(sockfd, (const struct sockaddr *)&localAddr, 
    //         sizeof(localAddr)) < 0 )
    // {
    //     perror("bind to local failed \n");
    //     exit(EXIT_FAILURE);
    // }
#pragma endregion

    int n;
    socklen_t len;
     
    const int8_t td_ByteSent = sendto(sockfd, (const char *)hello, strlen(hello),
        MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
        sizeof(servaddr));

    if(td_ByteSent < 0)
    {
        int td_Errno = errno;
        printf("Hello message sent failed: %d | %s \n", td_Errno, strerror(td_Errno));
    }
    else
    {
        printf("Hello message sent: %d Byte\n", td_ByteSent);
            
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                    MSG_WAITALL, (struct sockaddr *) &servaddr,
                    &len);

        if(n<0)
        {
            int td_Errno = errno;
            printf("Receive message failed: %d | %s \n", td_Errno, strerror(td_Errno));
        }
        else
        {
            buffer[n] = '\0';
            printf("Server : %s |%d %u|\n", buffer,n , len);
        }
    }

 
    close(sockfd);
    return 0;
}