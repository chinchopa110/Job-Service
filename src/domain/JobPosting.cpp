#include "domain/JobPosting.hpp"

JobPosting::JobPosting()
    : id_(0), employerId_(0), salary_(0), isRemote_(false),
      publishDate_(std::chrono::system_clock::now()) {}

JobPosting::JobPosting(size_t id, size_t employerId, const std::string& title,
                       const std::string& description, int salary, const std::string& city,
                       bool isRemote, const std::chrono::system_clock::time_point& publishDate,
                       const std::string& profession)
    : id_(id), employerId_(employerId), title_(title), description_(description),
      salary_(salary), city_(city), isRemote_(isRemote), publishDate_(publishDate),
      profession_(profession) {}

size_t JobPosting::getId() const {
    return id_;
}

void JobPosting::setId(size_t id) {
    id_ = id;
}

size_t JobPosting::getEmployerId() const {
    return employerId_;
}

void JobPosting::setEmployerId(size_t employerId) {
    employerId_ = employerId;
}

std::string JobPosting::getTitle() const {
    return title_;
}

void JobPosting::setTitle(const std::string& title) {
    title_ = title;
}

std::string JobPosting::getDescription() const {
    return description_;
}

void JobPosting::setDescription(const std::string& description) {
    description_ = description;
}

int JobPosting::getSalary() const {
    return salary_;
}

void JobPosting::setSalary(int salary) {
    salary_ = salary;
}

std::string JobPosting::getCity() const {
    return city_;
}

void JobPosting::setCity(const std::string& city) {
    city_ = city;
}

bool JobPosting::isRemote() const {
    return isRemote_;
}

void JobPosting::setRemote(bool remote) {
    isRemote_ = remote;
}

std::chrono::system_clock::time_point JobPosting::getPublishDate() const {
    return publishDate_;
}

void JobPosting::setPublishDate(const std::chrono::system_clock::time_point& publishDate) {
    publishDate_ = publishDate;
}

std::string JobPosting::getProfession() const {
    return profession_;
}

void JobPosting::setProfession(const std::string& profession) {
    profession_ = profession;
}

bool JobPosting::operator==(const JobPosting& other) const {
    return id_ == other.id_;
}

bool JobPosting::operator!=(const JobPosting& other) const {
    return !(*this == other);
}