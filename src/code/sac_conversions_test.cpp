// Low-level reading and writing functions
// including conversion to/from binary...
#include "sac_io.hpp"

// This program tests converting from binary to variables
// using the SAC binary data format specification
// See: https://ds.iris.edu/files/sac-manual/file_format.html
// for details

int main()
{
  std::cout << "Integer tests..." << '\n';
  
  std::cout << "Positive integer:\n";
  constexpr int i1{12345};
  std::cout << "\tTest:\t" << i1 << '\n';
  std::bitset<SAC::binary_word_size> one_word{};
  one_word = SAC::int_to_binary(i1);
  std::cout << "\tBinary:\t" << one_word << '\n';
  std::cout << "\tBack:\t" << SAC::binary_to_int(one_word) << '\n';
  std::cout << "\tPass:\t" << (i1 == SAC::binary_to_int(one_word)) << '\n';

  std::cout << "\nNegative integer:\n";
  constexpr int i2{-12345};
  std::cout << "\tTest:\t" << i2 << '\n';
  one_word = SAC::int_to_binary(i2);
  std::cout << "\tBinary:\t" << one_word << '\n';
  std::cout << "\tBack:\t" << SAC::binary_to_int(one_word) << '\n';
  std::cout << "\tPass:\t" << (i2 == SAC::binary_to_int(one_word)) << '\n';

  std::cout << "\nFloat tests..." << '\n';
  std::cout << "Positive float:\n";
  constexpr float f1{1.2345f};
  std::cout << "\tTest:\t" << f1 << '\n';
  one_word = SAC::float_to_binary(f1);
  std::cout << "\tBinary:\t" << one_word << '\n';
  std::cout << "\tBack:\t" << SAC::binary_to_float(one_word) << '\n';
  std::cout << "\tPass:\t" << (f1 == SAC::binary_to_float(one_word)) << '\n';

  std::cout << "\nNegative float:\n";
  constexpr float f2{-1.2345f};
  std::cout << "\tTest:\t" << f2 << '\n';
  one_word = SAC::float_to_binary(f2);
  std::cout << "\tBinary:\t" << one_word << '\n';
  std::cout << "\tBack:\t" << SAC::binary_to_float(one_word) << '\n';
  std::cout << "\tPass:\t" << (f2 == SAC::binary_to_float(one_word)) << '\n';

  std::cout << "\nDouble tests..." << '\n';
  std::cout << "Positive double:\n";
  constexpr double d1{1.2345};
  std::cout << "\tTest:\t" << d1 << '\n';
  std::bitset<2 * SAC::binary_word_size> two_words{};
  two_words = SAC::double_to_binary(d1);
  std::cout << "\tBinary:\t" << two_words << '\n';
  std::cout << "\tBack:\t" << SAC::binary_to_double(two_words) << '\n';
  std::cout << "\tPass:\t" << (d1 == SAC::binary_to_double(two_words)) << '\n';

  std::cout << "\nNegative double:\n";
  constexpr double d2{-1.2345};
  std::cout << "\tTest:\t" << d2 << '\n';
  two_words = SAC::double_to_binary(d2);
  std::cout << "\tBinary:\t" << two_words << '\n';
  std::cout << "\tBack:\t" << SAC::binary_to_double(two_words) << '\n';
  std::cout << "\tPass:\t" << (d2 == SAC::binary_to_double(two_words)) << '\n';

  std::cout << "\nString tests..." << '\n';
  std::cout << "\nPerfect two word string:\n";
  const std::string s1{"01234567"};
  std::cout << "\tTest:\t" << s1 << '\n';
  two_words = SAC::string_to_binary(s1);
  std::cout << "\tBinary:\t" << two_words << '\n';
  std::cout << "\tBack:\t" << SAC::binary_to_string(two_words) << '\n';
  std::cout << "\tPass:\t" << (s1 == SAC::binary_to_string(two_words)) << '\n';

  std::cout << "\nShort two word string:\n";
  const std::string s2{"0123"};
  std::cout << "\tTest:\t" << s2 << '\n';
  two_words = SAC::string_to_binary(s2);
  std::cout << "\tBinary:\t" << two_words << '\n';
  std::cout << "\tBack:\t" << SAC::binary_to_string(two_words) << '\n';
  // After removing the padding they should match
  std::string tmp{SAC::binary_to_string(two_words)};
  std::cout << "\tPass:\t" << (s2 == tmp.substr(0, s2.length())) << '\n';

  std::cout << "\nLong two word string:\n";
  const std::string s3{"01234567891234"};
  std::cout << "\tTest:\t" << s3 << '\n';
  two_words = SAC::string_to_binary(s3);
  std::cout << "\tBinary:\t" << two_words << '\n';
  std::cout << "\tBack:\t" << SAC::binary_to_string(two_words) << '\n';
  // Because the strings have limited final length defined in the SAC format
  // The test here is only if it matches the truncated string
  std::cout << "\tPass:\t" << (s1 == SAC::binary_to_string(two_words)) << '\n';

  std::cout << "\nPerfect four word string:\n";
  const std::string s4{"ABCDEFGHIJKLMNOP"};
  std::cout << "\tTest:\t" << s4 << '\n';
  std::bitset<4 * SAC::binary_word_size> four_words{};
  four_words = SAC::long_string_to_binary(s4);
  std::cout << "\tBinary:\t" << four_words << '\n';
  std::cout << "\tBack:\t" << SAC::binary_to_long_string(four_words) << '\n';
  std::cout << "\tPass:\t" << (s4 == SAC::binary_to_long_string(four_words)) << '\n';

  std::cout << "\nShort four word string:\n";
  const std::string s5{"ABCDEF"};
  std::cout << "\tTest:\t" << s5 << '\n';
  four_words = SAC::long_string_to_binary(s5);
  std::cout << "\tBinary:\t" << four_words << '\n';
  std::cout << "\tBack:\t" << SAC::binary_to_long_string(four_words) << '\n';
  // After removing padding they should match
  tmp = SAC::binary_to_long_string(four_words);
  std::cout << "\tPass:\t" << (s5 == tmp.substr(0, s5.length())) << '\n';

  std::cout << "\nLong four word string:\n";
  const std::string s6{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
  std::cout << "\tTest:\t" << s6 << '\n';
  four_words = SAC::long_string_to_binary(s6);
  std::cout << "\tBinary:\t" << four_words << '\n';
  std::cout << "\tBack:\t" << SAC::binary_to_long_string(four_words) << '\n';
  // Must match up with the truncated version
  std::cout << "\tPass:\t" << (s4 == SAC::binary_to_long_string(four_words)) << '\n';

  return 0;
}
