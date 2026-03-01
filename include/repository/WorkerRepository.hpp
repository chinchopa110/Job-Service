#pragma once

#include "domain/Worker.hpp"
#include <unordered_map>
#include <shared_mutex>
#include <vector>
#include <optional>

class WorkerRepository {
public:
    WorkerRepository();
    ~WorkerRepository();

    size_t add(const Worker& worker);
    bool update(const Worker& worker);
    bool remove(size_t id);
    std::optional<Worker> findById(size_t id) const;
    std::vector<Worker> findAll() const;
    std::vector<Worker> findByCity(const std::string& city) const;
    std::vector<Worker> findBySkill(const std::string& skill) const;

private:
    std::unordered_map<size_t, Worker> workers_;
    mutable std::shared_mutex mutex_;
    size_t nextId_;
};