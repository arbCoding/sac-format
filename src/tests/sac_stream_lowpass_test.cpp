#include "sac_stream.hpp"
#include "sac_spectral.hpp"

#include <iostream>
#include <string>

int main(int arg_count, char* arg_array[])
{
  if (arg_count != 4)
  {
    std::printf("Usage: %s <file> <order> <cutoff>\n", arg_array[0]);
    return 1;
  }
  if (*arg_array[2] <= 0)
  {
    std::cout << "Error: Filter order must be >= 1, given: " << arg_array[2] << '\n';
    return 1;
  }
  if (*arg_array[3] <= 0.0)
  {
    std::cout << "Error: Cutoff frequency must be positive, given: " << arg_array[3] << '\n';
    return 1;
  }

  const std::string file_name{arg_array[1]};
  std::cout << "Reading file:\t" << file_name << '\n';
  SAC::SacStream sac(file_name);
  std::cout << "Success!\n\n";

  const int order{std::stoi(arg_array[2])};
  const double cutoff{std::stod(arg_array[3])};
  std::cout << "Butterworth lowpass filter: order " << order << ", cutoff " << cutoff << '\n';
  SAC::lowpass(sac, order, cutoff);

  std::string prefix{};
  std::string suffix{};
  std::size_t final_period{file_name.find_last_of('.')};
  std::string v6_file{};
  if (final_period != std::string::npos)
  {
    prefix = file_name.substr(0, final_period);
    suffix = file_name.substr(final_period + 1, file_name.size() - final_period - 1);
    v6_file = prefix + ".lp.v6." + suffix;
  }
  else
  {
    v6_file = file_name + ".lp.v6";
  }

  std::cout << "\nWriting v6 (legacy) file:\t" << v6_file << '\n';
  sac.legacy_write(v6_file);
  std::cout << "Success!\n";


  return 0;
}
