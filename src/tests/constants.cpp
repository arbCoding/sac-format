// Copyright 2023-2024 Alexander R. Blanchette

// sac-format
#include "sac-format/sac_format.hpp"
// Standard Library
//   https://en.cppreference.com/w/cpp/standard_library
// Catch2 https://github.com/catchorg/Catch2/tree/v3.4.0
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_MAIN
// testing macros
// TEST_CASE, SECTION, REQUIRE
#include <catch2/catch_test_macros.hpp>

namespace sacfmt {
// NOLINTBEGIN(readability-magic-numbers)
TEST_CASE("Fundamental Constants") {
  REQUIRE(word_length == 4);
  REQUIRE(bits_per_byte == 8);
  REQUIRE(binary_word_size == 32);
  REQUIRE(data_word == 158);
  REQUIRE(unset_int == -12345);
  REQUIRE(unset_float == -12345.0F);
  REQUIRE(unset_double == -12345.0);
  REQUIRE(unset_bool == false);
  REQUIRE(unset_word == "-12345");
}

TEST_CASE("Data Word calcualtion") { REQUIRE(word_position(data_word) == 632); }
// NOLINTEND(readability-magic-numbers)
}  // namespace sacfmt
