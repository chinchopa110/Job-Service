#pragma once

#include "domain/Reply.hpp"
#include "repository/ReplyRepository.hpp"
#include <vector>
#include <optional>

class ReplyService {
public:
    ReplyService();
    ~ReplyService();

    size_t createReply(size_t jobPostingId, size_t workerId);
    bool updateReply(const Reply& reply);
    bool deleteReply(size_t id);
    std::optional<Reply> getReply(size_t id) const;
    std::vector<Reply> getAllReplies() const;
    std::vector<Reply> getRepliesByJobPosting(size_t jobPostingId) const;
    std::vector<Reply> getRepliesByWorker(size_t workerId) const;
    std::vector<Reply> getRepliesByStatus(ReplyStatus status) const;
    bool hasWorkerReplied(size_t jobPostingId, size_t workerId) const;

private:
    ReplyRepository replyRepository_;
};