#include <iostream>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "UnixPtyBackend.hpp"
#include "threadsafequeue.hpp"

UnixPtyBackend::UnixPtyBackend(const std::string& shellName, ThreadSafeQueue<std::vector<char>>* q) : queue(q) {

    masterFd = posix_openpt(O_RDWR | O_NOCTTY);

    if (masterFd < 0) {
        std::cout << "Failed to posix_openpt" << std::endl;
        _exit(1);
    }

    if (grantpt(masterFd) < 0) {
        std::cout << "Failed to grantpt" << std::endl;
        _exit(1);
    }

    if (unlockpt(masterFd) < 0) {
        std::cout << "Failed to unlockpt" << std::endl;
        _exit(1);
    }

    const char* slaveName = ptsname(masterFd);

    int slaveFd = open(slaveName, O_RDWR);
    if (slaveFd < 0) {
        std::cout << "Failed to open slave fd" << std::endl;
        _exit(1);
    }

    const struct winsize ws {20, 80, 0, 0};
    ioctl(slaveFd, TIOCSWINSZ, &ws);

    int childPid = fork();
    if (childPid < 0) return;

    if (childPid == 0) {
        close(masterFd);
        setsid();
        ioctl(slaveFd, TIOCSCTTY, 0);
        dup2(slaveFd, STDIN_FILENO);
        dup2(slaveFd, STDOUT_FILENO);
        dup2(slaveFd, STDERR_FILENO);
        if (slaveFd > STDERR_FILENO) close(slaveFd);

        setenv("TERM", "xterm-256color", 1);
        setenv("COLORTERM", "truecolor", 1);

        const char* args[] = {shellName.c_str(), nullptr};
        execvp("bash", const_cast<char * const*>(args));
        _exit(1);
    }

    close(slaveFd);
    slaveFd = -1;

    startReading();
}

void UnixPtyBackend::resize(int cols, int rows) {
    struct winsize ws { (unsigned short)rows, (unsigned short)cols, 0, 0 };
    ioctl(masterFd, TIOCSWINSZ, &ws);
}

void UnixPtyBackend::write(const std::string& input) {
    ::write(masterFd, input.c_str(), input.size());
};
void UnixPtyBackend::startReading() {
    readerThread = std::thread([this]() {
        char buf[4096];

        while (true) {
            ssize_t n = read(masterFd, buf, sizeof(buf));

            if (n > 0) {
                queue->push(std::vector<char>(buf, buf+n));
            }
        }
    });
}

UnixPtyBackend::~UnixPtyBackend() {
    if (readerThread.joinable()) {
        readerThread.join();
    }
}
