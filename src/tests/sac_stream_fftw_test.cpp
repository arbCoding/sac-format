// SacClass
#include "sac_stream.hpp"

#include <iostream>
#include <string>
// File IO
#include <fstream>
// FFTW
#include <cmath>
#include <fftw3.h>

// FFTW is working
// Note that there this is a raw FFT (no windowing, nothing)

int main(int arg_count, char *arg_array[])
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

  // Dynamic memory allocation since the size is not known at compile-time
  //fftw_complex* signal = new fftw_complex[n_points]; // apparently bad form as it won't be contiguous in memory
  // fftw_malloc is btter
  fftw_complex* signal = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sac.data1.size());
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    // static_cast to double to be sure it uses doubles instead of trying to use float
    signal[i][0] = static_cast<double>(sac.data1[i]);
    // Our signal is real!
    signal[i][1] = 0.0;
  }

  // More dynamic memory allocation since the size is not known at compile-time
  fftw_complex* spectrum = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sac.data1.size());
  // Make the plan to calculate the FFT
  fftw_plan plan = fftw_plan_dft_1d(sac.npts, signal, spectrum, FFTW_FORWARD,  FFTW_ESTIMATE);
  // DO IT
  fftw_execute(plan);
  // Cleanup
  fftw_destroy_plan(plan);
  fftw_cleanup();
  // Release memory related to signal
  //delete[] signal; // for non-malloc (bad form) version
  // For malloc verison
  fftw_free(signal);
  std::cout << "FFT successfully calculated!\n\n";

  // Let's spit it out to a CSV for plotting since I have yet to find a nice C++ plotting library that doesn't break
  // my project
  const std::string csv_name{"./fftw.csv"};
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
  const int n_freqs{(sac.npts / 2) + 1};
  csv_file << "Frequency,Real,Imaginary\n";
  for (int i{0}; i < n_freqs; ++i)
  {
    frequency = i * freq_step;
    csv_file << frequency << ',' << spectrum[i][0] << ',' << spectrum[i][1] << '\n';
  }
  // Release memory related to spectrum
  fftw_free(spectrum);

  return 0;
}
