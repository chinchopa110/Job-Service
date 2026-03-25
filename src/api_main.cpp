#include "api/HttpServer.hpp"
#include <iostream>
#include <csignal>
#include <memory>

std::unique_ptr<HttpServer> server;

void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received. Shutting down..." << std::endl;
    if (server) {
        server->stop();
    }
    exit(signum);
}

int main(int argc, char* argv[]) {
    try {
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);
        
        std::string address = "0.0.0.0";
        unsigned short port = 8080;
        int threads = std::thread::hardware_concurrency();
        
        if (argc >= 2) {
            port = static_cast<unsigned short>(std::atoi(argv[1]));
        }
        if (argc >= 3) {
            threads = std::atoi(argv[2]);
        }
        
        if (threads <= 0) {
            threads = 1;
        }
        
        server = std::make_unique<HttpServer>(address, port, threads);
        server->run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
