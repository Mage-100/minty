#pragma once
#include <functional>
#include <atomic>

template<typename Tag>
struct TypedID {
    bool operator==(const TypedID&) const = default;

    TypedID() : _value(-1){};

    static TypedID make() {
        static std::atomic<int> counter{0};
        return TypedID{counter++};
    }

    int value() const { return _value; }

private:
    int _value = -1;
    explicit TypedID(int v) : _value(v) {}
};

namespace std {
    template<typename Tag>
    struct hash<TypedID<Tag>> {
        size_t operator()(const TypedID<Tag>& id) const noexcept {
            return std::hash<int>{}(id.value());
        }
    };
}

struct FaceIDTag {};
struct FontIDTag {};

using FaceID = TypedID<FaceIDTag>;
using FontID = TypedID<FontIDTag>;
