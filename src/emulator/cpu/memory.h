#ifndef EMULATOR_CPU_MEMORY_H_
#define EMULATOR_CPU_MEMORY_H_

#include <array>
#include <cstdint>
#include <limits>

namespace emulator {

inline constexpr uint64_t kMemorySize =
    std::numeric_limits<uint32_t>::max() + 1;

using Memory = std::array<uint8_t, kMemorySize>;

}  // namespace emulator

#endif  // EMULATOR_CPU_MEMORY_H_
