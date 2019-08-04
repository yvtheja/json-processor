#ifndef JSONOBJECTPROCESSOR_BLOCKINGQUEUE_H
#define JSONOBJECTPROCESSOR_BLOCKINGQUEUE_H


#include <mutex>
#include <condition_variable>
#include <deque>
#include <queue>

class BlockingQueue {
private:
    std::condition_variable cvCanPop;
    std::condition_variable cvCanPush;
    std::mutex sync;
    std::queue<std::string> qu;
    bool bShutdown = false;
    long size;
public:
    BlockingQueue(long size) { this->size = size; }
    void push(std::string const value);
    bool pop(std::string& value);
    void requestShutdown();
};


#endif //JSONOBJECTPROCESSOR_BLOCKINGQUEUE_H
