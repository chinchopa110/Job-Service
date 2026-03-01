#pragma once

#include <string>
#include <vector>
#include <cstdint>

class Worker {
public:
    Worker();
    Worker(size_t id, const std::string& name, const std::string& email, 
           const std::vector<std::string>& skills, int experience, 
           const std::string& city, const std::string& resume);

    size_t getId() const;
    void setId(size_t id);

    std::string getName() const;
    void setName(const std::string& name);

    std::string getEmail() const;
    void setEmail(const std::string& email);

    std::vector<std::string> getSkills() const;
    void setSkills(const std::vector<std::string>& skills);

    int getExperience() const;
    void setExperience(int experience);

    std::string getCity() const;
    void setCity(const std::string& city);

    std::string getResume() const;
    void setResume(const std::string& resume);

    bool operator==(const Worker& other) const;
    bool operator!=(const Worker& other) const;

private:
    size_t id_;
    std::string name_;
    std::string email_;
    std::vector<std::string> skills_;
    int experience_;
    std::string city_;
    std::string resume_;
};