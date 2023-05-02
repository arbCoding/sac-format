# seismic

---

This repository has my C++ seismic processing codes.

Presently, SAC binary files are supported. Other formats may come later.

By default, the build process uses debug-mode, which has a lot of error checking and results
in longer compilation times, larger compiled programs and objects, and slower runtimes.

---

## Dependencies
**BEFORE COMPILING** make sure you have all **required** dependencies installed on your system. Be sure
to also check the **optional** dependencies to determine if you need them for your purposes.

While everything could be re-created from scratch, it is sometimes better to use pre-existing libraries.

This list of **required** dependencies is as follows:
    - None as of yet

The list of **optional** dependencies is as follows:
- [FFTW](https://www.fftw.org/)
    - This provides very-fast FFT functionality. See `./src/tests/sac_stream_fftw_test.cpp` to see it in action.
    - **IMPORTANT** If you're not using the spectral functions (fft, ifft, lowpass, highpass, bandpass), this is 
    unnecessary. You can still work with the SacStream class, or the low-level i/o functions in sac_io.hpp without
    installing FFTW3.

---

## Compiling
**BEFORE COMPILING** make sure to set your c++ compiler in the Makefile:

As stated in the Makefile, I use g++-12 (GCC version 12.2.0). GCC 13.1.0 will not compile under my debug paramters.
 It will, however, compile under my release parameters (set `debug=false` in the Makefile).

---

### ON COMPILERS 
I use g++, the Makefile works with g++. If you want to use clang *and* compile the debug
version (more on that below) you will need to remove the `-fanalyzer` argument from the parameter list

G++:
```Makefile
# G++
debug_param = -fanalyzer -Weffc++ -Wextra -Wsign-conversion -Werror -Wshadow -ggdb
```

Clang:
```Makefile
# Clang
debug_param = -Weffc++ -Wextra -Wsign_conversion -Werror -Wshadow -ggdb
```

---

### Debug vs Release
If you'd rather compile in release-mode you will need to edit the Makefile as follows.

Debug-Mode:
`debug = true`

Release-Mode:
`debug = false`

---

### Build tests
To build the test programs, in the top-level directory (same as this README.md file) run:
```shell
make tests
```

You can then find the test programs in the `./bin/tests/` folder.

The tests:
* `sac_type_test`
    - This does not take any input
    - This tests converting between standard types and binary, and back, following the SAC conventions.
* `sac_io_test`
    - This requires an input sac file
    - This tests opening a sac file (low-level io) and reading some of the header information, which it will spit out to the console.
* `sac_stream_read_test`
    - This requires an input sac file
    - This tests reading a sac file as a `SacStream` object (high-level io)
    - It prints out all the header/footer values to the console
* `sac_stream_write_test`
    - This requires an input sac file
    - This tests writing out a v7 (modern) sac file (write)
    - This tests writing out a v6 (legacy) sac file (legacy_write)
    - It prints out the names of the two new files to the console
* `sac_stream_fftw_test`
    - Calculates the FFT of the data1 component of the sac file (assumes a time-series)
    - It tests both to/from real/imaginary **and** amplitude/phase.
    - Requires FFTW.
* `sac_stream_lowpass_test`
    - Performs a low-pass filter on the data.
    - Requires FFTW.

---

### Build sac_format
If you're only interested in being able to read/write binary SAC-files without tests or anything else:
```shell
make sac_format
```

The above command will build `./src/objects/sac_format.o` which you can use in your program
(don't forget to include `./src/header` to have the `sac_io.hpp` and `sac_stream.hpp` interfaces).

This will build three object files:
* `sac_io.o`
    - low-level sac-file IO functions
* `sac_stream.o`
    - high-level SacStream class
* `sac_format.o`
    - Both `sac_io.o` and `sac_stream.o` combined (linked)
* `sac_spectral.o`
    - Spectral functionality (fft, ifft, lowpass, highpass, bandpass)
    - Requires FFTW
    - Optional

#### IMPORTANT 
If you *only* want **low-level** sac-file IO you *can* use `sac_io.o` exclusively (interface is `./src/header/sac_io.hpp`)

If you want **high-level** sac-file IO you *can* use `sac_io.o` and `sac_stream.o` (inferfaces in `./src/header/`)

**Preferred method**

Use `sac_format.o` and include the interfaces in `./src/header`. If you're using the high-level stuff,
you'll need the low-level stuff anyway (since it uses it behind the scenes), might as well have a shorter list of object files.

---

## Organization

I have split functionality between 
* **Interface** files 
    - `./src/header/*.hpp`
* **Implementation** files
    - `./src/implementation/*.cpp`

The interface files have the definitions of the necessary constants, the function names and return types, and comments on what they do (but **not** the details).

If you're curious about the details of a function, you can find it in the implementation file that corresponds to its interface.

---

## TODO:

- [X] Read [SAC](https://ds.iris.edu/files/sac-manual/manual/file_format.html) file format

- [X] Write SAC file format

- [X] Prinout SAC header information to command line

- [X] Implement SacStream class

- [X] Compatible with both v6 (legacy, several decades) or v7 (new 2020+) binary SAC files

- [X] Read irregularly sampled data

- [X] Read spectral data

- [X] Read general XY data

- [X] Forward- and inverse-fast Fourier transforms (using FFTW).

- [ ] Read general XYZ data (not enough information in specification to implement at the moment...)

- [X] Lowpass, highpass, bandpass Butterworth filters

- [X] Time-series preprocessing functions (demean, detrend)

- [ ] Geometric analysis functions (3C seismic data, rotating components)

- [ ] Geographic analysis functions (distance, great-circle arc (gcarc), azimuth, back-azimuth)

- [ ] Particle motion analysis (polarization)

- [ ] Time-series plotting

- [ ] Time-series stacking (Moveout stacking [PRF], slant stacking)

- [ ] Make installation/compilation easier (dependency checking, etc.)

---

## Why does this exist?

A few reasons:
* I wanted to sharpen my C++ skills.
* I've never written code to handle binary file IO before, I've only used code that already did it for me.
* Seemed like fun.

Also I feel that there are some issues with presently available seismological software these days:
* Documentation issues:
    - Undocumented (no documentation)
    - Underdocumented (incomplete documentation)
    - Incorrectly documented (it was correct **X years ago**, but unfortuantely it is no longer correct).
* Compilation issues:
    - Dated programming habits that are now considered bad-practices are all over the place.
        - Scientists don't often keep up on modern programming practices
        because programming is a necessary, but minor, component of their actual work. It doesn't matter if other people can understand it, or if it compiles
        without warnings, or if it compiles for anyone other than the person that wrote it, so long as it allows the author to do their work.
    - Won't compile on standard compilers
        - It's cool when people get to use super-fast/efficient compilers, I get it. For code very specific code, that will never get distributed, thats awesome.
        - My priority is for the code to be accessible. I use GCC because anyone cant get it, anywhere, for free, with relatively little effort/trouble.
    - Won't compile on modern hardware
        - (Generic scenario) Someone wrote some awesome code, X years ago, for the PowerPC MacOs (before Mac went to Intel, and definitely before they went to their mX cpu line). Too bad it's impossible/very difficult to run it on a modern computer (or a different OS).
