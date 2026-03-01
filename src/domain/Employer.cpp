#include "domain/Employer.hpp"

Employer::Employer()
    : id_(0) {}

Employer::Employer(size_t id, const std::string& companyName, const std::string& email,
                   const std::string& description, const std::string& city)
    : id_(id), companyName_(companyName), email_(email),
      description_(description), city_(city) {}

size_t Employer::getId() const {
    return id_;
}

void Employer::setId(size_t id) {
    id_ = id;
}

std::string Employer::getCompanyName() const {
    return companyName_;
}

void Employer::setCompanyName(const std::string& companyName) {
    companyName_ = companyName;
}

std::string Employer::getEmail() const {
    return email_;
}

void Employer::setEmail(const std::string& email) {
    email_ = email;
}

std::string Employer::getDescription() const {
    return description_;
}

void Employer::setDescription(const std::string& description) {
    description_ = description;
}

std::string Employer::getCity() const {
    return city_;
}

void Employer::setCity(const std::string& city) {
    city_ = city;
}

bool Employer::operator==(const Employer& other) const {
    return id_ == other.id_;
}

bool Employer::operator!=(const Employer& other) const {
    return !(*this == other);
}