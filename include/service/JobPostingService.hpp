#pragma once

#include "domain/JobPosting.hpp"
#include "repository/JobPostingRepository.hpp"
#include "filter/JobFilter.hpp"
#include <vector>
#include <optional>
#include <chrono>

class JobPostingService {
public:
    JobPostingService();
    ~JobPostingService();

    size_t createJobPosting(size_t employerId, const std::string& title,
                            const std::string& description, int salary,
                            const std::string& city, bool isRemote,
                            const std::string& profession);
    
    bool deleteJobPosting(size_t id);
    std::vector<JobPosting> getJobPostingsByEmployer(size_t employerId) const;
    std::vector<JobPosting> searchJobPostings(const JobFilter& filter) const;

private:
    JobPostingRepository jobPostingRepository_;
};