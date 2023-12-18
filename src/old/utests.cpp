// Copyright 2023 Alexander R. Blanchette

// sac-format
#include "sac-format/sac_format.hpp"
#include "sac-format/util.hpp"
// Standard Library
//   https://en.cppreference.com/w/cpp/standard_library
// std::filesystem::path, std::filesystem::temp_directory_path()
#include <filesystem>
// Catch2 https://github.com/catchorg/Catch2/tree/v3.4.0
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_MAIN
// testing macros
// TEST_CASE, SECTION, REQUIRE, CAPTURE, REQUIRE_THROWS, REQUIRE_NOTHROW
#include <catch2/catch_test_macros.hpp>
// from catch_matchers.hpp (any matcher includes it)
// REQUIRE_THAT
// Catch::Matchers::WithinAbs
#include <catch2/matchers/catch_matchers_floating_point.hpp>
// Catch::Matchers::ContainsSubstring
#include <catch2/matchers/catch_matchers_string.hpp>

using namespace sacfmt;
namespace fs = std::filesystem;
using Catch::Matchers::ContainsSubstring;
using Catch::Matchers::WithinAbs;
// NOLINTBEGIN(readability-magic-numbers)
//==============================================================================
//==============================================================================
// Trace tests
//------------------------------------------------------------------------------
TEST_CASE("Linked Headers") {
  const fs::path tmp_dir{fs::temp_directory_path()};
  const fs::path tmp_file{tmp_dir / "linked_parameter_test.sac"};
  SECTION("LEven") {
    Trace trace{};
    // Even non-2D data
    trace.leven(true);
    trace.iftype(1);
    trace.npts(10);
    trace.nvhdr(7);
    REQUIRE(trace.leven() == true);
    REQUIRE(trace.iftype() == 1);
    REQUIRE(trace.data2().size() == 0);
    quick_io_check(tmp_file, trace);
    // Uneven data
    trace.leven(false);
    REQUIRE(trace.iftype() == 1);
    quick_io_check(tmp_file, trace);
    // Return to even data
    trace.leven(true);
    REQUIRE(trace.iftype() == 1);
    REQUIRE(trace.data2().size() == 0);
    quick_io_check(tmp_file, trace);
    // Spectral even
    trace.iftype(2);
    REQUIRE(trace.iftype() == 2);
    REQUIRE(trace.leven() == true);
    REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
    quick_io_check(tmp_file, trace);
    // Uneven
    trace.leven(false);
    REQUIRE(trace.iftype() == unset_int);
    REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
    quick_io_check(tmp_file, trace);
    // Return to even
    trace.leven(true);
    REQUIRE(trace.iftype() == unset_int);
    REQUIRE(trace.data2().size() == 0);
    quick_io_check(tmp_file, trace);
  }
  SECTION("IFType") {
    Trace trace{};
    // Even non-2D data
    trace.leven(true);
    trace.iftype(1);
    trace.npts(10);
    trace.nvhdr(7);
    REQUIRE(trace.leven() == true);
    REQUIRE(trace.iftype() == 1);
    REQUIRE(trace.data2().size() == 0);
    quick_io_check(tmp_file, trace);
    // Spectral data
    trace.iftype(2);
    REQUIRE(trace.leven() == true);
    REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
    quick_io_check(tmp_file, trace);
    // Return to even non-2D data
    trace.iftype(1);
    REQUIRE(trace.leven() == true);
    REQUIRE(trace.data2().size() == 0);
    quick_io_check(tmp_file, trace);
  }
  SECTION("Npts") {
    SECTION("Without Data2") {
      Trace trace{};
      // Even non-2D data
      trace.leven(true);
      trace.iftype(1);
      trace.npts(0);
      trace.nvhdr(7);
      REQUIRE(trace.npts() == 0);
      REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
      REQUIRE(trace.data2().size() == 0);
      quick_io_check(tmp_file, trace);
      SECTION("Npts 10") {
        trace.npts(10);
        REQUIRE(trace.npts() == 10);
        REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
        REQUIRE(trace.data2().size() == 0);
        quick_io_check(tmp_file, trace);
      }
      SECTION("Grow 10, shrink 5") {
        trace.npts(10);
        trace.npts(5);
        REQUIRE(trace.npts() == 5);
        REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
        REQUIRE(trace.data2().size() == 0);
        quick_io_check(tmp_file, trace);
      }
    }
    SECTION("With Data2") {
      SECTION("Uneven timeseries") {
        Trace trace{};
        trace.leven(false);
        trace.iftype(1);
        trace.npts(0);
        trace.nvhdr(7);
        REQUIRE(trace.npts() == 0);
        REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
        REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
        quick_io_check(tmp_file, trace);
        SECTION("Npts 10") {
          trace.npts(10);
          REQUIRE(trace.npts() == 10);
          REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
          REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
          quick_io_check(tmp_file, trace);
        }
        SECTION("Grow 10, shrink 5") {
          trace.npts(10);
          trace.npts(5);
          REQUIRE(trace.npts() == 5);
          REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
          REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
          quick_io_check(tmp_file, trace);
        }
      }
      SECTION("Even Spectral") {
        Trace trace{};
        trace.leven(true);
        trace.iftype(2);
        trace.npts(0);
        trace.nvhdr(7);
        REQUIRE(trace.npts() == 0);
        REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
        REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
        quick_io_check(tmp_file, trace);
        SECTION("Npts 10") {
          trace.npts(10);
          REQUIRE(trace.npts() == 10);
          REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
          REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
          quick_io_check(tmp_file, trace);
        }
        SECTION("Grow 10, shrink 5") {
          trace.npts(10);
          trace.npts(5);
          REQUIRE(trace.npts() == 5);
          REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
          REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
          quick_io_check(tmp_file, trace);
        }
      }
    }
  }
  SECTION("Data1") {
    SECTION("Even timeseries") {
      Trace trace{};
      trace.leven(true);
      trace.iftype(1);
      trace.npts(0);
      trace.nvhdr(7);
      REQUIRE(trace.npts() == 0);
      REQUIRE(trace.data1().size() == 0);
      REQUIRE(trace.data2().size() == 0);
      quick_io_check(tmp_file, trace);
      // Change the vector
      std::vector<double> new_data1{};
      new_data1.resize(10);
      trace.data1(new_data1);
      REQUIRE(trace.npts() == 10);
      REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
      REQUIRE(trace.data2().size() == 0);
      quick_io_check(tmp_file, trace);
    }
    SECTION("Uneven timeseries") {
      Trace trace{};
      trace.leven(false);
      trace.iftype(1);
      trace.npts(0);
      trace.nvhdr(7);
      REQUIRE(trace.npts() == 0);
      REQUIRE(trace.data1().size() == 0);
      REQUIRE(trace.data2().size() == 0);
      quick_io_check(tmp_file, trace);
      // Change the vector
      std::vector<double> new_data1{};
      new_data1.resize(10);
      trace.data1(new_data1);
      REQUIRE(trace.npts() == 10);
      REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
      REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
      quick_io_check(tmp_file, trace);
    }
    SECTION("Spectral") {
      Trace trace{};
      trace.leven(true);
      trace.iftype(2);
      trace.npts(0);
      trace.nvhdr(7);
      REQUIRE(trace.npts() == 0);
      REQUIRE(trace.data1().size() == 0);
      REQUIRE(trace.data2().size() == 0);
      quick_io_check(tmp_file, trace);
      // Change the vector
      std::vector<double> new_data1{};
      new_data1.resize(10);
      trace.data1(new_data1);
      REQUIRE(trace.npts() == 10);
      REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
      REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
      quick_io_check(tmp_file, trace);
    }
  }
  SECTION("Data2") {
    SECTION("Even timeseries") {
      Trace trace{};
      trace.leven(true);
      trace.iftype(1);
      trace.npts(0);
      trace.nvhdr(7);
      REQUIRE(trace.npts() == 0);
      REQUIRE(trace.data1().size() == 0);
      REQUIRE(trace.data2().size() == 0);
      quick_io_check(tmp_file, trace);
      // Change the vector
      std::vector<double> new_data2{};
      new_data2.resize(15);
      trace.data2(new_data2);
      REQUIRE(trace.npts() == 15);
      REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
      REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
      quick_io_check(tmp_file, trace);
      // Remove data2
      trace.iftype(1);
      REQUIRE(trace.npts() == 15);
      REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
      REQUIRE(trace.data2().size() == 0);
      quick_io_check(tmp_file, trace);
    }
    SECTION("Uneven timeseries") {
      Trace trace{};
      trace.leven(false);
      trace.iftype(1);
      trace.npts(0);
      trace.nvhdr(7);
      REQUIRE(trace.npts() == 0);
      REQUIRE(trace.data1().size() == 0);
      REQUIRE(trace.data2().size() == 0);
      quick_io_check(tmp_file, trace);
      // Change the vector
      std::vector<double> new_data2{};
      new_data2.resize(25);
      trace.data2(new_data2);
      REQUIRE(trace.npts() == 25);
      REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
      REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
      quick_io_check(tmp_file, trace);
      // Remove data2
      trace.leven(true);
      REQUIRE(trace.npts() == 25);
      REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
      REQUIRE(trace.data2().size() == 0);
      quick_io_check(tmp_file, trace);
    }
    SECTION("Spectral") {
      Trace trace{};
      trace.leven(true);
      trace.iftype(2);
      trace.npts(0);
      trace.nvhdr(7);
      REQUIRE(trace.npts() == 0);
      REQUIRE(trace.data1().size() == 0);
      REQUIRE(trace.data2().size() == 0);
      quick_io_check(tmp_file, trace);
      // Change the vector
      std::vector<double> new_data2{};
      new_data2.resize(12);
      trace.data2(new_data2);
      REQUIRE(trace.npts() == 12);
      REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
      REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
      quick_io_check(tmp_file, trace);
      // Remove data2
      trace.iftype(1);
      REQUIRE(trace.npts() == 12);
      REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
      REQUIRE(trace.data2().size() == 0);
      quick_io_check(tmp_file, trace);
    }
  }
}

TEST_CASE("Unsetting Trace") {
  Trace trace = gen_fake_trace();
  unset_trace(&trace);
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
  SECTION("Integers") {
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
    REQUIRE(trace.data1().size() == 0);
    REQUIRE(trace.data2().size() == 0);
  }
  SECTION("Convenience Functions") {
    REQUIRE(trace.frequency() == unset_double);
    trace.calc_geometry();
    REQUIRE(trace.gcarc() == unset_double);
    REQUIRE(trace.dist() == unset_double);
    REQUIRE(trace.az() == unset_double);
    REQUIRE(trace.baz() == unset_double);
  }
  fs::path tmp_dir{fs::temp_directory_path()};
  fs::path tmp_file{tmp_dir / "test.SaC"};
  trace.nvhdr(7);
  quick_io_check(tmp_file, trace);
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
    const fs::path tmp_dir2{fs::temp_directory_path() / "not_a_dir"};
    const fs::path tmp_file2{tmp_dir2 / "not_real.sac"};
    REQUIRE_THROWS_WITH(Trace(tmp_file2),
                        ContainsSubstring("cannot be opened to read."));
  }
  SECTION("Write Throw") {
    const fs::path tmp_dir2{fs::temp_directory_path() / "not_a_dir"};
    const fs::path tmp_file2{tmp_dir2 / "not_real.sac"};
    REQUIRE_THROWS_WITH(trace.write(tmp_file2),
                        ContainsSubstring("cannot be opened to write."));
  }
}

TEST_CASE("Corrupt SAC-File") {
  const fs::path tmp_dir{fs::temp_directory_path()};
  const fs::path tmp_file{tmp_dir / "corruption_test.sac"};
  SECTION("Header throw") {
    const std::string throw_str{"Insufficient filesize for header."};
    SECTION("10 Header Fields") {
      write_corrupt_sac(tmp_file, 10);
      REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
      fs::remove(tmp_file);
    }
    SECTION("0 Header Fields") {
      write_corrupt_sac(tmp_file, 0);
      REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
      fs::remove(tmp_file);
    }
    SECTION("75 Header Fields") {
      write_corrupt_sac(tmp_file, 75);
      REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
      fs::remove(tmp_file);
    }
    SECTION("76 Header Fields") {
      write_corrupt_sac(tmp_file, 76);
      REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
      fs::remove(tmp_file);
    }
    SECTION("data_word - 1 Header Fields") {
      write_corrupt_sac(tmp_file, data_word - 1);
      REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
      fs::remove(tmp_file);
    }
    SECTION("data_word Header Fields") {
      // 632 bytes written
      write_corrupt_sac(tmp_file, data_word);
      REQUIRE_NOTHROW(Trace(tmp_file));
      fs::remove(tmp_file);
    }
  }
  SECTION("Data1 throw") {
    std::string throw_str{"Insufficient filesize for data1."};
    SECTION("Fake_Npts 10, Real_Npts 0") {
      write_corrupt_sac(tmp_file, data_word, 10, 0);
      REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
      fs::remove(tmp_file);
    }
    SECTION("Fake_Npts 1, Real_Npts 0") {
      write_corrupt_sac(tmp_file, data_word, 1, 0);
      REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
      fs::remove(tmp_file);
    }
    SECTION("Fake_Npts 1000, Real_Npts 999") {
      write_corrupt_sac(tmp_file, data_word, 1000, 999);
      REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
      fs::remove(tmp_file);
    }
    SECTION("Fake_Npts 10, Real_Npts 10") {
      write_corrupt_sac(tmp_file, data_word, 10, 10);
      REQUIRE_NOTHROW(Trace(tmp_file));
      fs::remove(tmp_file);
    }
    SECTION("Fake_Npts 0, Real_Npts 0") {
      write_corrupt_sac(tmp_file, data_word, 0, 0);
      REQUIRE_NOTHROW(Trace(tmp_file));
      fs::remove(tmp_file);
    }
  }
  SECTION("Data2 throw") {
    std::string throw_str{"Insufficient filesize for data2."};
    SECTION("Fake_Npts 10, Real_Npts 0") {
      write_corrupt_sac(tmp_file, data_word, 10, 10, true, 0);
      REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
      fs::remove(tmp_file);
    }
    SECTION("Fake_Npts 1, Real_Npts 0") {
      write_corrupt_sac(tmp_file, data_word, 1, 1, true, 0);
      REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
      fs::remove(tmp_file);
    }
    SECTION("Fake_Npts 1000, Real_Npts 999") {
      write_corrupt_sac(tmp_file, data_word, 1000, 1000, true, 999);
      REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
      fs::remove(tmp_file);
    }
    SECTION("Fake_Npts 10, Real_Npts 10") {
      write_corrupt_sac(tmp_file, data_word, 10, 10, true, 10);
      REQUIRE_NOTHROW(Trace(tmp_file), ContainsSubstring(throw_str));
      fs::remove(tmp_file);
    }
    SECTION("Fake_npts 0, Real_Npts 0") {
      write_corrupt_sac(tmp_file, data_word, 0, 0, true, 0);
      REQUIRE_NOTHROW(Trace(tmp_file), ContainsSubstring(throw_str));
      fs::remove(tmp_file);
    }
  }
  SECTION("Footer throw") {
    const std::string throw_str{"Insufficient filesize for footer."};
    SECTION("Without Data2") {
      SECTION("Npts 0, Footer 0") {
        write_corrupt_sac(tmp_file, data_word, 0, 0, false, 0, 7, 0);
        REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
        fs::remove(tmp_file);
      }
      SECTION("Npts 10, Footer 0") {
        write_corrupt_sac(tmp_file, data_word, 10, 10, false, 0, 7, 0);
        REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
        fs::remove(tmp_file);
      }
      SECTION("Npts 0, Footer 10") {
        write_corrupt_sac(tmp_file, data_word, 0, 0, false, 0, 7, 10);
        REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
        fs::remove(tmp_file);
      }
      SECTION("Npts 10, Footer 10") {
        write_corrupt_sac(tmp_file, data_word, 10, 10, false, 0, 7, 10);
        REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
        fs::remove(tmp_file);
      }
      SECTION("Npts 10, Footer num_footer - 1") {
        write_corrupt_sac(tmp_file, data_word, 10, 10, false, 0, 7,
                          num_footer - 1);
        REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
        fs::remove(tmp_file);
      }
      SECTION("Npts 1000, Footer num_footer - 1") {
        write_corrupt_sac(tmp_file, data_word, 1000, 1000, false, 0, 7,
                          num_footer - 1);
        REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
        fs::remove(tmp_file);
      }
      SECTION("Npts 0, Footer num_footer") {
        write_corrupt_sac(tmp_file, data_word, 0, 0, false, 0, 7, num_footer);
        REQUIRE_NOTHROW(Trace(tmp_file));
        fs::remove(tmp_file);
      }
      SECTION("Npts 1000, Footer num_footer") {
        write_corrupt_sac(tmp_file, data_word, 1000, 1000, false, 0, 7,
                          num_footer);
        REQUIRE_NOTHROW(Trace(tmp_file));
        fs::remove(tmp_file);
      }
    }
    SECTION("With Data2") {
      SECTION("Npts 0, Footer 0") {
        write_corrupt_sac(tmp_file, data_word, 0, 0, true, 0, 7, 0);
        REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
        fs::remove(tmp_file);
      }
      SECTION("Npts 10, Footer 0") {
        write_corrupt_sac(tmp_file, data_word, 10, 10, true, 10, 7, 0);
        REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
        fs::remove(tmp_file);
      }
      SECTION("Npts 10, Footer 10") {
        write_corrupt_sac(tmp_file, data_word, 10, 10, true, 10, 7, 10);
        REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
        fs::remove(tmp_file);
      }
      SECTION("Npts 10, Footer num_footer - 1") {
        write_corrupt_sac(tmp_file, data_word, 10, 10, true, 10, 7,
                          num_footer - 1);
        REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
        fs::remove(tmp_file);
      }
      SECTION("Npts 1000, Footer num_footer - 1") {
        write_corrupt_sac(tmp_file, data_word, 1000, 1000, true, 1000, 7,
                          num_footer - 1);
        REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str));
        fs::remove(tmp_file);
      }
      SECTION("Npts 1, Footer num_footer") {
        write_corrupt_sac(tmp_file, data_word, 1, 1, true, 1, 7, num_footer);
        REQUIRE_NOTHROW(Trace(tmp_file));
        fs::remove(tmp_file);
      }
      SECTION("Npts 0, Footer num_footer") {
        write_corrupt_sac(tmp_file, data_word, 0, 0, true, 0, 7, num_footer);
        REQUIRE_NOTHROW(Trace(tmp_file));
        fs::remove(tmp_file);
      }
      SECTION("Npts 100, Footer num_footer") {
        write_corrupt_sac(tmp_file, data_word, 100, 100, true, 100, 7,
                          num_footer);
        REQUIRE_NOTHROW(Trace(tmp_file));
        fs::remove(tmp_file);
      }
    }
  }
  SECTION("Excess data throw") {
    const std::string throw_str1{"Filesize exceeds data specification with "};
    const std::string throw_str2{" bytes excess. Data corruption suspected."};
    SECTION("Without Footer") {
      SECTION("Without Data2") {
        SECTION("Npts 0, Excess data 1") {
          const int npts{0};
          write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 6, 0,
                            1);
          REQUIRE_THROWS_WITH(Trace(tmp_file),
                              ContainsSubstring(throw_str1) &&
                                  ContainsSubstring(throw_str2));
          fs::remove(tmp_file);
        }
        SECTION("Npts 10, Excess data 1") {
          const int npts{10};
          write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 6, 0,
                            1);
          REQUIRE_THROWS_WITH(Trace(tmp_file),
                              ContainsSubstring(throw_str1) &&
                                  ContainsSubstring(throw_str2));
          fs::remove(tmp_file);
        }
        SECTION("Npts 100, Excess data 1") {
          const int npts{100};
          write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 6, 0,
                            1);
          REQUIRE_THROWS_WITH(Trace(tmp_file),
                              ContainsSubstring(throw_str1) &&
                                  ContainsSubstring(throw_str2));
          fs::remove(tmp_file);
        }
        SECTION("Npts 0, Excess data 100") {
          const int npts{0};
          write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 6, 0,
                            100);
          REQUIRE_THROWS_WITH(Trace(tmp_file),
                              ContainsSubstring(throw_str1) &&
                                  ContainsSubstring(throw_str2));
          fs::remove(tmp_file);
        }
        SECTION("Npts 57, Excess data 0") {
          const int npts{57};
          write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 6, 0,
                            0);
          REQUIRE_NOTHROW(Trace(tmp_file));
          fs::remove(tmp_file);
        }
      }
      SECTION("With Data2") {
        SECTION("Npts 0, Excess data 1") {
          const int npts{0};
          write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 6, 0,
                            1);
          REQUIRE_THROWS_WITH(Trace(tmp_file),
                              ContainsSubstring(throw_str1) &&
                                  ContainsSubstring(throw_str2));
          fs::remove(tmp_file);
        }
        SECTION("Npts 100, Excess data 1") {
          const int npts{100};
          write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 6, 0,
                            1);
          REQUIRE_THROWS_WITH(Trace(tmp_file),
                              ContainsSubstring(throw_str1) &&
                                  ContainsSubstring(throw_str2));
          fs::remove(tmp_file);
        }
        SECTION("Npts 0, Excess data 1000") {
          const int npts{1000};
          write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 6, 0,
                            1);
          REQUIRE_THROWS_WITH(Trace(tmp_file),
                              ContainsSubstring(throw_str1) &&
                                  ContainsSubstring(throw_str2));
          fs::remove(tmp_file);
        }
        SECTION("Npts 100, Excess data 0") {
          const int npts{1000};
          write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 6, 0,
                            0);
          REQUIRE_NOTHROW(Trace(tmp_file));
          fs::remove(tmp_file);
        }
      }
    }
    SECTION("With Footer") {
      SECTION("Without Data2") {
        SECTION("Npts 0, Excess data 1") {
          const int npts{0};
          write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 7,
                            num_footer, 1);
          REQUIRE_THROWS_WITH(Trace(tmp_file),
                              ContainsSubstring(throw_str1) &&
                                  ContainsSubstring(throw_str2));
          fs::remove(tmp_file);
        }
        SECTION("Npts 100, Excess data 1") {
          const int npts{100};
          write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 7,
                            num_footer, 1);
          REQUIRE_THROWS_WITH(Trace(tmp_file),
                              ContainsSubstring(throw_str1) &&
                                  ContainsSubstring(throw_str2));
          fs::remove(tmp_file);
        }
        SECTION("Npts 100, Excess data 101") {
          const int npts{100};
          write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 7,
                            num_footer, 101);
          REQUIRE_THROWS_WITH(Trace(tmp_file),
                              ContainsSubstring(throw_str1) &&
                                  ContainsSubstring(throw_str2));
          fs::remove(tmp_file);
        }
        SECTION("Npts 10, Excess data 0") {
          const int npts{10};
          write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 7,
                            num_footer, 0);
          REQUIRE_NOTHROW(Trace(tmp_file));
          fs::remove(tmp_file);
        }
      }
      SECTION("With Data2") {
        SECTION("Npts 0, Excess data 1") {
          const int npts{0};
          write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 7,
                            num_footer, 1);
          REQUIRE_THROWS_WITH(Trace(tmp_file),
                              ContainsSubstring(throw_str1) &&
                                  ContainsSubstring(throw_str2));
          fs::remove(tmp_file);
        }
        SECTION("Npts 100, Excess data 1") {
          const int npts{100};
          write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 7,
                            num_footer, 1);
          REQUIRE_THROWS_WITH(Trace(tmp_file),
                              ContainsSubstring(throw_str1) &&
                                  ContainsSubstring(throw_str2));
          fs::remove(tmp_file);
        }
        SECTION("Npts 100, Excess data 100") {
          const int npts{100};
          write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 7,
                            num_footer, 100);
          REQUIRE_THROWS_WITH(Trace(tmp_file),
                              ContainsSubstring(throw_str1) &&
                                  ContainsSubstring(throw_str2));
          fs::remove(tmp_file);
        }
        SECTION("Npts 100, Excess data 0") {
          const int npts{100};
          write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 7,
                            num_footer, 0);
          REQUIRE_NOTHROW(Trace(tmp_file));
          fs::remove(tmp_file);
        }
      }
    }
  }
}

TEST_CASE("Geometric operations") {
  SECTION("Helper Functions") {
    SECTION("Limit 360 [0, 360.0]") {
      SECTION("No Adjustment") {
        REQUIRE(limit_360(0) == 0);
        REQUIRE(limit_360(-0) == 0);
        REQUIRE(limit_360(90) == 90);
        REQUIRE(limit_360(180) == 180);
        REQUIRE(limit_360(270) == 270);
        REQUIRE(limit_360(360) == 360);
      }
      SECTION("Negatives") {
        REQUIRE(limit_360(-180) == 180);
        REQUIRE(limit_360(-270) == 90);
        REQUIRE(limit_360(-90) == 270);
      }
      SECTION("Adjust By One Loop") {
        REQUIRE(limit_360(1 + circle_deg) == 1);
        REQUIRE(limit_360(-1 - circle_deg) == circle_deg - 1);
        REQUIRE(limit_360(90 + circle_deg) == 90);
        REQUIRE(limit_360(-90 - circle_deg) == 270);
        REQUIRE(limit_360(180 + circle_deg) == 180);
        REQUIRE(limit_360(-180 - circle_deg) == 180);
        REQUIRE(limit_360(270 + circle_deg) == 270);
        REQUIRE(limit_360(-270 - circle_deg) == 90);
        REQUIRE(limit_360(2 * circle_deg) == circle_deg);
        REQUIRE(limit_360(-2 * circle_deg) == 0);
      }
      SECTION("Adjust By Many Loops") {
        REQUIRE(limit_360(10 * circle_deg + 10) == 10);
        REQUIRE(limit_360(8 * circle_deg + 75) == 75);
        REQUIRE(limit_360(5 * circle_deg + 35) == 35);
        REQUIRE(limit_360(-7 * circle_deg - 10) == circle_deg - 10);
        REQUIRE(limit_360(-3 * circle_deg - 180) == 180);
      }
    }
    SECTION("Limit 180 [-180.0, 180.0]") {
      SECTION("No Adjustment") {
        REQUIRE(limit_180(0) == 0);
        REQUIRE(limit_180(90) == 90);
        REQUIRE(limit_180(180) == 180);
        REQUIRE(limit_180(-0) == 0);
        REQUIRE(limit_180(-10) == -10);
        REQUIRE(limit_180(-90) == -90);
        REQUIRE(limit_180(-179) == -179);
      }
      SECTION("Adjustment Positives") {
        REQUIRE(limit_180(360) == 0);
        REQUIRE(limit_180(270) == -90);
        REQUIRE(limit_180(181) == -179);
      }
      SECTION("Adjustment Negatives") {
        REQUIRE(limit_180(-180) == 180);
        REQUIRE(limit_180(-360) == 0);
        REQUIRE(limit_180(-270) == 90);
      }
      SECTION("Multiples of 360") {
        SECTION("Positive Multiples") {
          REQUIRE(limit_180(2 * circle_deg - 180) == 180);
          REQUIRE(limit_180(10 * circle_deg + 10) == 10);
          REQUIRE(limit_180(8 * circle_deg + 75) == 75);
          REQUIRE(limit_180(5 * circle_deg + 35) == 35);
        }
        SECTION("Negative Multiples") {
          REQUIRE(limit_180(-7 * circle_deg - 10) == -10);
          REQUIRE(limit_180(-3 * circle_deg - 180) == 180);
        }
      }
    }
    SECTION("Limit 90 [-90.0, 90.0]") {
      SECTION("No Adjustment") {
        REQUIRE(limit_90(0) == 0);
        REQUIRE(limit_90(90) == 90);
        REQUIRE(limit_90(45) == 45);
        REQUIRE(limit_90(-90) == -90);
        REQUIRE(limit_90(-45) == -45);
        REQUIRE(limit_90(-74) == -74);
      }
      SECTION("Adustment Positives") {
        REQUIRE(limit_90(180) == 0);
        REQUIRE(limit_90(91) == 89);
        REQUIRE(limit_90(135) == 45);
        REQUIRE(limit_90(360) == 0);
        REQUIRE(limit_90(270) == -90);
      }
      SECTION("Adjustment Negatives") {
        REQUIRE(limit_90(-180) == 0);
        REQUIRE(limit_90(-91) == -89);
        REQUIRE(limit_90(-135) == -45);
        REQUIRE(limit_90(-270) == 90);
        REQUIRE(limit_90(-360) == 0);
      }
    }
  }
  SECTION("Same location") {
    constexpr double tol{1e-4};
    SECTION("Lat: 0, Lon: 0") {
      constexpr double lat{0};
      constexpr double lon{0};
      const double est_gcarc{gcarc(lat, lon, lat, lon)};
      REQUIRE_THAT(est_gcarc, WithinAbs(0, tol));
      const double est_az{azimuth(lat, lon, lat, lon)};
      REQUIRE_THAT(est_az, WithinAbs(0, tol));
    }
    SECTION("Lat: 90, Lon: 0") {
      constexpr double lat{90};
      constexpr double lon{0};
      const double est_gcarc{gcarc(lat, lon, lat, lon)};
      REQUIRE_THAT(est_gcarc, WithinAbs(0, tol));
      const double est_az{azimuth(lat, lon, lat, lon)};
      REQUIRE_THAT(est_az, WithinAbs(0, tol));
    }
    SECTION("Lat: -90, Lon: 0") {
      constexpr double lat{-90};
      constexpr double lon{0};
      const double est_gcarc{gcarc(lat, lon, lat, lon)};
      REQUIRE_THAT(est_gcarc, WithinAbs(0, tol));
      const double est_az{azimuth(lat, lon, lat, lon)};
      REQUIRE_THAT(est_az, WithinAbs(0, tol));
    }
    SECTION("Lat: 0, Lon: 180") {
      constexpr double lat{0};
      constexpr double lon{180};
      const double est_gcarc{gcarc(lat, lon, lat, lon)};
      REQUIRE_THAT(est_gcarc, WithinAbs(0, tol));
      const double est_az{azimuth(lat, lon, lat, lon)};
      REQUIRE_THAT(est_az, WithinAbs(0, tol));
    }
    SECTION("Lat: 0, Lon: -180") {
      constexpr double lat{0};
      constexpr double lon{-180};
      const double est_gcarc{gcarc(lat, lon, lat, lon)};
      REQUIRE_THAT(est_gcarc, WithinAbs(0, tol));
      const double est_az{azimuth(lat, lon, lat, lon)};
      REQUIRE_THAT(est_az, WithinAbs(0, tol));
    }
  }
  SECTION("Easy Values") {
    constexpr double tol{1e-4};
    SECTION("Equator vs. Pole") {
      constexpr double eqla{0};
      constexpr double npla{90};
      constexpr double spla{-90};
      constexpr double lon{0};
      SECTION("North Pole") {
        const double est_gcarc{gcarc(eqla, lon, npla, lon)};
        REQUIRE_THAT(est_gcarc, WithinAbs(90, tol));
        const double est_az{azimuth(eqla, lon, npla, lon)};
        REQUIRE_THAT(est_az, WithinAbs(0, tol));
        const double est_baz{azimuth(npla, lon, eqla, lon)};
        REQUIRE_THAT(est_baz, WithinAbs(180, tol));
      }
      SECTION("South Pole") {
        const double est_gcarc{gcarc(eqla, lon, spla, lon)};
        REQUIRE_THAT(est_gcarc, WithinAbs(90, tol));
        const double est_az{azimuth(eqla, lon, spla, lon)};
        REQUIRE_THAT(est_az, WithinAbs(180, tol));
        const double est_baz{azimuth(spla, lon, eqla, lon)};
        REQUIRE_THAT(est_baz, WithinAbs(0, tol));
      }
      SECTION("North Pole to South Pole") {
        const double est_gcarc{gcarc(npla, lon, spla, lon)};
        REQUIRE_THAT(est_gcarc, WithinAbs(180, tol));
        const double est_az{azimuth(npla, lon, spla, lon)};
        REQUIRE_THAT(est_az, WithinAbs(180, tol));
        const double est_baz{azimuth(spla, lon, npla, lon)};
        REQUIRE_THAT(est_baz, WithinAbs(0, tol));
      }
      SECTION("South Pole to North Pole") {
        const double est_gcarc{gcarc(spla, lon, npla, lon)};
        REQUIRE_THAT(est_gcarc, WithinAbs(180, tol));
      }
    }
  }
  SECTION("Over Poles") {
    constexpr double tol{1e-4};
    SECTION("North Pole") {
      constexpr double lat{70};
      constexpr double lon1{0};
      constexpr double lon2{180};
      constexpr double exp_gcarc{2 * (90 - lat)};
      const double est_gcarc{gcarc(lat, lon1, lat, lon2)};
      REQUIRE_THAT(est_gcarc, WithinAbs(exp_gcarc, tol));
      const double est_az{azimuth(lat, lon1, lat, lon2)};
      REQUIRE_THAT(est_az, WithinAbs(0, tol));
      const double est_baz{azimuth(lat, lon2, lat, lon1)};
      REQUIRE_THAT(est_baz, WithinAbs(360, tol));
    }
    SECTION("South Pole") {
      constexpr double lat{-65};
      constexpr double lon1{90};
      constexpr double lon2{-90};
      constexpr double exp_gcarc{2 * (lat + 90)};
      const double est_gcarc{gcarc(lat, lon1, lat, lon2)};
      REQUIRE_THAT(est_gcarc, WithinAbs(exp_gcarc, tol));
      const double est_az{azimuth(lat, lon1, lat, lon2)};
      REQUIRE_THAT(est_az, WithinAbs(180, tol));
      const double est_baz{azimuth(lat, lon2, lat, lon1)};
      REQUIRE_THAT(est_baz, WithinAbs(180, tol));
    }
  }
  SECTION("Along Equator") {
    constexpr double tol{1e-4};
    constexpr double lat{0};
    double lon1{0};
    double lon2{90};
    double est_gcarc{gcarc(lat, lon1, lat, lon2)};
    REQUIRE_THAT(est_gcarc, WithinAbs(90, tol));
    double est_az{azimuth(lat, lon1, lat, lon2)};
    REQUIRE_THAT(est_az, WithinAbs(90, tol));
    double est_baz{azimuth(lat, lon2, lat, lon1)};
    REQUIRE_THAT(est_baz, WithinAbs(270, tol));
    lon1 = 30;
    lon2 = 70;
    est_gcarc = gcarc(lat, lon1, lat, lon2);
    REQUIRE_THAT(est_gcarc, WithinAbs(lon2 - lon1, tol));
    est_az = azimuth(lat, lon1, lat, lon2);
    REQUIRE_THAT(est_az, WithinAbs(90, tol));
    est_baz = azimuth(lat, lon2, lat, lon1);
    REQUIRE_THAT(est_baz, WithinAbs(270, tol));
    lon1 = 90;
    lon2 = -90;
    est_gcarc = gcarc(lat, lon1, lat, lon2);
    REQUIRE_THAT(est_gcarc, WithinAbs(lon1 - lon2, tol));
    est_az = azimuth(lat, lon1, lat, lon2);
    REQUIRE_THAT(est_az, WithinAbs(270, tol));
    est_baz = azimuth(lat, lon2, lat, lon1);
    REQUIRE_THAT(est_baz, WithinAbs(90, tol));
  }
  SECTION("Known Values from a Real SAC File") {
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
  SECTION("Trace geometry") {
    constexpr float gcarc{115.0889663696F};
    constexpr float az{33.3167991638F};
    constexpr float baz{185.6029968262F};
    constexpr float dist{12811.65234375F};
    Trace trace = gen_fake_trace();
    REQUIRE(trace.gcarc() != gcarc);
    REQUIRE(trace.az() != gcarc);
    REQUIRE(trace.baz() != baz);
    REQUIRE(trace.dist() != dist);
    trace.calc_geometry();
    REQUIRE_THAT(trace.gcarc(), WithinAbs(gcarc, 1e-4));
    REQUIRE_THAT(trace.az(), WithinAbs(az, 1e-4));
    REQUIRE_THAT(trace.baz(), WithinAbs(baz, 1e-4));
    REQUIRE_THAT(trace.dist(), WithinAbs(dist, 1e-4));
  }
  SECTION("Unset Trace geometry") {
    Trace trace{};
    REQUIRE(trace.stla() == unset_double);
    REQUIRE(trace.stlo() == unset_double);
    REQUIRE(trace.evla() == unset_double);
    REQUIRE(trace.evlo() == unset_double);
    trace.calc_geometry();
    REQUIRE(trace.gcarc() == unset_double);
    REQUIRE(trace.dist() == unset_double);
    REQUIRE(trace.az() == unset_double);
    REQUIRE(trace.baz() == unset_double);
  }
}

TEST_CASE("Dates and Times") {
  SECTION("Default Trace") {
    Trace trace{};
    const std::string date{trace.date()};
    REQUIRE(date == unset_word);
    const std::string time{trace.time()};
    REQUIRE(time == unset_word);
  }
  SECTION("Default Fake Trace") {
    Trace trace = gen_fake_trace();
    const std::string date{trace.date()};
    REQUIRE(date == "2023-123");
    const std::string time{trace.time()};
    REQUIRE(time == "13:57:34.0");
  }
}
// NOLINTEND(readability-magic-numbers)
