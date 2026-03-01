#include "domain/Worker.hpp"

Worker::Worker()
    : id_(0), experience_(0) {}

Worker::Worker(size_t id, const std::string& name, const std::string& email,
               const std::vector<std::string>& skills, int experience,
               const std::string& city, const std::string& resume)
    : id_(id), name_(name), email_(email), skills_(skills),
      experience_(experience), city_(city), resume_(resume) {}

size_t Worker::getId() const {
    return id_;
}

void Worker::setId(size_t id) {
    id_ = id;
}

std::string Worker::getName() const {
    return name_;
}

void Worker::setName(const std::string& name) {
    name_ = name;
}

std::string Worker::getEmail() const {
    return email_;
}

void Worker::setEmail(const std::string& email) {
    email_ = email;
}

std::vector<std::string> Worker::getSkills() const {
    return skills_;
}

void Worker::setSkills(const std::vector<std::string>& skills) {
    skills_ = skills;
}

int Worker::getExperience() const {
    return experience_;
}

void Worker::setExperience(int experience) {
    experience_ = experience;
}

std::string Worker::getCity() const {
    return city_;
}

void Worker::setCity(const std::string& city) {
    city_ = city;
}

std::string Worker::getResume() const {
    return resume_;
}

void Worker::setResume(const std::string& resume) {
    resume_ = resume;
}

bool Worker::operator==(const Worker& other) const {
    return id_ == other.id_;
}

bool Worker::operator!=(const Worker& other) const {
    return !(*this == other);
}