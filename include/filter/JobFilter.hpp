#pragma once

#include <optional>
#include <string>
#include <chrono>

class JobFilter {
public:
    JobFilter();

    std::optional<int> getMinSalary() const;
    void setMinSalary(int minSalary);

    std::optional<int> getMaxSalary() const;
    void setMaxSalary(int maxSalary);

    std::optional<std::string> getCity() const;
    void setCity(const std::string& city);

    std::optional<bool> getIsRemote() const;
    void setIsRemote(bool isRemote);

    std::optional<std::chrono::system_clock::time_point> getStartDate() const;
    void setStartDate(const std::chrono::system_clock::time_point& startDate);

    std::optional<std::chrono::system_clock::time_point> getEndDate() const;
    void setEndDate(const std::chrono::system_clock::time_point& endDate);

    std::optional<std::string> getProfession() const;
    void setProfession(const std::string& profession);

    bool hasFilters() const;

private:
    std::optional<int> minSalary_;
    std::optional<int> maxSalary_;
    std::optional<std::string> city_;
    std::optional<bool> isRemote_;
    std::optional<std::chrono::system_clock::time_point> startDate_;
    std::optional<std::chrono::system_clock::time_point> endDate_;
    std::optional<std::string> profession_;
};