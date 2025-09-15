#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <csignal>
#include <memory>
#include <thread>
#include <chrono>
#include "server.h"

// Global server pointer for signal handling
std::unique_ptr<Server> g_server;

void signal_handler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        std::cout << "\nReceived shutdown signal, stopping server...\n";
        if (g_server) {
            g_server->stop();
        }
    }
}

void print_usage(const char* program_name) {
    std::cout << "QueueBee - Simple Message Queue System\n";
    std::cout << "Version: 0.1.0\n\n";
    std::cout << "Usage: " << program_name << " [OPTIONS]\n\n";
    std::cout << "Modes:\n";
    std::cout << "  Service mode (default):  " << program_name << " [--port PORT]\n";
    std::cout << "  Send mode:               " << program_name << " --send MESSAGE --topic TOPIC [--port PORT]\n";
    std::cout << "  Receive mode:            " << program_name << " --receive --topic TOPIC [--port PORT]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --send <message>     Send a message to the queue\n";
    std::cout << "  --receive            Receive a message from the queue\n";
    std::cout << "  --topic <name>       Specify topic (default: default)\n";
    std::cout << "  --port <number>      Port number (default: 9542)\n";
    std::cout << "  --help, -h           Show this help message\n";
    std::cout << "  --version, -v        Show version information\n\n";
    std::cout << "Examples:\n";
    std::cout << "  # Start server\n";
    std::cout << "  " << program_name << "\n";
    std::cout << "  " << program_name << " --port 8080\n\n";
    std::cout << "  # Send message\n";
    std::cout << "  " << program_name << " --send \"Hello World\" --topic orders\n";
    std::cout << "  " << program_name << " --send \"Test\" --topic orders --port 8080\n\n";
    std::cout << "  # Receive message\n";
    std::cout << "  " << program_name << " --receive --topic orders\n";
    std::cout << "  " << program_name << " --receive --topic orders --port 8080\n";
}

void print_version() {
    std::cout << "QueueBee version 0.1.0\n";
    std::cout << "Built with C++20\n";
}

int run_service_mode(int port) {
    std::cout << "Starting QueueBee server...\n";

    // Set up signal handlers
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    // Create and start server
    g_server = std::make_unique<Server>(port);

    if (!g_server->start()) {
        std::cerr << "Failed to start server\n";
        return 1;
    }

    std::cout << "Server running on port " << port << ". Press Ctrl+C to stop.\n";

    // Keep running until server is stopped
    while (g_server->is_running()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}

int run_send_mode(const std::string& message, const std::string& topic, int port) {
    std::cout << "Sending message to topic '" << topic << "' on port " << port << ": " << message << "\n";
    std::cout << "[Note: Client functionality not yet implemented]\n";
    // TODO: Implement TCP client connection
    return 0;
}

int run_receive_mode(const std::string& topic, int port) {
    std::cout << "Receiving from topic '" << topic << "' on port " << port << "...\n";
    std::cout << "[Note: Client functionality not yet implemented]\n";
    // TODO: Implement TCP client connection
    return 0;
}

int main(int argc, char* argv[]) {
    std::string topic = "default";
    std::string message;
    int port = 9542;
    bool send_mode = false;
    bool receive_mode = false;
    bool help_requested = false;
    bool version_requested = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help" || arg == "-h") {
            help_requested = true;
        }
        else if (arg == "--version" || arg == "-v") {
            version_requested = true;
        }
        else if (arg == "--send") {
            if (i + 1 < argc) {
                send_mode = true;
                message = argv[++i];
            } else {
                std::cerr << "Error: --send requires a message argument\n";
                return 1;
            }
        }
        else if (arg == "--receive") {
            receive_mode = true;
        }
        else if (arg == "--topic") {
            if (i + 1 < argc) {
                topic = argv[++i];
            } else {
                std::cerr << "Error: --topic requires a name argument\n";
                return 1;
            }
        }
        else if (arg == "--port") {
            if (i + 1 < argc) {
                try {
                    port = std::stoi(argv[++i]);
                    if (port < 1 || port > 65535) {
                        std::cerr << "Error: Port must be between 1 and 65535\n";
                        return 1;
                    }
                } catch (...) {
                    std::cerr << "Error: Invalid port number\n";
                    return 1;
                }
            } else {
                std::cerr << "Error: --port requires a number argument\n";
                return 1;
            }
        }
        else {
            std::cerr << "Error: Unknown option: " << arg << "\n";
            print_usage(argv[0]);
            return 1;
        }
    }

    // Handle help and version
    if (help_requested) {
        print_usage(argv[0]);
        return 0;
    }

    if (version_requested) {
        print_version();
        return 0;
    }

    // Check for conflicting modes
    if (send_mode && receive_mode) {
        std::cerr << "Error: Cannot use --send and --receive together\n";
        return 1;
    }

    // Execute appropriate mode
    if (send_mode) {
        return run_send_mode(message, topic, port);
    } else if (receive_mode) {
        return run_receive_mode(topic, port);
    } else {
        // Default: service mode
        return run_service_mode(port);
    }
}