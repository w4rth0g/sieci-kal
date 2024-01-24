#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <sstream>
#include <iterator>
#include <vector>
#include <map>
#include "DbLogic.h"

std::map<std::string, std::string> activeSessions;

std::string generateSessionToken() {
    return std::to_string(rand());
}

void responseF(std::string resp, int client_socket) {
    send(client_socket, resp.c_str(), resp.size(), 0);
    close(client_socket);
}

std::string findSessionTokenByUserId(const std::string& userId) {
    for (const auto& pair : activeSessions) {
        if (pair.second == userId) {
            return pair.first;
        }
    }
    return "";
}

void handle_client(int client_socket) {
    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);
    std::string request = std::string(buffer);

    std::istringstream iss(request);
    std::vector<std::string> tokens(std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>{});

    if (tokens.empty()) {
        responseF("INVALID_REQUEST", client_socket);
        return;
    }
    if (tokens[0] == "LOGIN") {
        DbLogic db("cal", "garry", "1111");

        std::string userId = db.getLoggedInUser(tokens[1], tokens[2]); 

        if (!findSessionTokenByUserId(userId).empty()) {
            responseF("EXCEPTION Uzytkownik_zalogowany", client_socket);
            return;
        }

        if (userId.empty()) {
            responseF("EXCEPTION Bledny_login_lub_haslo", client_socket);
            return;
        }
        std::string newToken = generateSessionToken();
        activeSessions[newToken] = userId;
        responseF("LOGIN_SUCCESS " + newToken + " " + userId, client_socket);
        return;
    }
    if (activeSessions.find(tokens[0]) != activeSessions.end()) {
        DbLogic db("cal", "garry", "1111");

        if (tokens[1] == "LOGOUT") {
            activeSessions.erase(tokens[0]);
            responseF("LOGOUT_SUCCESS", client_socket);
        } else if (tokens[1] == "ADD_EVENT") {
            std::string userId = activeSessions.find(tokens[0])->second;

            bool wasEvtAdded = db.addEvent(userId, tokens[2], tokens[3], tokens[4], tokens[5]);

            if (!wasEvtAdded) {
                responseF("EXCEPTION Dodanie_wydarzenia_nie_powiodlo_sie", client_socket);
                return;
            }
            responseF("EVENT_ADDED", client_socket);
        } else if (tokens[1] == "DELETE_EVENT") {
            bool wasEvtDel = db.deleteEvent(tokens[2]);

            if (!wasEvtDel) {
                responseF("EXCEPTION Usuniecie_wydarzenia_nie_powiodlo_sie", client_socket);
                return;
            }
            responseF("EVENT_DELETED", client_socket);
        } else if (tokens[1] == "LIST_EVENTS") {
            std::string res = db.getEvents();
            if (res.empty()) {
                responseF("NO_EVENTS", client_socket);
            }
            responseF(res, client_socket);
        } else if (tokens[1] == "GET_USER") {
            std::string username = db.getUserById(tokens[2]);

            if (username.empty()) {
                responseF("EXCEPTION Uzytkownik_nie_istnieje", client_socket);
                return;
            }
            responseF("USER " + username, client_socket);
        } else {
            responseF("UNKNOWN_COMMAND", client_socket);
        }
    } else {
        responseF("NOT_AUTHENTICATED", client_socket);
    }
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

