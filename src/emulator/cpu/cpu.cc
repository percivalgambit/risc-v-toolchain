#include "emulator/cpu/cpu.h"

#include "emulator/cpu/registers.h"
#include "emulator/instruction/instruction.h"
#include "emulator/instruction/instruction_operation.h"
#include "emulator/instruction/opcode.h"
#include "util/bit_manip.h"
#include "util/logging.h"
#include "util/status.h"

#include <cassert>
#include <cinttypes>
#include <istream>
#include <type_traits>

namespace emulator {

using ::util::BitSlice;
using ::util::Status;

Cpu::Cpu(std::istream *program) {
  program->read(reinterpret_cast<char *>(memory_.data()), kMemorySize);
  program_size_ = static_cast<uint32_t>(program->gcount());
}

Status Cpu::Run() {
  while (registers_[RegisterIndex::PC] < program_size_) {
    const Instruction instruction(registers_[RegisterIndex::PC]);
    Execute(instruction);
    registers_[RegisterIndex::PC] += static_cast<uint32_t>(sizeof(uint32_t));
  }
  return Status::OK;
}

void Cpu::Execute(const Instruction &instruction) {
  switch (instruction.Code()) {
    case Opcode::LOAD: {
      ExecuteLoad(instruction);
      break;
    }
    case Opcode::STORE:
      ExecuteStore(instruction);
      break;
    case Opcode::BRANCH:
      ExecuteBranch(instruction);
      break;
    case Opcode::JALR:
      ExecuteJalr(instruction);
      break;
    case Opcode::MISC_MEM:
      ExecuteMiscMem(instruction);
      break;
    case Opcode::JAL:
      registers_[instruction.Dest()] = registers_[RegisterIndex::PC] +
                                       static_cast<uint32_t>(sizeof(uint32_t));
      registers_[RegisterIndex::PC] += instruction.Immediate();
      break;
    case Opcode::OP_IMM:
      ExecuteOpImm(instruction);
      break;
    case Opcode::OP:
      ExecuteOp(instruction);
      break;
    case Opcode::SYSTEM:
      ExecuteSystem(instruction);
      break;
    case Opcode::AUIPC:
      registers_[instruction.Dest()] =
          instruction.Immediate() + registers_[RegisterIndex::PC];
      break;
    case Opcode::LUI:
      registers_[instruction.Dest()] = instruction.Immediate();
      break;
    default:
      FATAL("Invalid opcode for instruction: %" PRIu8,
            static_cast<std::underlying_type_t<Opcode>>(instruction.Code()));
  }
}

void Cpu::ExecuteLoad(const Instruction &instruction) {
  uint32_t *dest = &registers_[instruction.Dest()];
  const uint32_t reg1 = registers_[instruction.Reg1()];
  const uint32_t imm = instruction.Immediate();
  *dest = 0;
  for (unsigned i = 0; i < sizeof(uint32_t); i++) {
    *dest |= static_cast<uint32_t>(memory_[reg1 + imm + i]) << (i * 8);
  }
}

void Cpu::ExecuteStore(const Instruction &instruction) {
  const uint32_t reg1 = registers_[instruction.Reg1()];
  const uint32_t reg2 = registers_[instruction.Reg1()];
  const uint32_t imm = instruction.Immediate();
  uint8_t *dest = &memory_[reg1 + imm];
  for (unsigned i = 0; i < sizeof(uint32_t); i++) {
    dest[i] = static_cast<uint8_t>(BitSlice(reg2, i * 8, (i + 1) * 8));
  }
}

void Cpu::ExecuteBranch(const Instruction &instruction) {
  const uint32_t reg1 = registers_[instruction.Reg1()];
  const uint32_t reg2 = registers_[instruction.Reg1()];
  bool cond;
  switch (instruction.Funct3()) {
    case InstructionOperation::BEQ:
      cond = reg1 == reg2;
      break;
    case InstructionOperation::BNE:
      cond = reg1 != reg2;
      break;
    case InstructionOperation::BLT:
      cond = static_cast<int32_t>(reg1) < static_cast<int32_t>(reg2);
      break;
    case InstructionOperation::BLTU:
      cond = reg1 < reg2;
      break;
    case InstructionOperation::BGE:
      cond = static_cast<int32_t>(reg1) > static_cast<int32_t>(reg2);
      break;
    case InstructionOperation::BGEU:
      cond = reg1 > reg2;
      break;
    default:
      FATAL("Invalid funct3 for BRANCH: %" PRIu8,
            static_cast<std::underlying_type_t<InstructionOperation>>(
                instruction.Funct3()));
  }
  if (cond) {
    registers_[RegisterIndex::PC] += instruction.Immediate();
  }
}

void Cpu::ExecuteJalr(const Instruction &instruction) {
  assert(instruction.Funct3() == InstructionOperation::JALR);
  registers_[instruction.Dest()] =
      registers_[RegisterIndex::PC] + static_cast<uint32_t>(sizeof(uint32_t));
  registers_[RegisterIndex::PC] =
      (registers_[instruction.Reg1()] + instruction.Immediate()) &
      (static_cast<uint32_t>(~0) << 1);
}

void Cpu::ExecuteMiscMem(const Instruction &instruction) {
  switch (instruction.Funct3()) {
    case InstructionOperation::FENCE:
    case InstructionOperation::FENCE_I:
      // TODO: implement MISC-MEM instructions and harts
      break;
    default:
      FATAL("Invalid funct3 for MISC-MEM: %" PRIu8,
            static_cast<std::underlying_type_t<InstructionOperation>>(
                instruction.Funct3()));
  }
}

void Cpu::ExecuteOpImm(const Instruction &instruction) {
  uint32_t *dest = &registers_[instruction.Dest()];
  const uint32_t reg1 = registers_[instruction.Reg1()];
  const uint32_t imm = instruction.Immediate();
  switch (instruction.Funct3()) {
    case InstructionOperation::ADDI:
      if (instruction.Dest() == RegisterIndex::X0 &&
          instruction.Reg1() == RegisterIndex::X0 &&
          instruction.Immediate() == 0) {
        // NOP
        break;
      }
      *dest = reg1 + imm;
      break;
    case InstructionOperation::SLTI:
      *dest = static_cast<int32_t>(reg1) < static_cast<int32_t>(imm) ? 1 : 0;
      break;
    case InstructionOperation::SLTIU:
      *dest = reg1 < imm ? 1 : 0;
      break;
    case InstructionOperation::ANDI:
      *dest = reg1 & imm;
      break;
    case InstructionOperation::ORI:
      *dest = reg1 | imm;
      break;
    case InstructionOperation::XORI:
      *dest = reg1 ^ imm;
      break;
    case InstructionOperation::SLLI: {
      assert(BitSlice(imm, 5, 11) == 0);
      const uint32_t shift = BitSlice(imm, 0, 4);
      *dest = reg1 << shift;
      break;
    }
    case InstructionOperation::SRLI: {
      const uint32_t funct7 = BitSlice(imm, 5, 11);
      const uint32_t shift = BitSlice(imm, 0, 4);
      if (shift == 0) {
        break;
      }
      const uint32_t right_fill_mask = static_cast<uint32_t>(~0)
                                       << (32 - shift);
      if (funct7 == 0) {
        // SRLI
        *dest = (reg1 >> shift) & ~right_fill_mask;
      } else if (funct7 == 0b0100000) {
        // SRAI
        *dest = (reg1 >> shift) | right_fill_mask;
      } else {
        FATAL("Invalid funct7 for SRLI: %" PRIu32, funct7);
      }
      break;
    }
    default:
      FATAL("Invalid funct3 for OP-IMM: %" PRIu8,
            static_cast<std::underlying_type_t<InstructionOperation>>(
                instruction.Funct3()));
  }
}

void Cpu::ExecuteOp(const Instruction &instruction) {
  uint32_t *dest = &registers_[instruction.Dest()];
  const uint32_t reg1 = registers_[instruction.Reg1()];
  const uint32_t reg2 = registers_[instruction.Reg2()];
  const uint32_t funct7 = instruction.Funct7();
  switch (instruction.Funct3()) {
    case InstructionOperation::ADD:
      if (funct7 == 0) {
        // ADD
        *dest = reg1 + reg2;
      } else if (funct7 == 0b0100000) {
        // SUB
        *dest = reg1 - reg2;
      } else {
        FATAL("Invalid funct7 for ADD: %" PRIu32, funct7);
      }
      break;
    case InstructionOperation::SLT:
      assert(funct7 == 0);
      *dest = static_cast<int32_t>(reg1) < static_cast<int32_t>(reg2) ? 1 : 0;
      break;
    case InstructionOperation::SLTU:
      assert(funct7 == 0);
      *dest = reg1 < reg2 ? 1 : 0;
      break;
    case InstructionOperation::AND:
      assert(funct7 == 0);
      *dest = reg1 & reg2;
      break;
    case InstructionOperation::OR:
      assert(funct7 == 0);
      *dest = reg1 | reg2;
      break;
    case InstructionOperation::XOR:
      assert(funct7 == 0);
      *dest = reg1 ^ reg2;
      break;
    case InstructionOperation::SLL: {
      assert(funct7 == 0);
      const uint32_t shift = BitSlice(reg2, 0, 4);
      *dest = reg1 << shift;
      break;
    }
    case InstructionOperation::SRL: {
      const uint32_t shift = BitSlice(reg2, 0, 4);
      if (shift == 0) {
        break;
      }
      const uint32_t right_fill_mask = static_cast<uint32_t>(~0)
                                       << (32 - shift);

      if (funct7 == 0) {
        // SRL
        *dest = (reg1 >> shift) & ~right_fill_mask;
      } else if (funct7 == 0b0100000) {
        // SRA
        *dest = (reg1 >> shift) | right_fill_mask;
      } else {
        FATAL("Invalid funct7 for SRL: %" PRIu32, funct7);
      }
      break;
    }
    default:
      FATAL("Invalid funct3 for OP: %" PRIu8,
            static_cast<std::underlying_type_t<InstructionOperation>>(
                instruction.Funct3()));
  }
}

void Cpu::ExecuteSystem(const Instruction &instruction) {
  // TODO: implrment SYSTEM instructions and CSRs
  switch (instruction.Funct3()) {
    case InstructionOperation::CSRRW:
    case InstructionOperation::CSRRS:
    case InstructionOperation::CSRRC:
    case InstructionOperation::CSRRWI:
    case InstructionOperation::CSRRSI:
    case InstructionOperation::CSRRCI:
    case InstructionOperation::PRIV:
    default:
      FATAL("Invalid funct3 for SYSTEM: %" PRIu8,
            static_cast<std::underlying_type_t<InstructionOperation>>(
                instruction.Funct3()));
  }
}

}  // namespace emulator
