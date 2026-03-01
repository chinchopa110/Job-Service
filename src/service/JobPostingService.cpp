#include "service/JobPostingService.hpp"

JobPostingService::JobPostingService() {}

JobPostingService::~JobPostingService() {}

size_t JobPostingService::createJobPosting(size_t employerId, const std::string& title,
                                           const std::string& description, int salary,
                                           const std::string& city, bool isRemote,
                                           const std::string& profession) {
    JobPosting jobPosting(0, employerId, title, description, salary, city, isRemote,
                          std::chrono::system_clock::now(), profession);
    return jobPostingRepository_.add(jobPosting);
}

bool JobPostingService::updateJobPosting(const JobPosting& jobPosting) {
    return jobPostingRepository_.update(jobPosting);
}

bool JobPostingService::deleteJobPosting(size_t id) {
    return jobPostingRepository_.remove(id);
}

std::optional<JobPosting> JobPostingService::getJobPosting(size_t id) const {
    return jobPostingRepository_.findById(id);
}

std::vector<JobPosting> JobPostingService::getAllJobPostings() const {
    return jobPostingRepository_.findAll();
}

std::vector<JobPosting> JobPostingService::getJobPostingsByEmployer(size_t employerId) const {
    return jobPostingRepository_.findByEmployerId(employerId);
}

std::vector<JobPosting> JobPostingService::searchJobPostings(const JobFilter& filter) const {
    std::vector<JobPosting> result = jobPostingRepository_.findAll();
    
    if (!filter.hasFilters()) {
        return result;
    }
    
    std::vector<JobPosting> filtered;
    for (const auto& job : result) {
        bool matches = true;
        
        if (filter.getMinSalary().has_value() && job.getSalary() < filter.getMinSalary().value()) {
            matches = false;
        }
        
        if (filter.getMaxSalary().has_value() && job.getSalary() > filter.getMaxSalary().value()) {
            matches = false;
        }
        
        if (filter.getCity().has_value() && job.getCity() != filter.getCity().value()) {
            matches = false;
        }
        
        if (filter.getIsRemote().has_value() && job.isRemote() != filter.getIsRemote().value()) {
            matches = false;
        }
        
        if (filter.getStartDate().has_value() && job.getPublishDate() < filter.getStartDate().value()) {
            matches = false;
        }
        
        if (filter.getEndDate().has_value() && job.getPublishDate() > filter.getEndDate().value()) {
            matches = false;
        }
        
        if (filter.getProfession().has_value() && job.getProfession() != filter.getProfession().value()) {
            matches = false;
        }
        
        if (matches) {
            filtered.push_back(job);
        }
    }
    
    return filtered;
}