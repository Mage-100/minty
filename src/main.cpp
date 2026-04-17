#include "mty_app.hpp"

int main() {
    std::unique_ptr<MintyApp> app = std::make_unique<MintyApp>();
    app->run();
    return 0;
}
