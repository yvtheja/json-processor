#include <iostream>
#include "SynchronizedQueue.h"

void SynchronizedQueue::push(const std::string value) {
    {
        std::unique_lock<std::mutex> lock(sync);
        if(qu.size() >= size) {
            cvCanPush.wait(lock);
        }
        qu.push(value);
    }
    cvCanPop.notify_one();
}

void SynchronizedQueue::requestShutdown() {
    {
        std::unique_lock<std::mutex> lock(sync);
        bShutdown = true;
    }
    cvCanPop.notify_all();
}

bool SynchronizedQueue::pop(std::string& value) {
    std::unique_lock<std::mutex> lock(sync);
    for (;;) {
        if (qu.empty()) {
            if (bShutdown) {
                return false;
            }
        }
        else {
            break;
        }
        cvCanPop.wait(lock);
    }
    value = std::move(qu.front());
    qu.pop();
    size--;
    cvCanPush.notify_one();

    return true;
}
