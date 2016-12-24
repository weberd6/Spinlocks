#ifndef ANDERSON_LOCK_HPP
#define ANDERSON_LOCK_HPP

#include <atomic>
#include <memory>
#include <thread>

/* Pause instruction to prevent excess processor bus usage */ 
#define cpu_relax() asm volatile("pause\n": : :"memory")

struct AndersonLock {

    void init(unsigned int max_threads = std::thread::hardware_concurrency()) {
        max_threads_ = max_threads;
        flags_ = std::make_unique<std::atomic<bool>[]>(max_threads_);
        flags_[0].store(true);
        for (unsigned int i = 1; i < max_threads_; i++) {
            flags_[i].store(false);
        }
    }

    void lock() {
        me_ = last_.fetch_add((unsigned int)1);
        while (flags_[me_ % max_threads_].load() == false) {
            cpu_relax();
        }
    }

    void unlock() {
        flags_[me_ % max_threads_].store(false);
        flags_[(me_ + 1) % max_threads_].store(true);
    }

private:

    unsigned int max_threads_;
    std::unique_ptr<std::atomic<bool>[]> flags_;

    std::atomic<unsigned int> last_ = ATOMIC_VAR_INIT(0);
    static thread_local unsigned int me_;
};

#endif
