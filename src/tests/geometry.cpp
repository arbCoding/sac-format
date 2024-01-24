// Copyright 2023-2024 Alexander R. Blanchette

// sac-format
#include "sac-format/sac_format.hpp"
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

using Catch::Matchers::WithinAbs;
namespace sacfmt {
TEST_CASE("Geometric Operations: Helper Functions: Limit 360 [0, 360]: No Adjustment") {
  REQUIRE(limit_360(0) == 0);
  REQUIRE(limit_360(-0) == 0);
  REQUIRE(limit_360(90) == 90);
  REQUIRE(limit_360(180) == 180);
  REQUIRE(limit_360(270) == 270);
  REQUIRE(limit_360(360) == 360);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 360 [0, 360]: Negatives") {
  REQUIRE(limit_360(-180) == 180);
  REQUIRE(limit_360(-270) == 90);
  REQUIRE(limit_360(-90) == 270);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 360 [0, 360]: Adjust by One Loop") {
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

TEST_CASE("Geometric Operations: Helper Functions: Limit 360 [0, 360]: Adjust by Many Loops") {
  REQUIRE(limit_360(10 * circle_deg + 10) == 10);
  REQUIRE(limit_360(8 * circle_deg + 75) == 75);
  REQUIRE(limit_360(5 * circle_deg + 35) == 35);
  REQUIRE(limit_360(-7 * circle_deg - 10) == circle_deg - 10);
  REQUIRE(limit_360(-3 * circle_deg - 180) == 180);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 180 [-180, 180]: No Adjustment") {
  REQUIRE(limit_180(0) == 0);
  REQUIRE(limit_180(90) == 90);
  REQUIRE(limit_180(180) == 180);
  REQUIRE(limit_180(-0) == 0);
  REQUIRE(limit_180(-10) == -10);
  REQUIRE(limit_180(-90) == -90);
  REQUIRE(limit_180(-179) == -179);
}


TEST_CASE("Geometric Operations: Helper Functions: Limit 180 [-180, 180]: Adjustment Positives") {
  REQUIRE(limit_180(360) == 0);
  REQUIRE(limit_180(270) == -90);
  REQUIRE(limit_180(181) == -179);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 180 [-180, 180]: Adjustment Negatives") {
  REQUIRE(limit_180(-180) == 180);
  REQUIRE(limit_180(-360) == 0);
  REQUIRE(limit_180(-270) == 90);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 180 [-180, 180]: Multiple Loops") {
  REQUIRE(limit_180(2 * circle_deg - 180) == 180);
  REQUIRE(limit_180(10 * circle_deg + 10) == 10);
  REQUIRE(limit_180(8 * circle_deg + 75) == 75);
  REQUIRE(limit_180(5 * circle_deg + 35) == 35);
  REQUIRE(limit_180(-7 * circle_deg - 10) == -10);
  REQUIRE(limit_180(-3 * circle_deg - 180) == 180);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 90 [-90, 90]: No Adjustment") {
  REQUIRE(limit_90(0) == 0);
  REQUIRE(limit_90(90) == 90);
  REQUIRE(limit_90(45) == 45);
  REQUIRE(limit_90(-90) == -90);
  REQUIRE(limit_90(-45) == -45);
  REQUIRE(limit_90(-74) == -74);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 90 [-90, 90]: Adjustment Positives") {
  REQUIRE(limit_90(180) == 0);
  REQUIRE(limit_90(91) == 89);
  REQUIRE(limit_90(135) == 45);
  REQUIRE(limit_90(360) == 0);
  REQUIRE(limit_90(270) == -90);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 90 [-90, 90]: Adjustment Negatives") {
  REQUIRE(limit_90(-180) == 0);
  REQUIRE(limit_90(-91) == -89);
  REQUIRE(limit_90(-135) == -45);
  REQUIRE(limit_90(-270) == 90);
  REQUIRE(limit_90(-360) == 0);
}

constexpr double tol{1e-4};
TEST_CASE("Geometric Operations: Calc Distance/Azimuth: Lat 0, Long 0") {
  constexpr double lat{0};
  constexpr double lon{0};
  const double est_gcarc{gcarc(lat, lon, lat, lon)};
  REQUIRE_THAT(est_gcarc, WithinAbs(0, tol));
  const double est_az{azimuth(lat, lon, lat, lon)};
  REQUIRE_THAT(est_az, WithinAbs(0, tol));
}

TEST_CASE("Geometric Operations: Calc Distance/Azimuth: Lat 90, Long 0") {
  constexpr double lat{90};
  constexpr double lon{0};
  const double est_gcarc{gcarc(lat, lon, lat, lon)};
  REQUIRE_THAT(est_gcarc, WithinAbs(0, tol));
  const double est_az{azimuth(lat, lon, lat, lon)};
  REQUIRE_THAT(est_az, WithinAbs(0, tol));
}



}  // namespace sacfmt
