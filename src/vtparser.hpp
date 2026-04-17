#ifndef VTPARSER_H
#define VTPARSER_H
#include <cstdint>
#include <functional>

class VTParser {
public:
    VTParser();
    ~VTParser();
    void parse(char *, uint32_t);

    std::function<void(char)> rendererWrite;
    std::function<void(char)> emitNewline;
    std::function<void(char)> emitCarriageReturn;
private:
};

#endif // VTPARSER_H
