#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

using namespace std;

void socket_thread(int client_socket, sockaddr_in client_address);

int main() {

    // 创建Socket套接字，TCP使用SOCK_STREAM
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        cerr << "Error creating socket" << endl;
        return 1;
    }

    // 绑定本地地址和端口号
    struct sockaddr_in local_address;
    memset(&local_address, 0, sizeof(local_address));
    local_address.sin_family = AF_INET;
    local_address.sin_addr.s_addr = INADDR_ANY; // 本地地址
    local_address.sin_port = htons(12345); // 监听端口号
    /**
     *  我们原本想要使用#include<WinSock2.h>头文件下的bind函数，结果却由于作用域的冲突造成，bind函数变成了std下的函数。
     *  std 下面也有一个bind函数，
     *  方案一：移除 using namespace std
     *  方案二：使用::域操作符
     */
    if (::bind(server_socket, (struct sockaddr *) &local_address, sizeof(local_address)) == -1) {
        cerr << "Error binding to port" << endl;
        close(server_socket);
        return 1;
    }

    // 监听客户端连接
    if (listen(server_socket, SOMAXCONN) == -1) {
        cerr << "Error listening for connections" << endl;
        close(server_socket);
        return 1;
    }
    cout << "Listening for connections on port " << ntohs(local_address.sin_port) << " ..." << endl;

    while (true) {
        // 接受客户端连接
        struct sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_address_size);
        if (client_socket == -1) {
            cerr << "Error accepting connection" << endl;
            continue;
        }
        cout << "Accepted connection from " << inet_ntoa(client_address.sin_addr) << ":"
             << ntohs(client_address.sin_port) << endl;

        thread socket_t(socket_thread, client_socket, client_address);
        socket_t.detach();
    }

    // 关闭服务器套接字
    close(server_socket);

    return 0;
}

///用线程去处理每个客户端
void socket_thread(int client_socket, sockaddr_in client_address) {
    // 接收客户端消息
    char buffer[1024];
    while (true) {
        int receive_size = recv(client_socket, buffer, sizeof(buffer), 0);
        if (receive_size == -1) {
            cerr << "Error receiving data: " << strerror(errno) << endl;
            break;
        } else if (receive_size == 0) {
            cout << "Client disconnected" << endl;
            break;
        }
        buffer[receive_size] = '\0';
        cout << "Received message from " << inet_ntoa(client_address.sin_addr) << ":"
             << ntohs(client_address.sin_port) << " - " << buffer << endl;

        ///客户端如果按行读取，数据后面必须加换行符\n,不然客户端读取不到换行符会一直阻塞
        const char *response = "";
        ///自定义服务端的回复
        if (::strcmp(buffer, "1") == 0) {
            response = "server receive 1";
        } else {
            response = buffer;
        }
        int send_size = send(client_socket, response, strlen(response), 0);
        if (send_size == -1) {
            cerr << "Error sending data: " << strerror(errno) << endl;
        } else {
            cout << "Sent " << buffer << " data to " << inet_ntoa(client_address.sin_addr) << ":"
                 << ntohs(client_address.sin_port) << endl;
        }
    }

    // 关闭客户端套接字
    close(client_socket);
}