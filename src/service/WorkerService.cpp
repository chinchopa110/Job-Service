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

bool WorkerService::deleteWorker(size_t id) {
    return workerRepository_.remove(id);
}

std::optional<Worker> WorkerService::getWorker(size_t id) const {
    return workerRepository_.findById(id);
}

std::vector<Worker> WorkerService::getAllWorkers() const {
    return workerRepository_.findAll();
}

std::vector<Worker> WorkerService::getWorkersByCity(const std::string& city) const {
    return workerRepository_.findByCity(city);
}

std::vector<Worker> WorkerService::getWorkersBySkill(const std::string& skill) const {
    return workerRepository_.findBySkill(skill);
}