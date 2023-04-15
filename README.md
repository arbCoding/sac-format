# seismic

---

This repository has my C++ seismic processing codes.

Presently, SAC binary files are supported. Other formats may come later.

By default, the build process uses debug-mode, which has a lot of error checking and results
in longer compilation times, larger compiled programs and objects, and slower runtimes.

---

## Compiling
**BEFORE COMPILING** make sure to set your c++ compiler in the Makefile:
```Makefile
# Linux or Mac
uname_s := $(shell uname -s)

ifeq ($(uname_s), Linux)
	compiler = g++
else
	compiler = g++-12 # Homebrew g++ install I have
#	compiler = clang
endif
```
You can see in the above snippet that for linux I use the default g++ compiler and on Mac I use
the g++-12 compiler I installed via homebrew. If you have a different compiler, you'll need to set
the compiler variable yourself.

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

Original:
```Makefile
# Debug version is substantially larger file size (and slower runtime)
# Debug compilation is extremely strict (warnings = errors)
debug = true
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Release version is substantially smaller file size (and faster runtime)
# Release compilation is extremely relaxed (possible bugs if using to by-pass failed debug compilation...)
#debug = false
```

New:
```Makefile
# Debug version is substantially larger file size (and slower runtime)
# Debug compilation is extremely strict (warnings = errors)
#debug = true
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Release version is substantially smaller file size (and faster runtime)
# Release compilation is extremely relaxed (possible bugs if using to by-pass failed debug compilation...)
debug = false
```

---

### Build tests
To build the test programs, in the top-level directory (same as this README.md file) run:
```shell
make tests
```

---

### Build sac_format
If you're only interested in being able to read/write binary SAC-files without tests or anything else:
```shell
make sac_format
```

The above command will build ./src/objects/sac_format.o which you can use in your program
(don't forget to include ./src/header to have the sac_io.hpp and sac_stream.hpp interfaces).

This will build three object files:
* sac_io.o - low-level sac-file IO functions
* sac_stream.o - high-level SacStream class
* sac_format.o - Both sac_io.o and sac_stream.o combined

#### IMPORTANT 
If you *only* want **low-level** sac-file IO you *can* use `sac_io.o` exclusively (interface in `./src/header/sac_io.hpp`)

If you want **high-level** sac-file IO you *can* use `sac_io.o` and `sac_stream.o` (inferfaces in `./src/header/`)

**Preferred method** Use `sac_format.o` and include the interfaces in `./src/header`. If you're using the high-level stuff,
you'll need the low-level anyway (since it uses it behind the scenes), so you might as well shrink your list of object files.

---

## Organization

I have split functionality between *interface* files `./src/header/*.hpp` and *implementation* files `./src/implementation/.*cpp`.

The interface files have the constant definitions, the function names and return types, and comments on what they do (but **not** the details).

If you're curious of the details on a function, you can find it in the implementation file that corresponds to its interface.

---

## TODO:

- [X] Read [SAC](https://ds.iris.edu/files/sac-manual/manual/file_format.html) file format

- [X] Write [SAC] file format

- [X] Prinout SAC header information to command line

- [X] Implement SacStream class

- [X] Compatible with both v6 (legacy, several decades) or v7 (new 2020+) binary SAC files

- [X] Read irregularly sampled data

- [X] Read spectral data

- [ ] Time-series analysis functions (demean, detrend, filtering, convolution, deconvolution, etc.)

- [ ] Geometric analysis functions (3C seismic data, rotating components)

- [ ] Geographic analysis functions (distance, great-circle arc (gcarc), azimuth, back-azimuth)

- [ ] Particle motion analysis (polarization)

- [ ] Time-series plotting

- [ ] Time-series stacking (Moveout stacking [PRF], slant stacking)

---

## Why does this exist?

A few reasons:
* I learn best not by following lessons etc., but by programming actual projects.
* I wanted to sharpen my C++ skills.
* I dislike the fact that IRIS requires a form to be filled out in order to receive SAC and its headers (and sometimes they refuse).
* I was unable to quickly get their IO files to work for myself, so I thought it'd be fun to write my own.
* I've never written code to handle binary file IO before, I've only used code that already did it for me.

Also I feel that there are a lot of problems with the seismological software available these days:
* Documentation issues:
    - Undocumented (no documentation)
    - Underdocumented (incomplete documentation)
    - Incorrectly documented (sure, it was correct **20 years ago**, but you've changed things since and never bothered to make sure other people can use it).
* Compilation issues:
    - Sloppy programming results in numerous errors and undefined behavior (too many warnings, etc.).
        - Sure the **author** can compile it on their system, but what about **everyone** else?
    - Won't compile on standard compilers
        - Love seeing quotes like "If you manage to get this to work on gfortran, please let me know" (since they're using some proprietary compiler).
    - Sometimes languages updated without being backwards compatible. Or sometimes a program was written specifically for PowerPC Macintosh computers in the mid-2000's.
        - Too bad that means it is either **extremely painful** to make the software usable (with much time and effort), or impossible.
* Compatibily issues
    - Oh, you've altered the format to have additional headers that you haven't documented (that are not standard). Now when I try to read it with some other software it crashes. **THANKS**
    - Or, cannot handle the new v7 sac format (it came out in 2020, it's currently 2023).
* Age:
    - A lot of scientific software is dated, extremely dated. Many very smart people have spent a lot of time thinking and implementing new ideas in the realm of programming.
    Yet, in science, you'll still see a lot of **new** code written in extremely old standards (like FORTRAN77 for instance, not even modern FORTRAN...).
    - This means that there are better ways to write code, that is clear and robust, to do the tasks we need done. Let's try to actually use that progress.
