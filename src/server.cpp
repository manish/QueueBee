#include "server.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Server::Server(int port)
    : port_(port), server_socket_(-1), running_(false) {
}

Server::~Server() {
    stop();
}

bool Server::start() {
    // Create socket
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ < 0) {
        log("ERROR: Failed to create socket");
        return false;
    }

    // Allow socket reuse
    int opt = 1;
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        log("ERROR: Failed to set socket options");
        close(server_socket_);
        return false;
    }

    // Bind socket
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_);

    if (bind(server_socket_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        log("ERROR: Failed to bind to port " + std::to_string(port_));
        close(server_socket_);
        return false;
    }

    // Listen for connections
    if (listen(server_socket_, 10) < 0) {
        log("ERROR: Failed to listen on socket");
        close(server_socket_);
        return false;
    }

    running_ = true;
    log("Server started on port " + std::to_string(port_));

    // Start accept thread
    accept_thread_ = std::thread(&Server::accept_loop, this);

    return true;
}

void Server::stop() {
    if (running_) {
        running_ = false;

        // Close server socket to interrupt accept()
        if (server_socket_ >= 0) {
            shutdown(server_socket_, SHUT_RDWR);
            close(server_socket_);
            server_socket_ = -1;
        }

        // Wait for accept thread to finish
        if (accept_thread_.joinable()) {
            accept_thread_.join();
        }

        log("Server stopped");
    }
}

void Server::accept_loop() {
    while (running_) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        int client_socket = accept(server_socket_, (struct sockaddr*)&client_addr, &client_len);

        if (client_socket < 0) {
            if (running_) {
                log("ERROR: Failed to accept client connection");
            }
            continue;
        }

        // Log client connection
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        log("Client connected from " + std::string(client_ip) + ":" +
            std::to_string(ntohs(client_addr.sin_port)));

        // Handle client in new thread
        std::thread client_thread(&Server::handle_client, this, client_socket);
        client_thread.detach(); // Let it run independently
    }
}

void Server::handle_client(int client_socket) {
    char buffer[1024] = {0};

    // Read command from client
    ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        std::string message(buffer);

        // Process command and get response
        std::string response = process_command(message);

        // Send response
        send(client_socket, response.c_str(), response.length(), 0);
    }

    close(client_socket);
}

std::string Server::process_command(const std::string& message) {
    std::string command, topic, data;

    if (!parse_message(message, command, topic, data)) {
        return "ERROR:Invalid message format\n";
    }

    log("Command: " + command + ", Topic: " + topic + ", Data: " + data);

    if (command == "SEND") {
        // TODO: Add message to queue (will be implemented with Message/TopicQueue classes)
        return "OK:Message queued\n";
    } else if (command == "RECEIVE") {
        // TODO: Get message from queue (will be implemented with Message/TopicQueue classes)
        return "OK:No messages available\n";
    } else {
        return "ERROR:Unknown command\n";
    }
}

bool Server::parse_message(const std::string& message,
                          std::string& command,
                          std::string& topic,
                          std::string& data) {
    // Remove trailing newline if present
    std::string msg = message;
    if (!msg.empty() && msg.back() == '\n') {
        msg.pop_back();
    }

    // Find first colon
    size_t first_colon = msg.find(':');
    if (first_colon == std::string::npos) {
        return false;
    }
    command = msg.substr(0, first_colon);

    // Find second colon
    size_t second_colon = msg.find(':', first_colon + 1);
    if (second_colon == std::string::npos) {
        return false;
    }
    topic = msg.substr(first_colon + 1, second_colon - first_colon - 1);

    // Rest is data (can be empty)
    data = msg.substr(second_colon + 1);

    return true;
}

void Server::log(const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    std::cout << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
              << "] " << message << std::endl;
}