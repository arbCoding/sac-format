#ifndef SAC_SPECTRAL_H
#define SAC_SPECTRAL_H

// SAC::SacStream
#include "sac_stream.hpp"

// FFTW library
#include <fftw3.h>
// Needed by FFTW library
#include <cmath>

// These are separate from sac_stream.hpp/sac_stream.cpp because they rely on
// FFTW, I prefer for dependencies to be optional (as much as possible)
// So if you're not using these, there is not reason to compile/link them

namespace SAC
{
//-----------------------------------------------------------------------------
// FFTW functions
//-----------------------------------------------------------------------------
// How FFTW normalizes things: FFT -> 1; IFFT -> 1/N
//
// How I normalize things: FFT -> 1/sqrt(N); IFFT -> 1/sqrt(N)
//
// I prefer the symmetry of the 1/sqrt(N) normalization condition (makes
// interpretation of resultant units simpler)
//
// Accuracy:
//  using fft_real_imaginary() followed by ifft_real_imaginary()
//  reproduces the original signal better by about an order of magnitude
//  (one additional deicmal place) than
//  using fft_amplitude_phase() followed by ifft_amplitude_phase()
//
// So unless you specifically need amplitude/phase, you should use
// real/imaginary
//
// FFT that sets data1 to the real and data2 to the imaginary
// Requires iftype = 1 (SAC ITIME)
void fft_real_imaginary(SacStream& sac);
// Inverse-FFT for data1 = read, data2 = imaginary
// Requires iftype = 2 (SAC IRLIM)
void ifft_real_imaginary(SacStream& sac);
// FFT that sets data1 to the amplitude and data2 to the phase
// Requires iftype = 1 (SAC ITIME)
void fft_amplitude_phase(SacStream& sac);
// Requires iftype = 3 (SAC IAMPH)
// Inverse-FFT for data1 = amplitude, data2 = phase
void ifft_amplitude_phase(SacStream& sac);
//-----------------------------------------------------------------------------
// End FFTW functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Filters
//-----------------------------------------------------------------------------
// Note: These filters are RAW
// As in, not safe if you don't know what you're doing
// They don't care about the Nyquist frequency
// They don't care about the sampling rate
// They don't care about spectral leakage
// They don't care about windowing functions
// These are all things you'll need to apply yourself (for now)
// Lowpass filter (butterworth)
void lowpass(SacStream& sac, int order, double cutoff);
// Highpass filter (butterworth)
void highpass(SacStream& sac, int order, double cutoff);
// Bandpass filter (butterworth)
void bandpass(SacStream& sac, int order, double lowpass, double highpass);
//-----------------------------------------------------------------------------
// End Filters
//-----------------------------------------------------------------------------
}

#endif
