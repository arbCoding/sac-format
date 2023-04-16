// Preferred for handling sac data
#include "sac_stream.hpp"
// Temporary for testing
//#include "sac_io.hpp"

#include <iostream>
#include <string>
#include <vector>

int main(int arg_count, char *arg_array[])
{
  if (arg_count != 2)
  {
    std::printf("Usage: `%s <file>`\n", arg_array[0]);
    return 1;
  }

  std::string file_name{arg_array[1]};
  // ObsPy cannot handle v7 data yet (thinks file is wrong size lol)
  std::cout << "File:\t\t" << file_name << '\n';
  SAC::SacStream sac(file_name);

  //---------------------------------------------------------------------------
  // Test Writing sac data
  //---------------------------------------------------------------------------
  // Build new names for our two output files from the name of the input file
  std::string prefix{};
  std::string suffix{};
  std::size_t final_period{file_name.find_last_of('.')};
  std::string v6_file{};
  std::string v7_file{};
  if (final_period != std::string::npos)
  {
    prefix = file_name.substr(0, final_period);
    suffix = file_name.substr(final_period + 1, file_name.size() - final_period - 1);
    v6_file = prefix + ".v6." + suffix;
    v7_file = prefix + ".v7." + suffix;
  }
  else
  {
    v6_file = file_name + ".v6";
    v7_file = file_name + ".v7";
  }

  std::cout << "\nWriting v7 (modern) file:\t" << v7_file << '\n';
  sac.write(v7_file);
  std::cout << "Success!\n";

  std::cout << "\nWriting v6 (legacy) file:\t" << v6_file << '\n';
  sac.legacy_write(v6_file);
  std::cout << "Success!\n";

  return 0;
}
