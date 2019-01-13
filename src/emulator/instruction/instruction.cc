#include "emulator/instruction/instruction.h"

#include "emulator/instruction/instruction_operation.h"
#include "emulator/instruction/opcode.h"
#include "emulator/instruction/register_index.h"
#include "util/bit_manip.h"
#include "util/logging.h"

#include <cassert>
#include <cinttypes>

namespace emulator {

using ::util::BitSlice;
using ::util::SetIf;

Instruction::Instruction(const uint32_t instruction_bytes)
    : opcode_(Opcode(BitSlice(instruction_bytes, 0, 6))),
      type_(InitType(opcode_)),
      dest_(RegisterIndex(BitSlice(instruction_bytes, 7, 11))),
      funct3_(InstructionOperation(BitSlice(instruction_bytes, 12, 14))),
      reg1_(RegisterIndex(BitSlice(instruction_bytes, 15, 19))),
      reg2_(RegisterIndex(BitSlice(instruction_bytes, 20, 24))),
      funct7_(static_cast<uint8_t>(BitSlice(instruction_bytes, 25, 31))),
      immediate_(InitImmediate(instruction_bytes, type_)) {}

Opcode Instruction::Code() const { return opcode_; }

RegisterIndex Instruction::Dest() const {
  assert(type_ == Type::REGISTER || type_ == Type::IMMEDIATE ||
         type_ == Type::UPPER || type_ == Type::JUMP);
  return dest_;
}

InstructionOperation Instruction::Funct3() const {
  assert(type_ == Type::REGISTER || type_ == Type::IMMEDIATE ||
         type_ == Type::STORE || type_ == Type::BRANCH);
  return funct3_;
}

RegisterIndex Instruction::Reg1() const {
  assert(type_ == Type::REGISTER || type_ == Type::IMMEDIATE ||
         type_ == Type::STORE || type_ == Type::BRANCH);
  return reg1_;
}

RegisterIndex Instruction::Reg2() const {
  assert(type_ == Type::REGISTER || type_ == Type::STORE ||
         type_ == Type::BRANCH);
  return reg2_;
}

uint8_t Instruction::Funct7() const {
  assert(type_ == Type::REGISTER);
  return funct7_;
}

uint32_t Instruction::Immediate() const { return immediate_; }

Instruction::Type Instruction::InitType(const Opcode opcode) {
  switch (opcode) {
    case Opcode::OP:
      return Type::REGISTER;
    case Opcode::LOAD:
    case Opcode::JALR:
    case Opcode::MISC_MEM:
    case Opcode::OP_IMM:
    case Opcode::SYSTEM:
      return Type::IMMEDIATE;
    case Opcode::STORE:
      return Type::STORE;
    case Opcode::BRANCH:
      return Type::BRANCH;
    case Opcode::AUIPC:
    case Opcode::LUI:
      return Type::UPPER;
    case Opcode::JAL:
      return Type::JUMP;
    default:
      FATAL("Unknown opcode %" PRIu8, static_cast<uint8_t>(opcode));
  }
}

uint32_t Instruction::InitImmediate(const uint32_t instruction_bytes,
                                    const Type type) {
  switch (type) {
    case Type::REGISTER: {
      return 0;
    }
    case Type::IMMEDIATE: {
      const uint32_t bit0_11 = BitSlice(instruction_bytes, 20, 31);
      return util::ExtendSign(bit0_11, 12);
    }
    case Type::STORE: {
      const uint32_t bit0_4 = BitSlice(instruction_bytes, 7, 11);
      const uint32_t bit5_11 = BitSlice(instruction_bytes, 25, 31) << 5;
      return util::ExtendSign(bit0_4 | bit5_11, 12);
    }
    case Type::BRANCH: {
      const uint32_t bit11 = BitSlice(instruction_bytes, 7, 7) << 11;
      const uint32_t bit1_4 = BitSlice(instruction_bytes, 8, 11) << 1;
      const uint32_t bit5_10 = BitSlice(instruction_bytes, 25, 30) << 5;
      const uint32_t bit12 = BitSlice(instruction_bytes, 31, 31) << 12;
      return util::ExtendSign(bit11 | bit1_4 | bit5_10 | bit12, 13);
    }
    case Type::UPPER: {
      return BitSlice(instruction_bytes, 12, 31) << 12;
    }
    case Type::JUMP: {
      const uint32_t bit12_19 = BitSlice(instruction_bytes, 12, 19) << 12;
      const uint32_t bit11 = BitSlice(instruction_bytes, 20, 20) << 11;
      const uint32_t bit1_10 = BitSlice(instruction_bytes, 21, 30) << 1;
      const uint32_t bit20 = BitSlice(instruction_bytes, 31, 31) << 20;
      return util::ExtendSign(bit12_19 | bit11 | bit1_10 | bit20, 21);
    }
    default:
      FATAL("Unknown instruction type %" PRIu8, static_cast<uint8_t>(type));
  }
}

}  // namespace emulator
