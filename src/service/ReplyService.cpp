#include "service/ReplyService.hpp"

ReplyService::ReplyService() {}

ReplyService::~ReplyService() {}

size_t ReplyService::createReply(size_t jobPostingId, size_t workerId) {
    Reply reply(0, jobPostingId, workerId, std::chrono::system_clock::now(),
                ReplyStatus::PENDING);
    return replyRepository_.add(reply);
}

std::vector<Reply> ReplyService::getRepliesByJobPosting(size_t jobPostingId) const {
    return replyRepository_.findByJobPostingId(jobPostingId);
}