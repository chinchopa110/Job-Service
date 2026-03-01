#pragma once

#include "domain/Reply.hpp"
#include <unordered_map>
#include <shared_mutex>
#include <vector>
#include <optional>

class ReplyRepository {
public:
    ReplyRepository();
    ~ReplyRepository();

    size_t add(const Reply& reply);
    bool update(const Reply& reply);
    bool remove(size_t id);
    std::optional<Reply> findById(size_t id) const;
    std::vector<Reply> findAll() const;
    std::vector<Reply> findByJobPostingId(size_t jobPostingId) const;
    std::vector<Reply> findByWorkerId(size_t workerId) const;
    std::vector<Reply> findByStatus(ReplyStatus status) const;
    bool existsByJobPostingAndWorker(size_t jobPostingId, size_t workerId) const;

private:
    std::unordered_map<size_t, Reply> replies_;
    mutable std::shared_mutex mutex_;
    size_t nextId_;
};