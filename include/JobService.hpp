#pragma once

#include "service/WorkerService.hpp"
#include "service/EmployerService.hpp"
#include "service/JobPostingService.hpp"
#include "service/ReplyService.hpp"
#include "filter/JobFilter.hpp"
#include "domain/Worker.hpp"
#include "domain/Employer.hpp"
#include "domain/JobPosting.hpp"
#include "domain/Reply.hpp"
#include <vector>
#include <optional>
#include <chrono>

class JobService {
public:
    JobService();
    ~JobService();

    size_t createWorker(const std::string& name, const std::string& email,
                        const std::vector<std::string>& skills, int experience,
                        const std::string& city, const std::string& resume);
    
    bool updateWorker(const Worker& worker);

    size_t createEmployer(const std::string& companyName, const std::string& email,
                          const std::string& description, const std::string& city);
    
    bool updateEmployer(const Employer& employer);
    std::vector<Employer> getAllEmployers() const;

    size_t createJobPosting(size_t employerId, const std::string& title,
                            const std::string& description, int salary,
                            const std::string& city, bool isRemote,
                            const std::string& profession);
    
    bool deleteJobPosting(size_t id);
    std::vector<JobPosting> getJobPostingsByEmployer(size_t employerId) const;
    std::vector<JobPosting> searchJobPostings(const JobFilter& filter) const;

    size_t createReply(size_t jobPostingId, size_t workerId);
    std::vector<Reply> getRepliesByJobPosting(size_t jobPostingId) const;

private:
    WorkerService workerService_;
    EmployerService employerService_;
    JobPostingService jobPostingService_;
    ReplyService replyService_;
};