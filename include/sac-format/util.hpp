// Copyright 2023 Alexander R. Blanchette

#ifndef SAC_FORMAT_UTIL_HPP_20231121_0727
#define SAC_FORMAT_UTIL_HPP_20231121_0727
#pragma once

// sac-format
#include "sac-format/sac_format.hpp"
// Xoshiro Random Number generator
//   https://github.com/Reputeless/Xoshiro-cpp
#include <XoshiroCpp.hpp>
// Standard Library
//   https://en.cppreference.com/w/cpp/standard_library
// std::chrono::steady_clock::now()
#include <chrono>
// std::filesystem::path
#include <filesystem>
// std::ofstream, std::ios::binary, std::ios:trunc
#include <fstream>
// std::setprecision
#include <iomanip>
// std::cout
#include <iostream>
// std::numeric_limits
#include <limits>
// std::random_device, std::uniform_real_distribution
#include <random>
// std::string
#include <string>
// std::vector
#include <vector>

namespace sacfmt {
namespace fs = std::filesystem;

// Testing Constants
template <typename T> std::string eps_string(const T value) noexcept {
  std::ostringstream oss{};
  oss << std::setprecision(std::numeric_limits<T>::max_digits10) << value;
  return oss.str();
}
// Ints
constexpr int lowest_i{std::numeric_limits<int>::lowest()};
constexpr int highest_i{std::numeric_limits<int>::max()};
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
constexpr double lowest_d{std::numeric_limits<double>::lowest()};
constexpr double neg_epsilon_d{-std::numeric_limits<double>::epsilon()};
const std::string s_neg_epsilon_d{eps_string(neg_epsilon_d)};
// Positive
constexpr double highest_d{std::numeric_limits<double>::max()};
constexpr double epsilon_d{std::numeric_limits<double>::epsilon()};
const std::string s_epsilon_d{eps_string(epsilon_d)};

// Self-seed the random number generator
inline XoshiroCpp::Xoshiro256Plus init() noexcept {
  // Random device for seeding
  std::random_device rand_dev{};
  // Two random runtime constants
  const uint64_t time1{static_cast<uint64_t>(
      std::chrono::steady_clock::now().time_since_epoch().count())};
  const uint64_t time2{static_cast<uint64_t>(
      std::chrono::steady_clock::now().time_since_epoch().count())};
  // Seed the initial state
  const XoshiroCpp::Xoshiro256Plus::state_type initial_state = {
      XoshiroCpp::SplitMix64{time1}(), XoshiroCpp::SplitMix64{rand_dev()}(),
      XoshiroCpp::SplitMix64{time2}(), XoshiroCpp::SplitMix64{rand_dev()}()};
  return XoshiroCpp::Xoshiro256Plus{initial_state};
}

// Build the PRNG
inline XoshiroCpp::Xoshiro256Plus xos{init()};

// Give a random double within the inclusive bounds [min, max]
inline double get(double min, double max) noexcept {
  std::uniform_real_distribution<> die{min, max};
  return die(xos);
}

void random_vector(std::vector<double> *data, const double minimum = -1.0,
                   const double maximum = 1.0) noexcept {
  for (std::size_t i{0}; i < data->size(); ++i) {
    (*data)[i] = get(minimum, maximum);
  }
}

// NOLINTBEGIN(readability-magic-numbers,
// readibility-function-cognitive-complexity)
Trace gen_fake_trace() noexcept {
  Trace sac{};
  sac.delta(0.025);
  sac.depmin(-1.11F);
  sac.depmax(1.23F);
  sac.odelta(0.026F);
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
  sac.resp0(0.1F);
  sac.resp1(-0.1F);
  sac.resp2(1.0F);
  sac.resp3(-1.0F);
  sac.resp4(10.0F);
  sac.resp5(-10.0F);
  sac.resp6(100.0F);
  sac.resp7(-100.0F);
  sac.resp8(1000.0F);
  sac.resp9(-1000.0F);
  sac.stla(32.21);
  sac.stlo(121.38);
  sac.stel(100.0F);
  sac.stdp(50.0F);
  sac.evla(-81.35);
  sac.evlo(85.37);
  sac.evel(0.5F);
  sac.evdp(30.25F);
  sac.mag(1.83F);
  sac.user0(9.0F);
  sac.user1(-9.0F);
  sac.user2(8.0F);
  sac.user3(-8.0F);
  sac.user4(7.0F);
  sac.user5(-7.0F);
  sac.user6(6.0F);
  sac.user7(-6.0F);
  sac.user8(5.0F);
  sac.user9(-5.0F);
  sac.dist(1300.0F);
  sac.az(35.0F);
  sac.baz(-275.0F);
  sac.gcarc(13.135F);
  sac.sb(-35.0);
  sac.sdelta(0.125);
  sac.depmen(0.1F);
  sac.cmpaz(97.3F);
  sac.cmpinc(273.0F);
  sac.xminimum(-5.0F);
  sac.xmaximum(5.0F);
  sac.yminimum(-10.0F);
  sac.ymaximum(10.0F);
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
    if (!sac.leven() || sac.iftype() > 1) {
      data.resize(sac.npts(), 0.0);
      sac.data2(data);
    }
  }
  return sac;
}
// NOLINTEND(readability-magic-numbers,
// readbility-function-cognitive-complexity)

void unset_trace(Trace *sac) noexcept {
  sac->delta(unset_double);
  sac->depmin(unset_float);
  sac->depmax(unset_float);
  sac->odelta(unset_float);
  sac->b(unset_double);
  sac->e(unset_double);
  sac->a(unset_double);
  sac->o(unset_double);
  sac->t0(unset_double);
  sac->t1(unset_double);
  sac->t2(unset_double);
  sac->t3(unset_double);
  sac->t4(unset_double);
  sac->t5(unset_double);
  sac->t6(unset_double);
  sac->t7(unset_double);
  sac->t8(unset_double);
  sac->t9(unset_double);
  sac->f(unset_double);
  sac->resp0(unset_float);
  sac->resp1(unset_float);
  sac->resp2(unset_float);
  sac->resp3(unset_float);
  sac->resp4(unset_float);
  sac->resp5(unset_float);
  sac->resp6(unset_float);
  sac->resp7(unset_float);
  sac->resp8(unset_float);
  sac->resp9(unset_float);
  sac->stla(unset_double);
  sac->stlo(unset_double);
  sac->stel(unset_float);
  sac->stdp(unset_float);
  sac->evla(unset_double);
  sac->evlo(unset_double);
  sac->evel(unset_float);
  sac->evdp(unset_float);
  sac->mag(unset_float);
  sac->user0(unset_float);
  sac->user1(unset_float);
  sac->user2(unset_float);
  sac->user3(unset_float);
  sac->user4(unset_float);
  sac->user5(unset_float);
  sac->user6(unset_float);
  sac->user7(unset_float);
  sac->user8(unset_float);
  sac->user9(unset_float);
  sac->dist(unset_float);
  sac->az(unset_float);
  sac->baz(unset_float);
  sac->gcarc(unset_float);
  sac->sb(unset_double);
  sac->sdelta(unset_double);
  sac->depmen(unset_float);
  sac->cmpaz(unset_float);
  sac->cmpinc(unset_float);
  sac->xminimum(unset_float);
  sac->xmaximum(unset_float);
  sac->yminimum(unset_float);
  sac->ymaximum(unset_float);
  sac->nzyear(unset_int);
  sac->nzjday(unset_int);
  sac->nzhour(unset_int);
  sac->nzmin(unset_int);
  sac->nzsec(unset_int);
  sac->nzmsec(unset_int);
  sac->nvhdr(unset_int);
  sac->norid(unset_int);
  sac->nevid(unset_int);
  sac->npts(unset_int);
  sac->nsnpts(unset_int);
  sac->nwfid(unset_int);
  sac->nxsize(unset_int);
  sac->nysize(unset_int);
  sac->iftype(unset_int);
  sac->idep(unset_int);
  sac->iztype(unset_int);
  sac->iinst(unset_int);
  sac->istreg(unset_int);
  sac->ievreg(unset_int);
  sac->ievtyp(unset_int);
  sac->iqual(unset_int);
  sac->isynth(unset_int);
  sac->imagtyp(unset_int);
  sac->imagsrc(unset_int);
  sac->ibody(unset_int);
  sac->leven(unset_bool);
  sac->lpspol(unset_bool);
  sac->lovrok(unset_bool);
  sac->lcalda(unset_bool);
  sac->kstnm(unset_word);
  sac->kevnm(unset_word);
  sac->khole(unset_word);
  sac->ko(unset_word);
  sac->ka(unset_word);
  sac->kt0(unset_word);
  sac->kt1(unset_word);
  sac->kt2(unset_word);
  sac->kt3(unset_word);
  sac->kt4(unset_word);
  sac->kt5(unset_word);
  sac->kt6(unset_word);
  sac->kt7(unset_word);
  sac->kt8(unset_word);
  sac->kt9(unset_word);
  sac->kf(unset_word);
  sac->kuser0(unset_word);
  sac->kuser1(unset_word);
  sac->kuser2(unset_word);
  sac->kcmpnm(unset_word);
  sac->knetwk(unset_word);
  sac->kdatrd(unset_word);
  sac->kinst(unset_word);
  // Empty data vectors
  std::vector<double> data{};
  sac->data1(data);
  sac->data2(data);
}

// Need flags for npts and data2
// NOLINTBEGIN(readability-magic-numbers,
// readability-function-cognitive-complexity)
void write_corrupt_sac(const fs::path &file, size_t n_hdr, int fake_npts = 0,
                       size_t real_npts = 0, bool data2 = false,
                       size_t real_npts2 = 0, const int header_version = 6,
                       size_t n_footer = 0, size_t extra_words = 0) {
  std::ofstream corrupt_sac(file,
                            std::ios::binary | std::ios::out | std::ios::trunc);
  constexpr size_t hdr_word{76};
  constexpr size_t npts_word{79};
  constexpr size_t leven_word{105};
  // Pre-nvhdr
  size_t limit_1{0};
  // Pre-npts
  size_t limit_2{0};
  // Pre-leven
  size_t limit_3{0};
  // Pre-data1
  size_t limit_4{0};
  // An oversized header is not useful for testing purposes.
  n_hdr = (n_hdr > data_word ? data_word : n_hdr);
  // No data if the header is small
  if (n_hdr < data_word) {
    real_npts = 0;
    fake_npts = 0;
    real_npts2 = 0;
    data2 = false;
    extra_words = 0;
  }
  if (!data2) {
    real_npts2 = 0;
  }
  n_footer = {n_footer > num_footer ? num_footer : n_footer};
  if (header_version != 7) {
    n_footer = 0;
  }
  real_npts = (n_hdr == data_word ? real_npts : 0);
  real_npts2 = (n_hdr == data_word ? real_npts2 : 0);
  if (n_hdr <= hdr_word) {
    limit_1 = n_hdr;
  } else {
    if (n_hdr <= npts_word) {
      limit_2 = n_hdr - hdr_word;
    } else {
      if (n_hdr <= leven_word) {
        limit_3 = n_hdr - npts_word;
      } else {
        limit_3 = leven_word - npts_word;
        limit_4 = n_hdr - leven_word;
      }
      limit_2 = npts_word - hdr_word;
    }
    limit_1 = hdr_word;
  }
  // Up to hdr_word
  for (size_t i{0}; i < limit_1; ++i) {
    // Just a ton of zeros (ints, one word)
    write_words(&corrupt_sac, convert_to_word(0));
  }
  // Only include hdr_word if there are more words to write
  if (limit_2 > 0) {
    --limit_2;
    write_words(&corrupt_sac, convert_to_word(header_version));
  }
  // Post hdr_word
  for (size_t i{0}; i < limit_2; ++i) {
    write_words(&corrupt_sac, convert_to_word(0));
  }
  // Only include npts_word if there are more words to write
  if (limit_3 > 0) {
    --limit_3;
    write_words(&corrupt_sac, convert_to_word(fake_npts));
  }
  // Post npts_word
  for (size_t i{0}; i < limit_3; ++i) {
    write_words(&corrupt_sac, convert_to_word(0));
  }
  // Only include leven if there are more words to write
  if (limit_4 > 0) {
    --limit_4;
    write_words(&corrupt_sac, bool_to_word(!data2));
  }
  // Post leven_word
  for (size_t i{0}; i < limit_4; ++i) {
    write_words(&corrupt_sac, convert_to_word(0));
  }
  // Now for data1 if it exists
  for (size_t i{0}; i < real_npts; ++i) {
    // writing floats (1 word)
    write_words(&corrupt_sac, convert_to_word(0.0F));
  }
  // Now for data2 if it exists
  if (data2) {
    for (size_t i{0}; i < real_npts2; ++i) {
      write_words(&corrupt_sac, convert_to_word(1.0F));
    }
  }
  // Footer if it exists
  for (size_t i{0}; i < n_footer; ++i) {
    // Doubles (2 words)
    write_words(&corrupt_sac, convert_to_word(0.0));
  }
  // Extra words if they exist
  for (size_t i{0}; i < extra_words; ++i) {
    write_words(&corrupt_sac, convert_to_word(3.0F));
  }
  corrupt_sac.close();
}
// NOLINTEND(readability-magic-numbers,
// readability-function-cognitive-complexity)
}  // namespace sacfmt

#endif
