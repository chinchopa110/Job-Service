#include "filter/JobFilter.hpp"

JobFilter::JobFilter() {}

std::optional<int> JobFilter::getMinSalary() const {
    return minSalary_;
}

void JobFilter::setMinSalary(int minSalary) {
    minSalary_ = minSalary;
}

std::optional<int> JobFilter::getMaxSalary() const {
    return maxSalary_;
}

void JobFilter::setMaxSalary(int maxSalary) {
    maxSalary_ = maxSalary;
}

std::optional<std::string> JobFilter::getCity() const {
    return city_;
}

void JobFilter::setCity(const std::string& city) {
    city_ = city;
}

std::optional<bool> JobFilter::getIsRemote() const {
    return isRemote_;
}

void JobFilter::setIsRemote(bool isRemote) {
    isRemote_ = isRemote;
}

std::optional<std::chrono::system_clock::time_point> JobFilter::getStartDate() const {
    return startDate_;
}

void JobFilter::setStartDate(const std::chrono::system_clock::time_point& startDate) {
    startDate_ = startDate;
}

std::optional<std::chrono::system_clock::time_point> JobFilter::getEndDate() const {
    return endDate_;
}

void JobFilter::setEndDate(const std::chrono::system_clock::time_point& endDate) {
    endDate_ = endDate;
}

std::optional<std::string> JobFilter::getProfession() const {
    return profession_;
}

void JobFilter::setProfession(const std::string& profession) {
    profession_ = profession;
}

bool JobFilter::hasFilters() const {
    return minSalary_.has_value() || maxSalary_.has_value() ||
           city_.has_value() || isRemote_.has_value() ||
           startDate_.has_value() || endDate_.has_value() ||
           profession_.has_value();
}