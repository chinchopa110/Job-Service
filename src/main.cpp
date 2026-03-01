#include "JobService.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

void workerThread(JobService& service, int workerId) {
    std::string name = "Worker" + std::to_string(workerId);
    std::string email = "worker" + std::to_string(workerId) + "@email.com";
    std::vector<std::string> skills = {"C++", "Python", "Java"};
    std::string city = "Moscow";
    std::string resume = "Resume for " + name;
    
    size_t id = service.createWorker(name, email, skills, workerId, city, resume);
    std::cout << "Created worker: " << name << " with ID: " << id << std::endl;
    
    auto worker = service.getWorker(id);
    if (worker.has_value()) {
        std::cout << "Retrieved worker: " << worker.value().getName() << std::endl;
    }
}

void employerThread(JobService& service, int employerId) {
    std::string companyName = "Company" + std::to_string(employerId);
    std::string email = "company" + std::to_string(employerId) + "@email.com";
    std::string description = "Description for " + companyName;
    std::string city = "Saint Petersburg";
    
    size_t id = service.createEmployer(companyName, email, description, city);
    std::cout << "Created employer: " << companyName << " with ID: " << id << std::endl;
    
    auto employer = service.getEmployer(id);
    if (employer.has_value()) {
        std::cout << "Retrieved employer: " << employer.value().getCompanyName() << std::endl;
    }
}

void jobPostingThread(JobService& service, size_t employerId, int jobId) {
    std::string title = "Job Title " + std::to_string(jobId);
    std::string description = "Job Description " + std::to_string(jobId);
    int salary = 50000 + jobId * 10000;
    std::string city = "Moscow";
    bool isRemote = jobId % 2 == 0;
    std::string profession = "Software Engineer";
    
    size_t id = service.createJobPosting(employerId, title, description, salary, city, isRemote, profession);
    std::cout << "Created job posting: " << title << " with ID: " << id << std::endl;
    
    auto job = service.getJobPosting(id);
    if (job.has_value()) {
        std::cout << "Retrieved job: " << job.value().getTitle() << " Salary: " << job.value().getSalary() << std::endl;
    }
}

void replyThread(JobService& service, size_t jobPostingId, size_t workerId) {
    size_t id = service.createReply(jobPostingId, workerId);
    std::cout << "Created reply for job " << jobPostingId << " by worker " << workerId << " with ID: " << id << std::endl;
    
    auto reply = service.getReply(id);
    if (reply.has_value()) {
        std::cout << "Retrieved reply with status: " << static_cast<int>(reply.value().getStatus()) << std::endl;
    }
}

void searchJobsThread(JobService& service, int threadId) {
    JobFilter filter;
    filter.setMinSalary(60000);
    filter.setMaxSalary(80000);
    filter.setCity("Moscow");
    filter.setIsRemote(true);
    
    auto jobs = service.searchJobPostings(filter);
    std::cout << "Thread " << threadId << " found " << jobs.size() << " jobs matching filter" << std::endl;
}

int main() {
    std::cout << "=== Job Posting Service - Multithreaded Demo ===" << std::endl;
    
    JobService service;
    
    std::cout << "\n--- Creating Workers ---" << std::endl;
    std::vector<std::thread> workerThreads;
    for (int i = 1; i <= 5; ++i) {
        workerThreads.emplace_back(workerThread, std::ref(service), i);
    }
    
    for (auto& t : workerThreads) {
        t.join();
    }
    
    std::cout << "\n--- Creating Employers ---" << std::endl;
    std::vector<std::thread> employerThreads;
    for (int i = 1; i <= 3; ++i) {
        employerThreads.emplace_back(employerThread, std::ref(service), i);
    }
    
    for (auto& t : employerThreads) {
        t.join();
    }
    
    std::cout << "\n--- Creating Job Postings ---" << std::endl;
    std::vector<std::thread> jobThreads;
    for (int i = 1; i <= 10; ++i) {
        size_t employerId = (i % 3) + 1;
        jobThreads.emplace_back(jobPostingThread, std::ref(service), employerId, i);
    }
    
    for (auto& t : jobThreads) {
        t.join();
    }
    
    std::cout << "\n--- Creating Replies ---" << std::endl;
    std::vector<std::thread> replyThreads;
    for (int i = 1; i <= 10; ++i) {
        size_t workerId = (i % 5) + 1;
        size_t jobPostingId = i;
        replyThreads.emplace_back(replyThread, std::ref(service), jobPostingId, workerId);
    }
    
    for (auto& t : replyThreads) {
        t.join();
    }
    
    std::cout << "\n--- Searching Jobs (Concurrent) ---" << std::endl;
    std::vector<std::thread> searchThreads;
    for (int i = 1; i <= 5; ++i) {
        searchThreads.emplace_back(searchJobsThread, std::ref(service), i);
    }
    
    for (auto& t : searchThreads) {
        t.join();
    }
    
    std::cout << "\n--- Summary ---" << std::endl;
    std::cout << "Total Workers: " << service.getAllWorkers().size() << std::endl;
    std::cout << "Total Employers: " << service.getAllEmployers().size() << std::endl;
    std::cout << "Total Job Postings: " << service.getAllJobPostings().size() << std::endl;
    std::cout << "Total Replies: " << service.getAllReplies().size() << std::endl;
    
    std::cout << "\n--- Job Postings by Employer 1 ---" << std::endl;
    auto employer1Jobs = service.getJobPostingsByEmployer(1);
    std::cout << "Employer 1 has " << employer1Jobs.size() << " job postings" << std::endl;
    
    std::cout << "\n--- Replies by Worker 1 ---" << std::endl;
    auto worker1Replies = service.getRepliesByWorker(1);
    std::cout << "Worker 1 has " << worker1Replies.size() << " replies" << std::endl;
    
    std::cout << "\n--- Replies for Job Posting 1 ---" << std::endl;
    auto job1Replies = service.getRepliesByJobPosting(1);
    std::cout << "Job Posting 1 has " << job1Replies.size() << " replies" << std::endl;
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    
    return 0;
}