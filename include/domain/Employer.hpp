#pragma once

#include <string>
#include <cstdint>

class Employer {
public:
    Employer();
    Employer(size_t id, const std::string& companyName, const std::string& email,
              const std::string& description, const std::string& city);

    size_t getId() const;
    void setId(size_t id);

    std::string getCompanyName() const;
    void setCompanyName(const std::string& companyName);

    std::string getEmail() const;
    void setEmail(const std::string& email);

    std::string getDescription() const;
    void setDescription(const std::string& description);

    std::string getCity() const;
    void setCity(const std::string& city);

    bool operator==(const Employer& other) const;
    bool operator!=(const Employer& other) const;

private:
    size_t id_;
    std::string companyName_;
    std::string email_;
    std::string description_;
    std::string city_;
};