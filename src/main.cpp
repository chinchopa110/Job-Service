#include "JobService.hpp"
#include "utils/ThreadPool.hpp"
#include <iostream>

int main() {
    const size_t numThreads = 4;
    
    std::cout << "=== JobService Application ===" << std::endl;
    std::cout << "Initializing with " << numThreads << " worker threads..." << std::endl;
    
    JobService service;
    
    ThreadPool pool(numThreads);
    
    std::cout << "Application initialized successfully." << std::endl;
    std::cout << "Thread pool is ready to process tasks." << std::endl;
    std::cout << "=== Ready ===" << std::endl;
    
    return 0;
}
