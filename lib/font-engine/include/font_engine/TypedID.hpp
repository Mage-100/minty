#pragma once
#include <functional>
#include <atomic>

template<typename Tag>
struct TypedID {
    int value = -1;

    bool operator==(const TypedID&) const = default;

    static TypedID make() {
        static std::atomic<int> counter{0};
        return TypedID{counter++};
    }

private:
    explicit TypedID(int v) : value(v) {}
};

namespace std {
    template<typename Tag>
    struct hash<TypedID<Tag>> {
        size_t operator()(const TypedID<Tag>& id) const noexcept {
            return std::hash<int>{}(id.value);
        }
    };
}

struct FaceIDTag {};
struct FontIDTag {};

using FaceID = TypedID<FaceIDTag>;
using FontID = TypedID<FontIDTag>;
