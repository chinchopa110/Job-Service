#include "../include/service/WorkerService.hpp"
#include "../include/service/EmployerService.hpp"
#include "../include/service/JobPostingService.hpp"
#include "../include/service/ReplyService.hpp"
#include "../include/JobService.hpp"
#include "../include/filter/JobFilter.hpp"
#include <iostream>
#include <cassert>
#include <vector>

int testsPassed = 0;
int testsFailed = 0;

#define TEST(name) \
    void name(); \
    void run_##name() { \
        std::cout << "Running test: " << #name << "... "; \
        try { \
            name(); \
            std::cout << "PASSED" << std::endl; \
            testsPassed++; \
        } catch (const std::exception& e) { \
            std::cout << "FAILED: " << e.what() << std::endl; \
            testsFailed++; \
        } \
    } \
    void name()

#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        throw std::runtime_error("Assertion failed: " #condition); \
    }

#define ASSERT_FALSE(condition) \
    if (condition) { \
        throw std::runtime_error("Assertion failed: !" #condition); \
    }

#define ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        throw std::runtime_error("Assertion failed: " #expected " != " #actual); \
    }

#define ASSERT_NE(expected, actual) \
    if ((expected) == (actual)) { \
        throw std::runtime_error("Assertion failed: " #expected " == " #actual); \
    }

TEST(test_createWorker) {
    WorkerService service;
    std::vector<std::string> skills = {"C++", "Python"};
    size_t id = service.createWorker("John Doe", "john@email.com", skills, 5, "Moscow", "Resume");
    ASSERT_NE(0, id);
}

TEST(test_updateWorker) {
    WorkerService service;
    std::vector<std::string> skills = {"C++"};
    size_t id = service.createWorker("John", "john@email.com", skills, 5, "Moscow", "Resume");
    
    Worker worker(id, "John Updated", "john.updated@email.com", {"C++", "Java"}, 6, "Saint Petersburg", "Updated Resume");
    bool result = service.updateWorker(worker);
    ASSERT_TRUE(result);
}

TEST(test_createEmployer) {
    EmployerService service;
    size_t id = service.createEmployer("Tech Corp", "info@techcorp.com", "Description", "Moscow");
    ASSERT_NE(0, id);
}

TEST(test_updateEmployer) {
    EmployerService service;
    size_t id = service.createEmployer("Tech Corp", "info@techcorp.com", "Description", "Moscow");
    
    Employer employer(id, "Tech Corp Updated", "updated@techcorp.com", "Updated Description", "Saint Petersburg");
    bool result = service.updateEmployer(employer);
    ASSERT_TRUE(result);
}

TEST(test_getAllEmployers) {
    EmployerService service;
    service.createEmployer("Company 1", "c1@email.com", "Desc 1", "Moscow");
    service.createEmployer("Company 2", "c2@email.com", "Desc 2", "Saint Petersburg");
    
    auto employers = service.getAllEmployers();
    ASSERT_EQ(2, employers.size());
}

// JobPostingService Tests
TEST(test_createJobPosting) {
    JobPostingService service;
    size_t id = service.createJobPosting(1, "Developer", "Job description", 100000, "Moscow", true, "Software Engineer");
    ASSERT_NE(0, id);
}

TEST(test_deleteJobPosting) {
    JobPostingService service;
    size_t id = service.createJobPosting(1, "Developer", "Job description", 100000, "Moscow", true, "Software Engineer");
    
    bool result = service.deleteJobPosting(id);
    ASSERT_TRUE(result);
}

TEST(test_getJobPostingsByEmployer) {
    JobPostingService service;
    service.createJobPosting(1, "Job 1", "Desc 1", 50000, "Moscow", false, "Engineer");
    service.createJobPosting(1, "Job 2", "Desc 2", 60000, "Moscow", true, "Engineer");
    service.createJobPosting(2, "Job 3", "Desc 3", 70000, "Saint Petersburg", false, "Manager");
    
    auto jobs = service.getJobPostingsByEmployer(1);
    ASSERT_EQ(2, jobs.size());
}

TEST(test_searchJobPostings_withFilters) {
    JobPostingService service;
    service.createJobPosting(1, "Job 1", "Desc 1", 50000, "Moscow", false, "Engineer");
    service.createJobPosting(1, "Job 2", "Desc 2", 60000, "Moscow", true, "Engineer");
    service.createJobPosting(1, "Job 3", "Desc 3", 70000, "Moscow", true, "Engineer");
    
    JobFilter filter;
    filter.setMinSalary(55000);
    filter.setIsRemote(true);
    
    auto jobs = service.searchJobPostings(filter);
    ASSERT_EQ(2, jobs.size());
}

TEST(test_searchJobPostings_noFilters) {
    JobPostingService service;
    service.createJobPosting(1, "Job 1", "Desc 1", 50000, "Moscow", false, "Engineer");
    service.createJobPosting(1, "Job 2", "Desc 2", 60000, "Moscow", true, "Engineer");
    
    JobFilter filter;
    auto jobs = service.searchJobPostings(filter);
    ASSERT_EQ(2, jobs.size());
}

TEST(test_createReply) {
    ReplyService service;
    size_t id = service.createReply(1, 1);
    ASSERT_NE(0, id);
}

TEST(test_getRepliesByJobPosting) {
    ReplyService service;
    service.createReply(1, 1);
    service.createReply(1, 2);
    service.createReply(2, 1);
    
    auto replies = service.getRepliesByJobPosting(1);
    ASSERT_EQ(2, replies.size());
}

TEST(test_jobService_createWorker) {
    JobService service;
    std::vector<std::string> skills = {"C++"};
    size_t id = service.createWorker("John", "john@email.com", skills, 5, "Moscow", "Resume");
    ASSERT_NE(0, id);
}

TEST(test_jobService_createEmployer) {
    JobService service;
    size_t id = service.createEmployer("Tech Corp", "info@techcorp.com", "Description", "Moscow");
    ASSERT_NE(0, id);
}

TEST(test_jobService_createJobPosting) {
    JobService service;
    size_t employerId = service.createEmployer("Tech Corp", "info@techcorp.com", "Description", "Moscow");
    size_t id = service.createJobPosting(employerId, "Developer", "Job description", 100000, "Moscow", true, "Software Engineer");
    ASSERT_NE(0, id);
}

TEST(test_jobService_createReply) {
    JobService service;
    size_t workerId = service.createWorker("John", "john@email.com", {"C++"}, 5, "Moscow", "Resume");
    size_t employerId = service.createEmployer("Tech Corp", "info@techcorp.com", "Description", "Moscow");
    size_t jobId = service.createJobPosting(employerId, "Developer", "Job description", 100000, "Moscow", true, "Software Engineer");
    
    size_t replyId = service.createReply(jobId, workerId);
    ASSERT_NE(0, replyId);
}

TEST(test_jobService_deleteJobPosting) {
    JobService service;
    size_t employerId = service.createEmployer("Tech Corp", "info@techcorp.com", "Description", "Moscow");
    size_t id = service.createJobPosting(employerId, "Developer", "Job description", 100000, "Moscow", true, "Software Engineer");
    
    bool result = service.deleteJobPosting(id);
    ASSERT_TRUE(result);
}

TEST(test_jobService_getAllEmployers) {
    JobService service;
    service.createEmployer("Company 1", "c1@email.com", "Desc 1", "Moscow");
    service.createEmployer("Company 2", "c2@email.com", "Desc 2", "Saint Petersburg");
    
    auto employers = service.getAllEmployers();
    ASSERT_EQ(2, employers.size());
}

TEST(test_jobService_getJobPostingsByEmployer) {
    JobService service;
    size_t employerId = service.createEmployer("Tech Corp", "info@techcorp.com", "Description", "Moscow");
    service.createJobPosting(employerId, "Job 1", "Desc 1", 50000, "Moscow", false, "Engineer");
    service.createJobPosting(employerId, "Job 2", "Desc 2", 60000, "Moscow", true, "Engineer");
    
    auto jobs = service.getJobPostingsByEmployer(employerId);
    ASSERT_EQ(2, jobs.size());
}

TEST(test_jobService_searchJobPostings) {
    JobService service;
    size_t employerId = service.createEmployer("Tech Corp", "info@techcorp.com", "Description", "Moscow");
    service.createJobPosting(employerId, "Job 1", "Desc 1", 50000, "Moscow", false, "Engineer");
    service.createJobPosting(employerId, "Job 2", "Desc 2", 60000, "Moscow", true, "Engineer");
    
    JobFilter filter;
    filter.setIsRemote(true);
    
    auto jobs = service.searchJobPostings(filter);
    ASSERT_EQ(1, jobs.size());
}

TEST(test_jobService_getRepliesByJobPosting) {
    JobService service;
    size_t workerId1 = service.createWorker("John", "john@email.com", {"C++"}, 5, "Moscow", "Resume");
    size_t workerId2 = service.createWorker("Jane", "jane@email.com", {"Python"}, 3, "Moscow", "Resume");
    size_t employerId = service.createEmployer("Tech Corp", "info@techcorp.com", "Description", "Moscow");
    size_t jobId = service.createJobPosting(employerId, "Developer", "Job description", 100000, "Moscow", true, "Software Engineer");
    
    service.createReply(jobId, workerId1);
    service.createReply(jobId, workerId2);
    
    auto replies = service.getRepliesByJobPosting(jobId);
    ASSERT_EQ(2, replies.size());
}

int main() {
    std::cout << "=== Running Unit Tests ===" << std::endl << std::endl;
    
    std::cout << "--- WorkerService Tests ---" << std::endl;
    run_test_createWorker();
    run_test_updateWorker();
    
    std::cout << "\n--- EmployerService Tests ---" << std::endl;
    run_test_createEmployer();
    run_test_updateEmployer();
    run_test_getAllEmployers();
    
    std::cout << "\n--- JobPostingService Tests ---" << std::endl;
    run_test_createJobPosting();
    run_test_deleteJobPosting();
    run_test_getJobPostingsByEmployer();
    run_test_searchJobPostings_withFilters();
    run_test_searchJobPostings_noFilters();
    
    std::cout << "\n--- ReplyService Tests ---" << std::endl;
    run_test_createReply();
    run_test_getRepliesByJobPosting();
    
    std::cout << "\n--- JobService Tests ---" << std::endl;
    run_test_jobService_createWorker();
    run_test_jobService_createEmployer();
    run_test_jobService_createJobPosting();
    run_test_jobService_createReply();
    run_test_jobService_deleteJobPosting();
    run_test_jobService_getAllEmployers();
    run_test_jobService_getJobPostingsByEmployer();
    run_test_jobService_searchJobPostings();
    run_test_jobService_getRepliesByJobPosting();
    
    std::cout << "\n=== Test Results ===" << std::endl;
    std::cout << "Passed: " << testsPassed << std::endl;
    std::cout << "Failed: " << testsFailed << std::endl;
    
    return testsFailed > 0 ? 1 : 0;
}