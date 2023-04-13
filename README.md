# seismic

---

The purpose is to work on seismic processing in C++.

I'll start off with the stuff I'm comfortable with (SAC files, PRF calculation)
and branch out from there.

TODO:
- [X] Read [SAC](https://ds.iris.edu/files/sac-manual/manual/file_format.html) file format

- [X] Write [SAC] file format

- [X] Prinout SAC header information to command line

- [X] Implement Sac_Class

- [ ] Split Sac_Class between Time-series and spectral types

- [ ] Handle v7 sac data (currently only v6 supported, requires 22 double-precision footer values)

- [ ] Handle irregularly sampled data (currently only evenly sampled data supported, requires reading two data vectors (dependent/independent))

- [ ] Handle Spectral data (currently only time-series handled, requires reading two data vectors (real/imaginary or amplitude/phase))

- [ ] Time-series analysis functions (demean, detrend, filtering, convolution, deconvolution, etc.)

- [ ] Geometric analysis functions (3C seismic data, rotating components)

- [ ] Geographic analysis functions (distance, great-circle arc (gcarc), azimuth, back-azimuth)

- [ ] Particle motion analysis (polarization)

- [ ] Time-series plotting

- [ ] Time-series stacking (Moveout stacking [PRF], slant stacking)
