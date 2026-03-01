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
    std::vector<Reply> getRepliesByJobPosting(size_t jobPostingId) const;

private:
    ReplyRepository replyRepository_;
};