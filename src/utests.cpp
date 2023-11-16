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
// Location tests
TEST_CASE("Location") {
  SECTION("Default") {
    const Location loc{};
    REQUIRE(loc.latitude == unset_double);
    REQUIRE(loc.longitude == unset_double);
    REQUIRE(loc.depth == unset_double);
    REQUIRE(loc.elevation == unset_double);
  }
}
//------------------------------------------------------------------------------
// User tests
TEST_CASE("User") {
  SECTION("Default") {
    const User user{};
    REQUIRE(user.t0 == unset_double);
    REQUIRE(user.t1 == unset_double);
    REQUIRE(user.t2 == unset_double);
    REQUIRE(user.t3 == unset_double);
    REQUIRE(user.t4 == unset_double);
    REQUIRE(user.t5 == unset_double);
    REQUIRE(user.t6 == unset_double);
    REQUIRE(user.t7 == unset_double);
    REQUIRE(user.t8 == unset_double);
    REQUIRE(user.t9 == unset_double);
    REQUIRE(user.kt0 == unset_word);
    REQUIRE(user.kt1 == unset_word);
    REQUIRE(user.kt2 == unset_word);
    REQUIRE(user.kt3 == unset_word);
    REQUIRE(user.kt4 == unset_word);
    REQUIRE(user.kt5 == unset_word);
    REQUIRE(user.kt6 == unset_word);
    REQUIRE(user.kt7 == unset_word);
    REQUIRE(user.kt8 == unset_word);
    REQUIRE(user.kt9 == unset_word);
    REQUIRE(user.user0 == unset_float);
    REQUIRE(user.user1 == unset_float);
    REQUIRE(user.user2 == unset_float);
    REQUIRE(user.user3 == unset_float);
    REQUIRE(user.user4 == unset_float);
    REQUIRE(user.user5 == unset_float);
    REQUIRE(user.user6 == unset_float);
    REQUIRE(user.user7 == unset_float);
    REQUIRE(user.user8 == unset_float);
    REQUIRE(user.user9 == unset_float);
    REQUIRE(user.kuser0 == unset_word);
    REQUIRE(user.kuser1 == unset_word);
    REQUIRE(user.kuser2 == unset_word);
  }
}
//------------------------------------------------------------------------------
// Instrument tests
TEST_CASE("Instrument") {
  SECTION("Default") {
    const Instrument inst{};
    REQUIRE(inst.resp0 == unset_float);
    REQUIRE(inst.resp1 == unset_float);
    REQUIRE(inst.resp2 == unset_float);
    REQUIRE(inst.resp3 == unset_float);
    REQUIRE(inst.resp4 == unset_float);
    REQUIRE(inst.resp5 == unset_float);
    REQUIRE(inst.resp6 == unset_float);
    REQUIRE(inst.resp7 == unset_float);
    REQUIRE(inst.resp8 == unset_float);
    REQUIRE(inst.resp9 == unset_float);
    REQUIRE(inst.cmpaz == unset_float);
    REQUIRE(inst.cmpinc == unset_float);
    REQUIRE(inst.iinst == unset_int);
    REQUIRE(inst.kcmpnm == unset_word);
    REQUIRE(inst.kinst == unset_word);
  }
}
//------------------------------------------------------------------------------
// Event tests
TEST_CASE("Event") {
  SECTION("Default") {
    const Event event{};
    const Location loc{};
    REQUIRE(event.location == loc);
    REQUIRE(event.origin_time == unset_double);
    REQUIRE(event.ko == unset_word);
    REQUIRE(event.first_arrival == unset_double);
    REQUIRE(event.ka == unset_word);
    REQUIRE(event.fini_time == unset_double);
    REQUIRE(event.kf == unset_word);
    REQUIRE(event.magnitude == unset_float);
    REQUIRE(event.nevid == unset_int);
    REQUIRE(event.ievreg == unset_int);
    REQUIRE(event.ievtyp == unset_int);
    REQUIRE(event.imagtyp == unset_int);
    REQUIRE(event.kevnm == unset_word);
  }
}
//------------------------------------------------------------------------------
// Station tests
TEST_CASE("Station") {
  SECTION("Default") {
    const Station station{};
    const Location loc{};
    const Instrument inst{};
    REQUIRE(station.location == loc);
    REQUIRE(station.instrument == inst);
    REQUIRE(station.istreg == unset_int);
    REQUIRE(station.lpspol == unset_bool);
    REQUIRE(station.kstnm == unset_word);
    REQUIRE(station.knetwk == unset_word);
    REQUIRE(station.khole == unset_word);
  }
}
//------------------------------------------------------------------------------
// Date tests
TEST_CASE("Date") {
  SECTION("Default") {
    const Date date{};
    REQUIRE(date.nzyear == unset_int);
    REQUIRE(date.nzjday == unset_int);
    REQUIRE(date.nzhour == unset_int);
    REQUIRE(date.nzmin == unset_int);
    REQUIRE(date.nzsec == unset_int);
    REQUIRE(date.nzmsec == unset_int);
  }
}
//------------------------------------------------------------------------------
// Data tests
TEST_CASE("Data") {
  SECTION("Default") {
    const Data data{};
    const Date date{};
    REQUIRE(data.depmin == unset_float);
    REQUIRE(data.depmen == unset_float);
    REQUIRE(data.depmax == unset_float);
    REQUIRE(data.begin_time == unset_double);
    REQUIRE(data.end_time == unset_double);
    REQUIRE(data.dist == unset_float);
    REQUIRE(data.az == unset_float);
    REQUIRE(data.baz == unset_float);
    REQUIRE(data.gcarc == unset_float);
    REQUIRE(data.origin_begin == unset_double);
    REQUIRE(data.delta == unset_double);
    REQUIRE(data.odelta == unset_double);
    REQUIRE(data.sdelta == unset_double);
    REQUIRE(data.xminimum == unset_float);
    REQUIRE(data.xmaximum == unset_float);
    REQUIRE(data.yminimum == unset_float);
    REQUIRE(data.ymaximum == unset_float);
    REQUIRE(data.reference_date == date);
    REQUIRE(data.origin_id == unset_int);
    REQUIRE(data.npts == unset_int);
    REQUIRE(data.nsnpts == unset_int);
    REQUIRE(data.waveform_id == unset_int);
    REQUIRE(data.nxsize == unset_int);
    REQUIRE(data.nysize == unset_int);
    REQUIRE(data.idep == unset_int);
    REQUIRE(data.reference_date_type == unset_int);
    REQUIRE(data.quality == unset_int);
    REQUIRE(data.synthetic == unset_int);
    REQUIRE(data.magnitude_source == unset_int);
    REQUIRE(data.reference_body == unset_int);
    REQUIRE(data.evenly_sampled == unset_bool);
    REQUIRE(data.calc_geometry == unset_bool);
    REQUIRE(data.kdatrd == unset_word);
    REQUIRE(data.data1.empty());
    REQUIRE(data.data2.empty());
  }
}
//------------------------------------------------------------------------------
// File tests
TEST_CASE("File") {
  SECTION("Default") {
    const File file{};
    REQUIRE(file.version == 7);
    REQUIRE(file.type == unset_int);
    REQUIRE(file.overwrite == unset_bool);
    REQUIRE(file.path.empty());
  }
}
//------------------------------------------------------------------------------
// Trace2 tests
TEST_CASE("Trace2") {
  SECTION("Default") {
    const Trace2 trace2{};
    const Station station{};
    const Event event{};
    const User user{};
    const Data data{};
    const File file{};
    REQUIRE(trace2.station == station);
    REQUIRE(trace2.event == event);
    REQUIRE(trace2.user == user);
    REQUIRE(trace2.data == data);
    REQUIRE(trace2.file == file);
  }
}
