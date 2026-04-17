#ifndef CONPTY_H
#define CONPTY_H

#include <Windows.h>
#include <string>
#include <functional>
#include <cstdint>

class ConPTY {
public:
    ConPTY();
    ~ConPTY();

    bool Write(const std::string &input);
    bool Resize(SHORT cols, SHORT rows);
    HANDLE  m_hPipeIn   = INVALID_HANDLE_VALUE;

    std::function<void(char *, uint32_t)> vtparserWrite;
private:
    HPCON   m_hPC       = INVALID_HANDLE_VALUE;
    HANDLE  m_hPipeOut  = INVALID_HANDLE_VALUE;
    HANDLE  m_hListener = INVALID_HANDLE_VALUE;
    PROCESS_INFORMATION m_piClient{};
};

#endif
