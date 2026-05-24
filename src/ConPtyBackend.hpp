#pragma once
#ifdef _WIN32

#include "interfaces/IPtyBackend.hpp"
#include "threadsafequeue.hpp"

class ConPtyBackend : public IPtyBackend {
public:
    ConPtyBackend(ThreadSafeQueue<std::vector<char>> *);
    ~ConPtyBackend();

    void write(const std::string &input) override;
    void resize(int cols, int rows) override;
    HANDLE  m_hPipeIn   = INVALID_HANDLE_VALUE;

    ThreadSafeQueue<std::vector<char>> *queue;
private:
    HPCON   m_hPC       = INVALID_HANDLE_VALUE;
    HANDLE  m_hPipeOut  = INVALID_HANDLE_VALUE;
    HANDLE  m_hListener = INVALID_HANDLE_VALUE;
    PROCESS_INFORMATION m_piClient{};
};

#endif
