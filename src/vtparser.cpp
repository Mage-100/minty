#include <iostream>
#include <string>
#include <cstdint>
#include "vtparser.hpp"
#include "threadsafequeue.hpp"

VTParser::VTParser(ThreadSafeQueue<std::vector<char>> *q) : queue(q) {

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

void VTParser::processByte(uint8_t byte) {
    switch (state) {
        case State::Ground:
            if (byte == 0x1B) { state = State::Escape; }
            else if (byte >= 0x20 && byte <= 0x7E) {
                cb.onPrintChar(cursorCol, cursorRow, byte);
                cursorCol++;
                if (cursorCol >= cols) {
                    cursorCol = 0;
                    cursorRow++;
                    if (cursorRow >= rows) {
                        cb.onScrollUp(1);
                        cursorRow = rows - 1;
                    }
                }
            }
            else if (byte == 0x0D) { // Carriage Return
                cursorCol = 0;
            }
            else if (byte == 0x0A) { // Linefee (newline)
                cursorCol = 0;
                cursorRow++;
                if (cursorRow >= rows) {
                    cb.onScrollUp(1);
                    cursorRow = rows - 1;
                }
            } else if (byte == 0x08) { // Backspace
                if (cursorCol != 0) {
                    cursorCol -= 1;
                }
            }
            break;

        case State::Escape:
            if (byte == '[') { state = State::CSIEntry; paramBuf.clear(); }
            else if (byte == ']') { state = State::OSCString; oscBuf.clear(); }
            else {
                // dispatchEsc(byte);
                state = State::Ground;
            }
            break;

        case State::CSIEntry:
        case State::CSIParam:
            state = State::CSIParam;
            if (byte >= 0x30 && byte <= 0x3F) {  // digits, ;, ?
                paramBuf += byte;
            } else if (byte >= 0x40 && byte <= 0x7E) {  // final byte a-zA-Z
                dispatchCSI(byte, paramBuf);
                paramBuf.clear();
                state = State::Ground;
            }
            break;

        case State::OSCString:
            if (byte == 0x07 || byte == 0x1B) {  // BEL or start of ESC '\'
                dispatchOSC(byte, oscBuf);
                oscBuf.clear();
                state = State::Ground;
            } else {
                oscBuf += byte;
            }
            break;
    }
}

std::vector<int> VTParser::parseParams(const std::string& params, int defaultValue) {
    if (params.empty()) return { defaultValue };
    std::vector<int> result;
    std::string current;

    for (char c : params) {
        if (c == ';') {
            result.push_back(current.empty() ? defaultValue : std::stoi(current));
            current.clear();
        } else if (c >= '0' && c <= '9') {
            current += c;
        }
    }

    result.push_back(current.empty() ? defaultValue : std::stoi(current));
    return result;
}

void VTParser::dispatchCSI(uint8_t final, const std::string& parambuf) {
    bool isPrivate = !paramBuf.empty() && paramBuf[0] == '?';
    std::string cleanBuf = isPrivate ? paramBuf.substr(1) : paramBuf; // Strips '?'

    bool isEraseCommand = (final == 'J' || final == 'K' || final == 'X');
    auto params = parseParams(cleanBuf, isEraseCommand ? 0 : 1);

    if (isPrivate) {
        // for now do nothing
        return;
    }

    if (final == 'H' || final == 'f') {
        std::vector<int> result;
        std::string current;

        for (char c : parambuf) {
            if (c == ';') {
                result.push_back(current.empty() ? 1 : std::stoi(current));
                current.clear();
            } else if (c >= '0' && c <= '9') {
                current += c;
            }
        }
        result.push_back(current.empty() ? 1 : std::stoi(current));

        if (result.size() == 1) {
            result.push_back(1);
        }
        cursorRow = result[0] - 1;
        cursorCol = result[1] - 1;
    }

    if (final == 'K') {
        if (parambuf.empty()) {
            cb.onEraseLine(cursorCol, cursorRow, 0);
        } else {
            cb.onEraseLine(cursorCol, cursorRow, std::stoi(parambuf));
        }

        return;
    }

    if (paramBuf[0] == '2' && final == 'J') {
        cb.onEraseEntireScreen();
    }
}

void VTParser::dispatchOSC(uint8_t final, const std::string& oscbuf) {
    if (final == '\a') {
        if (oscbuf == "133;A") {
            cb.onPromptStart(cursorCol, cursorRow);
            return;
        }
        if (oscbuf == "133;B") {
            cb.onPromptEnd(cursorCol, cursorRow);
            return;
        }
    }
}

void VTParser::feed(const std::vector<char>& data) {
    for (char byte : data) {
        processByte(static_cast<uint8_t>(byte));
    }
}
