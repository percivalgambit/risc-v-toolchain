$(call add_flag,extra_warnings)

SHELL := bash
CXX := g++
CXXFLAGS := -std=c++17 -pedantic -Wall -Wextra -march=native -Isrc/ -pipe -MMD -MP -fdiagnostics-show-template-tree -fno-exceptions -fno-rtti

release_CPPFLAGS := -DNDEBUG
release_CXXFLAGS := -O3 -flto -fno-fat-lto-objects
release_LDFLAGS := -flto -fno-fat-lto-objects

debug_CPPFLAGS := -DDEBUG
debug_CXXFLAGS := -Og -ggdb3 -Werror
debug_LDFLAGS :=

WARNINGS_CXXFLAGS := \
  -Wunused -Wunused-macros \
  -Wdouble-promotion -Wfloat-equal \
  -Wformat=2  \
  -Wnull-dereference \
  -Wuninitialized -Winit-self -Wundef \
  -Wmissing-include-dirs \
  -Wstrict-overflow=5 \
  -Wwrite-strings \
  -Wconversion \
  -Wshadow -Wredundant-decls \
  -Wmissing-declarations \
  -Wpacked \
  -Wold-style-cast \
  -Wzero-as-null-pointer-constant \
  -Wextra-semi
ifeq ($(CXX),g++)
WARNINGS_CXXFLAGS += \
  -Wunused-const-variable=2 \
  -Wformat-overflow=2 -Wformat-truncation=2 -Wformat-signedness \
  -Wshift-overflow=2 \
  -Wstringop-overflow=4 -Wstringop-truncation \
  -Walloc-zero \
  -Warray-bounds=2 \
  -Wcast-align=strict \
  -Wduplicated-cond -Wduplicated-branches \
  -Wlogical-op \
  -Wunsafe-loop-optimizations \
  -Wtrampolines \
  -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override \
  -Wuseless-cast \
  -Wplacement-new=2
endif
debug_CXXFLAGS += $(WARNINGS_CXXFLAGS)
release_CXXFLAGS += $(if $(extra_warnings),$(WARNINGS_CXXFLAGS))
