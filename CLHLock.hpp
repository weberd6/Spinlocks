#ifndef CLH_LOCK_HPP
#define CLH_LOCK_HPP

#include <atomic>

/* Pause instruction to prevent excess processor bus usage */ 
#define cpu_relax() asm volatile("pause\n": : :"memory")

struct CLHNode {
    std::atomic<CLHNode*> prev_ = ATOMIC_VAR_INIT(nullptr);
    bool succ_must_wait_ = false;
};

struct CLHLock {

    void lock() {
        
        me_ = new CLHNode;

        me_->succ_must_wait_ = true;
        me_->prev_ = tail_.exchange(me_);

        while (me_->prev_.load()->succ_must_wait_) {
            cpu_relax();
        }
    }

    void unlock() {
        auto tail = me_->prev_.load();
        me_->succ_must_wait_ = false;
        me_ = tail_;
    }

private:

    std::atomic<CLHNode*> tail_ = ATOMIC_VAR_INIT(new CLHNode);

    static thread_local CLHNode* me_;

};

#endif
