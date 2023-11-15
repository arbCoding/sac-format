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
#include "sac_format.hpp"

using namespace sacfmt;
namespace fs = std::filesystem;

//==============================================================================
// Basic I/O tests (file writing/reading goes with Trace)
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
      //cppcheck-suppress unreadVariable
      float lowest{std::numeric_limits<float>::lowest()};
      REQUIRE(binary_to_float(float_to_binary(lowest)) == lowest);
      float neg_epsilon{-std::numeric_limits<float>::epsilon()};
      std::ostringstream oss{};
      oss << std::setprecision(std::numeric_limits<float>::max_digits10)
        << neg_epsilon;
      //cppcheck-suppress unreadVariable
      std::string s_neg_epsilon{oss.str()};
      CAPTURE(s_neg_epsilon);
      REQUIRE(binary_to_float(float_to_binary(neg_epsilon)) == neg_epsilon);
    }
    SECTION("Positive") {
      REQUIRE(binary_to_float(float_to_binary(1.0f)) == 1.0f);
      REQUIRE(binary_to_float(float_to_binary(0.1f)) == 0.1f);
      REQUIRE(binary_to_float(float_to_binary(10.0f)) == 10.0f);
      //cppcheck-suppress unreadVariable
      float highest{std::numeric_limits<float>::max()};
      REQUIRE(binary_to_float(float_to_binary(highest)) == highest);
      float epsilon{std::numeric_limits<float>::epsilon()};
      std::ostringstream oss{};
      oss << std::setprecision(std::numeric_limits<float>::max_digits10)
        << epsilon;
      //cppcheck-suppress unreadVariable
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
      //cppcheck-suppress unreadVariable
      double lowest{std::numeric_limits<double>::lowest()};
      REQUIRE(binary_to_double(double_to_binary(lowest)) == lowest);
      double neg_epsilon{-std::numeric_limits<double>::epsilon()};
      std::ostringstream oss{};
      oss << std::setprecision(std::numeric_limits<double>::max_digits10)
        << neg_epsilon;
      //cppcheck-suppress unreadVariable
      std::string s_neg_epsilon{oss.str()};
      CAPTURE(s_neg_epsilon);
      REQUIRE(binary_to_double(double_to_binary(neg_epsilon)) == neg_epsilon);
    }
    SECTION("Positive") {
      REQUIRE(binary_to_double(double_to_binary(1.0)) == 1.0);
      REQUIRE(binary_to_double(double_to_binary(0.1)) == 0.1);
      REQUIRE(binary_to_double(double_to_binary(10.0)) == 10.0);
      //cppcheck-suppress unreadVariable
      double highest{std::numeric_limits<double>::max()};
      REQUIRE(binary_to_double(double_to_binary(highest)) == highest);
      double epsilon{std::numeric_limits<double>::epsilon()};
      std::ostringstream oss{};
      oss << std::setprecision(std::numeric_limits<double>::max_digits10)
        << epsilon;
      //cppcheck-suppress unreadVariable
      std::string s_epsilon{oss.str()};
      CAPTURE(s_epsilon);
      REQUIRE(binary_to_double(double_to_binary(epsilon)) == epsilon);
    }
  }
  SECTION("Strings") {
    SECTION("Regular - Two Words") {
      SECTION("Perfect") {
        //cppcheck-suppress unreadVariable
        const std::string test_str{"01234567"};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
      }
      SECTION("Empty") {
        //cppcheck-suppress unreadVariable
        const std::string test_str{""};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
      }
      SECTION("One Word") {
        //cppcheck-suppress unreadVariable
        const std::string test_str{"0123"};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
      }
      SECTION("Half Word") {
        //cppcheck-suppress unreadVariable
        const std::string test_str{"012"};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
      }
      SECTION("Overflow") {
        //cppcheck-suppress unreadVariable
        const std::string test_str{"0123456789ABCDEFG"};
        //cppcheck-suppress unreadVariable
        const std::string result_str{"01234567"};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == result_str);
        REQUIRE(binary_to_string(string_to_binary(test_str))
                == test_str.substr(0, 8));
      }
    }
    SECTION("Long - Four Words") {
      SECTION("Perfect") {
        //cppcheck-suppress unreadVariable
        const std::string test_str{"0123456789ABCDEF"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str))
                == test_str);
      }
      SECTION("Empty") {
        //cppcheck-suppress unreadVariable
        const std::string test_str{""};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str))
                == test_str);
      }
      SECTION("One Word") {
        //cppcheck-suppress unreadVariable
        const std::string test_str{"0123"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str))
                == test_str);
      }
      SECTION("Half Word") {
        //cppcheck-suppress unreadVariable
        const std::string test_str{"01"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str))
                == test_str);
      }
      SECTION("Three Words") {
        //cppcheck-suppress unreadVariable
        const std::string test_str{"0123456789AB"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str))
                == test_str);
      }
      SECTION("Three and a Half Words") {
        //cppcheck-suppress unreadVariable
        const std::string test_str{"0123456789ABCD"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str))
                == test_str);
      }
      SECTION("Overflow") {
        //cppcheck-suppress unreadVariable
        const std::string test_str{"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
        //cppcheck-suppress unreadVariable
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
// Debugging print
void print_control_characters(const std::string& str)
{
    for (char c : str)
    {
        if (std::iscntrl(static_cast<unsigned char>(c))) { std::cout << "\\x" << std::hex << static_cast<int>(c); }
        else { std::cout << c; }
    }
    std::cout << '\n';
}
//==============================================================================
// Trace tests
TEST_CASE("Empty Trace Default Creation") {
  Trace test_sac{};
  SECTION("Default Headers/Footers") {
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
      REQUIRE(test_sac.resp2 == unset_float);
      REQUIRE(test_sac.resp3 == unset_float);
      REQUIRE(test_sac.resp4 == unset_float);
      REQUIRE(test_sac.resp5 == unset_float);
      REQUIRE(test_sac.resp6 == unset_float);
      REQUIRE(test_sac.resp7 == unset_float);
      REQUIRE(test_sac.resp8 == unset_float);
      REQUIRE(test_sac.resp9 == unset_float);
      REQUIRE(test_sac.f_stla == unset_float);
      REQUIRE(test_sac.f_stlo == unset_float);
      REQUIRE(test_sac.stel == unset_float);
      REQUIRE(test_sac.stdp == unset_float);
      REQUIRE(test_sac.f_evla == unset_float);
      REQUIRE(test_sac.f_evlo == unset_float);
      REQUIRE(test_sac.evel == unset_float);
      REQUIRE(test_sac.evdp == unset_float);
      REQUIRE(test_sac.mag == unset_float);
      REQUIRE(test_sac.user0 == unset_float);
      REQUIRE(test_sac.user1 == unset_float);
      REQUIRE(test_sac.user2 == unset_float);
      REQUIRE(test_sac.user3 == unset_float);
      REQUIRE(test_sac.user4 == unset_float);
      REQUIRE(test_sac.user5 == unset_float);
      REQUIRE(test_sac.user6 == unset_float);
      REQUIRE(test_sac.user7 == unset_float);
      REQUIRE(test_sac.user8 == unset_float);
      REQUIRE(test_sac.user9 == unset_float);
      REQUIRE(test_sac.dist == unset_float);
      REQUIRE(test_sac.az == unset_float);
      REQUIRE(test_sac.baz == unset_float);
      REQUIRE(test_sac.gcarc == unset_float);
      REQUIRE(test_sac.f_sb == unset_float);
      REQUIRE(test_sac.f_sdelta == unset_float);
      REQUIRE(test_sac.depmen == unset_float);
      REQUIRE(test_sac.cmpaz == unset_float);
      REQUIRE(test_sac.cmpinc == unset_float);
      REQUIRE(test_sac.xminimum == unset_float);
      REQUIRE(test_sac.xmaximum == unset_float);
      REQUIRE(test_sac.yminimum == unset_float);
      REQUIRE(test_sac.ymaximum == unset_float);
    }
    SECTION("Ints") {
      REQUIRE(test_sac.nzyear == unset_int);
      REQUIRE(test_sac.nzjday == unset_int);
      REQUIRE(test_sac.nzhour == unset_int);
      REQUIRE(test_sac.nzmin == unset_int);
      REQUIRE(test_sac.nzsec == unset_int);
      REQUIRE(test_sac.nzmsec == unset_int);
      REQUIRE(test_sac.nvhdr == 7);
      REQUIRE(test_sac.norid == unset_int);
      REQUIRE(test_sac.nevid == unset_int);
      REQUIRE(test_sac.npts == unset_int);
      REQUIRE(test_sac.nsnpts == unset_int);
      REQUIRE(test_sac.nwfid == unset_int);
      REQUIRE(test_sac.nxsize == unset_int);
      REQUIRE(test_sac.nysize == unset_int);
      REQUIRE(test_sac.iftype == unset_int);
      REQUIRE(test_sac.idep == unset_int);
      REQUIRE(test_sac.iztype == unset_int);
      REQUIRE(test_sac.iinst == unset_int);
      REQUIRE(test_sac.istreg == unset_int);
      REQUIRE(test_sac.ievreg == unset_int);
      REQUIRE(test_sac.ievtyp == unset_int);
      REQUIRE(test_sac.iqual == unset_int);
      REQUIRE(test_sac.isynth == unset_int);
      REQUIRE(test_sac.imagtyp == unset_int);
      REQUIRE(test_sac.imagsrc == unset_int);
      REQUIRE(test_sac.ibody == unset_int);
    }
    SECTION("Bools") {
      REQUIRE(test_sac.leven == unset_bool);
      REQUIRE(test_sac.lpspol == unset_bool);
      REQUIRE(test_sac.lovrok == unset_bool);
      REQUIRE(test_sac.lcalda == unset_bool);
    }
    SECTION("Strings") {
      SECTION("Standard") {
        REQUIRE(test_sac.kstnm == unset_word);
        REQUIRE(test_sac.khole == unset_word);
        REQUIRE(test_sac.ko == unset_word);
        REQUIRE(test_sac.ka == unset_word);
        REQUIRE(test_sac.kt0 == unset_word);
        REQUIRE(test_sac.kt1 == unset_word);
        REQUIRE(test_sac.kt2 == unset_word);
        REQUIRE(test_sac.kt3 == unset_word);
        REQUIRE(test_sac.kt4 == unset_word);
        REQUIRE(test_sac.kt5 == unset_word);
        REQUIRE(test_sac.kt6 == unset_word);
        REQUIRE(test_sac.kt7 == unset_word);
        REQUIRE(test_sac.kt8 == unset_word);
        REQUIRE(test_sac.kt9 == unset_word);
        REQUIRE(test_sac.kf == unset_word);
        REQUIRE(test_sac.kuser0 == unset_word);
        REQUIRE(test_sac.kuser1 == unset_word);
        REQUIRE(test_sac.kuser2 == unset_word);
        REQUIRE(test_sac.kcmpnm == unset_word);
        REQUIRE(test_sac.knetwk == unset_word);
        REQUIRE(test_sac.kdatrd == unset_word);
        REQUIRE(test_sac.kinst == unset_word);
      }
      SECTION("Double") {
        REQUIRE(test_sac.kevnm == unset_word);
      }
    }
    SECTION("Data Vectors") {
      REQUIRE(test_sac.data1.empty());
      REQUIRE(test_sac.data2.empty());
    }
    SECTION("Doubles") {
      REQUIRE(test_sac.delta == unset_double);
      REQUIRE(test_sac.b == unset_double);
      REQUIRE(test_sac.e == unset_double);
      REQUIRE(test_sac.o == unset_double);
      REQUIRE(test_sac.a == unset_double);
      REQUIRE(test_sac.t0 == unset_double);
      REQUIRE(test_sac.t1 == unset_double);
      REQUIRE(test_sac.t2 == unset_double);
      REQUIRE(test_sac.t3 == unset_double);
      REQUIRE(test_sac.t4 == unset_double);
      REQUIRE(test_sac.t5 == unset_double);
      REQUIRE(test_sac.t6 == unset_double);
      REQUIRE(test_sac.t7 == unset_double);
      REQUIRE(test_sac.t8 == unset_double);
      REQUIRE(test_sac.t9 == unset_double);
      REQUIRE(test_sac.f == unset_double);
      REQUIRE(test_sac.evlo == unset_double);
      REQUIRE(test_sac.evla == unset_double);
      REQUIRE(test_sac.stlo == unset_double);
      REQUIRE(test_sac.stla == unset_double);
      REQUIRE(test_sac.sb == unset_double);
      REQUIRE(test_sac.sdelta == unset_double);
    }
  }
}

Trace gen_fake_sac() {
  Trace sac{};
  sac.f_delta = 0.025f;
  sac.delta = 0.025;
  sac.depmin = -1.11f;
  sac.depmax = 1.23f;
  sac.odelta = 0.026f;
  sac.f_b = 0.1f;
  sac.b = 0.1;
  sac.f_e = 0.0f;
  sac.e = 0.0;
  sac.f_a = 5.78f;
  sac.a = 5.78;
  sac.f_t0 = 1.234f;
  sac.t0 = 1.234;
  sac.f_t1 = -1.234f;
  sac.t1 = -1.234;
  sac.f_t2 = 2.345f;
  sac.t2 = 2.345;
  sac.f_t3 = -2.345f;
  sac.t3 = -2.345;
  sac.f_t4 = 3.456f;
  sac.t4 = 3.456;
  sac.f_t5 = -3.456f;
  sac.t5 = -3.456;
  sac.f_t6 = 4.567f;
  sac.t6 = 4.567;
  sac.f_t7 = -4.567f;
  sac.t7 = -4.567;
  sac.f_t8 = 5.678f;
  sac.t8 = 5.678;
  sac.f_t9 = -5.678f;
  sac.t9 = -5.678;
  sac.f_f = 12345.0f;
  sac.f = 12345.0;
  sac.resp0 = 0.1f;
  sac.resp1 = -0.1f;
  sac.resp2 = 1.0f;
  sac.resp3 = -1.0f;
  sac.resp4 = 10.0f;
  sac.resp5 = -10.0f;
  sac.resp6 = 100.0f;
  sac.resp7 = -100.0f;
  sac.resp8 = 1000.0f;
  sac.resp9 = -1000.0f;
  sac.f_stla = 32.21f;
  sac.stla = 32.21;
  sac.f_stlo = -121.38f;
  sac.stlo = 121.38;
  sac.stel = 100.0f;
  sac.stdp = 50.0f;
  sac.f_evla = -81.35f;
  sac.evla = -81.35;
  sac.f_evlo = 85.37f;
  sac.evlo = 85.37;
  sac.evel = 0.5f;
  sac.evdp = 30.25f;
  sac.mag = 1.83f;
  sac.user0 = 9.0f;
  sac.user1 = -9.0f;
  sac.user2 = 8.0f;
  sac.user3 = -8.0f;
  sac.user4 = 7.0f;
  sac.user5 = -7.0f;
  sac.user6 = 6.0f;
  sac.user7 = -6.0f;
  sac.user8 = 5.0f;
  sac.user9 = -5.0f;
  sac.dist = 1300.0f;
  sac.az = 35.0f;
  sac.baz = -275.0f;
  sac.gcarc = 13.135f;
  sac.f_sb = -35.0f;
  sac.sb = -35.0;
  sac.f_sdelta = 0.125;
  sac.sdelta = 0.125;
  sac.depmen = 0.1f;
  sac.cmpaz = 97.3f;
  sac.cmpinc = 273.0f;
  sac.xminimum = -5.0f;
  sac.xmaximum = 5.0f;
  sac.yminimum = -10.0f;
  sac.ymaximum = 10.0f;
  sac.nzyear = 2023;
  sac.nzjday = 123;
  sac.nzhour = 13;
  sac.nzmin = 57;
  sac.nzsec = 34;
  sac.nzmsec = 0;
  sac.nvhdr = 7;
  sac.norid = 1;
  sac.nevid = 2;
  sac.npts = 100000;
  sac.nsnpts = 50000;
  sac.nwfid = 3;
  sac.nxsize = 35;
  sac.nysize = 15;
  sac.iftype = 1;
  sac.idep = 50;
  sac.iztype = 12;
  sac.iinst = 83;
  sac.istreg = 82;
  sac.ievreg = 81;
  sac.ievtyp = 73;
  sac.iqual = 45;
  sac.isynth = 1;
  sac.imagtyp = 57;
  sac.imagsrc = 70;
  sac.ibody = 99;
  sac.leven = true;
  sac.lpspol = true;
  sac.lovrok = true;
  sac.lcalda = true;
  sac.kstnm = "Test1";
  sac.kevnm = "Fake Quake";
  sac.khole = "AB";
  sac.ko = "Hi";
  sac.ka = "Yup";
  sac.kt0 = "Zero 0";
  sac.kt1 = "One 1";
  sac.kt2 = "Two 2";
  sac.kt3 = "Three 3";
  sac.kt4 = "Four 4";
  sac.kt5 = "Five 5";
  sac.kt6 = "Six 6";
  sac.kt7 = "Seven 7";
  sac.kt8 = "Eight 8";
  sac.kt9 = "Nine 9";
  sac.kf = "Fini";
  sac.kuser0 = "User0 Ze";
  sac.kuser1 = "User1 On";
  sac.kuser2 = "User2 Tw";
  sac.kcmpnm = "HHZ";
  sac.knetwk = "ZZ";
  sac.kdatrd = "None";
  sac.kinst = "Not Real";
  // Resize data vectors to new size, zero-filled by default
  if (sac.npts > 0) {
    sac.data1.resize(sac.npts, 0.0);
    if (sac.leven == false || sac.iftype > 1) {
      sac.data2.resize(sac.npts, 0.0);
    }
  }
  return sac;
}

TEST_CASE("Trace Save/Load") {
  Trace test_sac = gen_fake_sac();
  fs::path tmp_dir{fs::temp_directory_path()};
  fs::path tmp_file{tmp_dir / "test.SAC"};
  SECTION("I/O Zeros") {
    test_sac.write(tmp_file);
    Trace in_sac = Trace(tmp_file.string());
    REQUIRE(test_sac == in_sac);
    fs::remove(tmp_file);
  }
  SECTION("I/O Random") {
    random_vector(test_sac.data1);
    if (!test_sac.data2.empty()) { random_vector(test_sac.data2); }
    test_sac.write(tmp_file);
    Trace in_sac = Trace(tmp_file.string());
    REQUIRE(test_sac == in_sac);
    fs::remove(tmp_file);
  }
}
