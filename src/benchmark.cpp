// Copyright 2023 Alexander R. Blanchette

#include "sac-format/sac_format.hpp"
#include "sac-format/util.hpp"
// Catch2 https://github.com/catchorg/Catch2/tree/v3.4.0
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_MAIN
// BENCHMARK
#include <catch2/benchmark/catch_benchmark.hpp>
// testing macros
// TEST_CASE, SECTION,
#include <catch2/catch_test_macros.hpp>
// Standard Library
//   https://en.cppreference.com/w/cpp/standard_library
#include <bitset>
#include <iomanip>
#include <limits>

// using namespace sacfmt;
namespace fs = std::filesystem;
// NOLINTBEGIN(readability-function-cognitive-complexity)
namespace sacfmt {
TEST_CASE("Binary Conversion") {
  SECTION("Booleans") {
    BENCHMARK("Bool->Binary False") { return bool_to_binary(unset_bool); };
    BENCHMARK("Bool->Binary->Bool False") {
      return binary_to_bool(bool_to_binary(unset_bool));
    };
  }
  SECTION("Integers") {
    SECTION("Zero") {
      BENCHMARK("Int->Binary 0") { return int_to_binary(0); };
      BENCHMARK("Int->Binary->Int 0") {
        return binary_to_int(int_to_binary(0));
      };
    }
    SECTION("Negative") {
      BENCHMARK("Int->Binary lowest_i") { return int_to_binary(lowest_i); };
      BENCHMARK("Int->Binary->Int lowest_i") {
        return binary_to_int(int_to_binary(lowest_i));
      };
    }
    SECTION("Positive") {
      BENCHMARK("Int->Binary highest_i") { return int_to_binary(highest_i); };
      BENCHMARK("Int->Binary->Int highest_i") {
        return binary_to_int(int_to_binary(highest_i));
      };
    }
  }
  SECTION("Floats") {
    SECTION("Zero") {
      BENCHMARK("Float->Binary 0.0F") { return float_to_binary(0.0F); };
      BENCHMARK("Float->Binary->Float 0.0F") {
        return binary_to_float(float_to_binary(0.0F));
      };
    }
    SECTION("Negative") {
      BENCHMARK("Float->Binary->Float std::numeric_limits<float>::lowest()") {
        return binary_to_float(float_to_binary(lowest_f));
      };
      CAPTURE(s_neg_epsilon_f);
      BENCHMARK("Float->Binary->Float negative"
                " std::numeric_limits<float>::epsilon()") {
        return binary_to_float(float_to_binary(neg_epsilon_f));
      };
    }
    SECTION("Positive") {
      BENCHMARK("Float->Binary->Float std::numeric_limits<float>::max()") {
        return binary_to_float(float_to_binary(highest_f));
      };
      CAPTURE(s_epsilon_f);
      BENCHMARK("Float->Binary->Float std::numeric_limits<float>::epsilon()") {
        return binary_to_float(float_to_binary(epsilon_f));
      };
    }
  }
  SECTION("Doubles") {
    SECTION("Zero") {
      BENCHMARK("Double->Binary->Double 0.0") {
        return binary_to_double(double_to_binary(0.0));
      };
    }
    SECTION("Negative") {
      BENCHMARK("Double->Binary->Double"
                " std::numeric_limits<double>::lowest()") {
        return binary_to_double(double_to_binary(lowest_d));
      };
      CAPTURE(s_neg_epsilon_d);
      BENCHMARK("Double->Binary->Double negative"
                " std::numeric_limits<double>::epsilon()") {
        return binary_to_double(double_to_binary(neg_epsilon_d));
      };
    }
    SECTION("Positive") {
      BENCHMARK("Double->Binary->Double std::numeric_limits<double>::max()") {
        return binary_to_double(double_to_binary(highest_d));
      };
      CAPTURE(s_epsilon_d);
      BENCHMARK("Double->Binary->Double"
                " std::numeric_limits<double>::epsilon()") {
        return binary_to_double(double_to_binary(epsilon_d));
      };
    }
  }
  SECTION("Strings") {
    SECTION("Regular - Two Words") {
      SECTION("Perfect") {
        const std::string test_str{"01234567"};
        CAPTURE(test_str);
        BENCHMARK("String->Binary->String Exact") {
          return binary_to_string(string_to_binary(test_str));
        };
      }
      SECTION("Empty") {
        const std::string test_str{};
        BENCHMARK("String->Binary->String Empty") {
          return binary_to_string(string_to_binary(test_str));
        };
      }
      SECTION("Small") {
        const std::string test_str{"0123"};
        BENCHMARK("String->Binary->String Half") {
          return binary_to_string(string_to_binary(test_str));
        };
      }
      SECTION("Overflow") {
        const std::string test_str{"0123456789ABCDEFG"};
        CAPTURE(test_str);
        BENCHMARK("String->Binary->String Overfull") {
          return binary_to_string(string_to_binary(test_str));
        };
      }
    }
    SECTION("Long - Four Words") {
      SECTION("Perfect") {
        const std::string test_str{"0123456789ABCDEF"};
        BENCHMARK("String->Binary->String Exact") {
          return binary_to_long_string(long_string_to_binary(test_str));
        };
      }
      SECTION("Empty") {
        const std::string test_str{};
        BENCHMARK("String->Binary->String Empty") {
          return binary_to_long_string(long_string_to_binary(test_str));
        };
      }
      SECTION("Small") {
        const std::string test_str{"01234567"};
        BENCHMARK("String->Binary->String Half") {
          return binary_to_long_string(long_string_to_binary(test_str));
        };
      }
      SECTION("Overflow") {
        const std::string test_str{"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
        CAPTURE(test_str);
        BENCHMARK("String->Binary->String Overfull") {
          return binary_to_long_string(long_string_to_binary(test_str));
        };
      }
    }
  }
}

TEST_CASE("Input/Output") {
  SECTION("Empty Trace") {
    Trace test_sac{};
    fs::path tmp_dir{fs::temp_directory_path()};
    fs::path tmp_file{tmp_dir / "test.SAC"};
    std::cout << "Test file: " << tmp_file << '\n';
    SECTION("Out") {
      BENCHMARK("Writing") {
        test_sac.write(tmp_file);
        return;
      };
      fs::remove(tmp_file);
    }
    SECTION("In") {
      test_sac.write(tmp_file);
      BENCHMARK("Reading") { Trace in_sac = Trace(tmp_file); };
      fs::remove(tmp_file);
    }
    SECTION("Comparison Between Out and In") {
      test_sac.write(tmp_file);
      Trace in_sac = Trace(tmp_file);
      BENCHMARK("Out vs In") { (void)(in_sac == test_sac); };
      fs::remove(tmp_file);
    }
  }
  SECTION("Non-Empty Trace") {
    Trace test_sac = gen_fake_trace();
    // Done building
    fs::path tmp_dir{fs::temp_directory_path()};
    fs::path tmp_file{tmp_dir / "test.SAC"};
    std::cout << "Test file: " << tmp_file << '\n';
    SECTION("Out") {
      BENCHMARK("Writing") {
        test_sac.write(tmp_file);
        return;
      };
      fs::remove(tmp_file);
    }
    SECTION("In") {
      test_sac.write(tmp_file);
      BENCHMARK("Reading") {
        Trace in_sac = Trace(tmp_file);
        return;
      };
      fs::remove(tmp_file);
    }
    SECTION("Comparison Between Out and In Zeros") {
      test_sac.write(tmp_file);
      Trace in_sac = Trace(tmp_file);
      BENCHMARK("Trace Comparison") { (void)(test_sac == in_sac); };
      fs::remove(tmp_file);
    }
    SECTION("Randomizing data") {
      BENCHMARK("Random vector generation.") {
        std::vector<double> data{};
        data.resize(static_cast<size_t>(test_sac.npts()));
        random_vector(&data);
        return;
      };
    }
    SECTION("Comparison Between Out and In Random") {
      std::vector<double> data{};
      data.resize(static_cast<size_t>(test_sac.npts()));
      random_vector(&data);
      test_sac.data1(data);
      if (!test_sac.leven() || test_sac.iftype() > 1) {
        data.resize(test_sac.data2().size());
        random_vector(&data);
        test_sac.data2(data);
      }
      test_sac.write(tmp_file);
      Trace in_sac = Trace(tmp_file);
      // Note that this equality tests to equality within tolerance of
      // what can be handled via a float this is because binary SAC files
      // use floats for the data values, not doubles
      BENCHMARK("Trace Comparison") { (void)(test_sac == in_sac); };
      fs::remove(tmp_file);
    }
  }
}
}  // namespace sacfmt
// NOLINTEND(readability-function-cognitive-complexity)
