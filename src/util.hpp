// Copyright 2023 Alexander R. Blanchette

#ifndef SAC_FORMAT_UTIL_HPP_20231121_0727
#define SAC_FORMAT_UTIL_HPP_20231121_0727

#pragma once

/* Standard library */
#include <random>
// INT_MIN and INT_MAX
#include <limits.h>
#include <limits>
#include <string>
// ostringstream
#include <sstream>
#include <vector>
/* Xoshiro Random Number generator */
#include <XoshiroCpp.hpp>
/* My stuff */
#include <sac_format.hpp>

namespace sacfmt {

// Testing Constants
template <typename T> std::string eps_string(const T f) {
  std::ostringstream oss{};
  oss << std::setprecision(std::numeric_limits<T>::max_digits10) << f;
  return oss.str();
}
// Floats
// Negative
constexpr float lowest_f{std::numeric_limits<float>::lowest()};
constexpr float neg_epsilon_f{-std::numeric_limits<float>::epsilon()};
const std::string s_neg_epsilon_f{eps_string(neg_epsilon_f)};
// Positive
constexpr float highest_f{std::numeric_limits<float>::max()};
constexpr float epsilon_f{std::numeric_limits<float>::epsilon()};
const std::string s_epsilon_f{eps_string(epsilon_f)};
// Doubles
// Negative
constexpr double lowest{std::numeric_limits<double>::lowest()};
constexpr double neg_epsilon{-std::numeric_limits<double>::epsilon()};
const std::string s_neg_epsilon{eps_string(neg_epsilon)};
// Positive
constexpr double highest{std::numeric_limits<double>::max()};
constexpr double epsilon{std::numeric_limits<double>::epsilon()};
const std::string s_epsilon{eps_string(epsilon)};

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
      XoshiroCpp::SplitMix64{t1}(), XoshiroCpp::SplitMix64{rd()}(),
      XoshiroCpp::SplitMix64{t2}(), XoshiroCpp::SplitMix64{rd()}()};
  return XoshiroCpp::Xoshiro256Plus{initial_state};
}

// Build the PRNG
inline XoshiroCpp::Xoshiro256Plus xos{init()};

// Give a random double within the inclusive bounds [min, max]
inline double get(double min, double max) {
  std::uniform_real_distribution<> die{min, max};
  return die(xos);
}

void random_vector(std::vector<double> *data, const double minimum = -1.0,
                   const double maximum = 1.0) {
  std::uniform_real_distribution<> die{minimum, maximum};
  for (std::size_t i{0}; i < data->size(); ++i) {
    (*data)[i] = die(xos);
  }
}

Trace gen_fake_trace() {
  Trace sac{};
  sac.delta(0.025);
  sac.depmin(-1.11f);
  sac.depmax(1.23f);
  sac.odelta(0.026f);
  sac.b(0.1);
  sac.e(0.0);
  sac.a(5.78);
  sac.o(83.213);
  sac.t0(1.234);
  sac.t1(-1.234);
  sac.t2(2.345);
  sac.t3(-2.345);
  sac.t4(3.456);
  sac.t5(-3.456);
  sac.t6(4.567);
  sac.t7(-4.567);
  sac.t8(5.678);
  sac.t9(-5.678);
  sac.f(12345.0);
  sac.resp0(0.1f);
  sac.resp1(-0.1f);
  sac.resp2(1.0f);
  sac.resp3(-1.0f);
  sac.resp4(10.0f);
  sac.resp5(-10.0f);
  sac.resp6(100.0f);
  sac.resp7(-100.0f);
  sac.resp8(1000.0f);
  sac.resp9(-1000.0f);
  sac.stla(32.21);
  sac.stlo(121.38);
  sac.stel(100.0f);
  sac.stdp(50.0f);
  sac.evla(-81.35);
  sac.evlo(85.37);
  sac.evel(0.5f);
  sac.evdp(30.25f);
  sac.mag(1.83f);
  sac.user0(9.0f);
  sac.user1(-9.0f);
  sac.user2(8.0f);
  sac.user3(-8.0f);
  sac.user4(7.0f);
  sac.user5(-7.0f);
  sac.user6(6.0f);
  sac.user7(-6.0f);
  sac.user8(5.0f);
  sac.user9(-5.0f);
  sac.dist(1300.0f);
  sac.az(35.0f);
  sac.baz(-275.0f);
  sac.gcarc(13.135f);
  sac.sb(-35.0);
  sac.sdelta(0.125);
  sac.depmen(0.1f);
  sac.cmpaz(97.3f);
  sac.cmpinc(273.0f);
  sac.xminimum(-5.0f);
  sac.xmaximum(5.0f);
  sac.yminimum(-10.0f);
  sac.ymaximum(10.0f);
  sac.nzyear(2023);
  sac.nzjday(123);
  sac.nzhour(13);
  sac.nzmin(57);
  sac.nzsec(34);
  sac.nzmsec(0);
  sac.nvhdr(7);
  sac.norid(1);
  sac.nevid(2);
  sac.npts(100000);
  sac.nsnpts(50000);
  sac.nwfid(3);
  sac.nxsize(35);
  sac.nysize(15);
  sac.iftype(1);
  sac.idep(50);
  sac.iztype(12);
  sac.iinst(83);
  sac.istreg(82);
  sac.ievreg(81);
  sac.ievtyp(73);
  sac.iqual(45);
  sac.isynth(1);
  sac.imagtyp(57);
  sac.imagsrc(70);
  sac.ibody(99);
  sac.leven(true);
  sac.lpspol(true);
  sac.lovrok(true);
  sac.lcalda(true);
  sac.kstnm("Test1");
  sac.kevnm("Fake Quake");
  sac.khole("AB");
  sac.ko("Hi");
  sac.ka("Yup");
  sac.kt0("Zero 0");
  sac.kt1("One 1");
  sac.kt2("Two 2");
  sac.kt3("Three 3");
  sac.kt4("Four 4");
  sac.kt5("Five 5");
  sac.kt6("Six 6");
  sac.kt7("Seven 7");
  sac.kt8("Eight 8");
  sac.kt9("Nine 9");
  sac.kf("Fini");
  sac.kuser0("User0 Ze");
  sac.kuser1("User1 On");
  sac.kuser2("User2 Tw");
  sac.kcmpnm("HHZ");
  sac.knetwk("ZZ");
  sac.kdatrd("None");
  sac.kinst("Not Real");
  if (sac.npts() > 0) {
    std::vector<double> data{};
    data.resize(sac.npts(), 0.0);
    sac.data1(data);
    if (sac.leven() == false || sac.iftype() > 1) {
      data.resize(sac.npts(), 0.0);
      sac.data2(data);
    }
  }
  return sac;
}
} // namespace sacfmt

#endif
