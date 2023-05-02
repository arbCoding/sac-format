// SacClass
#include "sac_stream.hpp"
#include "sac_spectral.hpp"

#include <iostream>
#include <string>
#include <fstream>
// FFTW is working
// Note that this is a raw FFT (no windowing, no tapering, no nothing)
// Of note:
// fft_real_imaginary() followed by ifft_real_imaginary() better reproduces the signal than
// fft_amplitude_phase() followed by ifft_amplitude_phase() by about 1 order of magnitude
// (one additional decimal place)

int main(int arg_count, char* arg_array[])
{
  if (arg_count != 2)
  {
    std::printf("Usage: %s <file>\n", arg_array[0]);
    return 1;
  }
  
  const std::string file_name{arg_array[1]};
  std::cout << "Reading file:\t" << file_name << '\n';
  SAC::SacStream sac(file_name);
  std::cout << "Success!\n\n";

  std::cout << "Size of data1:\t" << sac.data1.size() << " points\n";
  std::cout << "Preparing to perform FFT...\n\n";
  const std::vector<double> data1_copy{sac.data1};
  // Forward
  std::cout << "Forward (Real/Imaginary)!\n";
  SAC::fft_real_imaginary(sac);
  
  // Let's spit it out to a CSV for plotting since I have yet to find a nice C++ plotting library that doesn't break
  // my project
  const std::string csv_name{"./fftw_real_imaginary.csv"};
  // If it exists, overwrite it (do not append)
  std::ofstream csv_file(csv_name, std::ios::trunc);
  if (!csv_file.is_open())
  {
    std::cerr << "Failed to open: " << csv_name << " for the fft results...\n";
    return 1;
  }

  std::cout << "Writing the results of the FFT to: " << csv_name << '\n';

  double frequency{};
  const double sampling_freq{1.0 / sac.delta};
  const double freq_step{sampling_freq / sac.npts};
  csv_file << "Frequency,Real,Imaginary\n";
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    frequency = i * freq_step;
    csv_file << frequency << ',' << sac.data1[i] << ',' << sac.data2[i] << '\n';
  }
  csv_file.close();

  // Inverse
  std::cout << "Inverse (Real/Imaginary)!\n";
  SAC::ifft_real_imaginary(sac);

  bool same{true};
  // Equality tolerance from FFT/IFFT
  constexpr double tolerance{1e-10};
  double diff{};
  double max_diff{0.0};
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    diff = std::abs(sac.data1[i] - data1_copy[i]);
    if (diff > max_diff)
    {
      max_diff = diff;
    }
  }

  if (max_diff > tolerance)
  {
    same = false;
  }

  std::cout << "Equal to Original? (Real/Imaginary)? " << (same ? "true" : "false") << '\n';
  std::cout << "Maximum difference: " << max_diff << '\n';

  std::cout << "\nForward! (Amplitude/Phase)\n";
  SAC::fft_amplitude_phase(sac);

  // Let's spit it out to a CSV for plotting since I have yet to find a nice C++ plotting library that doesn't break
  // my project
  const std::string csv_name2{"./fftw_amplitude_phase.csv"};
  // If it exists, overwrite it (do not append)
  std::ofstream csv_file2(csv_name2, std::ios::trunc);
  if (!csv_file2.is_open())
  {
    std::cerr << "Failed to open: " << csv_name2 << " for the fft results...\n";
    return 1;
  }

  std::cout << "Writing the results of the FFT to: " << csv_name2 << '\n';

  csv_file2 << "Frequency,Amplitude,Phase\n";
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    frequency = i * freq_step;
    csv_file2 << frequency << ',' << sac.data1[i] << ',' << sac.data2[i] << '\n';
  }
  csv_file2.close();

  std::cout << "Inverse! (Amplitude/Phase)\n";
  SAC::ifft_amplitude_phase(sac);
  same = true;
  max_diff = 0.0;
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    diff = std::abs(sac.data1[i] - data1_copy[i]);
    if (diff > max_diff)
    {
      max_diff = diff;
    }
  }

  if (max_diff > tolerance)
  {
    same = false;
  }

  std::cout << "Equal to Original? (Amplitude/Phase)? " << (same ? "true" : "false") << '\n';
  std::cout << "Maximum difference: " << max_diff << '\n';

  return 0;
}
