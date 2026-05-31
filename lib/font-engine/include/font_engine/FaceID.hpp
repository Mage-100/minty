#pragma once
#include <functional>

struct FaceID {
    const int value;
    bool operator==(const FaceID&) const = default;

    FaceID(int v) : value(v) {};
};

namespace std {
    template<>
    struct hash<FaceID> {
        size_t operator()(const FaceID& id) const {
            return std::hash<int>{}(id.value);
        }
    };
}

inline FaceID makeFaceID() {
    static int counter = 0;
    return FaceID{counter++};
}
