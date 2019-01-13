#include "emulator/instruction/instruction.h"

#include "test_util/catch.hpp"

#include "emulator/instruction/instruction_operation.h"
#include "emulator/instruction/opcode.h"
#include "emulator/instruction/register_index.h"

using emulator::Instruction;
using emulator::InstructionOperation;
using emulator::Opcode;
using emulator::RegisterIndex;

TEST_CASE("Bits can be decoded into Instruction objects") {
  Instruction register_instruction(0b0100000'00010'00001'000'00000'0110011);
  REQUIRE(register_instruction.Code() == Opcode::OP);
  REQUIRE(register_instruction.Dest() == RegisterIndex::X0);
  REQUIRE(register_instruction.Funct3() == InstructionOperation::SUB);
  REQUIRE(register_instruction.Reg1() == RegisterIndex::X1);
  REQUIRE(register_instruction.Reg2() == RegisterIndex::X2);
  REQUIRE(register_instruction.Funct7() == 0b0100000);
  REQUIRE(register_instruction.Immediate() == 0);

  Instruction immediate_instruction(0b010000000000'11111'000'00001'0010011);
  REQUIRE(immediate_instruction.Code() == Opcode::OP_IMM);
  REQUIRE(immediate_instruction.Dest() == RegisterIndex::X1);
  REQUIRE(immediate_instruction.Funct3() == InstructionOperation::ADDI);
  REQUIRE(immediate_instruction.Reg1() == RegisterIndex::X31);
  REQUIRE(immediate_instruction.Immediate() == 0b010000000000);

  Instruction store_instruction(0b1000000'00001'00000'011'11111'0100011);
  REQUIRE(store_instruction.Code() == Opcode::STORE);
  REQUIRE(store_instruction.Funct3() == InstructionOperation(0b011));
  REQUIRE(store_instruction.Reg1() == RegisterIndex::X0);
  REQUIRE(store_instruction.Reg2() == RegisterIndex::X1);
  REQUIRE(store_instruction.Immediate() == 0b11111111111111111111100000011111);

  Instruction branch_instruction(0b0'000000'00011'00010'001'0000'1'1100011);
  REQUIRE(branch_instruction.Code() == Opcode::BRANCH);
  REQUIRE(branch_instruction.Funct3() == InstructionOperation::BNE);
  REQUIRE(branch_instruction.Reg1() == RegisterIndex::X2);
  REQUIRE(branch_instruction.Reg2() == RegisterIndex::X3);
  REQUIRE(branch_instruction.Immediate() == 0b0'1'000000'0000'0);

  Instruction upper_instruction(0b10000000000000000001'00001'0010111);
  REQUIRE(upper_instruction.Code() == Opcode::AUIPC);
  REQUIRE(upper_instruction.Dest() == RegisterIndex::X1);
  REQUIRE(upper_instruction.Immediate() == 0b10000000000000000001'000000000000);

  Instruction jump_instruction(0b1'0000000001'1'00000000'00001'1101111);
  REQUIRE(jump_instruction.Code() == Opcode::JAL);
  REQUIRE(jump_instruction.Dest() == RegisterIndex::X1);
  REQUIRE(jump_instruction.Immediate() ==
          0b111111111111'00000000'1'0000000001'0);
}
