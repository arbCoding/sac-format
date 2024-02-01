// Copyright 2023-2024 Alexander R. Blanchette

// sac-format
#include "sac-format/sac_format.hpp"
#include "sac-format/util.hpp"
// Standard Library
//   https://en.cppreference.com/w/cpp/standard_library
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

using Catch::Matchers::ContainsSubstring;
using Catch::Matchers::WithinAbs;
namespace sacfmt {
void quick_io_check(const fs::path &tmp_file, const Trace &trace) {
  // Write/load and check
  trace.write(tmp_file);
  Trace input{tmp_file};
  fs::remove(tmp_file);
  REQUIRE(input == trace);
}  // namespace sacfmt

// NOLINTBEGIN(readability-magic-numbers,
// readability-function-cognitive-complexity)
TEST_CASE("Trace: Default: Floats") {
  const Trace trace{};
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

TEST_CASE("Trace: Default: Doubles") {
  const Trace trace{};
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

TEST_CASE("Trace: Default: Ints") {
  const Trace trace{};
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

TEST_CASE("Trace: Default: Bools") {
  const Trace trace{};
  REQUIRE(trace.leven() == unset_bool);
  REQUIRE(trace.lpspol() == unset_bool);
  REQUIRE(trace.lovrok() == unset_bool);
  REQUIRE(trace.lcalda() == unset_bool);
}

TEST_CASE("Trace: Default: Strings") {
  const Trace trace{};
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

TEST_CASE("Trace: Default: Data") {
  const Trace trace{};
  REQUIRE(trace.data1().empty());
  REQUIRE(trace.data2().empty());
}

TEST_CASE("Trace: Setters: Floats") {
  Trace trace{};
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

TEST_CASE("Trace: Setters: Doubles") {
  Trace trace{};
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

  value = -59;
  trace.evlo(value);
  REQUIRE(trace.evlo() == value);

  value = 500;
  trace.sb(value);
  REQUIRE(trace.sb() == value);

  value = -5000;
  trace.sdelta(value);
  REQUIRE(trace.sdelta() == value);
}

TEST_CASE("Trace: Setters: Ints") {
  Trace trace{};
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

TEST_CASE("Trace: Setters: Bools") {
  Trace trace{};
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

TEST_CASE("Trace: Setters: String") {
  Trace trace{};
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

TEST_CASE("Trace: Setters: Data") {
  Trace trace{};
  std::vector<double> value{};
  value.resize(20);
  random_vector(&value);
  trace.data1(value);
  REQUIRE(trace.data1() == value);

  random_vector(&value);
  trace.data2(value);
  REQUIRE(trace.data2() == value);
}

TEST_CASE("Trace: Convenience Functions") {
  Trace trace{};
  trace.delta(0.025);
  REQUIRE(trace.frequency() == 40);
}

TEST_CASE("Trace: Equality: Default") {
  const Trace trace1{};
  const Trace trace2{};
  REQUIRE(trace1 == trace2);
  REQUIRE(trace1.frequency() == trace2.frequency());
}

TEST_CASE("Trace: Equality: Inequality") {
  const Trace trace1{};
  Trace trace2{};
  trace2.kinst("Hello");
  REQUIRE(trace1 != trace2);
}

TEST_CASE("Trace: Equality: Inequality: Data2") {
  Trace trace1{gen_fake_trace()};
  if (trace1.data2().empty()) {
    trace1.leven(false);
  }
  Trace trace2{trace1};
  std::vector<double> new_data2{trace2.data2()};
  new_data2[0] += 10.0;
  trace2.data2(new_data2);
  REQUIRE(trace1 != trace2);
}

// Constants for Trace I/O
const fs::path tmp_dir{fs::temp_directory_path()};
const fs::path tmp_file{tmp_dir / "test.sac"};

TEST_CASE("Trace: I/O: Default Construction: Float") {
  Trace trace = gen_fake_trace();
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
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

TEST_CASE("Trace: I/O: Default Construction: Doubles") {
  Trace trace = gen_fake_trace();
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
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

TEST_CASE("Trace: I/O: Default Construction: Integers") {
  Trace trace = gen_fake_trace();
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
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

TEST_CASE("Trace: I/O: Default Construction: Booleans") {
  Trace trace = gen_fake_trace();
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
  REQUIRE(trace.leven() == in.leven());
  REQUIRE(trace.lpspol() == in.lpspol());
  REQUIRE(trace.lovrok() == in.lovrok());
  REQUIRE(trace.lcalda() == in.lcalda());
}

TEST_CASE("Trace: I/O: Default Construction: Strings") {
  Trace trace = gen_fake_trace();
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
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

TEST_CASE("Trace: I/O: Default Construction: Data Vectors") {
  Trace trace = gen_fake_trace();
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
  REQUIRE(equal_within_tolerance(trace.data1(), in.data1()));
  REQUIRE(equal_within_tolerance(trace.data2(), in.data2()));
}

TEST_CASE("Trace: I/O: Default Construction: Everything") {
  Trace trace = gen_fake_trace();
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
  REQUIRE(trace == in);
}

TEST_CASE("Trace: I/O: Random Data Vectors: Floats") {
  Trace trace = gen_fake_trace();
  std::vector data{trace.data1()};
  random_vector(&data);
  trace.data1(data);
  data = trace.data2();
  if (!data.empty()) {
    random_vector(&data);
    trace.data2(data);
  }
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
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

TEST_CASE("Trace: I/O: Random Data Vectors: Doubles") {
  Trace trace = gen_fake_trace();
  std::vector data{trace.data1()};
  random_vector(&data);
  trace.data1(data);
  data = trace.data2();
  if (!data.empty()) {
    random_vector(&data);
    trace.data2(data);
  }
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
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

TEST_CASE("Trace: I/O: Random Data Vectors: Integers") {
  Trace trace = gen_fake_trace();
  std::vector data{trace.data1()};
  random_vector(&data);
  trace.data1(data);
  data = trace.data2();
  if (!data.empty()) {
    random_vector(&data);
    trace.data2(data);
  }
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
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

TEST_CASE("Trace: I/O: Random Data Vectors: Booleans") {
  Trace trace = gen_fake_trace();
  std::vector data{trace.data1()};
  random_vector(&data);
  trace.data1(data);
  data = trace.data2();
  if (!data.empty()) {
    random_vector(&data);
    trace.data2(data);
  }
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
  REQUIRE(trace.leven() == in.leven());
  REQUIRE(trace.lpspol() == in.lpspol());
  REQUIRE(trace.lovrok() == in.lovrok());
  REQUIRE(trace.lcalda() == in.lcalda());
}

TEST_CASE("Trace: I/O: Random Data Vectors: Strings") {
  Trace trace = gen_fake_trace();
  std::vector data{trace.data1()};
  random_vector(&data);
  trace.data1(data);
  data = trace.data2();
  if (!data.empty()) {
    random_vector(&data);
    trace.data2(data);
  }
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
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

TEST_CASE("Trace: I/O: Random Data Vectors: Data Vectors") {
  Trace trace = gen_fake_trace();
  std::vector data{trace.data1()};
  random_vector(&data);
  trace.data1(data);
  data = trace.data2();
  if (!data.empty()) {
    random_vector(&data);
    trace.data2(data);
  }
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
  REQUIRE(equal_within_tolerance(trace.data1(), in.data1()));
  REQUIRE(equal_within_tolerance(trace.data2(), in.data2()));
}

TEST_CASE("Trace: I/O: Random Data Vectors: Everything") {
  Trace trace = gen_fake_trace();
  std::vector data{trace.data1()};
  random_vector(&data);
  trace.data1(data);
  data = trace.data2();
  if (!data.empty()) {
    random_vector(&data);
    trace.data2(data);
  }
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
  REQUIRE(trace == in);
}

TEST_CASE("Trace: I/O: Random Data Vectors: Resize Data") {
  Trace trace = gen_fake_trace();
  std::vector data{trace.data1()};
  random_vector(&data);
  trace.data1(data);
  data = trace.data2();
  if (!data.empty()) {
    random_vector(&data);
    trace.data2(data);
  }
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
  std::vector<double> tmp{in.data1()};
  tmp.resize(10);
  in.data1(tmp);
  REQUIRE(!equal_within_tolerance(trace.data1(), in.data1()));
}

TEST_CASE("Trace: I/O: Random Data Vectors: Modify Header: EvDp") {
  Trace trace = gen_fake_trace();
  std::vector data{trace.data1()};
  random_vector(&data);
  trace.data1(data);
  data = trace.data2();
  if (!data.empty()) {
    random_vector(&data);
    trace.data2(data);
  }
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
  in.evdp(in.evdp() - 1.0f);
  REQUIRE(trace != in);
}

TEST_CASE("Trace: I/O: Random Data Vectors: Modify Data") {
  Trace trace = gen_fake_trace();
  std::vector data{trace.data1()};
  random_vector(&data);
  trace.data1(data);
  data = trace.data2();
  if (!data.empty()) {
    random_vector(&data);
    trace.data2(data);
  }
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
  std::vector<double> tmp{in.data1()};
  tmp[0] -= 1.0;
  in.data1(tmp);
  REQUIRE(trace != in);
}

TEST_CASE("Trace: I/O: Legacy Format: nVHdr Conversions") {
  Trace trace = gen_fake_trace();
  trace.nvhdr(6);
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
  REQUIRE(in.nvhdr() == 7);
  // Make sure we didn't change the original trace
  REQUIRE(trace.nvhdr() == 6);
}

TEST_CASE("Trace: I/O: Legacy Format: Floats") {
  Trace trace = gen_fake_trace();
  trace.nvhdr(6);
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
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

TEST_CASE("Trace: I/O: Legacy Format: Doubles") {
  Trace trace = gen_fake_trace();
  trace.nvhdr(6);
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
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

TEST_CASE("Trace: I/O: Legacy Format: Integers") {
  Trace trace = gen_fake_trace();
  trace.nvhdr(6);
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
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

TEST_CASE("Trace: I/O: Legacy Format: Booleans") {
  Trace trace = gen_fake_trace();
  trace.nvhdr(6);
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
  REQUIRE(trace.leven() == in.leven());
  REQUIRE(trace.lpspol() == in.lpspol());
  REQUIRE(trace.lovrok() == in.lovrok());
  REQUIRE(trace.lcalda() == in.lcalda());
}

TEST_CASE("Trace: I/O: Legacy Format: Strings") {
  Trace trace = gen_fake_trace();
  trace.nvhdr(6);
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
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

TEST_CASE("Trace: I/O: Legacy Format: Data") {
  Trace trace = gen_fake_trace();
  trace.nvhdr(6);
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
  REQUIRE(equal_within_tolerance(trace.data1(), in.data1()));
  REQUIRE(equal_within_tolerance(trace.data2(), in.data2()));
}

TEST_CASE("Trace: I/O: Legacy Format: Everything") {
  Trace trace = gen_fake_trace();
  trace.nvhdr(6);
  trace.write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
  REQUIRE(trace != in);
}

TEST_CASE("Trace: Linked Headers: lEven") {
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

TEST_CASE("Trace: Linked Headers: iFType") {
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

TEST_CASE("Trace: Linked Headers: nPts: No Data2") {
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
  trace.npts(10);
  REQUIRE(trace.npts() == 10);
  REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
  REQUIRE(trace.data2().size() == 0);
  quick_io_check(tmp_file, trace);
  trace.npts(10);
  trace.npts(5);
  REQUIRE(trace.npts() == 5);
  REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
  REQUIRE(trace.data2().size() == 0);
  quick_io_check(tmp_file, trace);
}

TEST_CASE("Trace: Linked Headers: nPts: Data2: Uneven Timeseries") {
  Trace trace{};
  trace.leven(false);
  trace.iftype(1);
  trace.npts(0);
  trace.nvhdr(7);
  REQUIRE(trace.npts() == 0);
  REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
  REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
  quick_io_check(tmp_file, trace);
  trace.npts(10);
  REQUIRE(trace.npts() == 10);
  REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
  REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
  quick_io_check(tmp_file, trace);
  trace.npts(10);
  trace.npts(5);
  REQUIRE(trace.npts() == 5);
  REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
  REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
  quick_io_check(tmp_file, trace);
}

TEST_CASE("Trace: Linked Headers: nPts: Data2: Even Spectral") {
  Trace trace{};
  trace.leven(true);
  trace.iftype(2);
  trace.npts(0);
  trace.nvhdr(7);
  REQUIRE(trace.npts() == 0);
  REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
  REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
  quick_io_check(tmp_file, trace);
  trace.npts(10);
  REQUIRE(trace.npts() == 10);
  REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
  REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
  quick_io_check(tmp_file, trace);
  trace.npts(10);
  trace.npts(5);
  REQUIRE(trace.npts() == 5);
  REQUIRE(trace.data1().size() == static_cast<size_t>(trace.npts()));
  REQUIRE(trace.data2().size() == static_cast<size_t>(trace.npts()));
  quick_io_check(tmp_file, trace);
}

TEST_CASE("Trace: Linked Headers: Data1: Even Timeseries") {
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

TEST_CASE("Trace: Linked Headers: Data1: Uneven Timeseries") {
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

TEST_CASE("Trace: Linked Headers: Data1: Spectral") {
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

TEST_CASE("Trace: Linked Headers: Data2: Even Timeseries") {
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

TEST_CASE("Trace: Linked Headers: Data2: Uneven Timeseries") {
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

TEST_CASE("Trace: Linked Headers: Data2: Spectral") {
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

TEST_CASE("Trace: Unsetting: Floats") {
  Trace trace = gen_fake_trace();
  unset_trace(&trace);
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

TEST_CASE("Trace: Unsetting: Doubles") {
  Trace trace = gen_fake_trace();
  unset_trace(&trace);
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

TEST_CASE("Trace: Unsetting: Integers") {
  Trace trace = gen_fake_trace();
  unset_trace(&trace);
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

TEST_CASE("Trace: Unsetting: Booleans") {
  Trace trace = gen_fake_trace();
  unset_trace(&trace);
  REQUIRE(trace.leven() == unset_bool);
  REQUIRE(trace.lpspol() == unset_bool);
  REQUIRE(trace.lovrok() == unset_bool);
  REQUIRE(trace.lcalda() == unset_bool);
}

TEST_CASE("Trace: Unsetting: Strings") {
  Trace trace = gen_fake_trace();
  unset_trace(&trace);
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

TEST_CASE("Trace: Unsetting: Data Vectors") {
  Trace trace = gen_fake_trace();
  unset_trace(&trace);
  REQUIRE(trace.data1().size() == 0);
  REQUIRE(trace.data2().size() == 0);
}

TEST_CASE("Trace: Unsetting: Convenience Functions") {
  Trace trace = gen_fake_trace();
  unset_trace(&trace);
  REQUIRE(trace.frequency() == unset_double);
  trace.calc_geometry();
  REQUIRE(trace.gcarc() == unset_double);
  REQUIRE(trace.dist() == unset_double);
  REQUIRE(trace.az() == unset_double);
  REQUIRE(trace.baz() == unset_double);
}

TEST_CASE("Trace: Unsetting: I/O") {
  Trace trace = gen_fake_trace();
  unset_trace(&trace);
  trace.nvhdr(7);
  quick_io_check(tmp_file, trace);
}

TEST_CASE("Trace: I/O: Legacy Write") {
  Trace trace = gen_fake_trace();
  trace.nvhdr(7);
  trace.legacy_write(tmp_file);
  Trace in{tmp_file};
  fs::remove(tmp_file);
  REQUIRE(in.nvhdr() == 6);
  // Make sure we didn't change the original trace
  REQUIRE(trace.nvhdr() == 7);
  REQUIRE(trace != in);
}

TEST_CASE("Trace: I/O: Execptions: Bad Read Path") {
  const fs::path fail_dir{fs::temp_directory_path() / "not_a_dir"};
  const fs::path fail_file{fail_dir / "not_real.sac"};
  REQUIRE_THROWS_WITH(Trace(fail_file),
                      ContainsSubstring("cannot be opened to read."));
}

TEST_CASE("Trace: I/O: Execptions: Bad Write Path") {
  Trace trace = gen_fake_trace();
  trace.nvhdr(7);
  const fs::path fail_dir{fs::temp_directory_path() / "not_a_dir"};
  const fs::path fail_file{fail_dir / "not_real.sac"};
  REQUIRE_THROWS_WITH(trace.write(fail_file),
                      ContainsSubstring("cannot be opened to write."));
}

const std::string throw_hdr_str{"Insufficient filesize for header."};
TEST_CASE("Trace: I/O: Corrupt SAC-File: Header Throw: 10 Header Fields") {
  write_corrupt_sac(tmp_file, 10);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_hdr_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Header Throw: 0 Header Fields") {
  write_corrupt_sac(tmp_file, 0);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_hdr_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Header Throw: 75 Header Fields") {
  write_corrupt_sac(tmp_file, 75);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_hdr_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Header Throw: 76 Header Fields") {
  write_corrupt_sac(tmp_file, 76);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_hdr_str));
  fs::remove(tmp_file);
}

TEST_CASE(
    "Trace: I/O: Corrupt SAC-File: Header Throw: data_word - 1 Header Fields") {
  write_corrupt_sac(tmp_file, data_word - 1);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_hdr_str));
  fs::remove(tmp_file);
}

TEST_CASE(
    "Trace: I/O: Corrupt SAC-File: Header Throw: data_word Header Fields") {
  // 632 bytes written
  write_corrupt_sac(tmp_file, data_word);
  REQUIRE_NOTHROW(Trace(tmp_file));
  fs::remove(tmp_file);
}

const std::string throw_data1_str{"Insufficient filesize for data1."};
TEST_CASE(
    "Trace: I/O: Corrupt SAC-File: Data1 Throw: Fake_nPts 10, Real_nPts 0") {
  write_corrupt_sac(tmp_file, data_word, 10, 0);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_data1_str));
  fs::remove(tmp_file);
}

TEST_CASE(
    "Trace: I/O: Corrupt SAC-File: Data1 Throw: Fake_nPts 1, Real_nPts 0") {
  write_corrupt_sac(tmp_file, data_word, 1, 0);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_data1_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Data1 Throw: Fake_nPts 1000, "
          "Real_nPts 999") {
  write_corrupt_sac(tmp_file, data_word, 1000, 999);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_data1_str));
  fs::remove(tmp_file);
}

TEST_CASE(
    "Trace: I/O: Corrupt SAC-File: Data1 Throw: Fake_nPts 10, Real_nPts 10") {
  write_corrupt_sac(tmp_file, data_word, 10, 10);
  REQUIRE_NOTHROW(Trace(tmp_file));
  fs::remove(tmp_file);
}

TEST_CASE(
    "Trace: I/O: Corrupt SAC-File: Data1 Throw: Fake_nPts 0, Real_nPts 0") {
  write_corrupt_sac(tmp_file, data_word, 0, 0);
  REQUIRE_NOTHROW(Trace(tmp_file));
  fs::remove(tmp_file);
}

const std::string throw_data2_str{"Insufficient filesize for data2."};
TEST_CASE(
    "Trace: I/O: Corrupt SAC-File: Data2 Throw: Fake_nPts 10, Real_nPts 0") {
  write_corrupt_sac(tmp_file, data_word, 10, 10, true, 0);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_data2_str));
  fs::remove(tmp_file);
}

TEST_CASE(
    "Trace: I/O: Corrupt SAC-File: Data2 Throw: Fake_nPts 1, Real_nPts 0") {
  write_corrupt_sac(tmp_file, data_word, 1, 1, true, 0);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_data2_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Data2 Throw: Fake_nPts 1000, "
          "Real_nPts 999") {
  write_corrupt_sac(tmp_file, data_word, 1000, 1000, true, 999);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_data2_str));
  fs::remove(tmp_file);
}

TEST_CASE(
    "Trace: I/O: Corrupt SAC-File: Data2 Throw: Fake_nPts 10, Real_nPts 10") {
  write_corrupt_sac(tmp_file, data_word, 10, 10, true, 10);
  REQUIRE_NOTHROW(Trace(tmp_file), ContainsSubstring(throw_data2_str));
  fs::remove(tmp_file);
}

TEST_CASE(
    "Trace: I/O: Corrupt SAC-File: Data2 Throw: Fake_nPts 0, Real_nPts 0") {
  write_corrupt_sac(tmp_file, data_word, 0, 0, true, 0);
  REQUIRE_NOTHROW(Trace(tmp_file), ContainsSubstring(throw_data2_str));
  fs::remove(tmp_file);
}

const std::string throw_footer_str{"Insufficient filesize for footer."};
TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: Without Data2: Npts 0, "
          "Footer 0") {
  write_corrupt_sac(tmp_file, data_word, 0, 0, false, 0, 7, 0);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_footer_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: Without Data2: Npts 10, "
          "Footer 0") {
  write_corrupt_sac(tmp_file, data_word, 10, 10, false, 0, 7, 0);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_footer_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: Without Data2: Npts 0, "
          "Footer 10") {
  write_corrupt_sac(tmp_file, data_word, 0, 0, false, 0, 7, 10);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_footer_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: Without Data2: Npts 10, "
          "Footer 10") {
  write_corrupt_sac(tmp_file, data_word, 10, 10, false, 0, 7, 10);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_footer_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: Without Data2: Npts 10, "
          "Footer num_footer - 1") {
  write_corrupt_sac(tmp_file, data_word, 10, 10, false, 0, 7, num_footer - 1);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_footer_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: Without Data2: Npts "
          "1000, Footer num_footer - 1") {
  write_corrupt_sac(tmp_file, data_word, 1000, 1000, false, 0, 7,
                    num_footer - 1);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_footer_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: Without Data2: Npts 0, "
          "Footer num_footer") {
  write_corrupt_sac(tmp_file, data_word, 0, 0, false, 0, 7, num_footer);
  REQUIRE_NOTHROW(Trace(tmp_file));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: Without Data2: Npts "
          "1000, Footer num_footer") {
  write_corrupt_sac(tmp_file, data_word, 1000, 1000, false, 0, 7, num_footer);
  REQUIRE_NOTHROW(Trace(tmp_file));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: With Data2: Npts 0, "
          "Footer 0") {
  write_corrupt_sac(tmp_file, data_word, 0, 0, true, 0, 7, 0);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_footer_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: With Data2: Npts 10, "
          "Footer 0") {
  write_corrupt_sac(tmp_file, data_word, 10, 10, true, 10, 7, 0);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_footer_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: With Data2: Npts 10, "
          "Footer 10") {
  write_corrupt_sac(tmp_file, data_word, 10, 10, true, 10, 7, 10);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_footer_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: With Data2: Npts 10, "
          "Footer num_footer -1") {
  write_corrupt_sac(tmp_file, data_word, 10, 10, true, 10, 7, num_footer - 1);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_footer_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: With Data2: Npts 1000, "
          "Footer num_footer - 1") {
  write_corrupt_sac(tmp_file, data_word, 1000, 1000, true, 1000, 7,
                    num_footer - 1);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_footer_str));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: With Data2: Npts 1, "
          "Footer num_footer") {
  write_corrupt_sac(tmp_file, data_word, 1, 1, true, 1, 7, num_footer);
  REQUIRE_NOTHROW(Trace(tmp_file));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: With Data2: Npts 0, "
          "Footer num_footer") {
  write_corrupt_sac(tmp_file, data_word, 0, 0, true, 0, 7, num_footer);
  REQUIRE_NOTHROW(Trace(tmp_file));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Footer Throw: With Data2: Npts 100, "
          "Footer num_footers") {
  write_corrupt_sac(tmp_file, data_word, 100, 100, true, 100, 7, num_footer);
  REQUIRE_NOTHROW(Trace(tmp_file));
  fs::remove(tmp_file);
}

const std::string throw_str1{"Filesize exceeds data specification with "};
const std::string throw_str2{" bytes excess. Data corruption suspected."};
TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: Without Data2: "
          "Without Footer: Npts 0, Excess Npts 1") {
  const int npts{0};
  write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 6, 0, 1);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str1) &&
                                           ContainsSubstring(throw_str2));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: Without Data2: "
          "Without Footer: Npts 10, Excess Npts 1") {
  const int npts{10};
  write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 6, 0, 1);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str1) &&
                                           ContainsSubstring(throw_str2));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: Without Data2: "
          "Without Footer: Npts 100, Excess Npts 1") {
  const int npts{100};
  write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 6, 0, 1);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str1) &&
                                           ContainsSubstring(throw_str2));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: Without Data2: "
          "Without Footer: Npts 0, Excess Npts 100") {
  const int npts{0};
  write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 6, 0, 100);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str1) &&
                                           ContainsSubstring(throw_str2));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: Without Data2: "
          "Without Footer: Npts 57, Excess Npts 0") {
  const int npts{57};
  write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 6, 0, 0);
  REQUIRE_NOTHROW(Trace(tmp_file));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: With Data2: "
          "Without Footer: Npts 0, Excess Npts 1") {
  const int npts{0};
  write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 6, 0, 1);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str1) &&
                                           ContainsSubstring(throw_str2));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: With Data2: "
          "Without Footer: Npts 100, Excess Npts 1") {
  const int npts{100};
  write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 6, 0, 1);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str1) &&
                                           ContainsSubstring(throw_str2));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: With Data2: "
          "Without Footer: Npts 1000, Excess Npts 1") {
  const int npts{1000};
  write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 6, 0, 1);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str1) &&
                                           ContainsSubstring(throw_str2));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: With Data2: "
          "Without Footer: Npts 100, Excess Npts 0") {
  const int npts{1000};
  write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 6, 0, 0);
  REQUIRE_NOTHROW(Trace(tmp_file));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: Without Data2: "
          "With Footer: Npts 0, Excess Npts 1") {
  const int npts{0};
  write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 7, num_footer,
                    1);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str1) &&
                                           ContainsSubstring(throw_str2));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: Without Data2: "
          "With Footer: Npts 100, Excess Npts 1") {
  const int npts{100};
  write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 7, num_footer,
                    1);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str1) &&
                                           ContainsSubstring(throw_str2));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: Without Data2: "
          "With Footer: Npts 100, Excess Npts 101") {
  const int npts{100};
  write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 7, num_footer,
                    101);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str1) &&
                                           ContainsSubstring(throw_str2));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: Without Data2: "
          "With Footer: Npts 10, Excess Npts 0") {
  const int npts{10};
  write_corrupt_sac(tmp_file, data_word, npts, npts, false, npts, 7, num_footer,
                    0);
  REQUIRE_NOTHROW(Trace(tmp_file));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: With Data2: With "
          "Footer: Npts 0, Excess Npts 1") {
  const int npts{0};
  write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 7, num_footer,
                    1);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str1) &&
                                           ContainsSubstring(throw_str2));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: With Data2: With "
          "Footer: Npts 100, Excess Npts 1") {
  const int npts{100};
  write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 7, num_footer,
                    1);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str1) &&
                                           ContainsSubstring(throw_str2));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: With Data2: With "
          "Footer: Npts 100, Excess Npts 100") {
  const int npts{100};
  write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 7, num_footer,
                    100);
  REQUIRE_THROWS_WITH(Trace(tmp_file), ContainsSubstring(throw_str1) &&
                                           ContainsSubstring(throw_str2));
  fs::remove(tmp_file);
}

TEST_CASE("Trace: I/O: Corrupt SAC-File: Excess Data Throw: With Data2: With "
          "Footer: Npts 100, Excess Npts 0") {
  const int npts{100};
  write_corrupt_sac(tmp_file, data_word, npts, npts, true, npts, 7, num_footer,
                    0);
  REQUIRE_NOTHROW(Trace(tmp_file));
  fs::remove(tmp_file);
}
// NOLINTEND(readability-magic-numbers,
// readability-function-cognitive-complexity)
}  // namespace sacfmt
