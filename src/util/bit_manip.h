#ifndef UTIL_BIT_MANIP_H_
#define UTIL_BIT_MANIP_H_

#include <cassert>
#include <climits>
#include <cstddef>
#include <type_traits>

#include <cstdio>

namespace util {

// Returns the bit in [high, low], shifted down to position 0.
template <typename T>
constexpr T BitSlice(const T bits, const unsigned low, const unsigned high) {
  const size_t slice_size = high - low + 1;
  const size_t bits_size = sizeof(bits) * 8;

  assert(low <= high);
  assert(high < bits_size);

  const T mask =
      slice_size == bits_size
          ? static_cast<T>(~0)
          : ~(static_cast<std::make_unsigned_t<T>>(~0) << slice_size);
  return (bits >> low) & mask;
}

template <typename T>
constexpr T SetIf(const T bits) {
  return bits ? static_cast<T>(~0) : 0;
}

// Returns true if bits fits in num_bits bits
template <typename T>
constexpr bool FitsBits(const T bits, const unsigned num_bits) {
  const size_t max_bit_size = sizeof(T) * 8;
  assert(num_bits <= max_bit_size);
  const T mask = num_bits == max_bit_size
                     ? 0
                     : static_cast<std::make_unsigned_t<T>>(~0) << num_bits;
  return !(bits & mask);
}

template <typename T>
constexpr T ExtendSign(const T bits, const unsigned bit_size) {
  assert(bit_size != 0);
  assert(bit_size <= sizeof(T) * 8);

  const T bit_pos = static_cast<T>(1 << (bit_size - 1));
  const T mask =
      static_cast<T>(~(static_cast<std::make_unsigned_t<T>>(~0) << bit_size));
  return static_cast<T>(((bits & mask) ^ bit_pos) - bit_pos);
}

}  // namespace util

#endif  // UTIL_BIT_MANIP_H_
