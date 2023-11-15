/* Standard Library */
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
/* Catch2 */
// testing macros
#include <catch2/catch_test_macros.hpp>
// floating point equality within tolerance
#include <catch2/matchers/catch_matchers_floating_point.hpp>
// benchmarking
//#include <catch2/benchmark/catch_benchmark.hpp>
/* My stuff */
#include "sac_stream.hpp"

using namespace SAC;

TEST_CASE("Basic constants") {
  SECTION("Constants (constexpr)") {
    REQUIRE(word_length == 4);
    REQUIRE(bits_per_byte == 8);
    REQUIRE(binary_word_size == 32);
    REQUIRE(data_word == 158);
    REQUIRE(unset_int == -12345);
    REQUIRE(unset_float == -12345.0f);
    REQUIRE(unset_double == -12345.0);
    REQUIRE(unset_bool == 0);
    REQUIRE(unset_word == "-12345");
  }
  SECTION("Derived constants") {
    REQUIRE(word_position(data_word) == 632);
  }
}

TEST_CASE("Binary Conversion") {
  SECTION("Booleans") {
    REQUIRE(unset_bool == false);
    REQUIRE(!unset_bool == true);
    REQUIRE(binary_to_bool(bool_to_binary(unset_bool)) == false);
    REQUIRE(binary_to_bool(bool_to_binary(!unset_bool)) == true);
  }
  SECTION("Integers") {
    SECTION("Zero") {
      REQUIRE(binary_to_int(int_to_binary(0)) == 0);
      REQUIRE(binary_to_int(int_to_binary(-0)) == 0);
    }
    SECTION("Negative") {
      REQUIRE(binary_to_int(int_to_binary(-1)) == -1);
      REQUIRE(binary_to_int(int_to_binary(-10)) == -10);
      REQUIRE(binary_to_int(int_to_binary(INT_MIN)) == INT_MIN);
    }
    SECTION("Positive") {
      REQUIRE(binary_to_int(int_to_binary(1)) == 1);
      REQUIRE(binary_to_int(int_to_binary(10)) == 10);
      REQUIRE(binary_to_int(int_to_binary(INT_MAX)) == INT_MAX);
    }
  }
  SECTION("Floats") {
    SECTION("Zero") {
      REQUIRE(binary_to_float(float_to_binary(0.0f)) == 0.0f);
      REQUIRE(binary_to_float(float_to_binary(-0.0f)) == 0.0f);
    }
    SECTION("Negative") {
      REQUIRE(binary_to_float(float_to_binary(-1.0f)) == -1.0f);
      REQUIRE(binary_to_float(float_to_binary(-0.1f)) == -0.1f);
      REQUIRE(binary_to_float(float_to_binary(-10.0f)) == -10.0f);
      float lowest{std::numeric_limits<float>::lowest()};
      REQUIRE(binary_to_float(float_to_binary(lowest)) == lowest);
      float neg_epsilon{-std::numeric_limits<float>::epsilon()};
      std::ostringstream oss{};
      oss << std::setprecision(std::numeric_limits<float>::max_digits10)
        << neg_epsilon;
      std::string s_neg_epsilon{oss.str()};
      CAPTURE(s_neg_epsilon);
      REQUIRE(binary_to_float(float_to_binary(neg_epsilon)) == neg_epsilon);
    }
    SECTION("Positive") {
      REQUIRE(binary_to_float(float_to_binary(1.0f)) == 1.0f);
      REQUIRE(binary_to_float(float_to_binary(0.1f)) == 0.1f);
      REQUIRE(binary_to_float(float_to_binary(10.0f)) == 10.0f);
      float highest{std::numeric_limits<float>::max()};
      REQUIRE(binary_to_float(float_to_binary(highest)) == highest);
      float epsilon{std::numeric_limits<float>::epsilon()};
      std::ostringstream oss{};
      oss << std::setprecision(std::numeric_limits<float>::max_digits10)
        << epsilon;
      std::string s_epsilon{oss.str()};
      CAPTURE(s_epsilon);
      REQUIRE(binary_to_float(float_to_binary(epsilon)) == epsilon);
    }
  }
  SECTION("Doubles") {
    SECTION("Zero") {
      REQUIRE(binary_to_double(double_to_binary(0.0)) == 0.0);
      REQUIRE(binary_to_double(double_to_binary(-0.0)) == 0.0);
    }
    SECTION("Negative") {
      REQUIRE(binary_to_double(double_to_binary(-1.0)) == -1.0);
      REQUIRE(binary_to_double(double_to_binary(-0.1)) == -0.1);
      REQUIRE(binary_to_double(double_to_binary(-10.0)) == -10.0f);
      double lowest{std::numeric_limits<double>::lowest()};
      REQUIRE(binary_to_double(double_to_binary(lowest)) == lowest);
      double neg_epsilon{-std::numeric_limits<double>::epsilon()};
      std::ostringstream oss{};

    }
    SECTION("Positive") {

    }
  }
  SECTION("Strings") {

  }
}
