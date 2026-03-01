#pragma once

#include <shared_mutex>

class LockGuard {
public:
    explicit LockGuard(std::shared_mutex& mutex);
    ~LockGuard();

    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;

    LockGuard(LockGuard&&) = delete;
    LockGuard& operator=(LockGuard&&) = delete;

private:
    std::unique_lock<std::shared_mutex> lock_;
};

class SharedLockGuard {
public:
    explicit SharedLockGuard(std::shared_mutex& mutex);
    ~SharedLockGuard();

    SharedLockGuard(const SharedLockGuard&) = delete;
    SharedLockGuard& operator=(const SharedLockGuard&) = delete;

    SharedLockGuard(SharedLockGuard&&) = delete;
    SharedLockGuard& operator=(SharedLockGuard&&) = delete;

private:
    std::shared_lock<std::shared_mutex> lock_;
};