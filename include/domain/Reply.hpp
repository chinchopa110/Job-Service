#pragma once

#include <string>
#include <cstdint>
#include <chrono>

enum class ReplyStatus {
    PENDING,
    REVIEWED,
    ACCEPTED,
    REJECTED
};

class Reply {
public:
    Reply();
    Reply(size_t id, size_t jobPostingId, size_t workerId,
          const std::chrono::system_clock::time_point& replyDate,
          ReplyStatus status);

    size_t getId() const;
    void setId(size_t id);

    size_t getJobPostingId() const;
    void setJobPostingId(size_t jobPostingId);

    size_t getWorkerId() const;
    void setWorkerId(size_t workerId);

    std::chrono::system_clock::time_point getReplyDate() const;
    void setReplyDate(const std::chrono::system_clock::time_point& replyDate);

    ReplyStatus getStatus() const;
    void setStatus(ReplyStatus status);

    bool operator==(const Reply& other) const;
    bool operator!=(const Reply& other) const;

private:
    size_t id_;
    size_t jobPostingId_;
    size_t workerId_;
    std::chrono::system_clock::time_point replyDate_;
    ReplyStatus status_;
};