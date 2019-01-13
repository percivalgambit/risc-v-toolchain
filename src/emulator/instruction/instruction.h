#ifndef EMULATOR_INSTRUCTION_INSTRUCTION_H_
#define EMULATOR_INSTRUCTION_INSTRUCTION_H_

#include "emulator/instruction/instruction_operation.h"
#include "emulator/instruction/opcode.h"
#include "emulator/instruction/register_index.h"

#include <cstdint>

namespace emulator {

class Instruction {
 public:
  explicit Instruction(const uint32_t instruction_bytes);

  Opcode Code() const;

  RegisterIndex Dest() const;

  InstructionOperation Funct3() const;

  RegisterIndex Reg1() const;

  RegisterIndex Reg2() const;

  uint8_t Funct7() const;

  uint32_t Immediate() const;

 private:
  enum class Type { REGISTER, IMMEDIATE, STORE, BRANCH, UPPER, JUMP };

  static Type InitType(const Opcode opcode);

  static uint32_t InitImmediate(const uint32_t instruction_bytes,
                                const Type type);

  Opcode opcode_;
  Type type_;
  RegisterIndex dest_;
  InstructionOperation funct3_;
  RegisterIndex reg1_;
  RegisterIndex reg2_;
  uint8_t funct7_;
  uint32_t immediate_;
};

}  // namespace emulator

#endif  // EMULATOR_INSTRUCTION_INSTRUCTION_H_
