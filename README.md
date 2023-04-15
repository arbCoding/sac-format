# seismic

---

This repository has my C++ seismic processing codes.

Presently, SAC binary files are supported. Other formats may come later.

By default, the build process uses debug-mode, which has a lot of error checking and results
in longer compilation times, larger compiled programs and objects, and slower runtimes.

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

To build the test programs, in the top-level directory (same as this README.md file) run:
```shell
make tests
```

If you're only interested in being able to read/write binary SAC-files without tests or anything else:
```shell
make sac_format
```

The above command will build ./src/objects/sac_format.o which you can use in your program
(don't forget to include ./src/header to have the sac_io.hpp and sac_stream.hpp interfaces).

TODO:
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
