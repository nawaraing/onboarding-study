#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <errno.h>  
#include <pthread.h>  // 스레드를 사용하기 위해 추가

#define PORT 8082  
#define BUF_SIZE 1024
#define THREAD_NAME_SIZE 16  // 스레드 이름 최대 길이

int sockfd, client_socket;  

void* handle_client_thread(void* arg) {
    int client_fd = *(int*)arg;
    char buffer[BUF_SIZE];
    char thread_name[THREAD_NAME_SIZE];

    // 스레드 이름 설정
    snprintf(thread_name, THREAD_NAME_SIZE, "Thread-%d", client_fd);
    pthread_setname_np(thread_name);  // 스레드 이름 설정

    // 현재 실행 중인 스레드 이름 출력
    pthread_getname_np(pthread_self(), thread_name, THREAD_NAME_SIZE);
    printf("Running thread: %s\n", thread_name);

    while (1) {
        int bytes_read = read(client_fd, buffer, BUF_SIZE);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';  
            printf("Received from client: %s\n", buffer);

            if (send(client_fd, buffer, bytes_read, 0) < 0) {
                perror("Send failed");
            } else {
                printf("Sent to client: %s\n", buffer);
                close(client_fd);
                break;  // 클라이언트 처리 후 종료
            }
        } else if (bytes_read == 0) {
            printf("Client disconnected.\n");
            close(client_fd);
            break;  
        } else {
            close(client_fd);
            break;
        }
    }

    free(arg);
    return NULL;
}

void sigio_handler(int signo) {
    pthread_t thread_id;

    int *client_fd_ptr = malloc(sizeof(int));
    *client_fd_ptr = client_socket;

    // 스레드 생성
    if (pthread_create(&thread_id, NULL, handle_client_thread, (void*)client_fd_ptr) != 0) {
        perror("Thread creation failed");
        close(client_socket);
    }

    pthread_detach(thread_id);  // 스레드를 분리하여 종료 시 자동으로 정리
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

        if (fcntl(client_socket, F_SETFL, O_ASYNC) < 0) { 
            perror("fcntl O_ASYNC failed for client_socket");
            close(client_socket);
            continue;
        }

        pause();  
    }

    close(sockfd);
    return 0;
}
