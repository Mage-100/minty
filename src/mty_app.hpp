#pragma once
#include "mty_runtime.hpp"
#include "mty_state.hpp"
#include "conpty.hpp"
#include "vtparser.hpp"
#include "threadsafequeue.hpp"
#include <memory>

class MintyApp {
private:
    std::unique_ptr<ThreadSafeQueue<std::vector<char>>> byte_queue;
    std::unique_ptr<MintyState> state;
    std::unique_ptr<MintyRuntime> runtime;
    std::unique_ptr<ConPTY> conpty;
    std::unique_ptr<VTParser> vtparser;
public:
    MintyApp();
    ~MintyApp();
    void run();
};
