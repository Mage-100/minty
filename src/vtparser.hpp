#ifndef VTPARSER_H
#define VTPARSER_H
#include "threadsafequeue.hpp"
#include "vtcallbacks.hpp"
#include <cstdint>
#include <functional>

class VTParser {
private:
    int cols, rows;
    int cursorCol = 0, cursorRow = 0;

    enum class State { Ground, Escape, CSIEntry, CSIParam, OSCString };
    State state = State::Ground;
    std::string paramBuf;
    std::string oscBuf;
    VTCallbacks cb;
    void processByte(uint8_t );
    std::vector<int> parseParams(const std::string&, int);
    void dispatchCSI(uint8_t final, const std::string& paramBuf);
    void dispatchOSC(uint8_t final, const std::string& paramBuf);

    ThreadSafeQueue<std::vector<char>> *queue;
public:
    VTParser(ThreadSafeQueue<std::vector<char>> *);
    ~VTParser();
    void parse(char *, uint32_t);
    void parse();
    void resize(int c, int r) {cols = c; rows = r;};

    void feed(const std::vector<char>&);
    void setCallbacks(VTCallbacks callbacks) {cb = std::move(callbacks);};

    std::function<void(char)> rendererWrite;
    std::function<void(char)> emitNewline;
    std::function<void(char)> emitCarriageReturn;
};

#endif // VTPARSER_H
