#include <cstdlib>
#include <fstream>
#include <iostream>

#include "emulator/cpu/cpu.h"
#include "util/logging.h"
#include "util/status.h"

using ::emulator::Cpu;
using ::util::Status;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " <program_file>" << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream program(argv[1], std::ios::binary);
  if (program.fail()) {
    FATAL("Unable to open %s", argv[1]);
  }
  Cpu cpu(&program);
  const Status& status = cpu.Run();
  if (!status.IsOk()) {
    ERROR("%s", status.ToString().c_str());
  }
  return static_cast<int>(status.StatusCode());
}
