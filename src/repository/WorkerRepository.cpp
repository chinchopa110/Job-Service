#include "repository/WorkerRepository.hpp"
#include "utils/LockGuard.hpp"

WorkerRepository::WorkerRepository()
    : nextId_(1) {}

WorkerRepository::~WorkerRepository() {}

size_t WorkerRepository::add(const Worker& worker) {
    LockGuard lock(mutex_);
    size_t id = nextId_++;
    Worker newWorker = worker;
    newWorker.setId(id);
    workers_[id] = newWorker;
    return id;
}

bool WorkerRepository::update(const Worker& worker) {
    LockGuard lock(mutex_);
    auto it = workers_.find(worker.getId());
    if (it != workers_.end()) {
        workers_[worker.getId()] = worker;
        return true;
    }
    return false;
}

bool WorkerRepository::remove(size_t id) {
    LockGuard lock(mutex_);
    return workers_.erase(id) > 0;
}

std::optional<Worker> WorkerRepository::findById(size_t id) const {
    SharedLockGuard lock(mutex_);
    auto it = workers_.find(id);
    if (it != workers_.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::vector<Worker> WorkerRepository::findAll() const {
    SharedLockGuard lock(mutex_);
    std::vector<Worker> result;
    result.reserve(workers_.size());
    for (const auto& pair : workers_) {
        result.push_back(pair.second);
    }
    return result;
}

std::vector<Worker> WorkerRepository::findByCity(const std::string& city) const {
    SharedLockGuard lock(mutex_);
    std::vector<Worker> result;
    for (const auto& pair : workers_) {
        if (pair.second.getCity() == city) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<Worker> WorkerRepository::findBySkill(const std::string& skill) const {
    SharedLockGuard lock(mutex_);
    std::vector<Worker> result;
    for (const auto& pair : workers_) {
        const auto& skills = pair.second.getSkills();
        for (const auto& s : skills) {
            if (s == skill) {
                result.push_back(pair.second);
                break;
            }
        }
    }
    return result;
}