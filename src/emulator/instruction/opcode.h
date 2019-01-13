#ifndef EMULATOR_INSTRUCTION_OPCODE_H_
#define EMULATOR_INSTRUCTION_OPCODE_H_

#include <cstdint>

namespace emulator {

// TODO: write StrongInt class + make sure all enums fit into x bits
enum class Opcode : uint8_t {
  LOAD = 0b0000011,
  STORE = 0b0100011,
  BRANCH = 0b1100011,
  JALR = 0b1100111,
  MISC_MEM = 0b0001111,
  JAL = 0b1101111,
  OP_IMM = 0b0010011,
  OP = 0b0110011,
  SYSTEM = 0b1110011,
  AUIPC = 0b0010111,
  LUI = 0b0110111,
};

}  // namespace emulator

#endif  // EMULATOR_INSTRUCTION_OPCODE_H_
