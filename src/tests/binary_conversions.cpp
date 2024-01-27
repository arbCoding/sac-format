// Copyright 2023-2024 Alexander R. Blanchette

// Required for Catch2 compatibility on Windows
#if defined(_MSC_VER) || defined(__MINW32__)
#define _ITERATOR_DEBUG_LEVEL 2
#endif

// sac-format
#include "sac-format/sac_format.hpp"
#include "sac-format/util.hpp"
// Standard Library
//   https://en.cppreference.com/w/cpp/standard_library
// Catch2 https://github.com/catchorg/Catch2/tree/v3.4.0
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_MAIN
// testing macros
// TEST_CASE, REQUIRE, CAPTURE
#include <catch2/catch_test_macros.hpp>

namespace sacfmt {
// NOLINTBEGIN(readability-magic-numbers)
TEST_CASE("Binary Conversion: Booleans") {
  REQUIRE(unset_bool == false);
  REQUIRE(!unset_bool == true);
  REQUIRE(binary_to_bool(bool_to_binary(unset_bool)) == false);
  REQUIRE(binary_to_bool(bool_to_binary(!unset_bool)) == true);
}

TEST_CASE("Binary Conversion: Integer: Zero") {
  REQUIRE(binary_to_int(int_to_binary(0)) == 0);
  REQUIRE(binary_to_int(int_to_binary(-0)) == 0);
}

TEST_CASE("Binary Conversion: Integer: Negative") {
  REQUIRE(binary_to_int(int_to_binary(-1)) == -1);
  REQUIRE(binary_to_int(int_to_binary(-10)) == -10);
  REQUIRE(binary_to_int(int_to_binary(lowest_i)) == lowest_i);
}

TEST_CASE("Binary Conversion: Integer: Positive") {
  REQUIRE(binary_to_int(int_to_binary(1)) == 1);
  REQUIRE(binary_to_int(int_to_binary(10)) == 10);
  REQUIRE(binary_to_int(int_to_binary(highest_i)) == highest_i);
}

TEST_CASE("Binary Conversion: Float: Zero") {
  REQUIRE(binary_to_float(float_to_binary(0.0F)) == 0.0F);
  REQUIRE(binary_to_float(float_to_binary(-0.0F)) == 0.0F);
}

TEST_CASE("Binary Conversion: Float: Negative") {
  REQUIRE(binary_to_float(float_to_binary(-1.0F)) == -1.0F);
  REQUIRE(binary_to_float(float_to_binary(-0.1F)) == -0.1F);
  REQUIRE(binary_to_float(float_to_binary(-10.0F)) == -10.0F);
  REQUIRE(binary_to_float(float_to_binary(lowest_f)) == lowest_f);
  CAPTURE(s_neg_epsilon_f);
  REQUIRE(binary_to_float(float_to_binary(neg_epsilon_f)) == neg_epsilon_f);
}

TEST_CASE("Binary Conversion: Float: Positive") {
  REQUIRE(binary_to_float(float_to_binary(1.0F)) == 1.0F);
  REQUIRE(binary_to_float(float_to_binary(0.1F)) == 0.1F);
  REQUIRE(binary_to_float(float_to_binary(10.0F)) == 10.0F);
  REQUIRE(binary_to_float(float_to_binary(highest_f)) == highest_f);
  CAPTURE(s_epsilon_f);
  REQUIRE(binary_to_float(float_to_binary(epsilon_f)) == epsilon_f);
}

TEST_CASE("Binary Conversion: Double: Zero") {
  REQUIRE(binary_to_double(double_to_binary(0.0)) == 0.0);
  REQUIRE(binary_to_double(double_to_binary(-0.0)) == 0.0);
}

TEST_CASE("Binary Conversion: Double: Negative") {
  REQUIRE(binary_to_double(double_to_binary(-1.0)) == -1.0);
  REQUIRE(binary_to_double(double_to_binary(-0.1)) == -0.1);
  REQUIRE(binary_to_double(double_to_binary(-10.0)) == -10.0);
  REQUIRE(binary_to_double(double_to_binary(lowest_d)) == lowest_d);
  CAPTURE(s_neg_epsilon_d);
  REQUIRE(binary_to_double(double_to_binary(neg_epsilon_d)) == neg_epsilon_d);
}

TEST_CASE("Binary Conversion: Double: Positive") {
  REQUIRE(binary_to_double(double_to_binary(1.0)) == 1.0);
  REQUIRE(binary_to_double(double_to_binary(0.1)) == 0.1);
  REQUIRE(binary_to_double(double_to_binary(10.0)) == 10.0);
  REQUIRE(binary_to_double(double_to_binary(highest_d)) == highest_d);
  CAPTURE(s_epsilon_d);
  REQUIRE(binary_to_double(double_to_binary(epsilon_d)) == epsilon_d);
}

TEST_CASE("Binary Conversion: Strings: Two Words: Perfect") {
  // cppcheck-suppress unreadVariable
  const std::string test_str{"01234567"};
  CAPTURE(test_str);
  REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
}

TEST_CASE("Binary Conversion: Strings: Two Words: Empty") {
  // cppcheck-suppress unreadVariable
  const std::string test_str{};
  CAPTURE(test_str);
  REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
}

TEST_CASE("Binary Conversion: Strings: Two Words: One Word") {
  // cppcheck-suppress unreadVariable
  const std::string test_str{"0123"};
  CAPTURE(test_str);
  REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
}

TEST_CASE("Binary Conversion: Strings: Two Words: Half Word") {
  // cppcheck-suppress unreadVariable
  const std::string test_str{"012"};
  CAPTURE(test_str);
  REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
}

TEST_CASE("Binary Conversion: Strings: Two Words: Overflow") {
  // cppcheck-suppress unreadVariable
  const std::string test_str{"0123456789ABCDEFG"};
  // cppcheck-suppress unreadVariable
  const std::string result_str{"01234567"};
  CAPTURE(test_str);
  REQUIRE(binary_to_string(string_to_binary(test_str)) == result_str);
  REQUIRE(binary_to_string(string_to_binary(test_str)) ==
          test_str.substr(0, 8));
}

TEST_CASE("Binary Conversion: Strings: Two Words: Multiple Spaces") {
  // cppcheck-suppress unreadVariable
  const std::string test_str{"Hi  Bye"};
  CAPTURE(test_str);
  REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
}

TEST_CASE("Binary Conversion: Strings: Two Words: Leading/Trailing Spaces") {
  // cppcheck-suppress unreadVariable
  const std::string test_str{"  Time  "};
  // cppcheck-suppress unreadVariable
  const std::string result_str{"Time"};
  CAPTURE(test_str);
  REQUIRE(binary_to_string(string_to_binary(test_str)) == result_str);
}

TEST_CASE("Binary Conversion: Strings: Four Words: Perfect") {
  // cppcheck-suppress unreadVariable
  const std::string test_str{"0123456789ABCDEF"};
  CAPTURE(test_str);
  REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) == test_str);
}

TEST_CASE("Binary Conversion: Strings: Four Words: Empty") {
  // cppcheck-suppress unreadVariable
  const std::string test_str{};
  CAPTURE(test_str);
  REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) == test_str);
}

TEST_CASE("Binary Conversion: Strings: Four Words: One Word") {
  // cppcheck-suppress unreadVariable
  const std::string test_str{"0123"};
  CAPTURE(test_str);
  REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) == test_str);
}

TEST_CASE("Binary Conversion: Strings: Four Words: Half Word") {
  // cppcheck-suppress unreadVariable
  const std::string test_str{"01"};
  CAPTURE(test_str);
  REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) == test_str);
}

TEST_CASE("Binary Conversion: Strings: Four Words: Three Words") {
  // cppcheck-suppress unreadVariable
  const std::string test_str{"0123456789AB"};
  CAPTURE(test_str);
  REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) == test_str);
}

TEST_CASE("Binary Conversion: Strings: Four Words: Three and a Half Words") {
  // cppcheck-suppress unreadVariable
  const std::string test_str{"0123456789ABCD"};
  CAPTURE(test_str);
  REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) == test_str);
}

TEST_CASE("Binary Conversion: Strings: Four Words: Overflow") {
  // cppcheck-suppress unreadVariable
  const std::string test_str{"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
  // cppcheck-suppress unreadVariable
  const std::string result_str{"0123456789ABCDEF"};
  CAPTURE(test_str);
  REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) == result_str);
  REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) ==
          test_str.substr(0, 16));
}

TEST_CASE("Binary Conversion: Strings: Four Words: Multiple Spaces") {
  // cppcheck-suppress unreadVariable
  const std::string test_str{"Hi         Bye"};
  CAPTURE(test_str);
  REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) == test_str);
}

TEST_CASE("Binary Conversion: Strings: Four Words: Leading/Trailing Spaces") {
  const std::string test_str{"  Time  to be "};
  const std::string result_str{"Time  to be"};
  CAPTURE(test_str);
  REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) == result_str);
}
// NOLINTEND(readability-magic-numbers)
}  // namespace sacfmt
