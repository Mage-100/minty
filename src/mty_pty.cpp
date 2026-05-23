#include "interfaces/IPtyBackend.hpp"
#include <memory>
#include "mty_pty.hpp"
#include "ConPtyBackend.hpp"
#include "threadsafequeue.hpp"

MintyPty::MintyPty(ThreadSafeQueue<std::vector<char>>* queue) {
#ifdef _WIN32
    pty_backend = std::make_unique<ConPtyBackend>(queue);
#endif
}

void MintyPty::write(const std::string& input) {
    pty_backend->write(input);
}

void MintyPty::resize(int cols, int rows) {
    pty_backend->resize(cols, rows);
}

MintyPty::~MintyPty() = default;
