// Reading/writing sac files, plus Sac_Class
#include "sac_class.hpp"
// Low-level reading and writing functions
// including conversion to/from binary...
#include "sac_io.hpp"

// This program tests converting from binary to variables
// using the SAC binary data format specification
// See: https://ds.iris.edu/files/sac-manual/file_format.html
// for details

int main()
{
  std::string file_name = "../data/IM.NV31..BHZ.M.2023.094.222304.SAC";
  std::cout << "File:\t\t" << file_name << '\n';
  SAC::Sac_Class sac(file_name);

  // Now for convesion tests...
  // Positive integer to binary and back
  std::cout << "\nNPTS:\t" << sac.npts << '\n';
  std::bitset<SAC::binary_word_size> bits{SAC::int_to_binary(sac.npts)};
  std::cout << "Binary:\t" << bits << '\n';
  std::cout << "Back:\t" << SAC::binary_to_int(bits) << '\n';
  // Negative integer to binary and back
  std::cout << "\nNwfID:\t" << sac.nwfid << '\n';
  bits = SAC::int_to_binary(sac.nwfid);
  std::cout << "Binary:\t" << bits << '\n';
  std::cout << "Back:\t" << SAC::binary_to_int(bits) << '\n';

  return 0;
}
