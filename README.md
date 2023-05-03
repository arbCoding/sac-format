# seismic

---

## What is this

This repository contains my code and libraries for reading/writing binary [SAC](https://ds.iris.edu/files/sac-manual/manual/file_format.html) files.

I wrote these following the specified format, up to the limitations that the specification is incomplete (looking at you, XYZ files).

These are written in C++, following modern programming guidelines and should be a good starting point for your seismic data-analysis journey.

---

## How to use

To use this in your own project, you will need to build the libaries and then link to them at compilation while including the headers.
See the section "How to build" for instructions on building.

If you want to use only low-level IO (converting to/from binary), then use sac_io.a (header is sac_io.hpp).

If you want to use the high-level SacStream class, then use sac_format.a (header is sac_stream.hpp).

The [header](src/header/) files are heavily commented, to provide insight into what the libaries do and why.

If you want to see the "how" then see the appropriate files the (implementation)[src/implementation/] folder.

---

## How to build

These are written in C++, using modern conventions, and are tested to compile with the G++ compiler. To build the libaries run:
```
make lib
```

To build everything, including the test programs, run:
```
make
```

You can look at the Makefile for more details (I've tried to be as explicit as possible in the comments inside).

---
