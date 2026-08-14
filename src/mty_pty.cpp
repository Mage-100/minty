#include "interfaces/IPtyBackend.hpp"
#include <string>
#include <memory>
#include "mty_pty.hpp"
#include "threadsafequeue.hpp"
#ifdef _WIN32
#include "ConPtyBackend.hpp"
#else
#include "UnixPtyBackend.hpp"
#endif

MintyPty::MintyPty(ThreadSafeQueue<std::vector<char>>* q) : queue(q) {
#ifdef _WIN32
    pty_backend = std::make_unique<ConPtyBackend>(queue);
#else
    const std::string shellName = "bash";
    pty_backend = std::make_unique<UnixPtyBackend>(shellName, queue);
#endif
}

void MintyPty::write(const std::string& input) {
    pty_backend->write(input);
}

void MintyPty::resize(int cols, int rows) {
    pty_backend->resize(cols, rows);
}

MintyPty::~MintyPty() = default;
