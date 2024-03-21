#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h>
#define MAX 80
#define PORT 7070
#define SA struct sockaddr
#define NUMUSERS 8

typedef struct {
    char username[MAX];
    char password[MAX];
} User;

User users[NUMUSERS] ={{"user1", "pass1"},{"user2", "pass2"},{"user3", "pass3"}};
int numUsers = 3;

void registerUser(char *username, char *password);
void func(int connfd);

void registerUser(char *username, char *password) {
    if (numUsers < NUMUSERS) {
        strcpy(users[numUsers].username, username);
        strcpy(users[numUsers].password, password);
        numUsers++;
        printf("User '%s' registered successfully!\n", username);
    } else {
        printf("User database is full. Cannot register new user.\n");
    }
}

int main() { 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 

    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    printf("Socket successfully created..\n"); 
    
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    printf("Socket successfully binded..\n"); 

    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    printf("Server listening..\n"); 
    len = sizeof(cli); 
    
    while(1) {
        connfd = accept(sockfd, (SA*)&cli, &len); 
        if (connfd < 0) { 
            printf("server accept failed...\n"); 
            exit(0); 
        } 
        printf("server accept the client...\n"); 
        func(connfd); 
    }
    
    close(sockfd); 
}

void func(int connfd) { 
    char username[MAX];
    char password[MAX];
    char response[MAX];

    int sign;
    int found = 0;
    
    read(connfd, &sign, sizeof(sign));
    
    if (sign == 0) {
        read(connfd, username, sizeof(username)); 
        read(connfd, password, sizeof(password)); 
        
        for (int i = 0; i < numUsers; i++) {
            if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
                found = 1;
                break;
            }
        }
        
        if (found == 1) {
            strcpy(response, "Valid User");
        } else {
            strcpy(response, "Not a Valid User");
        }
    } 
    else if (sign == 1) {
        read(connfd, username, sizeof(username)); 
        read(connfd, password, sizeof(password)); 
        
        found = 0;
        for (int i = 0; i < numUsers; i++) {
            if (strcmp(users[i].username, username) == 0) {
                found = 1;
                break;
            }
        }
        
        if (found == 0) {
            registerUser(username, password);
            strcpy(response, "User added successfully");
        } else {
            strcpy(response, "Username already exists");
        }
        
        if (numUsers == NUMUSERS) {
            strcpy(response, "User database is full. Cannot register new user.");
        }
    }
    
    write(connfd, response, sizeof(response));
    close(connfd); 
}

