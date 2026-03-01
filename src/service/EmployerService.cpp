#include "service/EmployerService.hpp"

EmployerService::EmployerService() {}

EmployerService::~EmployerService() {}

size_t EmployerService::createEmployer(const std::string& companyName, const std::string& email,
                                       const std::string& description, const std::string& city) {
    Employer employer(0, companyName, email, description, city);
    return employerRepository_.add(employer);
}

bool EmployerService::updateEmployer(const Employer& employer) {
    return employerRepository_.update(employer);
}

bool EmployerService::deleteEmployer(size_t id) {
    return employerRepository_.remove(id);
}

std::optional<Employer> EmployerService::getEmployer(size_t id) const {
    return employerRepository_.findById(id);
}

std::vector<Employer> EmployerService::getAllEmployers() const {
    return employerRepository_.findAll();
}

std::vector<Employer> EmployerService::getEmployersByCity(const std::string& city) const {
    return employerRepository_.findByCity(city);
}