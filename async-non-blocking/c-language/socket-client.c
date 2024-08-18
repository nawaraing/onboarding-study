// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <fcntl.h>
// #include <sys/select.h>
// #include <errno.h>

// #define PORT 8080
// #define BUFFER_SIZE 1024

// int main() {
//     int sock = 0;
//     struct sockaddr_in serv_addr;
//     char buffer[BUFFER_SIZE] = {0};
//     fd_set readfds;
//     struct timeval tv;
//     int flags, ret;

//     // 소켓 생성
//     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         printf("\n 소켓 생성 실패 \n");
//         return -1;
//     }

//     // 소켓을 논블로킹 모드로 설정
//     flags = fcntl(sock, F_GETFL, 0);
//     fcntl(sock, F_SETFL, flags | O_NONBLOCK);

//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT);

//     // 서버 주소 변환
//     if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
//         printf("\n잘못된 주소\n");
//         return -1;
//     }

//     // 서버에 연결 시도 (비동기)
//     ret = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
//     if (ret < 0) {
//         if (errno != EINPROGRESS) {
//             printf("\n연결 실패\n");
//             return -1;
//         }
//     }

//     // 연결 시도 완료까지 대기
//     FD_ZERO(&readfds);
//     FD_SET(sock, &readfds);

//     // 타임아웃 설정
//     tv.tv_sec = 5;
//     tv.tv_usec = 0;

//     ret = select(sock + 1, NULL, &readfds, NULL, &tv);
//     if (ret <= 0 || !FD_ISSET(sock, &readfds)) {
//         printf("서버에 연결하지 못했습니다.\n");
//         return -1;
//     }

//     // 서버로부터 환영 메시지 읽기 (논블로킹)
//     ret = read(sock, buffer, BUFFER_SIZE);
//     if (ret > 0) {
//         printf("서버로부터 받은 메시지: %s\n", buffer);
//     } else {
//         printf("서버로부터 메시지를 읽는 데 실패했습니다.\n");
//     }

//     // 서버에 메시지 보내기 (비동기)
//     const char *message = "안녕하세요, 서버!";
//     send(sock, message, strlen(message), 0);
//     printf("서버로 메시지를 보냈습니다: %s\n", message);

//     // 메시지 전송 완료 후 읽기 대기
//     FD_ZERO(&readfds);
//     FD_SET(sock, &readfds);

//     tv.tv_sec = 5;
//     tv.tv_usec = 0;

//     ret = select(sock + 1, &readfds, NULL, NULL, &tv);
//     if (ret > 0 && FD_ISSET(sock, &readfds)) {
//         ret = read(sock, buffer, BUFFER_SIZE);
//         if (ret > 0) {
//             buffer[ret] = '\0';
//             printf("서버로부터 받은 응답: %s\n", buffer);
//         }
//     }

//     close(sock);
//     return 0;
// }
