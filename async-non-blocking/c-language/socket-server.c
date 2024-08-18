#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8082
#define BUF_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUF_SIZE] = {0};
    char *message = "Hello from server";

    // 1. 소켓 파일 디스크립터 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. 소켓 옵션 설정 (재사용 가능한 주소를 사용하도록 설정)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // 3. 서버 주소 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // 모든 네트워크 인터페이스에서 연결 허용
    address.sin_port = htons(PORT);        // 포트 번호 설정

    // 4. 소켓을 주소와 바인딩
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 5. 대기열을 생성하여 클라이언트의 연결을 기다림
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting for connections...\n");

    while (1) {
        // 6. 클라이언트가 연결 요청 시 이를 받아들임
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        } else {
            printf("accept succ!\n");
        }

        // 7. 클라이언트로부터 메시지 수신
        memset(buffer, 0, BUF_SIZE); // 버퍼 초기화
        read(new_socket, buffer, BUF_SIZE);
        printf("Message from client: %s\n", buffer);

        // 8. 클라이언트로 메시지 전송
        send(new_socket, message, strlen(message), 0);
        printf("Hello message sent to client\n");

        // 9. 클라이언트 소켓 종료
        close(new_socket);
        printf("Connection with client closed, waiting for next connection...\n");
    }

    // 10. 서버 소켓 종료 (여기서는 도달하지 않지만, 서버 종료 시 필요)
    close(server_fd);
    
    return 0;
}
