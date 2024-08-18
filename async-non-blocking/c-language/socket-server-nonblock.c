#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 8082
#define BUF_SIZE 1024

int set_non_blocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl failed");
        return -1;
    }
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl failed");
        return -1;
    }
    return 0;
}

int main() {
    int server_fd, new_socket, max_sd, activity, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUF_SIZE];
    fd_set readfds, writefds;

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

    // 6. 비동기 처리를 위한 논블로킹 설정
    if (set_non_blocking(server_fd) < 0) {
        exit(EXIT_FAILURE);
    }

    while (1) {
        // 7. 파일 디스크립터 셋 초기화 및 설정
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);  // 서버 소켓 추가
        max_sd = server_fd;

        // 8. 클라이언트 연결 및 읽기/쓰기 상태 감시
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            perror("select error");
            exit(EXIT_FAILURE);
        }

        // 9. 새 클라이언트 연결 확인
        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                perror("accept failed");
                exit(EXIT_FAILURE);
            }

            printf("New connection, socket fd is %d\n", new_socket);

            // 새로 연결된 소켓을 논블로킹으로 설정
            if (set_non_blocking(new_socket) < 0) {
                exit(EXIT_FAILURE);
            }

            // 10. 클라이언트 소켓에서 반복적으로 읽기 (비동기 처리)
            int total_read = 0;
            memset(buffer, 0, BUF_SIZE);  // 버퍼 초기화

            while (1) {
                FD_ZERO(&readfds);              // 매번 readfds 초기화
                FD_SET(new_socket, &readfds);   // 클라이언트 소켓 감시
                activity = select(new_socket + 1, &readfds, NULL, NULL, NULL);

                if ((activity < 0) && (errno != EINTR)) {
                    perror("select error");
                    close(new_socket);
                    break;
                }

                // 읽기 준비가 된 경우
                if (FD_ISSET(new_socket, &readfds)) {
                    valread = read(new_socket, buffer + total_read, BUF_SIZE - total_read);
                    if (valread > 0) {
                        total_read += valread;
                        printf("Reading... (%d bytes)\n", total_read);

                        // 데이터를 모두 읽었으면 종료 처리
                        if (total_read >= BUF_SIZE) {
                            break;
                        }
                    } else if (valread == 0) {
                        // 클라이언트 연결 종료
                        printf("Client disconnected\n");
                        close(new_socket);
                        break;
                    } else if (valread < 0 && errno != EWOULDBLOCK) {
                        // EWOULDBLOCK이 아닌 경우의 읽기 오류 처리
                        perror("read failed");
                        close(new_socket);
                        break;
                    }
                }
            }

            printf("Total message from client: %s\n", buffer);

            // 11. 클라이언트에게 반복적으로 메시지 전송 (비동기 처리)
            int total_sent = 0;
            const char *response = "Hello from server";
            int response_len = strlen(response);

            while (total_sent < response_len) {
                FD_ZERO(&writefds);            // 매번 writefds 초기화
                FD_SET(new_socket, &writefds);  // 클라이언트 소켓 감시
                activity = select(new_socket + 1, NULL, &writefds, NULL, NULL);

                if ((activity < 0) && (errno != EINTR)) {
                    perror("select error");
                    close(new_socket);
                    break;
                }

                if (FD_ISSET(new_socket, &writefds)) {
                    int sent = send(new_socket, response + total_sent, response_len - total_sent, 0);
                    if (sent > 0) {
                        total_sent += sent;
                        printf("Sending... (%d bytes)\n", total_sent);
                    } else if (sent < 0 && errno != EWOULDBLOCK) {
                        perror("send failed");
                        close(new_socket);
                        break;
                    }
                }
            }

            printf("Total sent to client: %d bytes\n", total_sent);
            close(new_socket);  // 소켓 종료
        }
    }

    // 12. 서버 소켓 종료
    close(server_fd);
    return 0;
}
