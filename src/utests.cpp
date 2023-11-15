/* Standard Library */
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <random>
/* Catch2 */
// testing macros
#include <catch2/catch_test_macros.hpp>
/* Xoshiro Random Number generator */
#include <XoshiroCpp.hpp>
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
      oss << std::setprecision(std::numeric_limits<double>::max_digits10)
        << neg_epsilon;
      std::string s_neg_epsilon{oss.str()};
      CAPTURE(s_neg_epsilon);
      REQUIRE(binary_to_double(double_to_binary(neg_epsilon)) == neg_epsilon);
    }
    SECTION("Positive") {
      REQUIRE(binary_to_double(double_to_binary(1.0)) == 1.0);
      REQUIRE(binary_to_double(double_to_binary(0.1)) == 0.1);
      REQUIRE(binary_to_double(double_to_binary(10.0)) == 10.0);
      double highest{std::numeric_limits<double>::max()};
      REQUIRE(binary_to_double(double_to_binary(highest)) == highest);
      double epsilon{std::numeric_limits<double>::epsilon()};
      std::ostringstream oss{};
      oss << std::setprecision(std::numeric_limits<double>::max_digits10)
        << epsilon;
      std::string s_epsilon{oss.str()};
      CAPTURE(s_epsilon);
      REQUIRE(binary_to_double(double_to_binary(epsilon)) == epsilon);
    }
  }
  SECTION("Strings") {
    SECTION("Regular - Two Words") {
      SECTION("Perfect") {
        const std::string test_str{"01234567"};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
      }
      SECTION("Empty") {
        const std::string test_str{""};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
      }
      SECTION("One Word") {
        const std::string test_str{"0123"};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
      }
      SECTION("Half Word") {
        const std::string test_str{"012"};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
      }
      SECTION("Overflow") {
        const std::string test_str{"0123456789ABCDEFG"};
        const std::string result_str{"01234567"};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == result_str);
        REQUIRE(binary_to_string(string_to_binary(test_str))
                == test_str.substr(0, 8));
      }
    }
    SECTION("Long - Four Words") {
      SECTION("Perfect") {
        const std::string test_str{"0123456789ABCDEF"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str))
                == test_str);
      }
      SECTION("Empty") {
        const std::string test_str{""};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str))
                == test_str);
      }
      SECTION("One Word") {
        const std::string test_str{"0123"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str))
                == test_str);
      }
      SECTION("Half Word") {
        const std::string test_str{"01"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str))
                == test_str);
      }
      SECTION("Three Words") {
        const std::string test_str{"0123456789AB"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str))
                == test_str);
      }
      SECTION("Three and a Half Words") {
        const std::string test_str{"0123456789ABCD"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str))
                == test_str);
      }
      SECTION("Overflow") {
        const std::string test_str{"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
        const std::string result_str{"0123456789ABCDEF"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str))
                == result_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str))
                == test_str.substr(0, 16));
      }
    }
  }
}

//==============================================================================
// Self-seed the random number generator
XoshiroCpp::Xoshiro256Plus init() {
  // Random device for seeding
  std::random_device rd{};
  // Two random runtime constants
  const uint64_t t1{static_cast<uint64_t>(
    std::chrono::steady_clock::now().time_since_epoch().count())};
  const uint64_t t2{static_cast<uint64_t>(
    std::chrono::steady_clock::now().time_since_epoch().count())};
  // Seed the initial state
  const XoshiroCpp::Xoshiro256Plus::state_type initial_state = {
    XoshiroCpp::SplitMix64{t1}(),
    XoshiroCpp::SplitMix64{rd()}(),
    XoshiroCpp::SplitMix64{t2}(),
    XoshiroCpp::SplitMix64{rd()}()
  };
  return XoshiroCpp::Xoshiro256Plus{initial_state};
}

// Build the PRNG
inline XoshiroCpp::Xoshiro256Plus xos{init()};

//Give a random double within the inclusive bounds [min, max]
inline double get(double min, double max) {
  std::uniform_real_distribution<> die{min, max};
  return die(xos);
}

void random_vector(std::vector<double>& data, const double minimum = -1.0,
                   const double maximum = 1.0) {
  std::uniform_real_distribution<> die{minimum, maximum};
  for (std::size_t i{0}; i < data.size(); ++i) { data[i] = die(xos); }
}
//==============================================================================

TEST_CASE("Emptry SacStream") {
  SacStream test_sac{};
  SECTION("Unset Headers") {
    SECTION("Floats") {
      REQUIRE(test_sac.f_delta == unset_float);
      REQUIRE(test_sac.depmin == unset_float);
      REQUIRE(test_sac.depmax == unset_float);
      REQUIRE(test_sac.odelta == unset_float);
      REQUIRE(test_sac.f_b == unset_float);
      REQUIRE(test_sac.f_e == unset_float);
      REQUIRE(test_sac.f_o == unset_float);
      REQUIRE(test_sac.f_a == unset_float);
      REQUIRE(test_sac.f_t0 == unset_float);
      REQUIRE(test_sac.f_t1 == unset_float);
      REQUIRE(test_sac.f_t2 == unset_float);
      REQUIRE(test_sac.f_t3 == unset_float);
      REQUIRE(test_sac.f_t4 == unset_float);
      REQUIRE(test_sac.f_t5 == unset_float);
      REQUIRE(test_sac.f_t6 == unset_float);
      REQUIRE(test_sac.f_t7 == unset_float);
      REQUIRE(test_sac.f_t8 == unset_float);
      REQUIRE(test_sac.f_t9 == unset_float);
      REQUIRE(test_sac.f_f == unset_float);
      REQUIRE(test_sac.resp0 == unset_float);
      REQUIRE(test_sac.resp1 == unset_float);
    }
  }
}
