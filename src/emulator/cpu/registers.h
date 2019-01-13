#ifndef EMULATOR_CPU_REGISTERS_H_
#define EMULATOR_CPU_REGISTERS_H_

#include "emulator/instruction/register_index.h"

#include <array>
#include <cstddef>
#include <cstdint>

namespace emulator {

inline constexpr std::size_t kNumRegisters = 32;

class Registers {
 public:
  uint32_t &operator[](const RegisterIndex index);

 private:
  std::array<uint32_t, kNumRegisters> data_;
};

}  // namespace emulator

#endif  // EMULATOR_CPU_REGISTERS_H_
