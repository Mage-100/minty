#pragma once
#include "mty_runtime.hpp"
#include "mty_state.hpp"
#include <memory>

class MintyApp {
private:
    std::unique_ptr<MintyState> state;
    std::unique_ptr<MintyRuntime> runtime;
public:
    MintyApp();
    ~MintyApp();
    void run();
};
