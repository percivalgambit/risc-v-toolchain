#ifndef EMULATOR_CPU_CPU_H_
#define EMULATOR_CPU_CPU_H_

#include "emulator/cpu/memory.h"
#include "emulator/cpu/registers.h"
#include "emulator/instruction/instruction.h"
#include "util/status.h"

#include <istream>

namespace emulator {

// TODO: Figure out how to test Cpu
class Cpu {
 public:
  explicit Cpu(std::istream *program);

  util::Status Run();

  void Execute(const Instruction &instruction);

 private:
  void ExecuteLoad(const Instruction &instruction);
  void ExecuteStore(const Instruction &instruction);
  void ExecuteBranch(const Instruction &instruction);
  void ExecuteJalr(const Instruction &instruction);
  void ExecuteMiscMem(const Instruction &instruction);
  void ExecuteOpImm(const Instruction &instruction);
  void ExecuteOp(const Instruction &instruction);
  void ExecuteSystem(const Instruction &instruction);

  Registers registers_;
  Memory memory_;
  uint32_t program_size_;
};

}  // namespace emulator

#endif  // EMULATOR_CPU_CPU_H_
