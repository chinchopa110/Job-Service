#include "domain/Reply.hpp"

Reply::Reply()
    : id_(0), jobPostingId_(0), workerId_(0),
      replyDate_(std::chrono::system_clock::now()),
      status_(ReplyStatus::PENDING) {}

Reply::Reply(size_t id, size_t jobPostingId, size_t workerId,
             const std::chrono::system_clock::time_point& replyDate,
             ReplyStatus status)
    : id_(id), jobPostingId_(jobPostingId), workerId_(workerId),
      replyDate_(replyDate), status_(status) {}

size_t Reply::getId() const {
    return id_;
}

void Reply::setId(size_t id) {
    id_ = id;
}

size_t Reply::getJobPostingId() const {
    return jobPostingId_;
}

void Reply::setJobPostingId(size_t jobPostingId) {
    jobPostingId_ = jobPostingId;
}

size_t Reply::getWorkerId() const {
    return workerId_;
}

void Reply::setWorkerId(size_t workerId) {
    workerId_ = workerId;
}

std::chrono::system_clock::time_point Reply::getReplyDate() const {
    return replyDate_;
}

void Reply::setReplyDate(const std::chrono::system_clock::time_point& replyDate) {
    replyDate_ = replyDate;
}

ReplyStatus Reply::getStatus() const {
    return status_;
}

void Reply::setStatus(ReplyStatus status) {
    status_ = status;
}

bool Reply::operator==(const Reply& other) const {
    return id_ == other.id_;
}

bool Reply::operator!=(const Reply& other) const {
    return !(*this == other);
}