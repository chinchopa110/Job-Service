#pragma once

#include <string>
#include <cstdint>
#include <chrono>

class JobPosting {
public:
    JobPosting();
    JobPosting(size_t id, size_t employerId, const std::string& title,
               const std::string& description, int salary, const std::string& city,
               bool isRemote, const std::chrono::system_clock::time_point& publishDate,
               const std::string& profession);

    size_t getId() const;
    void setId(size_t id);

    size_t getEmployerId() const;
    void setEmployerId(size_t employerId);

    std::string getTitle() const;
    void setTitle(const std::string& title);

    std::string getDescription() const;
    void setDescription(const std::string& description);

    int getSalary() const;
    void setSalary(int salary);

    std::string getCity() const;
    void setCity(const std::string& city);

    bool isRemote() const;
    void setRemote(bool remote);

    std::chrono::system_clock::time_point getPublishDate() const;
    void setPublishDate(const std::chrono::system_clock::time_point& publishDate);

    std::string getProfession() const;
    void setProfession(const std::string& profession);

    bool operator==(const JobPosting& other) const;
    bool operator!=(const JobPosting& other) const;

private:
    size_t id_;
    size_t employerId_;
    std::string title_;
    std::string description_;
    int salary_;
    std::string city_;
    bool isRemote_;
    std::chrono::system_clock::time_point publishDate_;
    std::string profession_;
};