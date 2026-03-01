#pragma once

#include "domain/Employer.hpp"
#include "repository/EmployerRepository.hpp"
#include <vector>
#include <optional>

class EmployerService {
public:
    EmployerService();
    ~EmployerService();

    size_t createEmployer(const std::string& companyName, const std::string& email,
                          const std::string& description, const std::string& city);
    
    bool updateEmployer(const Employer& employer);
    std::vector<Employer> getAllEmployers() const;

private:
    EmployerRepository employerRepository_;
};