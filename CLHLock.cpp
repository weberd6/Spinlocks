
#include "CLHLock.hpp"

thread_local CLHNode* CLHLock::me_ = nullptr;
thread_local CLHNode* CLHLock::pred_ = nullptr;

