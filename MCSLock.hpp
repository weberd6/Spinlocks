#ifndef MCS_LOCK_HPP
#define MCS_LOCK_HPP

#include <cassert>
#include <atomic>

/* Pause instruction to prevent excess processor bus usage */ 
#define cpu_relax() asm volatile("pause\n": : :"memory")

struct MCSNode {
    std::atomic<MCSNode*> next_ = ATOMIC_VAR_INIT(nullptr);
    bool locked_ = false;
};

struct MCSLock {

    void lock() {
     
        if (!me_) me_ = new MCSNode;

        me_->next_.store(nullptr);
        me_->locked_ = false;

        auto tail = list_.next_.exchange(me_);

        /* Just return if no one is holding lock */
        if (tail) {

            /* Someone there, need to link in */
            tail->next_ = me_;
            
            /* Wait for update on signal */
            while (!me_->locked_) cpu_relax();
        }
    }

    void unlock() {

        /* No successor yet? */
        if (!me_->next_) {
        
            /* Try to atomically unlock */
            auto old_me = me_;
            if (list_.next_.compare_exchange_strong(me_, nullptr)) {
                return;
            } else {
                me_ = old_me;
            }

            /* Wait for successor to appear */
            while (!me_->next_) cpu_relax();
        }
        
        /* Unlock next one */
        me_->next_.load()->locked_ = true;

    }

private:

    MCSNode list_;

    static thread_local MCSNode* me_;

};

#endif
