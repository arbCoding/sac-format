#include "sac_spectral.hpp"

namespace SAC
{
//-----------------------------------------------------------------------------
// FFTW functions
//-----------------------------------------------------------------------------
void fft_real_imaginary(SacStream& sac)
{
  if (sac.iftype != 1)
  {
    std::cerr << "Incorrect iftype: expected `1` (SAC ITIME), received `" << sac.iftype << "`\n";
  }
  double* signal = (double*) fftw_malloc(sizeof(double) * sac.data1.size());
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    signal[i] = sac.data1[i];
  }
  // We'll do full size as it isn't a huge time savings to do only half
  // Can changed later if it becomes an issue
  fftw_complex* spectrum = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sac.data1.size());
  // Make the plan to calculate the FFT
  // r2c is always FFTW_FORWARD (real-to-complex)
  fftw_plan plan = fftw_plan_dft_r2c_1d(sac.npts, signal, spectrum, FFTW_ESTIMATE);
  // DO IT
  fftw_execute(plan);
  // Cleanup
  fftw_free(signal);
  // Destory the plan, if you want to do many one after the other (same size input)
  // don't destroy (it's faster to re-use!)
  fftw_destroy_plan(plan);
  fftw_cleanup();
  // Set the new values
  const double norm{std::sqrt(sac.npts)};
  sac.data2.resize(sac.data1.size());
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    sac.data1[i] = spectrum[i][0] / norm; // Real
    sac.data2[i] = spectrum[i][1] / norm; // Imaginary
  }
  fftw_free(spectrum);
  // Set to IRLIM data type
  sac.iftype = 2;
}

void ifft_real_imaginary(SacStream& sac)
{
  if (sac.iftype !=2)
  {
    std::cerr << "Incorrect iftype: expected `2` (SAC IRLIM), received `" << sac.iftype << "`\n";
    return;
  }
  fftw_complex* spectrum = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sac.data1.size());
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    spectrum[i][0] = sac.data1[i]; // Real
    spectrum[i][1] = sac.data2[i]; // Imaginary
  }
  // Clear contents and set to size 0
  sac.data2.clear();
  double* signal = (double*) fftw_malloc(sizeof(double) * sac.data1.size());
  // c2r is always FFTW_BACKWARD (complex-to-real)
  fftw_plan plan = fftw_plan_dft_c2r_1d(sac.npts, spectrum, signal, FFTW_ESTIMATE);
  fftw_execute(plan);
  fftw_free(spectrum);
  fftw_destroy_plan(plan);
  fftw_cleanup();
  const double norm{std::sqrt(sac.npts)};
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    sac.data1[i] = signal[i] / norm;
  }
  fftw_free(signal);
  // Set to ITIME data type
  sac.iftype = 1;
}

void fft_amplitude_phase(SacStream& sac)
{
  if (sac.iftype != 1)
  {
    std::cerr << "Incorrect iftype: expected `1` (SAC ITIME), received `" << sac.iftype << "`\n";
    return;
  }
  double* signal = (double*) fftw_malloc(sizeof(double) * sac.data1.size());
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    signal[i] = sac.data1[i];
  }
  // We'll do full size as it isn't a huge time savings to do only half
  // Can changed later if it becomes an issue
  fftw_complex* spectrum = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sac.data1.size());
  // Make the plan to calculate the FFT
  // r2c is always FFTW_FORWARD (real-to-complex)
  fftw_plan plan = fftw_plan_dft_r2c_1d(sac.npts, signal, spectrum, FFTW_ESTIMATE);
  // DO IT
  fftw_execute(plan);
  // Cleanup
  fftw_free(signal);
  // Destory the plan, if you want to do many one after the other (same size input) don't destroy (it's faster to re-use!)
  fftw_destroy_plan(plan);
  fftw_cleanup();
  // Set the new values
  const double norm{std::sqrt(sac.npts)};
  sac.data2.resize(sac.data1.size());
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    sac.data1[i] = std::sqrt(pow(spectrum[i][0], 2.0) + pow(spectrum[i][1], 2.0)) / norm; // Amplitude
    sac.data2[i] = atan2(spectrum[i][1], spectrum[i][0]); // Phase
  }
  fftw_free(spectrum);
  // Set to IAMPH data type
  sac.iftype = 3;
}

void ifft_amplitude_phase(SacStream& sac)
{
  if (sac.iftype != 3)
  {
    std::cerr << "Incorrect `iftype`: expected `3` (SAC IAMPH), received `" << sac.iftype << "`\n";
    return;
  }

  fftw_complex* spectrum = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sac.data1.size());
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    // Z = X + iY
    // |Z| = sqrt((X*X) + (Y*Y))
    // theta = atan(Y / X)
    // X = |Z| * cos(theta)
    // Y = |Z| * sin(theta)
    spectrum[i][0] = sac.data1[i] * cos(sac.data2[i]); // Real
    spectrum[i][1] = sac.data1[i] * sin(sac.data2[i]); // Imaginary
  }
  // Clear contents and set to size 0
  sac.data2.clear();
  double* signal = (double*) fftw_malloc(sizeof(double) * sac.data1.size());
  // c2r is always FFTW_BACKWARD (complex-to-real)
  fftw_plan plan = fftw_plan_dft_c2r_1d(sac.npts, spectrum, signal, FFTW_ESTIMATE);
  fftw_execute(plan);
  fftw_free(spectrum);
  fftw_destroy_plan(plan);
  fftw_cleanup();
  const double norm{std::sqrt(sac.npts)};
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    sac.data1[i] = signal[i] / norm;
  }
  fftw_free(signal);
  // Set to ITIME data type
  sac.iftype = 1;
}
//-----------------------------------------------------------------------------
// End FFTW functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Filters
//-----------------------------------------------------------------------------
void lowpass(SacStream& sac, int order, double cutoff)
{
  if (sac.iftype != 1)
  {
    std::cerr << "Incorrect `iftype`: expected 1 (SAC ITIME), received `" << sac.iftype << "`\n";
    return;
  }
  // FFT of our signal
  fft_real_imaginary(sac);
  double frequency{};
  const double sampling_freq{1.0 / sac.delta};
  const double freq_step{sampling_freq / sac.npts};
  double denominator{};
  double gain{};
  // Build and apply filter to each element
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    // Make filter for this frequency
    frequency = i * freq_step;
    denominator = std::pow(frequency / cutoff, 2.0 * order);
    // Splitting the steps across lines causes it to correctly use the sqrt overload for doubles
    denominator = std::sqrt(1.0 + denominator);
    //std::cout << "Denominator: " << denominator << '\n';
    gain = 1.0 / denominator;
    sac.data1[i] *= gain;
    sac.data2[i] *= gain;
  }
  // Return the filtered signal
  ifft_real_imaginary(sac);
}

void highpass(SacStream& sac, int order, double cutoff)
{
  if (sac.iftype != 1)
  {
    std::cerr << "Incorrect `iftype`: expected 1 (SAC ITIME), received `" << sac.iftype << "`\n";
    return;
  }
  fft_real_imaginary(sac);
  double frequency{};
  const double sampling_freq{1.0 / sac.delta};
  const double freq_step{sampling_freq / sac.npts};
  double denominator{};
  double gain{};
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    frequency = i * freq_step;
    denominator = std::pow(cutoff / frequency, 2.0 * order);
    denominator = std::sqrt(1.0 + denominator);
    gain = 1.0 / denominator;
    sac.data1[i] *= gain;
    sac.data2[i] *= gain;
  }
  ifft_real_imaginary(sac);
}

void bandpass(SacStream& sac, int order, double lowpass, double highpass)
{
  if (sac.iftype != 1)
  {
    std::cerr << "Incorrect `iftype`: expected 1 (SAC ITIME), received `" << sac.iftype << "`\n";
    return;
  }
  fft_real_imaginary(sac);
  double frequency{};
  const double sampling_freq{1.0 / sac.delta};
  const double freq_step{sampling_freq / sac.npts};
  double denominator{};
  double gain{};
  const double bandwidth = highpass - lowpass;
  const double central_freq = lowpass + (bandwidth / 2.0);
  for (std::size_t i{0}; i < sac.data1.size(); ++i)
  {
    frequency = i * freq_step;
    denominator = (frequency - central_freq) / (frequency - bandwidth);
    denominator = std::pow(denominator, 2.0 * order);
    denominator = std::sqrt(1.0 + denominator);
    gain = 1.0 / denominator;
    sac.data1[i] *= gain;
    sac.data2[i] *= gain;
  }
  ifft_real_imaginary(sac);
}
//-----------------------------------------------------------------------------
// End filters
//-----------------------------------------------------------------------------
}
