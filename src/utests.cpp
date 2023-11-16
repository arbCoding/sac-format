/* Standard Library */
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
/* Catch2 */
// testing macros
#include <catch2/catch_test_macros.hpp>
/* My stuff */
#include "sac_format.hpp"
#include "util.hpp"

using namespace sacfmt;
namespace fs = std::filesystem;

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
      constexpr float lowest{std::numeric_limits<float>::lowest()};
      REQUIRE(binary_to_float(float_to_binary(lowest)) == lowest);
      constexpr float neg_epsilon{-std::numeric_limits<float>::epsilon()};
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
      constexpr float highest{std::numeric_limits<float>::max()};
      REQUIRE(binary_to_float(float_to_binary(highest)) == highest);
      constexpr float epsilon{std::numeric_limits<float>::epsilon()};
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
      constexpr double lowest{std::numeric_limits<double>::lowest()};
      REQUIRE(binary_to_double(double_to_binary(lowest)) == lowest);
      constexpr double neg_epsilon{-std::numeric_limits<double>::epsilon()};
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
      constexpr double highest{std::numeric_limits<double>::max()};
      REQUIRE(binary_to_double(double_to_binary(highest)) == highest);
      constexpr double epsilon{std::numeric_limits<double>::epsilon()};
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

TEST_CASE("Trace Save/Load") {
  Trace test_sac = gen_fake_sac();
  fs::path tmp_dir{fs::temp_directory_path()};
  fs::path tmp_file{tmp_dir / "test.SAC"};
  SECTION("I/O Zeros") {
    test_sac.write(tmp_file);
    Trace in_sac = Trace(tmp_file);
    REQUIRE(test_sac == in_sac);
    fs::remove(tmp_file);
  }
  SECTION("I/O Random") {
    random_vector(test_sac.data1);
    if (!test_sac.data2.empty()) { random_vector(test_sac.data2); }
    test_sac.write(tmp_file);
    Trace in_sac = Trace(tmp_file);
    REQUIRE(test_sac == in_sac);
    fs::remove(tmp_file);
  }
}
//==============================================================================
// Trace2 tests
//------------------------------------------------------------------------------
TEST_CASE("Trace 2 Default Construction") {
  const Trace2 trace{};
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

TEST_CASE("Trace 2 Setters") {
  Trace2 trace{};
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
    random_vector(value);
    trace.data1(value);
    REQUIRE(trace.data1() == value);

    random_vector(value);
    trace.data2(value);
    REQUIRE(trace.data2() == value);
  }
}

TEST_CASE("Trace2 Equality") {
  SECTION("Default Construction") {
    const Trace2 trace1{};
    const Trace2 trace2{};
    REQUIRE(trace1 == trace2);
  }
  // Confirmed it'll fail if you try to make them equal
  SECTION("Inequality") {
    const Trace2 trace1{};
    Trace2 trace2{};
    trace2.kinst("Hello");
    REQUIRE(trace1 != trace2);
  }
}
