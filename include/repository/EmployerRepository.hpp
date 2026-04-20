#pragma once

#include "domain/Employer.hpp"
#include <unordered_map>
#include <shared_mutex>
#include <vector>
#include <optional>

class EmployerRepository {
public:
    EmployerRepository();
    ~EmployerRepository();

    size_t add(const Employer& employer);
    bool update(const Employer& employer);
    bool remove(size_t id);
    std::optional<Employer> findById(size_t id) const;
    std::optional<Employer> findByEmail(const std::string& email) const;
    std::vector<Employer> findAll() const;
    std::vector<Employer> findByCity(const std::string& city) const;

private:
    std::unordered_map<size_t, Employer> employers_;
    mutable std::shared_mutex mutex_;
    size_t nextId_;
};