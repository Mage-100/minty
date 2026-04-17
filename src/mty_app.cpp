#include "mty_app.hpp"
#include "mty_runtime.hpp"

MintyApp::MintyApp() {
    state = std::make_unique<MintyState>();
    runtime = std::make_unique<MintyRuntime>(state.get());
}

void MintyApp::run() {
    runtime->start();
}

MintyApp::~MintyApp() {

}
