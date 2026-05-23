#pragma once
#include <memory>
#include <string>
#include "interfaces/IPtyBackend.hpp"
#include "threadsafequeue.hpp"

class MintyPty {
public:
    MintyPty(ThreadSafeQueue<std::vector<char>> *);
    ~MintyPty();

    void write(const std::string& );
    void resize(int, int);
private:
    ThreadSafeQueue<std::vector<char>>* queue;
    std::unique_ptr<IPtyBackend> pty_backend;
};
