#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "domain/Worker.hpp"
#include "domain/Employer.hpp"
#include "domain/JobPosting.hpp"
#include "domain/Reply.hpp"
#include "filter/JobFilter.hpp"

class JsonSerializer {
public:
    static std::string escapeJson(const std::string& str);
    
    static std::string serializeWorker(const Worker& worker);
    static std::string serializeEmployer(const Employer& employer);
    static std::string serializeJobPosting(const JobPosting& job);
    static std::string serializeReply(const Reply& reply);
    
    static std::string serializeWorkers(const std::vector<Worker>& workers);
    static std::string serializeEmployers(const std::vector<Employer>& employers);
    static std::string serializeJobPostings(const std::vector<JobPosting>& jobs);
    static std::string serializeReplies(const std::vector<Reply>& replies);
    
    static std::string serializeId(size_t id);
    static std::string serializeSuccess(bool success);
    static std::string serializeError(const std::string& message);
    
    static std::string timePointToString(const std::chrono::system_clock::time_point& tp);
    static std::chrono::system_clock::time_point stringToTimePoint(const std::string& str);
};
