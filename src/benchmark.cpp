#include <bitset>
#include <iomanip>
#include <limits>
#include <sstream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include "sac_format.hpp"

using namespace sacfmt;

TEST_CASE("Binary Conversion") {
  SECTION("Booleans") {
    BENCHMARK("Bool->Binary False") {
      return bool_to_binary(unset_bool);
    };
    BENCHMARK("Bool->Binary->Bool False") {
      return binary_to_bool(bool_to_binary(unset_bool));
    };
  }
}
