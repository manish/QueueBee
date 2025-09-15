#include <iostream>
#include <string>
#include <vector>
#include <cstring>

void print_usage(const char* program_name) {
    std::cout << "QueueBee - Simple Message Queue System\n";
    std::cout << "Version: 0.1.0\n\n";
    std::cout << "Usage: " << program_name << " [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --send <message>     Send a message to the queue\n";
    std::cout << "  --receive            Receive a message from the queue\n";
    std::cout << "  --topic <name>       Specify topic (default: default)\n";
    std::cout << "  --help, -h           Show this help message\n";
    std::cout << "  --version, -v        Show version information\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << program_name << " --send \"Hello World\" --topic orders\n";
    std::cout << "  " << program_name << " --receive --topic orders\n";
}

void print_version() {
    std::cout << "QueueBee version 0.1.0\n";
    std::cout << "Built with C++17\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 0;
    }

    std::string topic = "default";
    std::string message;
    bool send_mode = false;
    bool receive_mode = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            return 0;
        }
        else if (arg == "--version" || arg == "-v") {
            print_version();
            return 0;
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
        else {
            std::cerr << "Error: Unknown option: " << arg << "\n";
            print_usage(argv[0]);
            return 1;
        }
    }

    if (send_mode && receive_mode) {
        std::cerr << "Error: Cannot use --send and --receive together\n";
        return 1;
    }

    if (send_mode) {
        std::cout << "Sending message to topic '" << topic << "': " << message << "\n";
        std::cout << "[Note: Queue functionality not yet implemented]\n";
    }
    else if (receive_mode) {
        std::cout << "Receiving from topic '" << topic << "'...\n";
        std::cout << "[Note: Queue functionality not yet implemented]\n";
    }
    else {
        std::cerr << "Error: Must specify either --send or --receive\n";
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}