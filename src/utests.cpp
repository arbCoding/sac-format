// Copyright 2023 Alexander R. Blanchette

/* Standard Library */
// std::filesystem::path, std::filesystem::temp_directory_path()
#include <filesystem>
/* Catch2 */
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_MAIN
// testing macros
// TEST_CASE, SECTION, REQUIRE, CAPTURE, REQUIRE_THROWS
#include <catch2/catch_test_macros.hpp>
// from catch_matchers.hpp (any matcher includes it)
// REQUIRE_THAT
// Catch::Matchers::WithinAbs
#include <catch2/matchers/catch_matchers_floating_point.hpp>
/* My stuff */
#include <sac_format.hpp>
#include <util.hpp>

using namespace sacfmt;
namespace fs = std::filesystem;
using Catch::Matchers::WithinAbs;

//==============================================================================
// Basic I/O tests (file writing/reading goes with Trace)
TEST_CASE("Basic constants") {
  SECTION("Fundamental Constants") {
    REQUIRE(word_length == 4);
    REQUIRE(bits_per_byte == 8);
    REQUIRE(binary_word_size == 32);
    REQUIRE(data_word == 158);
    REQUIRE(unset_int == -12345);
    REQUIRE(unset_float == -12345.0f);
    REQUIRE(unset_double == -12345.0);
    REQUIRE(unset_bool == false);
    REQUIRE(unset_word == "-12345");
  }
  SECTION("Derived constants") { REQUIRE(word_position(data_word) == 632); }
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
      REQUIRE(binary_to_float(float_to_binary(lowest_f)) == lowest_f);
      CAPTURE(s_neg_epsilon_f);
      REQUIRE(binary_to_float(float_to_binary(neg_epsilon_f)) == neg_epsilon_f);
    }
    SECTION("Positive") {
      REQUIRE(binary_to_float(float_to_binary(1.0f)) == 1.0f);
      REQUIRE(binary_to_float(float_to_binary(0.1f)) == 0.1f);
      REQUIRE(binary_to_float(float_to_binary(10.0f)) == 10.0f);
      REQUIRE(binary_to_float(float_to_binary(highest_f)) == highest_f);
      CAPTURE(s_epsilon_f);
      REQUIRE(binary_to_float(float_to_binary(epsilon_f)) == epsilon_f);
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
      REQUIRE(binary_to_double(double_to_binary(lowest)) == lowest);
      CAPTURE(s_neg_epsilon);
      REQUIRE(binary_to_double(double_to_binary(neg_epsilon)) == neg_epsilon);
    }
    SECTION("Positive") {
      REQUIRE(binary_to_double(double_to_binary(1.0)) == 1.0);
      REQUIRE(binary_to_double(double_to_binary(0.1)) == 0.1);
      REQUIRE(binary_to_double(double_to_binary(10.0)) == 10.0);
      REQUIRE(binary_to_double(double_to_binary(highest)) == highest);
      CAPTURE(s_epsilon);
      REQUIRE(binary_to_double(double_to_binary(epsilon)) == epsilon);
    }
  }
  SECTION("Strings") {
    SECTION("Regular - Two Words") {
      SECTION("Perfect") {
        // cppcheck-suppress unreadVariable
        const std::string test_str{"01234567"};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
      }
      SECTION("Empty") {
        // cppcheck-suppress unreadVariable
        const std::string test_str{""};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
      }
      SECTION("One Word") {
        // cppcheck-suppress unreadVariable
        const std::string test_str{"0123"};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
      }
      SECTION("Half Word") {
        // cppcheck-suppress unreadVariable
        const std::string test_str{"012"};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
      }
      SECTION("Overflow") {
        // cppcheck-suppress unreadVariable
        const std::string test_str{"0123456789ABCDEFG"};
        // cppcheck-suppress unreadVariable
        const std::string result_str{"01234567"};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == result_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) ==
                test_str.substr(0, 8));
      }
      SECTION("Multiple Spaces") {
        // cppcheck-suppress unreadVariable
        const std::string test_str{"Hi  Bye"};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == test_str);
      }
      SECTION("Leading/Trailing Spaces") {
        // cppcheck-suppress unreadVariable
        const std::string test_str{"  Time  "};
        // cppcheck-suppress unreadVariable
        const std::string result_str{"Time"};
        CAPTURE(test_str);
        REQUIRE(binary_to_string(string_to_binary(test_str)) == result_str);
      }
    }
    SECTION("Long - Four Words") {
      SECTION("Perfect") {
        // cppcheck-suppress unreadVariable
        const std::string test_str{"0123456789ABCDEF"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) ==
                test_str);
      }
      SECTION("Empty") {
        // cppcheck-suppress unreadVariable
        const std::string test_str{""};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) ==
                test_str);
      }
      SECTION("One Word") {
        // cppcheck-suppress unreadVariable
        const std::string test_str{"0123"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) ==
                test_str);
      }
      SECTION("Half Word") {
        // cppcheck-suppress unreadVariable
        const std::string test_str{"01"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) ==
                test_str);
      }
      SECTION("Three Words") {
        // cppcheck-suppress unreadVariable
        const std::string test_str{"0123456789AB"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) ==
                test_str);
      }
      SECTION("Three and a Half Words") {
        // cppcheck-suppress unreadVariable
        const std::string test_str{"0123456789ABCD"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) ==
                test_str);
      }
      SECTION("Overflow") {
        // cppcheck-suppress unreadVariable
        const std::string test_str{"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
        // cppcheck-suppress unreadVariable
        const std::string result_str{"0123456789ABCDEF"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) ==
                result_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) ==
                test_str.substr(0, 16));
      }
      SECTION("Multiple Spaces") {
        // cppcheck-suppress unreadVariable
        const std::string test_str{"Hi         Bye"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) ==
                test_str);
      }
      SECTION("Leading/Trailing Spaces") {
        const std::string test_str{"  Time  to be "};
        const std::string result_str{"Time  to be"};
        CAPTURE(test_str);
        REQUIRE(binary_to_long_string(long_string_to_binary(test_str)) ==
                result_str);
      }
    }
  }
}
//==============================================================================
// Trace tests
//------------------------------------------------------------------------------
TEST_CASE("Trace Default Construction") {
  const Trace trace{};
  // Floats
  SECTION("Floats") {
    REQUIRE(trace.depmin() == unset_float);
    REQUIRE(trace.depmax() == unset_float);
    REQUIRE(trace.odelta() == unset_float);
    REQUIRE(trace.resp0() == unset_float);
    REQUIRE(trace.resp1() == unset_float);
    REQUIRE(trace.resp2() == unset_float);
    REQUIRE(trace.resp3() == unset_float);
    REQUIRE(trace.resp4() == unset_float);
    REQUIRE(trace.resp5() == unset_float);
    REQUIRE(trace.resp6() == unset_float);
    REQUIRE(trace.resp7() == unset_float);
    REQUIRE(trace.resp8() == unset_float);
    REQUIRE(trace.resp9() == unset_float);
    REQUIRE(trace.stel() == unset_float);
    REQUIRE(trace.stdp() == unset_float);
    REQUIRE(trace.evel() == unset_float);
    REQUIRE(trace.evdp() == unset_float);
    REQUIRE(trace.mag() == unset_float);
    REQUIRE(trace.user0() == unset_float);
    REQUIRE(trace.user1() == unset_float);
    REQUIRE(trace.user2() == unset_float);
    REQUIRE(trace.user3() == unset_float);
    REQUIRE(trace.user4() == unset_float);
    REQUIRE(trace.user5() == unset_float);
    REQUIRE(trace.user6() == unset_float);
    REQUIRE(trace.user7() == unset_float);
    REQUIRE(trace.user8() == unset_float);
    REQUIRE(trace.user9() == unset_float);
    REQUIRE(trace.dist() == unset_float);
    REQUIRE(trace.az() == unset_float);
    REQUIRE(trace.baz() == unset_float);
    REQUIRE(trace.gcarc() == unset_float);
    REQUIRE(trace.depmen() == unset_float);
    REQUIRE(trace.cmpaz() == unset_float);
    REQUIRE(trace.cmpinc() == unset_float);
    REQUIRE(trace.xminimum() == unset_float);
    REQUIRE(trace.xmaximum() == unset_float);
    REQUIRE(trace.yminimum() == unset_float);
    REQUIRE(trace.ymaximum() == unset_float);
  }
  SECTION("Doubles") {
    REQUIRE(trace.delta() == unset_double);
    REQUIRE(trace.b() == unset_double);
    REQUIRE(trace.e() == unset_double);
    REQUIRE(trace.o() == unset_double);
    REQUIRE(trace.a() == unset_double);
    REQUIRE(trace.t0() == unset_double);
    REQUIRE(trace.t1() == unset_double);
    REQUIRE(trace.t2() == unset_double);
    REQUIRE(trace.t3() == unset_double);
    REQUIRE(trace.t4() == unset_double);
    REQUIRE(trace.t5() == unset_double);
    REQUIRE(trace.t6() == unset_double);
    REQUIRE(trace.t7() == unset_double);
    REQUIRE(trace.t8() == unset_double);
    REQUIRE(trace.t9() == unset_double);
    REQUIRE(trace.f() == unset_double);
    REQUIRE(trace.stla() == unset_double);
    REQUIRE(trace.stlo() == unset_double);
    REQUIRE(trace.evla() == unset_double);
    REQUIRE(trace.evlo() == unset_double);
    REQUIRE(trace.sb() == unset_double);
    REQUIRE(trace.sdelta() == unset_double);
  }
  SECTION("Ints") {
    REQUIRE(trace.nzyear() == unset_int);
    REQUIRE(trace.nzjday() == unset_int);
    REQUIRE(trace.nzhour() == unset_int);
    REQUIRE(trace.nzmin() == unset_int);
    REQUIRE(trace.nzsec() == unset_int);
    REQUIRE(trace.nzmsec() == unset_int);
    REQUIRE(trace.nvhdr() == unset_int);
    REQUIRE(trace.norid() == unset_int);
    REQUIRE(trace.nevid() == unset_int);
    REQUIRE(trace.npts() == unset_int);
    REQUIRE(trace.nsnpts() == unset_int);
    REQUIRE(trace.nwfid() == unset_int);
    REQUIRE(trace.nxsize() == unset_int);
    REQUIRE(trace.nysize() == unset_int);
    REQUIRE(trace.iftype() == unset_int);
    REQUIRE(trace.idep() == unset_int);
    REQUIRE(trace.iztype() == unset_int);
    REQUIRE(trace.iinst() == unset_int);
    REQUIRE(trace.istreg() == unset_int);
    REQUIRE(trace.ievreg() == unset_int);
    REQUIRE(trace.ievtyp() == unset_int);
    REQUIRE(trace.iqual() == unset_int);
    REQUIRE(trace.isynth() == unset_int);
    REQUIRE(trace.imagtyp() == unset_int);
    REQUIRE(trace.imagsrc() == unset_int);
    REQUIRE(trace.ibody() == unset_int);
  }
  SECTION("Bools") {
    REQUIRE(trace.leven() == unset_bool);
    REQUIRE(trace.lpspol() == unset_bool);
    REQUIRE(trace.lovrok() == unset_bool);
    REQUIRE(trace.lcalda() == unset_bool);
  }
  SECTION("Strings") {
    REQUIRE(trace.kstnm() == unset_word);
    REQUIRE(trace.kevnm() == unset_word);
    REQUIRE(trace.khole() == unset_word);
    REQUIRE(trace.ko() == unset_word);
    REQUIRE(trace.ka() == unset_word);
    REQUIRE(trace.kt0() == unset_word);
    REQUIRE(trace.kt1() == unset_word);
    REQUIRE(trace.kt2() == unset_word);
    REQUIRE(trace.kt3() == unset_word);
    REQUIRE(trace.kt4() == unset_word);
    REQUIRE(trace.kt5() == unset_word);
    REQUIRE(trace.kt6() == unset_word);
    REQUIRE(trace.kt7() == unset_word);
    REQUIRE(trace.kt8() == unset_word);
    REQUIRE(trace.kt9() == unset_word);
    REQUIRE(trace.kf() == unset_word);
    REQUIRE(trace.kuser0() == unset_word);
    REQUIRE(trace.kuser1() == unset_word);
    REQUIRE(trace.kuser2() == unset_word);
    REQUIRE(trace.kcmpnm() == unset_word);
    REQUIRE(trace.knetwk() == unset_word);
    REQUIRE(trace.kdatrd() == unset_word);
    REQUIRE(trace.kinst() == unset_word);
  }
  SECTION("Data") {
    REQUIRE(trace.data1().empty());
    REQUIRE(trace.data2().empty());
  }
}

TEST_CASE("Trace Setters") {
  Trace trace{};
  SECTION("Floats") {
    float value{0.001f};
    trace.depmin(value);
    REQUIRE(trace.depmin() == value);

    value = 0.01f;
    trace.depmax(value);
    REQUIRE(trace.depmax() == value);

    value = 0.1f;
    trace.odelta(value);
    REQUIRE(trace.odelta() == value);

    value = 1.0f;
    trace.resp0(value);
    REQUIRE(trace.resp0() == value);

    value = 10.0f;
    trace.resp1(value);
    REQUIRE(trace.resp1() == value);

    value = 100.0f;
    trace.resp2(value);
    REQUIRE(trace.resp2() == value);

    value = -0.01f;
    trace.resp3(value);
    REQUIRE(trace.resp3() == value);

    value = -0.1f;
    trace.resp4(value);
    REQUIRE(trace.resp4() == value);

    value = -1.0f;
    trace.resp5(value);
    REQUIRE(trace.resp5() == value);

    value = -10.0f;
    trace.resp6(value);
    REQUIRE(trace.resp6() == value);

    value = -100.0f;
    trace.resp7(value);
    REQUIRE(trace.resp7() == value);

    value = 1.2345f;
    trace.resp8(value);
    REQUIRE(trace.resp8() == value);

    value = 2.3456f;
    trace.resp9(value);
    REQUIRE(trace.resp9() == value);

    value = 3.4567f;
    trace.stel(value);
    REQUIRE(trace.stel() == value);

    value = 4.5678f;
    trace.stdp(value);
    REQUIRE(trace.stdp() == value);

    value = 5.6789f;
    trace.evel(value);
    REQUIRE(trace.evel() == value);

    value = -1.2345f;
    trace.evdp(value);
    REQUIRE(trace.evdp() == value);

    value = -2.3456f;
    trace.mag(value);
    REQUIRE(trace.mag() == value);

    value = -3.4567f;
    trace.user0(value);
    REQUIRE(trace.user0() == value);

    value = -4.5678f;
    trace.user1(value);
    REQUIRE(trace.user1() == value);

    value = -5.6789f;
    trace.user2(value);
    REQUIRE(trace.user2() == value);

    value = 9.8765f;
    trace.user3(value);
    REQUIRE(trace.user3() == value);

    value = 8.7654f;
    trace.user4(value);
    REQUIRE(trace.user4() == value);

    value = 7.6543f;
    trace.user5(value);
    REQUIRE(trace.user5() == value);

    value = 6.5432f;
    trace.user6(value);
    REQUIRE(trace.user6() == value);

    value = 5.4321f;
    trace.user7(value);
    REQUIRE(trace.user7() == value);

    value = -9.8765f;
    trace.user8(value);
    REQUIRE(trace.user8() == value);

    value = -8.7654f;
    trace.user9(value);
    REQUIRE(trace.user9() == value);

    value = -7.6543f;
    trace.dist(value);
    REQUIRE(trace.dist() == value);

    value = -6.5432f;
    trace.az(value);
    REQUIRE(trace.az() == value);

    value = -5.4321f;
    trace.baz(value);
    REQUIRE(trace.baz() == value);

    value = 2.9f;
    trace.gcarc(value);
    REQUIRE(trace.gcarc() == value);

    value = 3.8f;
    trace.depmen(value);
    REQUIRE(trace.depmen() == value);

    value = 4.7f;
    trace.cmpaz(value);
    REQUIRE(trace.cmpaz() == value);

    value = 97.3f;
    trace.cmpinc(value);
    REQUIRE(trace.cmpinc() == value);

    value = 5.6f;
    trace.xminimum(value);
    REQUIRE(trace.xminimum() == value);

    value = 6.5f;
    trace.xmaximum(value);
    REQUIRE(trace.xmaximum() == value);

    value = 7.4f;
    trace.yminimum(value);
    REQUIRE(trace.yminimum() == value);

    value = 8.3f;
    trace.ymaximum(value);
    REQUIRE(trace.ymaximum() == value);
  }
  SECTION("Doubles") {
    double value{0.001};
    trace.delta(value);
    REQUIRE(trace.delta() == value);

    value = -0.0001;
    trace.b(value);
    REQUIRE(trace.b() == value);

    value = 0.0001;
    trace.e(value);
    REQUIRE(trace.e() == value);

    value = -0.001;
    trace.o(value);
    REQUIRE(trace.o() == value);

    value = 0.001;
    trace.a(value);
    REQUIRE(trace.a() == value);

    value = -0.1;
    trace.t0(value);
    REQUIRE(trace.t0() == value);

    value = 0.1;
    trace.t1(value);
    REQUIRE(trace.t1() == value);

    value = -1.0;
    trace.t2(value);
    REQUIRE(trace.t2() == value);

    value = 1.0;
    trace.t3(value);
    REQUIRE(trace.t3() == value);

    value = -10.0;
    trace.t4(value);
    REQUIRE(trace.t4() == value);

    value = 10.0;
    trace.t5(value);
    REQUIRE(trace.t5() == value);

    value = -100.0;
    trace.t6(value);
    REQUIRE(trace.t6() == value);

    value = 100.0;
    trace.t7(value);
    REQUIRE(trace.t7() == value);

    value = -1000.0;
    trace.t8(value);
    REQUIRE(trace.t8() == value);

    value = -1000.0;
    trace.t9(value);
    REQUIRE(trace.t9() == value);

    value = -5;
    trace.f(value);
    REQUIRE(trace.f() == value);

    value = 5;
    trace.stla(value);
    REQUIRE(trace.stla() == value);

    value = -50;
    trace.stlo(value);
    REQUIRE(trace.stlo() == value);

    value = 50;
    trace.evla(value);
    REQUIRE(trace.evla() == value);

    value = -500;
    trace.evlo(value);
    REQUIRE(trace.evlo() == value);

    value = 500;
    trace.sb(value);
    REQUIRE(trace.sb() == value);

    value = -5000;
    trace.sdelta(value);
    REQUIRE(trace.sdelta() == value);
  }
  SECTION("Ints") {
    int value{0};
    trace.nzyear(value);
    REQUIRE(trace.nzyear() == value);

    value = 1;
    trace.nzjday(value);
    REQUIRE(trace.nzjday() == value);

    value = 2;
    trace.nzhour(value);
    REQUIRE(trace.nzhour() == value);

    value = 3;
    trace.nzmin(value);
    REQUIRE(trace.nzmin() == value);

    value = 4;
    trace.nzsec(value);
    REQUIRE(trace.nzsec() == value);

    value = 5;
    trace.nzmsec(value);
    REQUIRE(trace.nzmsec() == value);

    value = 6;
    trace.nvhdr(value);
    REQUIRE(trace.nvhdr() == value);

    value = 7;
    trace.norid(value);
    REQUIRE(trace.norid() == value);

    value = 8;
    trace.nevid(value);
    REQUIRE(trace.nevid() == value);

    value = 9;
    trace.npts(value);
    REQUIRE(trace.npts() == value);

    value = 10;
    trace.nsnpts(value);
    REQUIRE(trace.nsnpts() == value);

    value = 11;
    trace.nwfid(value);
    REQUIRE(trace.nwfid() == value);

    value = 12;
    trace.nxsize(value);
    REQUIRE(trace.nxsize() == value);

    value = 13;
    trace.nysize(value);
    REQUIRE(trace.nysize() == value);

    value = 14;
    trace.iftype(value);
    REQUIRE(trace.iftype() == value);

    value = 15;
    trace.idep(value);
    REQUIRE(trace.idep() == value);

    value = 16;
    trace.iztype(value);
    REQUIRE(trace.iztype() == value);

    value = 17;
    trace.iinst(value);
    REQUIRE(trace.iinst() == value);

    value = 18;
    trace.istreg(value);
    REQUIRE(trace.istreg() == value);

    value = 19;
    trace.ievreg(value);
    REQUIRE(trace.ievreg() == value);

    value = 20;
    trace.ievtyp(value);
    REQUIRE(trace.ievtyp() == value);

    value = 21;
    trace.iqual(value);
    REQUIRE(trace.iqual() == value);

    value = 22;
    trace.isynth(value);
    REQUIRE(trace.isynth() == value);

    value = 23;
    trace.imagtyp(value);
    REQUIRE(trace.imagtyp() == value);

    value = 24;
    trace.imagsrc(value);
    REQUIRE(trace.imagsrc() == value);

    value = 25;
    trace.ibody(value);
    REQUIRE(trace.ibody() == value);
  }
  SECTION("Bools") {
    trace.leven(true);
    REQUIRE(trace.leven() == true);

    trace.leven(false);
    REQUIRE(trace.leven() == false);

    trace.lpspol(true);
    REQUIRE(trace.lpspol() == true);

    trace.lpspol(false);
    REQUIRE(trace.lpspol() == false);

    trace.lovrok(true);
    REQUIRE(trace.lovrok() == true);

    trace.lovrok(false);
    REQUIRE(trace.lovrok() == false);

    trace.lcalda(true);
    REQUIRE(trace.lcalda() == true);

    trace.lcalda(false);
    REQUIRE(trace.lcalda() == false);
  }
  SECTION("String") {
    std::string value{"Hello"};
    trace.kstnm(value);
    REQUIRE(trace.kstnm() == value);

    value = "Hi";
    trace.kevnm(value);
    REQUIRE(trace.kevnm() == value);

    value = "Test";
    trace.khole(value);
    REQUIRE(trace.khole() == value);

    value = "Testing";
    trace.ko(value);
    REQUIRE(trace.ko() == value);

    value = "Bye";
    trace.ka(value);
    REQUIRE(trace.ka() == value);

    value = "Goodbye";
    trace.kt0(value);
    REQUIRE(trace.kt0() == value);

    value = "Later";
    trace.kt1(value);
    REQUIRE(trace.kt1() == value);

    value = "Welcome";
    trace.kt2(value);
    REQUIRE(trace.kt2() == value);

    value = "Silly";
    trace.kt3(value);
    REQUIRE(trace.kt3() == value);

    value = "Strange";
    trace.kt4(value);
    REQUIRE(trace.kt4() == value);

    value = "Weird";
    trace.kt5(value);
    REQUIRE(trace.kt5() == value);

    value = "Love";
    trace.kt6(value);
    REQUIRE(trace.kt6() == value);

    value = "Peace";
    trace.kt7(value);
    REQUIRE(trace.kt7() == value);

    value = "Cartoon";
    trace.kt8(value);
    REQUIRE(trace.kt8() == value);

    value = "Movie";
    trace.kt9(value);
    REQUIRE(trace.kt9() == value);

    value = "Book";
    trace.kf(value);
    REQUIRE(trace.kf() == value);

    value = "Paper";
    trace.kuser0(value);
    REQUIRE(trace.kuser0() == value);

    value = "Novel";
    trace.kuser1(value);
    REQUIRE(trace.kuser1() == value);

    value = "Sleep";
    trace.kuser2(value);
    REQUIRE(trace.kuser2() == value);

    value = "Awake";
    trace.kcmpnm(value);
    REQUIRE(trace.kcmpnm() == value);

    value = "Work";
    trace.knetwk(value);
    REQUIRE(trace.knetwk() == value);

    value = "Play";
    trace.kdatrd(value);
    REQUIRE(trace.kdatrd() == value);

    value = "Random";
    trace.kinst(value);
    REQUIRE(trace.kinst() == value);
  }
  SECTION("Data") {
    std::vector<double> value{};
    value.resize(20);
    random_vector(&value);
    trace.data1(value);
    REQUIRE(trace.data1() == value);

    random_vector(&value);
    trace.data2(value);
    REQUIRE(trace.data2() == value);
  }
}

TEST_CASE("Trace Equality") {
  SECTION("Default Construction") {
    const Trace trace1{};
    const Trace trace2{};
    REQUIRE(trace1 == trace2);
  }
  // Confirmed it'll fail if you try to make them equal
  SECTION("Inequality") {
    const Trace trace1{};
    Trace trace2{};
    trace2.kinst("Hello");
    REQUIRE(trace1 != trace2);
  }
}

TEST_CASE("Trace Read/Write") {
  Trace trace = gen_fake_trace();
  fs::path tmp_dir{fs::temp_directory_path()};
  fs::path tmp_file{tmp_dir / "test.SAC"};
  SECTION("Default Construction") {
    trace.write(tmp_file);
    Trace in = Trace(tmp_file);
    fs::remove(tmp_file);
    SECTION("Float") {
      REQUIRE(trace.depmin() == in.depmin());
      REQUIRE(trace.depmax() == in.depmax());
      REQUIRE(trace.odelta() == in.odelta());
      REQUIRE(trace.resp0() == in.resp0());
      REQUIRE(trace.resp1() == in.resp1());
      REQUIRE(trace.resp2() == in.resp2());
      REQUIRE(trace.resp3() == in.resp3());
      REQUIRE(trace.resp4() == in.resp4());
      REQUIRE(trace.resp5() == in.resp5());
      REQUIRE(trace.resp6() == in.resp6());
      REQUIRE(trace.resp7() == in.resp7());
      REQUIRE(trace.resp8() == in.resp8());
      REQUIRE(trace.resp9() == in.resp9());
      REQUIRE(trace.stel() == in.stel());
      REQUIRE(trace.stdp() == in.stdp());
      REQUIRE(trace.evel() == in.evel());
      REQUIRE(trace.evdp() == in.evdp());
      REQUIRE(trace.mag() == in.mag());
      REQUIRE(trace.user1() == in.user1());
      REQUIRE(trace.user2() == in.user2());
      REQUIRE(trace.user3() == in.user3());
      REQUIRE(trace.user4() == in.user4());
      REQUIRE(trace.user5() == in.user5());
      REQUIRE(trace.user6() == in.user6());
      REQUIRE(trace.user7() == in.user7());
      REQUIRE(trace.user8() == in.user8());
      REQUIRE(trace.user9() == in.user9());
      REQUIRE(trace.dist() == in.dist());
      REQUIRE(trace.az() == in.az());
      REQUIRE(trace.baz() == in.baz());
      REQUIRE(trace.gcarc() == in.gcarc());
      REQUIRE(trace.depmen() == in.depmen());
      REQUIRE(trace.cmpaz() == in.cmpaz());
      REQUIRE(trace.cmpinc() == in.cmpinc());
      REQUIRE(trace.xminimum() == in.xminimum());
      REQUIRE(trace.xmaximum() == in.xmaximum());
      REQUIRE(trace.yminimum() == in.yminimum());
      REQUIRE(trace.ymaximum() == in.ymaximum());
    }
    SECTION("Doubles") {
      REQUIRE(trace.delta() == in.delta());
      REQUIRE(trace.b() == in.b());
      REQUIRE(trace.e() == in.e());
      REQUIRE(trace.o() == in.o());
      REQUIRE(trace.a() == in.a());
      REQUIRE(trace.t0() == in.t0());
      REQUIRE(trace.t1() == in.t1());
      REQUIRE(trace.t2() == in.t2());
      REQUIRE(trace.t3() == in.t3());
      REQUIRE(trace.t4() == in.t4());
      REQUIRE(trace.t5() == in.t5());
      REQUIRE(trace.t6() == in.t6());
      REQUIRE(trace.t7() == in.t7());
      REQUIRE(trace.t8() == in.t8());
      REQUIRE(trace.t9() == in.t9());
      REQUIRE(trace.f() == in.f());
      REQUIRE(trace.stla() == in.stla());
      REQUIRE(trace.stlo() == in.stlo());
      REQUIRE(trace.evla() == in.evla());
      REQUIRE(trace.evlo() == in.evlo());
      REQUIRE(trace.sb() == in.sb());
      REQUIRE(trace.sdelta() == in.sdelta());
    }
    SECTION("Integers") {
      REQUIRE(trace.nzyear() == in.nzyear());
      REQUIRE(trace.nzjday() == in.nzjday());
      REQUIRE(trace.nzhour() == in.nzhour());
      REQUIRE(trace.nzmin() == in.nzmin());
      REQUIRE(trace.nzsec() == in.nzsec());
      REQUIRE(trace.nzmsec() == in.nzmsec());
      REQUIRE(trace.nvhdr() == in.nvhdr());
      REQUIRE(trace.norid() == in.norid());
      REQUIRE(trace.nevid() == in.nevid());
      REQUIRE(trace.npts() == in.npts());
      REQUIRE(trace.nsnpts() == in.nsnpts());
      REQUIRE(trace.nwfid() == in.nwfid());
      REQUIRE(trace.nxsize() == in.nxsize());
      REQUIRE(trace.nysize() == in.nysize());
      REQUIRE(trace.iftype() == in.iftype());
      REQUIRE(trace.idep() == in.idep());
      REQUIRE(trace.iztype() == in.iztype());
      REQUIRE(trace.iinst() == in.iinst());
      REQUIRE(trace.istreg() == in.istreg());
      REQUIRE(trace.ievreg() == in.ievreg());
      REQUIRE(trace.ievtyp() == in.ievtyp());
      REQUIRE(trace.iqual() == in.iqual());
      REQUIRE(trace.isynth() == in.isynth());
      REQUIRE(trace.imagtyp() == in.imagtyp());
      REQUIRE(trace.imagsrc() == in.imagsrc());
      REQUIRE(trace.ibody() == in.ibody());
    }
    SECTION("Booleans") {
      REQUIRE(trace.leven() == in.leven());
      REQUIRE(trace.lpspol() == in.lpspol());
      REQUIRE(trace.lovrok() == in.lovrok());
      REQUIRE(trace.lcalda() == in.lcalda());
    }
    SECTION("Strings") {
      REQUIRE(trace.kstnm() == in.kstnm());
      REQUIRE(trace.kevnm() == in.kevnm());
      REQUIRE(trace.khole() == in.khole());
      REQUIRE(trace.ko() == in.ko());
      REQUIRE(trace.ka() == in.ka());
      REQUIRE(trace.kt0() == in.kt0());
      REQUIRE(trace.kt1() == in.kt1());
      REQUIRE(trace.kt2() == in.kt2());
      REQUIRE(trace.kt3() == in.kt3());
      REQUIRE(trace.kt4() == in.kt4());
      REQUIRE(trace.kt5() == in.kt5());
      REQUIRE(trace.kt6() == in.kt6());
      REQUIRE(trace.kt7() == in.kt7());
      REQUIRE(trace.kt8() == in.kt8());
      REQUIRE(trace.kt9() == in.kt9());
      REQUIRE(trace.kf() == in.kf());
      REQUIRE(trace.kuser0() == in.kuser0());
      REQUIRE(trace.kuser1() == in.kuser1());
      REQUIRE(trace.kuser2() == in.kuser2());
      REQUIRE(trace.kcmpnm() == in.kcmpnm());
      REQUIRE(trace.knetwk() == in.knetwk());
      REQUIRE(trace.kdatrd() == in.kdatrd());
      REQUIRE(trace.kinst() == in.kinst());
    }
    SECTION("Data") {
      REQUIRE(equal_within_tolerance(trace.data1(), in.data1()));
      REQUIRE(equal_within_tolerance(trace.data2(), in.data2()));
    }
    SECTION("Everthing") { REQUIRE(trace == in); }
  }
  SECTION("Random I/O") {
    std::vector data{trace.data1()};
    random_vector(&data);
    trace.data1(data);
    data = trace.data2();
    if (!data.empty()) {
      random_vector(&data);
      trace.data2(data);
    }
    trace.write(tmp_file);
    Trace in = Trace(tmp_file);
    fs::remove(tmp_file);
    SECTION("Floats") {
      REQUIRE(trace.depmin() == in.depmin());
      REQUIRE(trace.depmax() == in.depmax());
      REQUIRE(trace.odelta() == in.odelta());
      REQUIRE(trace.resp0() == in.resp0());
      REQUIRE(trace.resp1() == in.resp1());
      REQUIRE(trace.resp2() == in.resp2());
      REQUIRE(trace.resp3() == in.resp3());
      REQUIRE(trace.resp4() == in.resp4());
      REQUIRE(trace.resp5() == in.resp5());
      REQUIRE(trace.resp6() == in.resp6());
      REQUIRE(trace.resp7() == in.resp7());
      REQUIRE(trace.resp8() == in.resp8());
      REQUIRE(trace.resp9() == in.resp9());
      REQUIRE(trace.stel() == in.stel());
      REQUIRE(trace.stdp() == in.stdp());
      REQUIRE(trace.evel() == in.evel());
      REQUIRE(trace.evdp() == in.evdp());
      REQUIRE(trace.mag() == in.mag());
      REQUIRE(trace.user0() == in.user0());
      REQUIRE(trace.user1() == in.user1());
      REQUIRE(trace.user2() == in.user2());
      REQUIRE(trace.user3() == in.user3());
      REQUIRE(trace.user4() == in.user4());
      REQUIRE(trace.user5() == in.user5());
      REQUIRE(trace.user6() == in.user6());
      REQUIRE(trace.user7() == in.user7());
      REQUIRE(trace.user8() == in.user8());
      REQUIRE(trace.user9() == in.user9());
      REQUIRE(trace.dist() == in.dist());
      REQUIRE(trace.az() == in.az());
      REQUIRE(trace.baz() == in.baz());
      REQUIRE(trace.gcarc() == in.gcarc());
      REQUIRE(trace.depmen() == in.depmen());
      REQUIRE(trace.cmpaz() == in.cmpaz());
      REQUIRE(trace.cmpinc() == in.cmpinc());
      REQUIRE(trace.xminimum() == in.xminimum());
      REQUIRE(trace.xmaximum() == in.xmaximum());
      REQUIRE(trace.yminimum() == in.yminimum());
      REQUIRE(trace.ymaximum() == in.ymaximum());
    }
    SECTION("Doubles") {
      REQUIRE(trace.delta() == in.delta());
      REQUIRE(trace.b() == in.b());
      REQUIRE(trace.e() == in.e());
      REQUIRE(trace.o() == in.o());
      REQUIRE(trace.a() == in.a());
      REQUIRE(trace.t0() == in.t0());
      REQUIRE(trace.t1() == in.t1());
      REQUIRE(trace.t2() == in.t2());
      REQUIRE(trace.t3() == in.t3());
      REQUIRE(trace.t4() == in.t4());
      REQUIRE(trace.t5() == in.t5());
      REQUIRE(trace.t6() == in.t6());
      REQUIRE(trace.t7() == in.t7());
      REQUIRE(trace.t8() == in.t8());
      REQUIRE(trace.t9() == in.t9());
      REQUIRE(trace.f() == in.f());
      REQUIRE(trace.stla() == in.stla());
      REQUIRE(trace.stlo() == in.stlo());
      REQUIRE(trace.evla() == in.evla());
      REQUIRE(trace.evlo() == in.evlo());
      REQUIRE(trace.sb() == in.sb());
      REQUIRE(trace.sdelta() == in.sdelta());
    }
    SECTION("Integers") {
      REQUIRE(trace.nzyear() == in.nzyear());
      REQUIRE(trace.nzjday() == in.nzjday());
      REQUIRE(trace.nzhour() == in.nzhour());
      REQUIRE(trace.nzmin() == in.nzmin());
      REQUIRE(trace.nzsec() == in.nzsec());
      REQUIRE(trace.nzmsec() == in.nzmsec());
      REQUIRE(trace.nvhdr() == in.nvhdr());
      REQUIRE(trace.norid() == in.norid());
      REQUIRE(trace.nevid() == in.nevid());
      REQUIRE(trace.npts() == in.npts());
      REQUIRE(trace.nsnpts() == in.nsnpts());
      REQUIRE(trace.nwfid() == in.nwfid());
      REQUIRE(trace.nxsize() == in.nxsize());
      REQUIRE(trace.nysize() == in.nysize());
      REQUIRE(trace.iftype() == in.iftype());
      REQUIRE(trace.idep() == in.idep());
      REQUIRE(trace.iztype() == in.iztype());
      REQUIRE(trace.iinst() == in.iinst());
      REQUIRE(trace.istreg() == in.istreg());
      REQUIRE(trace.ievreg() == in.ievreg());
      REQUIRE(trace.ievtyp() == in.ievtyp());
      REQUIRE(trace.iqual() == in.iqual());
      REQUIRE(trace.isynth() == in.isynth());
      REQUIRE(trace.imagtyp() == in.imagtyp());
      REQUIRE(trace.imagsrc() == in.imagsrc());
      REQUIRE(trace.ibody() == in.ibody());
    }
    SECTION("Booleans") {
      REQUIRE(trace.leven() == in.leven());
      REQUIRE(trace.lpspol() == in.lpspol());
      REQUIRE(trace.lovrok() == in.lovrok());
      REQUIRE(trace.lcalda() == in.lcalda());
    }
    SECTION("Strings") {
      REQUIRE(trace.kstnm() == in.kstnm());
      REQUIRE(trace.kevnm() == in.kevnm());
      REQUIRE(trace.khole() == in.khole());
      REQUIRE(trace.ko() == in.ko());
      REQUIRE(trace.ka() == in.ka());
      REQUIRE(trace.kt0() == in.kt0());
      REQUIRE(trace.kt1() == in.kt1());
      REQUIRE(trace.kt2() == in.kt2());
      REQUIRE(trace.kt3() == in.kt3());
      REQUIRE(trace.kt4() == in.kt4());
      REQUIRE(trace.kt5() == in.kt5());
      REQUIRE(trace.kt6() == in.kt6());
      REQUIRE(trace.kt7() == in.kt7());
      REQUIRE(trace.kt8() == in.kt8());
      REQUIRE(trace.kt9() == in.kt9());
      REQUIRE(trace.kf() == in.kf());
      REQUIRE(trace.kuser0() == in.kuser0());
      REQUIRE(trace.kuser1() == in.kuser1());
      REQUIRE(trace.kuser2() == in.kuser2());
      REQUIRE(trace.kcmpnm() == in.kcmpnm());
      REQUIRE(trace.knetwk() == in.knetwk());
      REQUIRE(trace.kdatrd() == in.kdatrd());
      REQUIRE(trace.kinst() == in.kinst());
    }
    SECTION("Data") {
      REQUIRE(equal_within_tolerance(trace.data1(), in.data1()));
      REQUIRE(equal_within_tolerance(trace.data2(), in.data2()));
    }
    SECTION("Everything") { REQUIRE(trace == in); }
    SECTION("Resize data") {
      Trace in2{};
      in2 = in;
      std::vector<double> tmp{in2.data1()};
      tmp.resize(10);
      in2.data1(tmp);
      REQUIRE(!equal_within_tolerance(trace.data1(), in2.data1()));
    }
    SECTION("Change a float") {
      Trace in2{};
      in2 = in;
      in2.evdp(in.evdp() - 1.0f);
      REQUIRE(trace != in2);
    }
    SECTION("Change a vector value") {
      Trace in2{};
      in2 = in;
      std::vector<double> tmp{in2.data1()};
      tmp[0] -= 1.0;
      in2.data1(tmp);
      REQUIRE(trace != in2);
    }
  }
  SECTION("Legacy Read/Write") {
    trace.nvhdr(6);
    trace.write(tmp_file);
    Trace in = Trace(tmp_file);
    fs::remove(tmp_file);
    SECTION("Convert header") {
      REQUIRE(in.nvhdr() == 7);
      // Make sure we didn't change the original trace
      REQUIRE(trace.nvhdr() == 6);
    }
    SECTION("Floats") {
      REQUIRE(trace.depmin() == in.depmin());
      REQUIRE(trace.depmax() == in.depmax());
      REQUIRE(trace.odelta() == in.odelta());
      REQUIRE(trace.resp0() == in.resp0());
      REQUIRE(trace.resp1() == in.resp1());
      REQUIRE(trace.resp2() == in.resp2());
      REQUIRE(trace.resp3() == in.resp3());
      REQUIRE(trace.resp4() == in.resp4());
      REQUIRE(trace.resp5() == in.resp5());
      REQUIRE(trace.resp6() == in.resp6());
      REQUIRE(trace.resp7() == in.resp7());
      REQUIRE(trace.resp8() == in.resp8());
      REQUIRE(trace.resp9() == in.resp9());
      REQUIRE(trace.stel() == in.stel());
      REQUIRE(trace.stdp() == in.stdp());
      REQUIRE(trace.evel() == in.evel());
      REQUIRE(trace.evdp() == in.evdp());
      REQUIRE(trace.mag() == in.mag());
      REQUIRE(trace.user0() == in.user0());
      REQUIRE(trace.user1() == in.user1());
      REQUIRE(trace.user2() == in.user2());
      REQUIRE(trace.user3() == in.user3());
      REQUIRE(trace.user4() == in.user4());
      REQUIRE(trace.user5() == in.user5());
      REQUIRE(trace.user6() == in.user6());
      REQUIRE(trace.user7() == in.user7());
      REQUIRE(trace.user8() == in.user8());
      REQUIRE(trace.user9() == in.user9());
      REQUIRE(trace.dist() == in.dist());
      REQUIRE(trace.az() == in.az());
      REQUIRE(trace.baz() == in.baz());
      REQUIRE(trace.gcarc() == in.gcarc());
      REQUIRE(trace.depmen() == in.depmen());
      REQUIRE(trace.cmpaz() == in.cmpaz());
      REQUIRE(trace.cmpinc() == in.cmpinc());
      REQUIRE(trace.xminimum() == in.xminimum());
      REQUIRE(trace.xmaximum() == in.xmaximum());
      REQUIRE(trace.yminimum() == in.yminimum());
      REQUIRE(trace.ymaximum() == in.ymaximum());
    }
    SECTION("Doubles") {
      REQUIRE_THAT(trace.delta(), WithinAbs(in.delta(), f_eps));
      REQUIRE_THAT(trace.b(), WithinAbs(in.b(), f_eps));
      REQUIRE_THAT(trace.e(), WithinAbs(in.e(), f_eps));
      REQUIRE_THAT(trace.o(), WithinAbs(in.o(), f_eps));
      REQUIRE_THAT(trace.a(), WithinAbs(in.a(), f_eps));
      REQUIRE_THAT(trace.t0(), WithinAbs(in.t0(), f_eps));
      REQUIRE_THAT(trace.t1(), WithinAbs(in.t1(), f_eps));
      REQUIRE_THAT(trace.t2(), WithinAbs(in.t2(), f_eps));
      REQUIRE_THAT(trace.t3(), WithinAbs(in.t3(), f_eps));
      REQUIRE_THAT(trace.t4(), WithinAbs(in.t4(), f_eps));
      REQUIRE_THAT(trace.t5(), WithinAbs(in.t5(), f_eps));
      REQUIRE_THAT(trace.t6(), WithinAbs(in.t6(), f_eps));
      REQUIRE_THAT(trace.t7(), WithinAbs(in.t7(), f_eps));
      REQUIRE_THAT(trace.t8(), WithinAbs(in.t8(), f_eps));
      REQUIRE_THAT(trace.t9(), WithinAbs(in.t9(), f_eps));
      REQUIRE_THAT(trace.f(), WithinAbs(in.f(), f_eps));
      REQUIRE_THAT(trace.stla(), WithinAbs(in.stla(), f_eps));
      REQUIRE_THAT(trace.stlo(), WithinAbs(in.stlo(), f_eps));
      REQUIRE_THAT(trace.evla(), WithinAbs(in.evla(), f_eps));
      REQUIRE_THAT(trace.evlo(), WithinAbs(in.evlo(), f_eps));
      REQUIRE_THAT(trace.sb(), WithinAbs(in.sb(), f_eps));
      REQUIRE_THAT(trace.sdelta(), WithinAbs(in.sdelta(), f_eps));
    }
    SECTION("Integers") {
      REQUIRE(trace.nzyear() == in.nzyear());
      REQUIRE(trace.nzjday() == in.nzjday());
      REQUIRE(trace.nzhour() == in.nzhour());
      REQUIRE(trace.nzmin() == in.nzmin());
      REQUIRE(trace.nzsec() == in.nzsec());
      REQUIRE(trace.nzmsec() == in.nzmsec());
      REQUIRE(trace.nvhdr() != in.nvhdr());
      REQUIRE(trace.norid() == in.norid());
      REQUIRE(trace.nevid() == in.nevid());
      REQUIRE(trace.npts() == in.npts());
      REQUIRE(trace.nsnpts() == in.nsnpts());
      REQUIRE(trace.nwfid() == in.nwfid());
      REQUIRE(trace.nxsize() == in.nxsize());
      REQUIRE(trace.nysize() == in.nysize());
      REQUIRE(trace.iftype() == in.iftype());
      REQUIRE(trace.idep() == in.idep());
      REQUIRE(trace.iztype() == in.iztype());
      REQUIRE(trace.iinst() == in.iinst());
      REQUIRE(trace.istreg() == in.istreg());
      REQUIRE(trace.ievreg() == in.ievreg());
      REQUIRE(trace.ievtyp() == in.ievtyp());
      REQUIRE(trace.iqual() == in.iqual());
      REQUIRE(trace.isynth() == in.isynth());
      REQUIRE(trace.imagtyp() == in.imagtyp());
      REQUIRE(trace.imagsrc() == in.imagsrc());
      REQUIRE(trace.ibody() == in.ibody());
    }
    SECTION("Booleans") {
      REQUIRE(trace.leven() == in.leven());
      REQUIRE(trace.lpspol() == in.lpspol());
      REQUIRE(trace.lovrok() == in.lovrok());
      REQUIRE(trace.lcalda() == in.lcalda());
    }
    SECTION("Strings") {
      REQUIRE(trace.kstnm() == in.kstnm());
      REQUIRE(trace.kevnm() == in.kevnm());
      REQUIRE(trace.khole() == in.khole());
      REQUIRE(trace.ko() == in.ko());
      REQUIRE(trace.ka() == in.ka());
      REQUIRE(trace.kt0() == in.kt0());
      REQUIRE(trace.kt1() == in.kt1());
      REQUIRE(trace.kt2() == in.kt2());
      REQUIRE(trace.kt3() == in.kt3());
      REQUIRE(trace.kt4() == in.kt4());
      REQUIRE(trace.kt5() == in.kt5());
      REQUIRE(trace.kt6() == in.kt6());
      REQUIRE(trace.kt7() == in.kt7());
      REQUIRE(trace.kt8() == in.kt8());
      REQUIRE(trace.kt9() == in.kt9());
      REQUIRE(trace.kf() == in.kf());
      REQUIRE(trace.kuser0() == in.kuser0());
      REQUIRE(trace.kuser1() == in.kuser1());
      REQUIRE(trace.kuser2() == in.kuser2());
      REQUIRE(trace.kcmpnm() == in.kcmpnm());
      REQUIRE(trace.knetwk() == in.knetwk());
      REQUIRE(trace.kdatrd() == in.kdatrd());
      REQUIRE(trace.kinst() == in.kinst());
    }
    SECTION("Data") {
      REQUIRE(equal_within_tolerance(trace.data1(), in.data1()));
      REQUIRE(equal_within_tolerance(trace.data2(), in.data2()));
    }
    SECTION("Everything") { REQUIRE(trace != in); }
    SECTION("Not equal within tolerance") {
      std::vector<double> data{trace.data1()};
      data[0] += 1.0;
      trace.data1(data);
      REQUIRE(!equal_within_tolerance(trace.data1(), in.data1()));
      REQUIRE(trace != in);
    }
  }
  SECTION("v6") {
    trace.nvhdr(7);
    trace.legacy_write(tmp_file);
    Trace in = Trace(tmp_file);
    fs::remove(tmp_file);
    SECTION("Convert header") {
      REQUIRE(in.nvhdr() == 6);
      // Make sure we didn't change the original trace
      REQUIRE(trace.nvhdr() == 7);
      REQUIRE(trace != in);
    }
  }
  SECTION("Data2") {
    // XY
    trace.iftype(4);
    trace.data2(trace.data1());
    SECTION("Equal") {
      trace.write(tmp_file);
      Trace in = Trace(tmp_file);
      fs::remove(tmp_file);
      REQUIRE(equal_within_tolerance(trace.data2(), in.data2()));
      REQUIRE(trace == in);
    }
    SECTION("Not Equal") {
      std::vector<double> data{trace.data2()};
      trace.write(tmp_file);
      Trace in = Trace(tmp_file);
      fs::remove(tmp_file);
      data[data.size() - 1] -= 1.0;
      trace.data2(data);
      REQUIRE(!equal_within_tolerance(trace.data2(), in.data2()));
      REQUIRE(trace != in);
    }
  }
  SECTION("Read Throw") {
    fs::path tmp_dir{fs::temp_directory_path() / "not_a_dir"};
    fs::path tmp_file{tmp_dir / "not_real.sac"};
    REQUIRE_THROWS(Trace(tmp_file));
  }
  SECTION("Write Throw") {
    fs::path tmp_dir{fs::temp_directory_path() / "not_a_dir"};
    fs::path tmp_file{tmp_dir / "not_real.sac"};
    REQUIRE_THROWS(trace.write(tmp_file));
  }
}

TEST_CASE("Geometric operations") {
  SECTION("Great Circle Arc (gcarc)") {
    constexpr double lat1{38.4328};
    constexpr double lon1{-118.155};
    constexpr double lat2{36.801};
    constexpr double lon2{-121.323};
    constexpr double expected_gcarc{2.99645};
    constexpr double expected_az{56.1169};
    constexpr double expected_baz{238.043};
    const double test_gcarc{gcarc(lat1, lon1, lat2, lon2)};
    REQUIRE_THAT(test_gcarc, WithinAbs(expected_gcarc, 5e-3));
    const double test_az{azimuth(lat2, lon2, lat1, lon1)};
    const double test_baz{azimuth(lat1, lon1, lat2, lon2)};
    REQUIRE_THAT(test_az, WithinAbs(expected_az, 0.2));
    REQUIRE_THAT(test_baz, WithinAbs(expected_baz, 0.2));
  }
}
