// Copyright 2023 Alexander R. Blanchette

// sac-format
#include "sac-format/sac_format.hpp"
#include "sac-format/util.hpp"
// Standard Library
//   https://en.cppreference.com/w/cpp/standard_library
// Catch2 https://github.com/catchorg/Catch2/tree/v3.4.0
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_MAIN
// testing macros
// TEST_CASE, REQUIRE
#include <catch2/catch_test_macros.hpp>

namespace sacfmt {
void quick_io_check(fs::path tmp_file, const Trace &trace) {
  // Write/load and check
  trace.write(tmp_file);
  Trace in = Trace(tmp_file);
  fs::remove(tmp_file);
  REQUIRE(in == trace);
}

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
// NOLINTBEGIN(readability-magic-numbers)
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

TEST_CASE("Convenience Functions") {
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
// NOLINTEND(readability-magic-numbers)
}  // namespace sacfmt
