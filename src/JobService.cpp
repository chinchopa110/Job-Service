#include "JobService.hpp"

JobService::JobService() {}

JobService::~JobService() {}

size_t JobService::createWorker(const std::string& name, const std::string& email,
                                const std::vector<std::string>& skills, int experience,
                                const std::string& city, const std::string& resume) {
    return workerService_.createWorker(name, email, skills, experience, city, resume);
}

bool JobService::updateWorker(const Worker& worker) {
    return workerService_.updateWorker(worker);
}

size_t JobService::createEmployer(const std::string& companyName, const std::string& email,
                                  const std::string& description, const std::string& city) {
    return employerService_.createEmployer(companyName, email, description, city);
}

bool JobService::updateEmployer(const Employer& employer) {
    return employerService_.updateEmployer(employer);
}

std::vector<Employer> JobService::getAllEmployers() const {
    return employerService_.getAllEmployers();
}

size_t JobService::createJobPosting(size_t employerId, const std::string& title,
                                    const std::string& description, int salary,
                                    const std::string& city, bool isRemote,
                                    const std::string& profession) {
    return jobPostingService_.createJobPosting(employerId, title, description, salary, city, isRemote, profession);
}

bool JobService::deleteJobPosting(size_t id) {
    return jobPostingService_.deleteJobPosting(id);
}

std::vector<JobPosting> JobService::getJobPostingsByEmployer(size_t employerId) const {
    return jobPostingService_.getJobPostingsByEmployer(employerId);
}

std::vector<JobPosting> JobService::searchJobPostings(const JobFilter& filter) const {
    return jobPostingService_.searchJobPostings(filter);
}

size_t JobService::createReply(size_t jobPostingId, size_t workerId) {
    return replyService_.createReply(jobPostingId, workerId);
}

std::vector<Reply> JobService::getRepliesByJobPosting(size_t jobPostingId) const {
    return replyService_.getRepliesByJobPosting(jobPostingId);
}