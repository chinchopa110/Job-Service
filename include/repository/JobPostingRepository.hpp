#pragma once

#include "domain/JobPosting.hpp"
#include <unordered_map>
#include <shared_mutex>
#include <vector>
#include <optional>
#include <chrono>

class JobPostingRepository {
public:
    JobPostingRepository();
    ~JobPostingRepository();

    size_t add(const JobPosting& jobPosting);
    bool update(const JobPosting& jobPosting);
    bool remove(size_t id);
    std::optional<JobPosting> findById(size_t id) const;
    std::optional<JobPosting> findByEmployerAndTitle(size_t employerId, const std::string& title, const std::string& profession) const;
    std::vector<JobPosting> findAll() const;
    std::vector<JobPosting> findByEmployerId(size_t employerId) const;
    std::vector<JobPosting> findByCity(const std::string& city) const;
    std::vector<JobPosting> findByProfession(const std::string& profession) const;
    std::vector<JobPosting> findByRemote(bool isRemote) const;
    std::vector<JobPosting> findBySalaryRange(int minSalary, int maxSalary) const;
    std::vector<JobPosting> findByDateRange(
        const std::chrono::system_clock::time_point& startDate,
        const std::chrono::system_clock::time_point& endDate) const;

private:
    std::unordered_map<size_t, JobPosting> jobPostings_;
    mutable std::shared_mutex mutex_;
    size_t nextId_;
};