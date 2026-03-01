#include "service/ReplyService.hpp"

ReplyService::ReplyService() {}

ReplyService::~ReplyService() {}

size_t ReplyService::createReply(size_t jobPostingId, size_t workerId) {
    Reply reply(0, jobPostingId, workerId, std::chrono::system_clock::now(),
                ReplyStatus::PENDING);
    return replyRepository_.add(reply);
}

bool ReplyService::updateReply(const Reply& reply) {
    return replyRepository_.update(reply);
}

bool ReplyService::deleteReply(size_t id) {
    return replyRepository_.remove(id);
}

std::optional<Reply> ReplyService::getReply(size_t id) const {
    return replyRepository_.findById(id);
}

std::vector<Reply> ReplyService::getAllReplies() const {
    return replyRepository_.findAll();
}

std::vector<Reply> ReplyService::getRepliesByJobPosting(size_t jobPostingId) const {
    return replyRepository_.findByJobPostingId(jobPostingId);
}

std::vector<Reply> ReplyService::getRepliesByWorker(size_t workerId) const {
    return replyRepository_.findByWorkerId(workerId);
}

std::vector<Reply> ReplyService::getRepliesByStatus(ReplyStatus status) const {
    return replyRepository_.findByStatus(status);
}

bool ReplyService::hasWorkerReplied(size_t jobPostingId, size_t workerId) const {
    return replyRepository_.existsByJobPostingAndWorker(jobPostingId, workerId);
}