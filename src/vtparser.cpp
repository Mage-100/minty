#include <iostream>
#include <cstdint>
#include "vtparser.hpp"

VTParser::VTParser() {

}

VTParser::~VTParser() {

}

void VTParser::parse(char *buf, uint32_t bytesread) {
    for (int i = 0; i < bytesread; i++) {
        if (buf[i] == '\r') emitCarriageReturn('\r');
        if (buf[i] == '\n') emitNewline('\n');
        if (buf[i] >= 32 && buf[i] <= 126) {
            rendererWrite(buf[i]);
        }
    }
}
