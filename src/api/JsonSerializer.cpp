#include "api/JsonSerializer.hpp"

std::string JsonSerializer::escapeJson(const std::string& str) {
    std::ostringstream oss;
    for (char c : str) {
        switch (c) {
            case '"': oss << "\\\""; break;
            case '\\': oss << "\\\\"; break;
            case '\b': oss << "\\b"; break;
            case '\f': oss << "\\f"; break;
            case '\n': oss << "\\n"; break;
            case '\r': oss << "\\r"; break;
            case '\t': oss << "\\t"; break;
            default:
                if (c < 0x20) {
                    oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
                } else {
                    oss << c;
                }
        }
    }
    return oss.str();
}

std::string JsonSerializer::timePointToString(const std::chrono::system_clock::time_point& tp) {
    auto time = std::chrono::system_clock::to_time_t(tp);
    std::ostringstream oss;
    oss << std::put_time(std::gmtime(&time), "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}

std::chrono::system_clock::time_point JsonSerializer::stringToTimePoint(const std::string& str) {
    std::tm tm = {};
    std::istringstream ss(str);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

std::string JsonSerializer::serializeWorker(const Worker& worker) {
    std::ostringstream oss;
    oss << "{";
    oss << "\"id\":" << worker.getId() << ",";
    oss << "\"name\":\"" << escapeJson(worker.getName()) << "\",";
    oss << "\"email\":\"" << escapeJson(worker.getEmail()) << "\",";
    oss << "\"skills\":[";
    auto skills = worker.getSkills();
    for (size_t i = 0; i < skills.size(); ++i) {
        oss << "\"" << escapeJson(skills[i]) << "\"";
        if (i < skills.size() - 1) oss << ",";
    }
    oss << "],";
    oss << "\"experience\":" << worker.getExperience() << ",";
    oss << "\"city\":\"" << escapeJson(worker.getCity()) << "\",";
    oss << "\"resume\":\"" << escapeJson(worker.getResume()) << "\"";
    oss << "}";
    return oss.str();
}

std::string JsonSerializer::serializeEmployer(const Employer& employer) {
    std::ostringstream oss;
    oss << "{";
    oss << "\"id\":" << employer.getId() << ",";
    oss << "\"companyName\":\"" << escapeJson(employer.getCompanyName()) << "\",";
    oss << "\"email\":\"" << escapeJson(employer.getEmail()) << "\",";
    oss << "\"description\":\"" << escapeJson(employer.getDescription()) << "\",";
    oss << "\"city\":\"" << escapeJson(employer.getCity()) << "\"";
    oss << "}";
    return oss.str();
}

std::string JsonSerializer::serializeJobPosting(const JobPosting& job) {
    std::ostringstream oss;
    oss << "{";
    oss << "\"id\":" << job.getId() << ",";
    oss << "\"employerId\":" << job.getEmployerId() << ",";
    oss << "\"title\":\"" << escapeJson(job.getTitle()) << "\",";
    oss << "\"description\":\"" << escapeJson(job.getDescription()) << "\",";
    oss << "\"salary\":" << job.getSalary() << ",";
    oss << "\"city\":\"" << escapeJson(job.getCity()) << "\",";
    oss << "\"isRemote\":" << (job.isRemote() ? "true" : "false") << ",";
    oss << "\"publishDate\":\"" << timePointToString(job.getPublishDate()) << "\",";
    oss << "\"profession\":\"" << escapeJson(job.getProfession()) << "\"";
    oss << "}";
    return oss.str();
}

std::string JsonSerializer::serializeReply(const Reply& reply) {
    std::ostringstream oss;
    oss << "{";
    oss << "\"id\":" << reply.getId() << ",";
    oss << "\"jobPostingId\":" << reply.getJobPostingId() << ",";
    oss << "\"workerId\":" << reply.getWorkerId() << ",";
    oss << "\"replyDate\":\"" << timePointToString(reply.getReplyDate()) << "\",";
    oss << "\"status\":";
    switch (reply.getStatus()) {
        case ReplyStatus::PENDING: oss << "\"PENDING\""; break;
        case ReplyStatus::REVIEWED: oss << "\"REVIEWED\""; break;
        case ReplyStatus::ACCEPTED: oss << "\"ACCEPTED\""; break;
        case ReplyStatus::REJECTED: oss << "\"REJECTED\""; break;
    }
    oss << "}";
    return oss.str();
}

std::string JsonSerializer::serializeWorkers(const std::vector<Worker>& workers) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < workers.size(); ++i) {
        oss << serializeWorker(workers[i]);
        if (i < workers.size() - 1) oss << ",";
    }
    oss << "]";
    return oss.str();
}

std::string JsonSerializer::serializeEmployers(const std::vector<Employer>& employers) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < employers.size(); ++i) {
        oss << serializeEmployer(employers[i]);
        if (i < employers.size() - 1) oss << ",";
    }
    oss << "]";
    return oss.str();
}

std::string JsonSerializer::serializeJobPostings(const std::vector<JobPosting>& jobs) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < jobs.size(); ++i) {
        oss << serializeJobPosting(jobs[i]);
        if (i < jobs.size() - 1) oss << ",";
    }
    oss << "]";
    return oss.str();
}

std::string JsonSerializer::serializeReplies(const std::vector<Reply>& replies) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < replies.size(); ++i) {
        oss << serializeReply(replies[i]);
        if (i < replies.size() - 1) oss << ",";
    }
    oss << "]";
    return oss.str();
}

std::string JsonSerializer::serializeId(size_t id) {
    std::ostringstream oss;
    oss << "{\"id\":" << id << "}";
    return oss.str();
}

std::string JsonSerializer::serializeSuccess(bool success) {
    std::ostringstream oss;
    oss << "{\"success\":" << (success ? "true" : "false") << "}";
    return oss.str();
}

std::string JsonSerializer::serializeError(const std::string& message) {
    std::ostringstream oss;
    oss << "{\"error\":\"" << escapeJson(message) << "\"}";
    return oss.str();
}
