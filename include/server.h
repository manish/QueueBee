#ifndef QUEUEBEE_SERVER_H
#define QUEUEBEE_SERVER_H

#include <atomic>
#include <thread>
#include <memory>
#include <string>

class Server {
public:
    Server(int port = 9542);
    ~Server();

    // Start the server
    bool start();

    // Stop the server gracefully
    void stop();

    // Check if server is running
    bool is_running() const { return running_.load(); }

    // Get the port server is listening on
    int get_port() const { return port_; }

private:
    // Accept loop for new client connections
    void accept_loop();

    // Handle individual client connection
    void handle_client(int client_socket);

    // Process protocol command
    std::string process_command(const std::string& command);

    // Parse protocol message
    bool parse_message(const std::string& message,
                      std::string& command,
                      std::string& topic,
                      std::string& data);

    int port_;
    int server_socket_;
    std::atomic<bool> running_;
    std::thread accept_thread_;

    // Simple logging
    void log(const std::string& message);
};

#endif // QUEUEBEE_SERVER_H