#include "service/EmployerService.hpp"

EmployerService::EmployerService() {}

EmployerService::~EmployerService() {}

size_t EmployerService::createEmployer(const std::string& companyName, const std::string& email,
                                       const std::string& description, const std::string& city) {
    auto existing = employerRepository_.findByEmail(email);
    if (existing.has_value()) {
        return existing->getId();
    }
    Employer employer(0, companyName, email, description, city);
    return employerRepository_.add(employer);
}

bool EmployerService::updateEmployer(const Employer& employer) {
    return employerRepository_.update(employer);
}

std::vector<Employer> EmployerService::getAllEmployers() const {
    return employerRepository_.findAll();
}