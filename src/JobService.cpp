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

bool JobService::deleteWorker(size_t id) {
    return workerService_.deleteWorker(id);
}

std::optional<Worker> JobService::getWorker(size_t id) const {
    return workerService_.getWorker(id);
}

std::vector<Worker> JobService::getAllWorkers() const {
    return workerService_.getAllWorkers();
}

std::vector<Worker> JobService::getWorkersByCity(const std::string& city) const {
    return workerService_.getWorkersByCity(city);
}

std::vector<Worker> JobService::getWorkersBySkill(const std::string& skill) const {
    return workerService_.getWorkersBySkill(skill);
}

size_t JobService::createEmployer(const std::string& companyName, const std::string& email,
                                  const std::string& description, const std::string& city) {
    return employerService_.createEmployer(companyName, email, description, city);
}

bool JobService::updateEmployer(const Employer& employer) {
    return employerService_.updateEmployer(employer);
}

bool JobService::deleteEmployer(size_t id) {
    return employerService_.deleteEmployer(id);
}

std::optional<Employer> JobService::getEmployer(size_t id) const {
    return employerService_.getEmployer(id);
}

std::vector<Employer> JobService::getAllEmployers() const {
    return employerService_.getAllEmployers();
}

std::vector<Employer> JobService::getEmployersByCity(const std::string& city) const {
    return employerService_.getEmployersByCity(city);
}

size_t JobService::createJobPosting(size_t employerId, const std::string& title,
                                    const std::string& description, int salary,
                                    const std::string& city, bool isRemote,
                                    const std::string& profession) {
    return jobPostingService_.createJobPosting(employerId, title, description, salary, city, isRemote, profession);
}

bool JobService::updateJobPosting(const JobPosting& jobPosting) {
    return jobPostingService_.updateJobPosting(jobPosting);
}

bool JobService::deleteJobPosting(size_t id) {
    return jobPostingService_.deleteJobPosting(id);
}

std::optional<JobPosting> JobService::getJobPosting(size_t id) const {
    return jobPostingService_.getJobPosting(id);
}

std::vector<JobPosting> JobService::getAllJobPostings() const {
    return jobPostingService_.getAllJobPostings();
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

bool JobService::updateReply(const Reply& reply) {
    return replyService_.updateReply(reply);
}

bool JobService::deleteReply(size_t id) {
    return replyService_.deleteReply(id);
}

std::optional<Reply> JobService::getReply(size_t id) const {
    return replyService_.getReply(id);
}

std::vector<Reply> JobService::getAllReplies() const {
    return replyService_.getAllReplies();
}

std::vector<Reply> JobService::getRepliesByJobPosting(size_t jobPostingId) const {
    return replyService_.getRepliesByJobPosting(jobPostingId);
}

std::vector<Reply> JobService::getRepliesByWorker(size_t workerId) const {
    return replyService_.getRepliesByWorker(workerId);
}

std::vector<Reply> JobService::getRepliesByStatus(ReplyStatus status) const {
    return replyService_.getRepliesByStatus(status);
}

bool JobService::hasWorkerReplied(size_t jobPostingId, size_t workerId) const {
    return replyService_.hasWorkerReplied(jobPostingId, workerId);
}