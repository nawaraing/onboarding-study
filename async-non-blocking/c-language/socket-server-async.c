#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define PORT 8082
#define BUF_SIZE 1024

// 시그널 핸들러 함수
void sigio_handler(int signo) {
    printf("SIGIO received! Ready to perform I/O operation\n");
}

int set_async_nonblock(int sockfd) {
    // 현재 프로세스를 소켓 소유자로 설정
    if (fcntl(sockfd, F_SETOWN, getpid()) < 0) {  
        perror("fcntl F_SETOWN failed");
        return -1;
    }
    
    // 소켓을 비동기 I/O 및 논블로킹 모드로 설정
    if (fcntl(sockfd, F_SETFL, O_ASYNC | O_NONBLOCK) < 0) {    
        perror("fcntl O_ASYNC | O_NONBLOCK failed");
        return -1;
    }
    
    return 0;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUF_SIZE];

    // 시그널 핸들러 등록
    signal(SIGIO, sigio_handler);

    // 1. 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. 소켓 옵션 설정
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // 3. 주소 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 4. 소켓을 바인딩
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 5. 클라이언트 연결 대기
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting for connections on port %d...\n", PORT);

    // 6. 서버 소켓을 비동기 I/O 모드로 설정
    // if (set_async(server_fd) < 0) {
    //     exit(EXIT_FAILURE);
    // }

    while (1) {
        // 비동기 신호를 대기함 (SIGIO 발생 시 처리)
        // pause();  // 시그널이 발생할 때까지 대기

        // 7. 새 클라이언트 연결 처리
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        printf("New connection, socket fd is %d\n", new_socket);

        // 8. 클라이언트 소켓에 대해 비동기 I/O 설정
        if (set_async_nonblock(new_socket) < 0) {
            exit(EXIT_FAILURE);
        }

        // 9. 클라이언트로부터 데이터 읽기
        memset(buffer, 0, BUF_SIZE);  // 버퍼 초기화
        int valread = read(new_socket, buffer, BUF_SIZE);
        if (valread > 0) {
            printf("Message from client: %s\n", buffer);
            send(new_socket, "Hello from server", strlen("Hello from server"), 0);
        } else {
            printf("No data received or connection closed\n");
        }

        close(new_socket);  // 클라이언트 소켓 닫기
    }

    // 서버 소켓 종료
    close(server_fd);
    return 0;
}
