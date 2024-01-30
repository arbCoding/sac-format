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
using Catch::Matchers::WithinAbs;

namespace sacfmt {
// NOLINTBEGIN(readability-magic-numbers)
TEST_CASE("Geometric Operations: Coordinates: Degrees In") {
  REQUIRE(coord{0.0, true}.degrees() == 0.0);
  REQUIRE(coord{90.0, true}.degrees() == 90.0);
  REQUIRE(coord{0.0, true}.radians() == 0.0);
  REQUIRE(coord{90.0, false}.radians() == 90.0);
  REQUIRE(coord{90.0, true}.radians() == std::numbers::pi_v<double> / 2.0);
  REQUIRE(coord{180.0, true}.radians() == std::numbers::pi_v<double>);
  REQUIRE(coord{-90.0, true}.radians() == -std::numbers::pi_v<double> / 2.0);
  REQUIRE(coord{-180.0, true}.radians() == -std::numbers::pi_v<double>);
}

TEST_CASE("Geometric Operations: Coordiantes: Radians In") {
  REQUIRE(coord{std::numbers::pi_v<double>, false}.degrees() == 180.0);
  REQUIRE(coord{std::numbers::pi_v<double> / 2.0, false}.degrees() == 90.0);
  REQUIRE(coord{-std::numbers::pi_v<double>, false}.degrees() == -180.0);
  REQUIRE(coord{-std::numbers::pi_v<double> / 2.0, false}.degrees() == -90.0);
}

TEST_CASE("Geometric Operations: Coordinates: Setters") {
  coord test_coord{0.0, true};
  REQUIRE(test_coord.degrees() == 0.0);
  test_coord.degrees(90.0);
  REQUIRE(test_coord.degrees() == 90.0);
  REQUIRE(test_coord.radians() == std::numbers::pi_v<double> / 2.0);
  test_coord.radians(std::numbers::pi_v<double>);
  REQUIRE(test_coord.radians() == std::numbers::pi_v<double>);
  REQUIRE(test_coord.degrees() == 180.0);
}

TEST_CASE("Geometric Operations: Coordinates: Getters") {
  coord test_coord{180.0, true};
  double degrees{test_coord.degrees()};
  REQUIRE(degrees == 180.0);
  double radians{test_coord.radians()};
  REQUIRE(radians == std::numbers::pi_v<double>);
}

TEST_CASE("Geometric Operations: Points") {
  REQUIRE(point{coord{90.0, true}, coord{0.0, true}}.latitude.degrees()
          == 90.0);
  REQUIRE(point{coord{90.0, true}, coord{0.0, true}}.latitude.radians()
          == std::numbers::pi_v<double> / 2.0);
  REQUIRE(point{coord{90.0, true}, coord{0.0, true}}.longitude.degrees() == 0);
  REQUIRE(point{coord{90.0, true}, coord{0.0, true}}.longitude.radians() == 0);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 360 [0, 360]: No "
          "Adjustment") {
  REQUIRE(limit_360(0) == 0);
  REQUIRE(limit_360(-0) == 0);
  REQUIRE(limit_360(90) == 90);
  REQUIRE(limit_360(180) == 180);
  REQUIRE(limit_360(270) == 270);
  REQUIRE(limit_360(360) == 360);
}

TEST_CASE(
    "Geometric Operations: Helper Functions: Limit 360 [0, 360]: Negatives") {
  REQUIRE(limit_360(-180) == 180);
  REQUIRE(limit_360(-270) == 90);
  REQUIRE(limit_360(-90) == 270);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 360 [0, 360]: Adjust "
          "by One Loop") {
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

TEST_CASE("Geometric Operations: Helper Functions: Limit 360 [0, 360]: Adjust "
          "by Many Loops") {
  REQUIRE(limit_360(10 * circle_deg + 10) == 10);
  REQUIRE(limit_360(8 * circle_deg + 75) == 75);
  REQUIRE(limit_360(5 * circle_deg + 35) == 35);
  REQUIRE(limit_360(-7 * circle_deg - 10) == circle_deg - 10);
  REQUIRE(limit_360(-3 * circle_deg - 180) == 180);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 180 [-180, 180]: No "
          "Adjustment") {
  REQUIRE(limit_180(0) == 0);
  REQUIRE(limit_180(90) == 90);
  REQUIRE(limit_180(180) == 180);
  REQUIRE(limit_180(-0) == 0);
  REQUIRE(limit_180(-10) == -10);
  REQUIRE(limit_180(-90) == -90);
  REQUIRE(limit_180(-179) == -179);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 180 [-180, 180]: "
          "Adjustment Positives") {
  REQUIRE(limit_180(360) == 0);
  REQUIRE(limit_180(270) == -90);
  REQUIRE(limit_180(181) == -179);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 180 [-180, 180]: "
          "Adjustment Negatives") {
  REQUIRE(limit_180(-180) == 180);
  REQUIRE(limit_180(-360) == 0);
  REQUIRE(limit_180(-270) == 90);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 180 [-180, 180]: "
          "Multiple Loops") {
  REQUIRE(limit_180(2 * circle_deg - 180) == 180);
  REQUIRE(limit_180(10 * circle_deg + 10) == 10);
  REQUIRE(limit_180(8 * circle_deg + 75) == 75);
  REQUIRE(limit_180(5 * circle_deg + 35) == 35);
  REQUIRE(limit_180(-7 * circle_deg - 10) == -10);
  REQUIRE(limit_180(-3 * circle_deg - 180) == 180);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 90 [-90, 90]: No "
          "Adjustment") {
  REQUIRE(limit_90(0) == 0);
  REQUIRE(limit_90(90) == 90);
  REQUIRE(limit_90(45) == 45);
  REQUIRE(limit_90(-90) == -90);
  REQUIRE(limit_90(-45) == -45);
  REQUIRE(limit_90(-74) == -74);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 90 [-90, 90]: "
          "Adjustment Positives") {
  REQUIRE(limit_90(180) == 0);
  REQUIRE(limit_90(91) == 89);
  REQUIRE(limit_90(135) == 45);
  REQUIRE(limit_90(360) == 0);
  REQUIRE(limit_90(270) == -90);
}

TEST_CASE("Geometric Operations: Helper Functions: Limit 90 [-90, 90]: "
          "Adjustment Negatives") {
  REQUIRE(limit_90(-180) == 0);
  REQUIRE(limit_90(-91) == -89);
  REQUIRE(limit_90(-135) == -45);
  REQUIRE(limit_90(-270) == 90);
  REQUIRE(limit_90(-360) == 0);
}

constexpr double tol{1e-4};
TEST_CASE("Geometric Operations: Calc Distance/Azimuth: Same: Lat 0, Long 0") {
  const point loc{coord{0.0, true}, coord{0.0, true}};
  const double est_gcarc{gcarc(loc, loc)};
  REQUIRE_THAT(est_gcarc, WithinAbs(0, tol));
  const double est_az{azimuth(loc, loc)};
  REQUIRE_THAT(est_az, WithinAbs(0, tol));
}

TEST_CASE("Geometric Operations: Calc Distance/Azimuth: Same: Lat 90, Long 0") {
  const point loc{coord{90.0, true}, coord{0.0, true}};
  const double est_gcarc{gcarc(loc, loc)};
  REQUIRE_THAT(est_gcarc, WithinAbs(0, tol));
  const double est_az{azimuth(loc, loc)};
  REQUIRE_THAT(est_az, WithinAbs(0, tol));
}

TEST_CASE(
    "Geometric Operations: Calc Distance/Azimuth: Same: Lat -90, Long 0") {
  const point loc{coord{-90.0, true}, coord{0.0, true}};
  const double est_gcarc{gcarc(loc, loc)};
  REQUIRE_THAT(est_gcarc, WithinAbs(0, tol));
  const double est_az{azimuth(loc, loc)};
  REQUIRE_THAT(est_az, WithinAbs(0, tol));
}

TEST_CASE(
    "Geometric Operations: Calc Distance/Azimuth: Same: Lat 0, Long 180") {
  const point loc{coord{0.0, true}, coord{180, true}};
  const double est_gcarc{gcarc(loc, loc)};
  REQUIRE_THAT(est_gcarc, WithinAbs(0, tol));
  const double est_az{azimuth(loc, loc)};
  REQUIRE_THAT(est_az, WithinAbs(0, tol));
}

TEST_CASE(
    "Geometric Operations: Calc Distance/Azimuth: Same: Lat 0, Long -180") {
  const point loc{coord{0.0, true}, coord{-180.0, true}};
  const double est_gcarc{gcarc(loc, loc)};
  REQUIRE_THAT(est_gcarc, WithinAbs(0, tol));
  const double est_az{azimuth(loc, loc)};
  REQUIRE_THAT(est_az, WithinAbs(0, tol));
}

const point equator{coord{0.0, true}, coord{0.0, true}};
const point n_pole{coord{90.0, true}, coord{0.0, true}};
const point s_pole{coord{-90.0, true}, coord{0.0, true}};
TEST_CASE("Geometric Operations: Calc Distance/Azimuth: Equator vs. Pole: "
          "North Pole") {
  const double est_gcarc{gcarc(equator, n_pole)};
  REQUIRE_THAT(est_gcarc, WithinAbs(90, tol));
  const double est_az{azimuth(equator, n_pole)};
  REQUIRE_THAT(est_az, WithinAbs(0, tol));
  const double est_baz{azimuth(n_pole, equator)};
  REQUIRE_THAT(est_baz, WithinAbs(180, tol));
}

TEST_CASE("Geometric Operations: Calc Distance/Azimuth: Equator vs. Pole: "
          "South Pole") {
  const double est_gcarc{gcarc(equator, s_pole)};
  REQUIRE_THAT(est_gcarc, WithinAbs(90, tol));
  const double est_az{azimuth(equator, s_pole)};
  REQUIRE_THAT(est_az, WithinAbs(180, tol));
  const double est_baz{azimuth(s_pole, equator)};
  REQUIRE_THAT(est_baz, WithinAbs(0, tol));
}

TEST_CASE("Geometric Operations: Calc Distance/Azimuth: Equator vs. Pole: "
          "North Pole to South Pole") {
  const double est_gcarc{gcarc(n_pole, s_pole)};
  REQUIRE_THAT(est_gcarc, WithinAbs(180, tol));
  const double est_az{azimuth(n_pole, s_pole)};
  REQUIRE_THAT(est_az, WithinAbs(180, tol));
  const double est_baz{azimuth(s_pole, n_pole)};
  REQUIRE_THAT(est_baz, WithinAbs(0, tol));
}

TEST_CASE("Geometric Operations: Calc Distance/Azimuth: Equator vs. Pole: "
          "South Pole to North Pole") {
  const double est_gcarc{gcarc(s_pole, n_pole)};
  REQUIRE_THAT(est_gcarc, WithinAbs(180, tol));
}

TEST_CASE(
    "Geometric Operations: Calc Distance/Azimuth: Over Pole: North Pole") {
  const point loc1{coord{70.0, true}, coord{0.0, true}};
  const point loc2{coord{70.0, true}, coord{180.0, true}};
  const double exp_gcarc{2 * (90 - loc1.latitude.degrees())};
  const double est_gcarc{gcarc(loc1, loc2)};
  REQUIRE_THAT(est_gcarc, WithinAbs(exp_gcarc, tol));
  const double est_az{azimuth(loc1, loc2)};
  REQUIRE_THAT(est_az, WithinAbs(0, tol));
  const double est_baz{azimuth(loc2, loc1)};
  REQUIRE_THAT(est_baz, WithinAbs(360, tol));
}

TEST_CASE(
    "Geometric Operations: Calc Distance/Azimuth: Over Pole: South Pole") {
  const point loc1{coord{-65.0, true}, coord{90.0, true}};
  const point loc2{coord{-65.0, true}, coord{-90.0, true}};
  const double exp_gcarc{2 * (loc1.latitude.degrees() + 90)};
  const double est_gcarc{gcarc(loc1, loc2)};
  REQUIRE_THAT(est_gcarc, WithinAbs(exp_gcarc, tol));
  const double est_az{azimuth(loc1, loc2)};
  REQUIRE_THAT(est_az, WithinAbs(180, tol));
  const double est_baz{azimuth(loc2, loc1)};
  REQUIRE_THAT(est_baz, WithinAbs(180, tol));
}

TEST_CASE("Geometric Operations: Calc Distance/Azimuth: Along Equator") {
  point loc1{coord{0.0, true}, coord{0.0, true}};
  point loc2{coord{0.0, true}, coord{90.0, true}};
  double est_gcarc{gcarc(loc1, loc2)};
  REQUIRE_THAT(est_gcarc, WithinAbs(90, tol));
  double est_az{azimuth(loc1, loc2)};
  REQUIRE_THAT(est_az, WithinAbs(90, tol));
  double est_baz{azimuth(loc2, loc1)};
  REQUIRE_THAT(est_baz, WithinAbs(270, tol));
  loc1.longitude = coord{30.0, true};
  loc2.longitude = coord{70.0, true};
  est_gcarc = gcarc(loc1, loc2);
  REQUIRE_THAT(
      est_gcarc,
      WithinAbs(loc2.longitude.degrees() - loc1.longitude.degrees(), tol));
  est_az = azimuth(loc1, loc2);
  REQUIRE_THAT(est_az, WithinAbs(90, tol));
  est_baz = azimuth(loc2, loc1);
  REQUIRE_THAT(est_baz, WithinAbs(270, tol));
  loc1.longitude = coord{90.0, true};
  loc2.longitude = coord{-90.0, true};
  est_gcarc = gcarc(loc1, loc2);
  REQUIRE_THAT(
      est_gcarc,
      WithinAbs(loc1.longitude.degrees() - loc2.longitude.degrees(), tol));
  est_az = azimuth(loc1, loc2);
  REQUIRE_THAT(est_az, WithinAbs(270, tol));
  est_baz = azimuth(loc2, loc1);
  REQUIRE_THAT(est_baz, WithinAbs(90, tol));
}

TEST_CASE(
    "Geometric Operations: Calc Distance/Azimuth: Taken From Real SAC File") {
  const point loc1{coord{38.4328, true}, coord{-118.155, true}};
  const point loc2{coord{36.801, true}, coord{-121.323, true}};
  constexpr double expected_gcarc{2.99645};
  constexpr double expected_az{56.1169};
  constexpr double expected_baz{238.043};
  const double test_gcarc{gcarc(loc1, loc2)};
  REQUIRE_THAT(test_gcarc, WithinAbs(expected_gcarc, 5e-3));
  const double test_az{azimuth(loc2, loc1)};
  const double test_baz{azimuth(loc1, loc2)};
  REQUIRE_THAT(test_az, WithinAbs(expected_az, 0.2));
  REQUIRE_THAT(test_baz, WithinAbs(expected_baz, 0.2));
}

TEST_CASE("Geometric Operations: Trace Geometry: Default Trace") {
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

TEST_CASE("Geometric Operations: Trace Geometry: Standard Fake Trace") {
  constexpr float gcarc{115.0889663696F};
  constexpr float azi{33.3167991638F};
  constexpr float baz{185.6029968262F};
  constexpr float dist{12811.65234375F};
  Trace trace = gen_fake_trace();
  REQUIRE(trace.gcarc() != gcarc);
  REQUIRE(trace.az() != gcarc);
  REQUIRE(trace.baz() != baz);
  REQUIRE(trace.dist() != dist);
  trace.calc_geometry();
  REQUIRE_THAT(trace.gcarc(), WithinAbs(gcarc, 1e-4));
  REQUIRE_THAT(trace.az(), WithinAbs(azi, 1e-4));
  REQUIRE_THAT(trace.baz(), WithinAbs(baz, 1e-4));
  REQUIRE_THAT(trace.dist(), WithinAbs(dist, 1e-4));
}
// NOLINTEND(readability-magic-numbers)
}  // namespace sacfmt
