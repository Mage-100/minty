#include "mty_app.hpp"
#include "conpty.hpp"
#include "mty_runtime.hpp"
#include "threadsafequeue.hpp"
#include "vtparser.hpp"
#include <memory>

MintyApp::MintyApp() {
    byte_queue = std::make_unique<ThreadSafeQueue<std::vector<char>>>();
    state = std::make_unique<MintyState>();
    conpty = std::make_unique<ConPTY>(byte_queue.get());
    vtparser = std::make_unique<VTParser>(byte_queue.get());
    runtime = std::make_unique<MintyRuntime>(byte_queue.get(), state.get(), vtparser.get());

    runtime->setPTYWriteCallback([&](const std::string& input) {
        conpty->Write(input);
    });

    runtime->setPTYResizeCallback([&](int cols, int rows) {
        conpty->Resize(cols, rows);
    });
}

void MintyApp::run() {
    runtime->start();
}

MintyApp::~MintyApp() {

}
