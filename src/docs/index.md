# Introduction {#mainpage}

sac-format is a single-header statically linked library designed to make working
with binary
[SAC](https://ds.iris.edu/files/sac-manual/manual/file_format.html)-files as
easy as possible. Written in C++20, it follows a modern and easy to read
programming-style while providing the high performance brought by C++.

sac-format's developed on [GitHub](https://github.com/arbCoding/sac-format)!

Download [sac-format](https://github.com/arbCoding/sac-format/releases) from the
GitHub release page.

[Download](https://arbcoding.github.io/sac-format/pdf/sac-format_manual.pdf) an
offline version of the documentation (PDF).

Get [help](https://github.com/arbCoding/sac-format/discussions/) from the
community forum.

## Why sac-format

sac-format is Free and Open Source Software (FOSS) released under the MIT
license. Anyone can use it, for any purpose (including proprietary software),
anywhere in the world. sac-format is operating system agnostic and confirmed
working on Windows, macOS, and Linux systems.

### Safe

sac-format is **safe** it conforms to a strict set of C++ programming
guidelines, chosen to ensure safe code-execution. The guideline conformance list
is in
[`cpp-linter.yml`](https://github.com/arbCoding/sac-format/blob/main/.github/workflows/cpp-linter.yml)
and can be cross-referenced against this [master
list](https://clang.llvm.org/extra/clang-tidy/checks/list.html). Results of
conformance checking are
[here](https://github.com/arbCoding/sac-format/actions/workflows/cpp-linter.yml).

Testing is an important part of software development; the sac-format library is
extensively tested using the [Catch2](https://github.com/catchorg/Catch2)
testing framework. Everything from low-level binary conversions to high-level
`Trace` reading/writing are tested and confirmed working. Check and run the
tests yourself. See the [Testing](@ref BasicDocumentation) section for more
information.

### Fast

sac-format is **fast** it's written in C++, carefully optimized, and extensively
benchmarked. You can run the benchmarks yourself to find out how sac-format
performs on your system. See the [Benchmarking](@ref BasicDocumentation) section
for more information.

### Easy

sac-format is **easy** single-header makes integration in any project
simple. Installation is easy with our automatic installers. Building is a breeze
with [CMake](https://cmake.org/), even on different platforms. Object-oriented
design makes use easy and intuitive. See the [Quickstart](@ref Quickstart) section
to get up and running.

### Small

sac-format is **small** in total (header +
implementation; excluding comments) the library is under 2300&lowast;
lines of code. Small size opens the door to using on any sort of hardware (old
or new) and makes it easy to expand upon.

&lowast; This value includes only the library, excluding all
testing/benchmarking and example codes. Including `utests.cpp`, `benchmark.cpp`,
`util.hpp`, the example program (`list_sac`), and sac-format totals just under
5500 lines of code.

### Documented

sac-format is extensively **documented** both online and in the code.
Nothing's hidden, nothing's obscured. Curious how something
works? Check the documentation and in-code comments.

### Transparent

sac-format is **transparent** all analysis and coverage information is
publicly available online.

- [CodeFactor](https://www.codefactor.io/repository/github/arbcoding/sac-format)
- [Codacy](https://app.codacy.com/gh/arbCoding/sac-format/dashboard)
- [CodeCov](https://app.codecov.io/gh/arbCoding/sac-format)
- [Coverity Scan](https://scan.coverity.com/projects/arbcoding-sac-format)

### Trace Class

sac-format includes the `Trace` class for seismic traces, providing high-level
object-oriented abstraction to seismic data. With the `Trace` class, you
don't need to worry about manually reading SAC-files word-by-word.
It's compatible with `v6` and `v7` SAC-files and can automatically detect
the version upon reading. File output defaults to `v7` SAC-files and there is a
`legacy_write` function for `v6` output.

### Low-Level I/O

If you want to roll your own SAC-file processing workflow you can use the
low-level I/O functionality built into sac-format. All functions tested and
confirmed working they're used to build the `Trace` class!
