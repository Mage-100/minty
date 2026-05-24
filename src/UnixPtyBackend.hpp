#pragma once
#include <thread>
#include <vector>
#include "interfaces/IPtyBackend.hpp"
#include "threadsafequeue.hpp"

class UnixPtyBackend : public IPtyBackend {
public:
    UnixPtyBackend(const std::string&, ThreadSafeQueue<std::vector<char>>*);
    ~UnixPtyBackend();

    void write(const std::string& input) override;
    void resize(int cols, int rows) override;

    void startReading();
private:
    ThreadSafeQueue<std::vector<char>>* queue;
    int masterFd = -1;

    std::thread readerThread;
};
