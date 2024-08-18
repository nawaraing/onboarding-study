#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <errno.h>  

#define PORT 8082  
#define BUF_SIZE 1024

int sockfd, client_socket;  

void sigio_handler(int signo) {
    char buffer[BUF_SIZE];

    while (1) {
        int bytes_read = read(client_socket, buffer, BUF_SIZE);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';  
            printf("Received from client: %s\n", buffer);

            if (send(client_socket, buffer, bytes_read, 0) < 0) {
                perror("Send failed");
            } else {
                printf("Sent to client: %s\n", buffer);
                close(client_socket);
            }
        } else if (bytes_read == 0) {
            printf("Client disconnected.\n");
            close(client_socket);
            break;  
        } else {
            // perror("Read failed");
            close(client_socket);
            break;
        }

    }
}

void handle_client(int client_socket) {
}

int main() {
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);  

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 3) < 0) {
        perror("Listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    signal(SIGIO, sigio_handler);

    while (1) {
        client_socket = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                continue;
            } else {
                perror("Accept failed");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
        }

        printf("Client connected. Waiting for data...\n");

        if (fcntl(client_socket, F_SETOWN, getpid()) < 0) {
            perror("fcntl F_SETOWN failed for client_socket");
            close(client_socket);
            continue;
        }

        if (fcntl(client_socket, F_SETFL, O_ASYNC | O_NONBLOCK) < 0) { 
            perror("fcntl O_ASYNC | O_NONBLOCK failed for client_socket");
            close(client_socket);
            continue;
        }

        pause();  
    }

    close(sockfd);
    return 0;
}
