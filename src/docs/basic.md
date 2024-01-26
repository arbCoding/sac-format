# Basic Documentation {#BasicDocumentation}

## Trace class

The `Trace` class provides easy access to SAC-files in C++. Each SAC-file is a
`Trace`; therefore, each `Trace` object is a seismic trace (seismogram).

### Reading SAC

SAC-files can be read in by using the parameterized constructor with a
[`std::filesystem::path`](https://en.cppreference.com/w/cpp/filesystem/path)
([`<filesystem>`](https://en.cppreference.com/w/cpp/header/filesystem)) or a
[`std::string`](https://en.cppreference.com/w/cpp/string/basic_string)
([`<string>`](https://en.cppreference.com/w/cpp/header/string)) variable that
corresponds to the location of the SAC-file.

For example:

```cpp
#include <sac_foramt.hpp>
#include <filesystem>

int main() {
  std::filesystem::path my_file{"/home/user/data/ANMO.SAC"};
  sacfmt::Trace anmo{sacfmt::Trace(my_file)};
  return EXIT_SUCCESS;
}
```


### Writing SAC

Writing SAC files can be done using one of two write functions.

#### v7 files

Use `write` (for example `trace.write(filename)`).

#### v6 files

Use `legacy_write` (for example `trace.legacy_write(filename)`).

### Getters and Setters

Every [SAC variable](#SAC-file%20format) is accessed via getters and setters of
the same name.

#### Example Getters

- `trace.npts()`
- `trace.data1()`
- `trace.kstnm()`

#### Example Setters

- `trace.kevnm("Event 1")`
- `trace.evla(32.89)`
- `trace.mag(3.21)`

#### Setter rules

Most of the setters are only constrained by the parameter type
(single-precision, double-precision, boolean, etc.). **Some** setters are
constrained by additional rules.

##### Required for sanity

Rules here are required because the sac-format library assumes them (not strictly required by the SAC format standard). For instance, the geometric functions assume certain bounds on latitudes and longitudes.

sac-format automatically imposes these rules.

###### `stla(input)`

Limited to [-90, 90] degrees, input that is outside that range is reduced
using circular symmetry.

###### `stlo(input)`

Limited to [-180, 180] degrees, input that is outside that range is reduced
using circular symmetry.

###### `evla(input)`

Limited to [-90, 90] degrees, input that is outside that range is reduced
using circular symmetry.

###### `evlo(input)`

Limited to [-180, 180] degrees, input that is outside that range is reduced
using circular symmetry.

##### Required for safety

Rules here are required by the SAC format standard. sac-format automatically
imposes these rules to prevent the creation of corrupt sac-files.

###### `npts(input)`

Because `npts` defines the size of the data vectors, changing this value will
change the size of `data1` and `data2`&lowast;. Increasing npts resizes the
vectors
([std::vector::resize](https://en.cppreference.com/w/cpp/container/vector/resize))
by placing zeros at the **end** of the vectors. Reducing npts resizes the
vectors down to the **first npts** values.

Therefore, care must be taken to maintain separate copies of `data1` and
`data2`&lowast; if you plan to manipulate the original data **after** resizing.

&lowast; data2 has `npts` only if it is legal, otherwise it is of size 0.

###### `leven(input)`

Changing the value of `leven` potentially changes the legality of `data2`, it
also potentially affects the value of `iftype`.

If iftype>1, then leven must be `true` (evenly sampled data). Therefore, if
leven is made `false` in this scenario (unevenly sampled data) then iftype
becomes unset&lowast;.

If changing leven makes data2 legal&lowast;&lowast;, then data2 is qresized to
have `npts` zeros.

&lowast; The SAC format defines the unset values for all data-types. For
integers (like iftype) it is the integer value `-12345`.

&lowast;&lowast; If data2 was already legal, then it is unaffected.

###### `iftype(input)`

Changing the value of `iftype` poentially changes the legality of `data2`, it
also potentially affects the value of `leven`.

If leven is `false`, then iftype must be either 1 or unset. Therefore, changing
iftype to have a value >1 requires that leven becomes `true` (evenly sampled
data).
            
If changing iftype makes data2 legal&lowast;, then data2 is resized to have
`npts` zeros.
            
&lowast; If data2 was already legal, then it is unaffected.
        
###### `data1(input)`
        
If the size of `data1` is changed, then `npts` must change to reflect the new
size. If `data2` is legal, this adjusts its size to match as well.
        
###### `data2(input)`
        
If the size of `data2` is changed to be larger than 0 and it is illegal, it is
made legal by setting `iftype(2)` (spectral-data).
            
When the size of data2 changes, `npts` is updated to the new size and `data1` is
resized to match.
            
If `data2` is made illegal, its size is reduced to 0 while `npts` and `data1`
are unaffected.

### Convenience Methods

#### calc_geometry

Calculate `gcarc`, `dist`, `az`, and `baz` assuming spherical Earth.

```cpp
trace.stla(45.3);
trace.stlo(34.5);
trace.evla(18.5);
trace.evlo(-34);
trace.calc_geometry();
std::cout << "GcArc: " << trace.gcarc() << '\n';
std::cout << "Dist: " << trace.dist() << '\n';
std::cout << "Azimuth: " << trace.az() << '\n';
std::cout << "BAzimuth: " << trace.baz() << '\n';
```

#### frequency

Calculate frequency from `delta`.

```cpp
double frequency{trace.frequency()};
```

#### date

Return `std::string` formatted as `YYYY-JJJ` from `nzyear` and `nzjday`.

```cpp
std::string date{trace.date()};
```

#### time

Return `std::string` formatted as `HH:MM:SS.xxx` from `nzhour`, `nzmin`,
`nzsec`, and `nzmsec`.

```cpp
std::string time{trace.time()};
```

### Exceptions

sac-format throws exceptions of type `sacfmt::io_error` (inherits
[`std::exception`](https://en.cppreference.com/w/cpp/error/exception)) in the
event of a failure to read/write a SAC-file.

## Convenience Functions

### degrees_to_radians

Convert decimal degrees to radians.

```cpp
double radians{sacfmt::degrees_to_radians(degrees)};
```

### radians_to_degrees

Convert radians to decimal degrees.

```cpp
double degrees{sacfmt::radians_to_degrees(radians)};
```

### gcarc

Calculate great-circle arc distance (spherical planet).

```cpp
double gcarc{sacfmt::gcarc(latitude1, longitude1, latitude2, longitude2)};
```

### azimuth

Calculate azimuth between two points (spherical planet).

```cpp
double azimuth{sacfmt::azimuth(latitude2, longitude2, latitude1, longitude1)};
double back_azimuth{sacfmt::azimuth(latitude1, longitude1, latitude2, longitude2)};
```

### limit_360

Take arbitrary value of degrees and unwrap to [0, 360].

```cpp
double degrees_limited{sacfmt::limit_360(degrees)};
```

### limit_180

Take arbitrary value of degrees and unwrap to [-180, 180]. Useful for
longitude.

```cpp
double degrees_limited{sacfmt::limit_180(degrees)};
```

### limit_90

Take arbitrary value of degrees and unwrap to [-90, 90]. Useful for latitude.

```cpp
double degrees_limited{sacfmt::limit_90(degrees)};
```

## Low-Level I/O

Low-level I/O functions are discussed below.

### Binary conversion

#### int_to_binary and binary_to_int

Conversion pair for binary representation of integer values.

```cpp
const int input{10};
// sacfmt::word_one is alias for std::bitset<32> (one word)
sacfmt::word_one binary{sacfmt::int_to_binary(input)};
const int output{sacfmt::binary_to_int(binary)};
std::cout << (input == output) << '\n';
```

#### float_to_binary and binary_to_float

Conversion pair for binary representation of floating-point values.

```cpp
const float input{5F};
sacfmt::word_one binary{sacfmt::float_to_binary(input)};
const float output{sacfmt::binary_to_float(binary)};
std::cout << (input == output) << '\n';
```

#### double_to_binary and binary_to_double

Conversion pair for binary representation of double-precision values.

```cpp
const double input{1e5};
// sacfmt::word_two is alias for std::bitset<64> (two words)
sacfmt::word_two binary{sacfmt::double_to_binary(input)};
const double output{sacfmt::binary_to_double(binary)};
std::cout << (input == output) << '\n';
```

#### string_to_binary and binary_to_string

Conversion pair for binary representation of two-word (regular) string values.

```cpp
const std::string input{"NmlStrng"};
sacfmt::word_two binary{sacfmt::string_to_binary(input)};
const std::string output{sacfmt::binary_to_string(binary)};
std::cout << (input == output) << '\n';
```

#### long_string_to_binary and binary_to_long_string

Conversion pair for binary representation of four-word (only `kstnm` string
values.

```cpp
const std::string input{"The Long String"};
// sacfmt::word_four is alias for std::bitset<128> (four words)
sacfmt::word_four binary{sacfmt::long_string_to_binary(input)};
const std::string output{sacfmt::binary_to_long_string(binary)};
std::cout << (input == output) << '\n';
```

### Reading/Writing

**NOTE** that care must be taken when using them to ensure that safe input is
provided; the `Trace` class ensures safe I/O, low-level I/O functions do not
necessarily ensure safety.

#### read_word, read_two_words, read_four_words, and read_data

Functions to read one-, two-, and four-word variables (depending on the header)
and an arbitrary amount of binary data (exclusive to `data1` and `data2`).

#### convert_to_word, convert_to_words, and bool_to_word

Takes objects and converts them into `std::vector<char>` (`convert_to_word` and
`bool_to_word`) or `std::array<char, N>` (`convert_to_words`, N = # of words).

#### write_words

Writes input words (as `std::vector<char>`) to a binary SAC-file.

### Utility

#### concat_words

Concatenates words taking into account the system endianness.

#### bits_string and string_bits

Template function that performs conversion of binary strings of arbitrary length
to an arbitrary number of words.

#### remove_leading_spaces and remove_trailing_spaces

Remove leading and trailing blank spaces from strings assuming ASCII convention
(space character is integer 32, below that value are control characters that
also appear as blank spaces).

#### string_cleaning

Ensures string does not contain an internal termination character (`\0`) and
removes it if present, then removes blank spaces.

#### prep_string

Performs `string_cleaning` followed by string truncation/padding to the
necessary length.

#### equal_within_tolerance

Floating-point/double-precision equality within a provided tolerance (default is
`f_eps`, defined in `sac_format.hpp`).

## Testing

`utests.cpp` contains the unit- and integration-tests, using Catch2. Test
coverage details are visible on
[CodeCov.io](https://app.codecov.io/gh/arbCoding/sac-format) and
[Codacy.com](https://app.codacy.com/gh/arbCoding/sac-format/dashboard). All
tests can be locally-run to ensure full functionality and compliance.

### Errors only

By default `utests` prints out a pass summary, without details unless an error
is encountered.

### Full output

By passing the `--success` flag (`utests --success`) you can see the full
results of all tests.

### Compact output

The [full output](#Full%20output) is verbose, using the compact reporter will
condense the test results (`utests --reporter=compact --success`).

### Additional options

To see additional options, run `utests -?`.

### Using ctest

If you have CMake install, you can run the tests using `ctest`.

## Benchmarking

`benchmark.cpp` contains the benchmarks. Running it locally will provide
information on how long each function takes; benchmarks start with the low-level
I/O function and build up to Trace reading, writing, and equality comparison.

To view available optional flags, run `becnhmark -?`.

## Source File List

### Core

The two core files are split in the standard interface (hpp)/implementation
(cpp) format.

1.  `sac_format.hpp`

    Interface: function declarations and constants.

2.  `sac_format.cpp`

    Implementation: function details.

### Testing and Benchmarking

1.  `util.hpp`

    Utility functions and constants exclusive to testing and benchmarking. Not
    split into interface/implementation.

2.  `utests.cpp`

3.  `benchmark.cpp`

### Example programs

1.  `list_sac.cpp`
