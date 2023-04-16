#include <iostream>
#include <cmath>
#include <fftw3.h>

int main()
{
  constexpr int N{128}; // Number of samples
  fftw_complex signal[N]; // Input signal
  fftw_complex spectrum[N]; // Output spectrum

  // Fill the input signal with some example data
  for (int i{0}; i < N; ++i) 
  {
    signal[i][0] = std::cos(2.0 * M_PI * i / N);
    signal[i][1] = 0.0;
  }

  // Create a FFTW plan to calculate the forward FFT
  fftw_plan plan = fftw_plan_dft_1d(N, signal, spectrum, FFTW_FORWARD, FFTW_ESTIMATE);

  // Execute the plan to calculate the FFT
  fftw_execute(plan);

  // Print the output spectrum
  for (int i{0}; i < N; ++i)
  {
    std::cout << "spectrum[" << i << "] = " << spectrum[i][0] << " + " << spectrum[i][1] << "i\n";
  }

  // Destroy the FFTW plan and free the memory
  fftw_destroy_plan(plan);
  fftw_cleanup();

  return 0;
}
