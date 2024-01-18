#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <sstream>
#include <iterator>
#include <vector>

void handle_client(int client_socket) {
    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);
    std::string request = std::string(buffer);
    std::string response;

    std::istringstream iss(request);
    std::vector<std::string> tokens(std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>{});

    if (!tokens.empty()) {
        if (tokens[0] == "LOGIN") {
            response = "LOGIN_SUCCESS";
        } else if (tokens[0] == "LOGOUT") {
            response = "LOGOUT_SUCCESS";
        } else if (tokens[0] == "ADD_EVENT") {
            response = "EVENT_ADDED";
        } else if (tokens[0] == "DELETE_EVENT") {
            response = "EVENT_DELETED";
        } else if (tokens[0] == "LIST_EVENTS") {
            response = "EVENT_LIST";
        } else {
            response = "UNKNOWN_COMMAND";
        }
    } else {
        response = "INVALID_REQUEST";
    }

    send(client_socket, response.c_str(), response.size(), 0);
    close(client_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(1234);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        std::thread client_thread(handle_client, new_socket);
        client_thread.detach();
    }

    return 0;
}

