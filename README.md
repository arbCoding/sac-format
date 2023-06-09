# sac-format

---

## What is this

This repository contains libraries for reading/writing binary [SAC](https://ds.iris.edu/files/sac-manual/manual/file_format.html) files, and some example/test
programs.

I wrote these following the specified format, up to the limitations that the specification is incomplete (looking at you, XYZ files).

These are written in C++, following modern programming guidelines and should be a good starting point for your seismic data-analysis journey.

If you're interested in processing passive-source seismic data, I'm working on a GUI tool to do just that called [PsSp](https://github.com/arbCoding/PsSp)
(Passive-source Seismic-processing) that is built upon the libraries here.

---

## How to use

To use this in your own project, you will need to build the libaries and then link to them at compilation while including the headers.
See the section "How to build" for instructions on building.

If you want to use only low-level IO (converting to/from binary), then use ./src/objects/sac_io.o (header is sac_io.hpp).

If you want to use the high-level SacStream class, then use ./src/objects/sac_format.o (header is sac_stream.hpp).

The [header](src/header/) files are heavily commented, to provide insight into what the libaries do and why.

If you want to see the "how" then see the appropriate [implementation](src/implementation/) file.

---

## How to build

These are written in C++, using modern conventions, and are tested to compile with the G++ compiler. To build the libaries run:
```shell
make lib
```

To build everything, including the test programs, run:
```shell
make
```

You can look at the [Makefile](Makefile) for more details (I've tried to be as explicit as possible in the comments inside).

---

## Note

The default build is the release build `debug = false`. If, for whatever reason, you want to use the debug release, set `debug = true`.
I use `debug = true` when developing these tools, to enforce good coding practices.

---

## The Tests

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

---

## Little-Endian vs Big-Endian

MacOS is a little-endian system, meaning it expects binary values to start from the Least-Significant Bit (LSB). Linux is a big-endian system 
and expects binary values to start from the Most-Significan Bit (MSB). This means that MacOS reads/writes chunks of binary data from right-to-left, 
while on Linux it is done from left-to-right. For most I/O this was a non-issue. However, for strings it resulted in some amusing results.

For example: on MacOS, KEVNM header would be read in as "Central Californ" (16 character limit for that header) and Linux would read it as "fornCaliral Cent".
The difference is easy to understand if we make the following map:

1 = "Cent", 2 = "ral ", 3 = "Cali", 4 = "forn"

The Mac output is "1234", while Linux output is "4321". I have fixed this by checking system endianness in the string reading functions. 
If you get any funky results, you should confirm the endianness of the input file.

This was not an issue with any other portions of the data in the binary file, only the strings.

---

## To Do

- [X] Read [SAC](https://ds.iris.edu/files/sac-manual/manual/file_format.html) file format
- [X] Write SAC file format
- [X] Prinout SAC header information to command line
- [X] Implement SacStream class
- [X] Compatible with both v6 (legacy, several decades) or v7 (new 2020+) binary SAC files
- [X] Read irregularly sampled data
- [X] Read spectral data
- [X] Read general XY data
- [X] Little-endian/Big-endian reading compatibility (Mac/Linux)
- [X] Confirm Little-endian/Big-endian writing compatibility (Mac/Linux)
- [ ] Read general XYZ data (not enough information in specification to implement at the moment...)

---

## Organization

I have split functionality between 
* **Interface** files 
    - `./src/header/*.hpp`
* **Implementation** files
    - `./src/implementation/*.cpp`

The interface files have the definitions of the necessary constants, the function names and return types, and comments on what they do (but **not** the details).

If you're curious about the details of a function, you can find it in the implementation file that corresponds to its interface.
