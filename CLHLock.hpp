#ifndef CLH_LOCK_HPP
#define CLH_LOCK_HPP

#include <atomic>

/* Pause instruction to prevent excess processor bus usage */ 
#define cpu_relax() asm volatile("pause\n": : :"memory")

struct CLHNode {
    bool succ_must_wait_ = false;
};

struct CLHLock {

    void lock() {
        
        if (!me_) me_ = new CLHNode;

        me_->succ_must_wait_ = true;
        pred_ = tail_.exchange(me_);

        while (pred_->succ_must_wait_) {
            cpu_relax();
        }
    }

    void unlock() {
        me_->succ_must_wait_ = false;
        me_ = pred_;
    }

private:

    std::atomic<CLHNode*> tail_ = ATOMIC_VAR_INIT(new CLHNode);

    static thread_local CLHNode* me_;
    static thread_local CLHNode* pred_;

};

#endif
