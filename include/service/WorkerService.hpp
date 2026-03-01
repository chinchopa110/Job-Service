#pragma once

#include "domain/Worker.hpp"
#include "repository/WorkerRepository.hpp"
#include <vector>
#include <optional>

class WorkerService {
public:
    WorkerService();
    ~WorkerService();

    size_t createWorker(const std::string& name, const std::string& email,
                        const std::vector<std::string>& skills, int experience,
                        const std::string& city, const std::string& resume);
    
    bool updateWorker(const Worker& worker);
    bool deleteWorker(size_t id);
    std::optional<Worker> getWorker(size_t id) const;
    std::vector<Worker> getAllWorkers() const;
    std::vector<Worker> getWorkersByCity(const std::string& city) const;
    std::vector<Worker> getWorkersBySkill(const std::string& skill) const;

private:
    WorkerRepository workerRepository_;
};