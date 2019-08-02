#ifndef JSONOBJECTPROCESSOR_SYNCHRONIZEDQUEUE_H
#define JSONOBJECTPROCESSOR_SYNCHRONIZEDQUEUE_H


#include <mutex>
#include <condition_variable>
#include <deque>
#include <queue>

class SynchronizedQueue {
private:
    std::condition_variable cvCanPop;
    std::condition_variable cvCanPush;
    std::mutex sync;
    std::queue<std::string> qu;
    bool bShutdown = false;
    long size;
public:
    SynchronizedQueue(long size) { this->size = size; }
    void push(std::string const value);
    bool pop(std::string& value);
    void requestShutdown();
};


#endif //JSONOBJECTPROCESSOR_SYNCHRONIZEDQUEUE_H
