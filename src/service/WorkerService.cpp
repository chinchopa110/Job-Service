#include "service/WorkerService.hpp"

WorkerService::WorkerService() {}

WorkerService::~WorkerService() {}

size_t WorkerService::createWorker(const std::string& name, const std::string& email,
                                    const std::vector<std::string>& skills, int experience,
                                    const std::string& city, const std::string& resume) {
    Worker worker(0, name, email, skills, experience, city, resume);
    return workerRepository_.add(worker);
}

bool WorkerService::updateWorker(const Worker& worker) {
    return workerRepository_.update(worker);
}