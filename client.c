#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h>

#define MAX 80
#define PORT 7070
#define SA struct sockaddr

int main() { 
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 

    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    printf("Socket successfully created..\n"); 
    
    bzero(&servaddr, sizeof(servaddr)); 

    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    printf("connected to the server..\n"); 
    
    int option;
    printf("Choose an option:\n");
    printf("1. Signup\n");
    printf("2. Login\n");
    scanf("%d", &option);
    
    if (option == 1) {
        int sign = 1; 
        write(sockfd, &sign, sizeof(sign));
        
        char username[MAX], password[MAX];
        printf("Enter username for signup: ");
        scanf("%s", username);
        
        printf("Enter password for signup: ");
        scanf("%s", password);
        
        write(sockfd, username, sizeof(username));
        write(sockfd, password, sizeof(password));
        
        char response[MAX];
        read(sockfd, response, sizeof(response));
        printf("Server response: %s\n", response);
    }
    else if (option == 2) {
        int sign = 0; 
        write(sockfd, &sign, sizeof(sign));
        
        char username[MAX], password[MAX];
        printf("Enter username for login: ");
        scanf("%s", username);
        
        printf("Enter password for login: ");
        scanf("%s", password);
        
        write(sockfd, username, sizeof(username));
        write(sockfd, password, sizeof(password));
        
        char response[MAX];
        read(sockfd, response, sizeof(response));
        printf("Server response: %s\n", response);
    }
    else {
        printf("Invalid option selected\n");
    }
    
    close(sockfd); 
}
