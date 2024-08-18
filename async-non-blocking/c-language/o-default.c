#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <errno.h>  // errno를 사용하기 위해 추가

#define PORT 8082  // 포트를 8082로 설정
#define BUF_SIZE 1024

int sockfd;  // 서버 소켓 파일 디스크립터

int main() {
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // 1. 소켓 생성
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);  // PORT를 8082로 설정

    // 3. 소켓 바인딩
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 4. 클라이언트 연결 대기
    if (listen(sockfd, 3) < 0) {
        perror("Listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // 5. 반복적인 클라이언트 연결 수락
    while (1) {
        int client_socket = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        printf("Client connected. Waiting for data...\n");

        // 클라이언트 처리
        char buffer[BUF_SIZE];

        // 클라이언트와 통신을 위한 while 루프
        while (1) {
            // 클라이언트로부터 데이터 읽기
            int bytes_read = read(client_socket, buffer, BUF_SIZE);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';  // null-terminate the string
                printf("Received from client: %s\n", buffer);

                // 클라이언트로 받은 데이터를 다시 전송 (Echo)
                if (send(client_socket, buffer, bytes_read, 0) < 0) {
                    perror("Send failed");
                } else {
                    printf("Sent to client: %s\n", buffer);
                    close(client_socket);
                }
            } else if (bytes_read == 0) {
                // 클라이언트가 연결을 닫음
                printf("Client disconnected.\n");
                close(client_socket);
                break;  // 클라이언트 소켓을 닫고 while 루프 종료
            } else {
                // perror("Read failed");
                close(client_socket);
                break;
            }
        }
    }

    close(sockfd);
    return 0;
}
