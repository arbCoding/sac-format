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
// So if you're not using these, there is no reason to compile/link them
//
//-----------------------------------------------------------------------------
// WARNING
//-----------------------------------------------------------------------------
// All of these functions have "side-effects". That means they modify the value
// of their input.
//
// Explanation:
// The input SacStream is passed by reference (SacStream&), not by value (SacStream).
// The computation is performed on the input SacStream in place.
//
// Reasoning:
// SacStreams are "big" objects. Copying them is not strictly desired when using
// these spectral functions. Might as work in place, unless you specifically want
// untouched copies.
//
// * If you want to keep a an unmodified copy of the data on hand:
//  I recommend copying the `data1` and/or `data2` std::vector<double>
//  objects manually on your own. <- There is not need to copy everything else.
//
// * Futhermore. If you don't plan on modifying the original copies of `data1` and `data1`:
//  Make the copies constants (opens up more possible compiler optimizations).
//
//  * For an idea of how one would do this, see `./src/tests/sac_stream_fftw_test.cpp`
//  Where I make a constant copy of the `data1` vector before performing an FFT+IFFT cycle
//  to determine how accurately the FFT+IFFT reproduces the original data.
//-----------------------------------------------------------------------------
// End WARNING
//-----------------------------------------------------------------------------

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
//  Using fft_real_imaginary() followed by ifft_real_imaginary()
//  reproduces the original signal better by about an order of magnitude
//  (one additional deicmal place) than
//  using fft_amplitude_phase() followed by ifft_amplitude_phase()
//
// Guess at cause:
//  I suspect the loss in accuracy comes from using the trig functions
//  from <cmath>. They likely use a fast polynomial expansion to calculate
//  the values that just happens to have lower numerical accuracy than the
//  FFT and IFFT routines.
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
// As in, not safe if you don't know what you're doing (helpful, right?)
// They don't care about the Nyquist frequency (meaning they don't check for aliasing)
// They don't care about the sampling rate (related to the nyquist frequency issue)
// They are designed assuming evenly sampled data.
// They don't care about spectral leakage (aliasing, wrap-around)
// They don't care about windowing functions (minimizing edge-effects)
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
