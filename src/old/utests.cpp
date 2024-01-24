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
TEST_CASE("Geometric Operations: Calc Distance/Azimuth: Lat , Long ") {
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
