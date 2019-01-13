#ifndef EMULATOR_INSTRUCTION_INSTRUCTION_OPERATION_H_
#define EMULATOR_INSTRUCTION_INSTRUCTION_OPERATION_H_

#include <cstdint>

namespace emulator {

enum class InstructionOperation : uint8_t {
  ADDI = 0b000,
  NOP = ADDI,
  SLTI = 0b010,
  SLTIU = 0b011,
  ANDI = 0b111,
  ORI = 0b110,
  XORI = 0b100,
  SLLI = 0b001,
  SRLI = 0b101,
  SRAI = SRLI,
  ADD = 0b000,
  SUB = ADD,
  SLT = 0b010,
  SLTU = 0b011,
  AND = 0b111,
  OR = 0b110,
  XOR = 0b100,
  SLL = 0b001,
  SRL = 0b101,
  SRA = SRL,
  JALR = 0b000,
  BEQ = 0b000,
  BNE = 0b001,
  BLT = 0b100,
  BLTU = 0b110,
  BGE = 0b101,
  BGEU = 0b111,
  FENCE = 0b000,
  FENCE_I = 0b001,
  CSRRW = 0b001,
  CSRRS = 0b010,
  CSRRC = 0b011,
  CSRRWI = 0b101,
  CSRRSI = 0b110,
  CSRRCI = 0b111,
  PRIV = 0b000,
};

}  // namespace emulator

#endif  // EMULATOR_INSTRUCTION_INSTRUCTION_OPERATION_H_
