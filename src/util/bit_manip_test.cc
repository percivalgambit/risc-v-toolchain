#include "util/bit_manip.h"

#include <cstdint>

#include "test_util/catch.hpp"

using util::BitSlice;
using util::ExtendSign;
using util::FitsBits;
using util::SetIf;

// TODO: Write a testable assert + test assertions
TEST_CASE("BitSlice can take slices of bits") {
  uint32_t bits = 0b110111;
  REQUIRE(BitSlice(bits, 2, 4) == 0b101);
  REQUIRE(BitSlice(static_cast<int32_t>(bits), 2, 4) == 0b101);
  REQUIRE(BitSlice(bits, 0, 31) == bits);
  REQUIRE(BitSlice(static_cast<int32_t>(bits), 0, 31) ==
          static_cast<int32_t>(bits));
}

TEST_CASE(
    "SetIf returns all bits set if its input is nonzero, and returns 0 "
    "otherwise") {
  REQUIRE(SetIf(2) == ~0);
  REQUIRE(SetIf(2u) == ~0);
  REQUIRE(SetIf(0) == 0);
  REQUIRE(SetIf(0u) == 0);
}

TEST_CASE(
    "FitsBits checks if its input can fit into a specified number of bits") {
  uint32_t bits = 0b110;
  REQUIRE(FitsBits(bits, 5));
  REQUIRE(FitsBits(static_cast<int32_t>(bits), 5));
  REQUIRE_FALSE(FitsBits(bits, 1));
  REQUIRE_FALSE(FitsBits(static_cast<int32_t>(bits), 1));
  REQUIRE(FitsBits(bits, 32));
  REQUIRE(FitsBits(static_cast<int32_t>(bits), 32));
}

TEST_CASE("ExtendSign extends the sign of the specified bits and bit size") {
  uint8_t bits = 0b101;
  REQUIRE(ExtendSign(bits, 4) == bits);
  REQUIRE(ExtendSign(static_cast<int8_t>(bits), 4) ==
          static_cast<int8_t>(bits));
  REQUIRE(ExtendSign(bits, 3) == 0b11111101);
  REQUIRE(ExtendSign(static_cast<int8_t>(bits), 3) ==
          static_cast<int8_t>(0b11111101));
  REQUIRE(ExtendSign(bits, 2) == 0b00000001);
  REQUIRE(ExtendSign(static_cast<int8_t>(bits), 2) ==
          static_cast<int8_t>(0b00000001));
  REQUIRE(ExtendSign(bits, 1) == 0b11111111);
  REQUIRE(ExtendSign(static_cast<int8_t>(bits), 1) ==
          static_cast<int8_t>(0b11111111));
}
