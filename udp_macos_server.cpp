#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

const int PORT = 12345;
const int BUFFER_SIZE = 1024;

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket.\n";
        return -1;
    }

    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to bind socket.\n";
        close(sockfd);
        return -1;
    }

    std::cout << "UDP server started on port " << PORT << ".\n";

    char buffer[BUFFER_SIZE];
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    while (true) {
        std::memset(buffer, 0, BUFFER_SIZE);
        int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client_addr, &client_addr_len);
        if (recv_len == -1) {
            std::cerr << "Failed to receive message.\n";
            break;
        }
        std::cout << "Received message from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port)
                  << "\n";
        std::cout << "Message: " << buffer << "\n";

        // do something with the message

        client_addr.sin_port = 8888;
        int send_len = sendto(sockfd, buffer, recv_len, 0, (struct sockaddr *) &client_addr, client_addr_len);
        if (send_len == -1) {
            std::cerr << "Failed to send message.\n";
            break;
        }
    }

    close(sockfd);

    return 0;
}
