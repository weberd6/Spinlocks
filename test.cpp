#include "TicketLock.hpp"
#include "AndersonLock.hpp"
#include "MCSLock.hpp"
#include "CLHLock.hpp"

#include <chrono>
#include <iostream>
#include <pthread.h>

pthread_barrier_t b;

TicketLock tl;
AndersonLock al;
MCSLock mcs;
CLHLock clh;

unsigned int counter = 0;

void* test_ticket(void*) {
    pthread_barrier_wait(&b);
    for (unsigned int i = 0; i < 100000; i++) {
        tl.lock();
        ++counter;
        tl.unlock();
    }
    pthread_exit(NULL);
}

void* test_anderson(void*) {
    pthread_barrier_wait(&b);
    for (unsigned int i = 0; i < 100000; i++) {
        al.lock();
        ++counter;
        al.unlock();
    }
    pthread_exit(NULL);
}

void* test_mcs(void*) {
    pthread_barrier_wait(&b);
    for (unsigned int i = 0; i < 100000; i++) {
        mcs.lock();
        ++counter;
        mcs.unlock();
    }
    pthread_exit(NULL);
}

void* test_clh(void*) {
    pthread_barrier_wait(&b);
    for (unsigned int i = 0; i < 100000; i++) {
        clh.lock();
        ++counter;
        clh.unlock();
    }
    pthread_exit(NULL);
}

int main() {

    pthread_barrier_init(&b, NULL, 4);

    auto start = std::chrono::steady_clock::now();
    pthread_t t1[4];
    for (unsigned int i = 0; i < 4; i++) {
        pthread_create(&t1[i], NULL, test_ticket, NULL);
    }

    for (unsigned int i = 0; i < 4; i++) {
        pthread_join(t1[i], NULL);
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    
    std::cout << counter << std::endl;
    std::cout << duration << " ms" << std::endl;

    ///////////////////////////////////////////////////////////
    
    counter = 0;
    al.init(4);

    start = std::chrono::steady_clock::now();
    pthread_t t2[4];
    for (unsigned int i = 0; i < 4; i++) {
        pthread_create(&t2[i], NULL, test_anderson, NULL);
    }

    for (unsigned int i = 0; i < 4; i++) {
        pthread_join(t2[i], NULL);
    }
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    std::cout << counter << std::endl;
    std::cout << duration << " ms" << std::endl;

    ///////////////////////////////////////////////////////////

    counter = 0;

    start = std::chrono::steady_clock::now();
    pthread_t t3[4];
    for (unsigned int i = 0; i < 4; i++) {
        pthread_create(&t3[i], NULL, test_mcs, NULL);
    }

    for (unsigned int i = 0; i < 4; i++) {
        pthread_join(t3[i], NULL);
    }
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    std::cout << counter << std::endl;
    std::cout << duration << " ms" << std::endl;

    ///////////////////////////////////////////////////////////

    counter = 0;

    start = std::chrono::steady_clock::now();
    pthread_t t4[4];
    for (unsigned int i = 0; i < 4; i++) {
        pthread_create(&t4[i], NULL, test_clh, NULL);
    }

    for (unsigned int i = 0; i < 4; i++) {
        pthread_join(t4[i], NULL);
    }
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    std::cout << counter << std::endl;
    std::cout << duration << " ms" << std::endl;

    ///////////////////////////////////////////////////////////

    pthread_barrier_destroy(&b);

    return 0;
}
