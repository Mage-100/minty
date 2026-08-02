#pragma once
#include <cstdint>

class UTF8Parser {
public:
    static uint32_t encode(uint32_t);
    static uint32_t decode(uint32_t);

    bool try_decode(uint8_t);
    uint32_t getValue32();
    uint8_t  getValue8();
private:
    enum CharClass : uint8_t {
        ILL = 0,    //- C0..C1, F5..FF  ILLEGAL octets that should never appear in a UTF-8 sequence
                    //
        ASC = 1,    //- 00..7F          ASCII leading byte range
                    //
        CR1 = 2,    //- 80..8F          Continuation range 1
        CR2 = 3,    //- 90..9F          Continuation range 2
        CR3 = 4,    //- A0..BF          Continuation range 3
                    //
        L2A = 5,    //- C2..DF          Leading byte range A / 2-byte sequence
                    //
        L3A = 6,    //- E0              Leading byte range A / 3-byte sequence
        L3B = 7,    //- E1..EC, EE..EF  Leading byte range B / 3-byte sequence
        L3C = 8,    //- ED              Leading byte range C / 3-byte sequence
                    //
        L4A = 9,    //- F0              Leading byte range A / 4-byte sequence
        L4B = 10,   //- F1..F3          Leading byte range B / 4-byte sequence
        L4C = 11,   //- F4              Leading byte range C / 4-byte sequence
    };
    enum State : uint8_t {
        BGN = 0,    //- Start
        ERR = 12,   //- Invalid sequence
                    //
        CS1 = 24,   //- Continuation state 1
        CS2 = 36,   //- Continuation state 2
        CS3 = 48,   //- Continuation state 3
                    //
        P3A = 60,   //- Partial 3-byte sequence state A
        P3B = 72,   //- Partial 3-byte sequence state B
                    //
        P4A = 84,   //- Partial 4-byte sequence state A
        P4B = 96,   //- Partial 4-byte sequence state B
                    //
        END = BGN,  //- Start and End are the same state!
        err = ERR,  //- For readability in the state transition table
    };

    struct FirstUnitInfo {
        char8_t     mFirstOctet;
        State       mNextState;
    };

    struct alignas(2048) LookupTables {
        FirstUnitInfo   maFirstUnitTable[256];
        CharClass       maOctetCategory[256];
        State           maTransitions[108];
        std::uint8_t    maFirstOctetMask[16];
    };

private:
    static LookupTables const smTables;
};
