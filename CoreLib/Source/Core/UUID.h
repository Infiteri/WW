#pragma once

#include <stdint.h>

namespace WW
{
    class UUID
    {
    private:
        uint64_t id;

    public:
        UUID();
        UUID(uint64_t id);
        UUID(const UUID &other);
        ~UUID() = default;

        operator uint64_t() const { return id; };
        inline uint64_t Get() const { return id; };
        bool operator==(const UUID &other) const { return id == other.id; };
        bool operator!=(const UUID &other) const { return id != other.id; };
    };
} // namespace Core
