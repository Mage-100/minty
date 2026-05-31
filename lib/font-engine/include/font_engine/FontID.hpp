#pragma once
#include <functional>

struct FontID {
    const int value;
    bool operator==(const FontID&) const = default;

    FontID(int v) : value(v) {};
};

namespace std {
    template<>
    struct hash<FontID> {
        size_t operator()(const FontID& id) const {
            return std::hash<int>{}(id.value);
        }
    };
}

inline FontID makeFontID() {
    static int counter = 0;
    return FontID{counter++};
}
