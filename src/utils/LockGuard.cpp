#include "utils/LockGuard.hpp"

LockGuard::LockGuard(std::shared_mutex& mutex)
    : lock_(mutex) {}

LockGuard::~LockGuard() {}

SharedLockGuard::SharedLockGuard(std::shared_mutex& mutex)
    : lock_(mutex) {}

SharedLockGuard::~SharedLockGuard() {}