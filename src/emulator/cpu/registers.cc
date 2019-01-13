#include "emulator/cpu/registers.h"

#include "emulator/instruction/register_index.h"

#include <cstdint>
#include <type_traits>

namespace emulator {

uint32_t &Registers::operator[](const RegisterIndex index) {
  data_[0] = 0;
  return data_[static_cast<std::underlying_type_t<RegisterIndex>>(index)];
}

}  // namespace emulator
