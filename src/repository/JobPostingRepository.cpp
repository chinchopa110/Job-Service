#include "repository/JobPostingRepository.hpp"
#include "utils/LockGuard.hpp"

JobPostingRepository::JobPostingRepository()
    : nextId_(1) {}

JobPostingRepository::~JobPostingRepository() {}

size_t JobPostingRepository::add(const JobPosting& jobPosting) {
    LockGuard lock(mutex_);
    size_t id = nextId_++;
    JobPosting newJobPosting = jobPosting;
    newJobPosting.setId(id);
    jobPostings_[id] = newJobPosting;
    return id;
}

bool JobPostingRepository::update(const JobPosting& jobPosting) {
    LockGuard lock(mutex_);
    auto it = jobPostings_.find(jobPosting.getId());
    if (it != jobPostings_.end()) {
        jobPostings_[jobPosting.getId()] = jobPosting;
        return true;
    }
    return false;
}

bool JobPostingRepository::remove(size_t id) {
    LockGuard lock(mutex_);
    return jobPostings_.erase(id) > 0;
}

std::optional<JobPosting> JobPostingRepository::findById(size_t id) const {
    SharedLockGuard lock(mutex_);
    auto it = jobPostings_.find(id);
    if (it != jobPostings_.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::vector<JobPosting> JobPostingRepository::findAll() const {
    SharedLockGuard lock(mutex_);
    std::vector<JobPosting> result;
    result.reserve(jobPostings_.size());
    for (const auto& pair : jobPostings_) {
        result.push_back(pair.second);
    }
    return result;
}

std::vector<JobPosting> JobPostingRepository::findByEmployerId(size_t employerId) const {
    SharedLockGuard lock(mutex_);
    std::vector<JobPosting> result;
    for (const auto& pair : jobPostings_) {
        if (pair.second.getEmployerId() == employerId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<JobPosting> JobPostingRepository::findByCity(const std::string& city) const {
    SharedLockGuard lock(mutex_);
    std::vector<JobPosting> result;
    for (const auto& pair : jobPostings_) {
        if (pair.second.getCity() == city) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<JobPosting> JobPostingRepository::findByProfession(const std::string& profession) const {
    SharedLockGuard lock(mutex_);
    std::vector<JobPosting> result;
    for (const auto& pair : jobPostings_) {
        if (pair.second.getProfession() == profession) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<JobPosting> JobPostingRepository::findByRemote(bool isRemote) const {
    SharedLockGuard lock(mutex_);
    std::vector<JobPosting> result;
    for (const auto& pair : jobPostings_) {
        if (pair.second.isRemote() == isRemote) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<JobPosting> JobPostingRepository::findBySalaryRange(int minSalary, int maxSalary) const {
    SharedLockGuard lock(mutex_);
    std::vector<JobPosting> result;
    for (const auto& pair : jobPostings_) {
        int salary = pair.second.getSalary();
        if (salary >= minSalary && salary <= maxSalary) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<JobPosting> JobPostingRepository::findByDateRange(
    const std::chrono::system_clock::time_point& startDate,
    const std::chrono::system_clock::time_point& endDate) const {
    SharedLockGuard lock(mutex_);
    std::vector<JobPosting> result;
    for (const auto& pair : jobPostings_) {
        const auto& publishDate = pair.second.getPublishDate();
        if (publishDate >= startDate && publishDate <= endDate) {
            result.push_back(pair.second);
        }
    }
    return result;
}