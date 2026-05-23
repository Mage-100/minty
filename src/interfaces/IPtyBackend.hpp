#pragma once
#include <string>

class IPtyBackend {
public:
    ~IPtyBackend() = default;

    virtual void write(const std::string& input) = 0;
    virtual void resize(int cols, int rows) = 0;
};
