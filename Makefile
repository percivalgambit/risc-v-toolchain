include make/all.mk
include make/flags.mk
include make/variables.mk
include make/functions.mk
include make/objects.mk
include make/special_rules.mk

# TODO: split up into per-directory Makefiles
STATUS_SRC := src/util/status.cc
STATUSOR_SRC := $(STATUS_SRC)
INSTRUCTION_SRC := $(shell find src/emulator/instruction/ -type f -name "*.cc" -not -name "*_test.cc")
CPU_SRC := $(shell find src/emulator/cpu/ -type f -name "*.cc" -not -name "*_test.cc") $(INSTRUCTION_SRC) $(STATUS_SRC)
EMULATOR_SRC := src/emulator/main.cc $(CPU_SRC) $(STATUS_SRC)

$(eval $(call binary,emulator,$(EMULATOR_SRC)))

$(eval $(call test,status_test,src/util/status_test.cc $(STATUS_SRC)))
$(eval $(call test,statusor_test,src/util/statusor_test.cc $(STATUSOR_SRC)))
$(eval $(call test,bit_manip_test,src/util/bit_manip_test.cc))
$(eval $(call test,instruction_test,src/emulator/instruction/instruction_test.cc $(INSTRUCTION_SRC)))
