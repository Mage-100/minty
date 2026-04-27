#pragma once
#include "pch.h"

#include <string>
#include <functional>
#include <cstdint>

#include "threadsafequeue.hpp"

class ConPTY {
public:
    ConPTY(ThreadSafeQueue<std::vector<char>> *);
    ~ConPTY();

    bool Write(const std::string &input);
    bool Resize(SHORT cols, SHORT rows);
    HANDLE  m_hPipeIn   = INVALID_HANDLE_VALUE;

    std::function<void(char *, uint32_t)> vtparserWrite;
    ThreadSafeQueue<std::vector<char>> *queue;
private:
    HPCON   m_hPC       = INVALID_HANDLE_VALUE;
    HANDLE  m_hPipeOut  = INVALID_HANDLE_VALUE;
    HANDLE  m_hListener = INVALID_HANDLE_VALUE;
    PROCESS_INFORMATION m_piClient{};
};
