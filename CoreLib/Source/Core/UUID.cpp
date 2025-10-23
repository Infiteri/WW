#include "UUID.h"
#include <random>

namespace WW
{
    static std::random_device randomDevice;
    static std::mt19937_64 engine(randomDevice());
    static std::uniform_int_distribution<uint64_t> uniformDistribution;

    UUID::UUID() { id = uniformDistribution(engine); }

    UUID::UUID(uint64_t id) : id(id) {}

    UUID::UUID(const UUID &other) : id(other.id) {}

} // namespace Core
