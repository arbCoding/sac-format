// Copyright 2023-2024 Alexander R. Blanchette

// Required for Catch2 compatibility on Windows
#if defined(_MSC_VER) || defined(__MINW32__)
#define _ITERATOR_DEBUG_LEVEL 2
#endif

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
TEST_CASE("DateTime: Default Trace") {
  Trace trace{};
  const std::string date{trace.date()};
  REQUIRE(date == unset_word);
  const std::string time{trace.time()};
  REQUIRE(time == unset_word);
}

TEST_CASE("DateTime: Standard Fake Trace") {
  Trace trace = gen_fake_trace();
  const std::string date{trace.date()};
  REQUIRE(date == "2023-123");
  const std::string time{trace.time()};
  REQUIRE(time == "13:57:34.0");
}
}  // namespace sacfmt
