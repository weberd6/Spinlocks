#ifndef TICKET_LOCK_HPP
#define TICKET_LOCK_HPP

#include <atomic>

/* Pause instruction to prevent excess processor bus usage */ 
#define cpu_relax() asm volatile("pause\n": : :"memory")

struct TicketLock {

    void lock() {
        unsigned int me = std::atomic_fetch_add(&users_, (unsigned int)1);
        while (ticket_.load() != me) {
            cpu_relax();
        }
    }

    void unlock() {
        std::atomic_fetch_add(&ticket_, (unsigned int)1);
    }

private:

    std::atomic<unsigned int> ticket_ = ATOMIC_VAR_INIT(0);

    std::atomic<unsigned int> users_ = ATOMIC_VAR_INIT(0);

};

#endif
