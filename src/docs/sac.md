# SAC-file format {#SacFileFormat}

@brief This section provides a centralized description of the SAC file format.

The official and up-to-date documentation for the SAC-file format is available
from the EarthScope Consortium (formerly IRIS/UNAVCO)
[here](https://ds.iris.edu/files/sac-manual/manual/file_format.html). The
following subsections constitute my notes on the format. Below is a quick
guide: all credit for the creation of, and documentation for, the SAC
file-format belongs to its developers and maintainers (details
[here](https://ds.iris.edu/ds/nodes/dmc/software/downloads/sac/)).

## Floating-point (39)

32-bit (1 word, 4 bytes)

### depmin

Minimum value of the dependent variable
(displacement/velocity/acceleration/volts/counts).

### depmen

Mean value of the dependent variable.

### depmax

Maximum value of the dependent variable.

### odelta

Modified (*observational*) value of `delta`.

### resp(0--9)

Instrument response parameters (poles, zeros, and a constant).

**Not used by SAC** they're free for other purposes.

### stel

Station elevation in meters above sea level (*m.a.s.l*).

**Not used by SAC** free for other purposes.

### stdp

Station depth in meters below surface (borehole/buried vault).

**Not used by SAC** free for other purposes.

### evel

Event elevation *m.a.s.l.*

**Not used by SAC** free for other purposes.

### evdp

Event depth in kilometers (*previously meters*) below surface.

### mag

Event magnitude.

### user(0--9)

Storage for user-defined values.

### dist

Station-Event distance in kilometers.

### az

Azimuth \f$\color{orange}\mathrm{\left(Event \to Station\right)}\f$, decimal
degrees from North.

### baz

Back-azimuth \f$\color{orange}\mathrm{\left(Station \to Event\right)}\f$,
decimal degrees from North.

### gcarc

Station-Event great circle arc-length, decimal degrees.

### cmpaz

Instrument measurement azimuth, decimal degrees from North.

| Value    | Direction |
|----------|-----------|
| 0&deg;   | North     |
| 90&deg;  | East      |
| 180&deg; | South     |
| 270&deg; | West      |
| Other    | 1/2/3     |

### cmpinc

Instrument measurement incident angle, decimal degrees from upward vertical
(incident 0&deg; = dip -90&deg;).

| Value    | Direction  |
|----------|------------|
| 0&deg;   | Up         |
| 90&deg;  | Horizontal |
| 180&deg; | Down       |
| 270&deg; | Horizontal |

**NOTE:** SEED/MINISEED use dip angle, decimal degrees down from horizontal (dip
0&deg; = incident 90&deg;).

### xminimum

Spectral-only equivalent of `depmin` (\f$\color{orange}f_{0}\f$ or
\f$\color{orange}\omega_{0}\f$).

### xmaximum

Spectral-only equivalent of `depmax` (\f$\color{orange}f_{max}\f$ or
\f$\color{orange}\omega_{max}\f$).

### yminimum

Spectral-only equivalent of `b`.

### ymaximum

Spectral-only equivalent of `e`.

## Double (22)

64-bit (2 words, 8 bytes)

**NOTE:** in the header section these are floats; they're doubles in
the footer section of `v7` SAC-files. In memory they're stored as doubles
regardless of the SAC-file version.

### delta

Increment between evenly spaced samples (\f$\color{orange}\Delta t\f$ for
timeseries, \f$\color{orange}\Delta f\f$ or \f$\color{orange}\Delta\omega\f$ for
spectra).

### b

First value (*begin*) of independent variable (\f$\color{orange}t_{0}\f$).

### e

Final value (*end*) of independent variable (\f$\color{orange}t_{max}\f$).

### o

Event *origin* time, in seconds relative to the reference time.

### a

Event first *arrival* time, in seconds relative to the reference time.

### t(0--9)

User defined *time* values, in seconds relative to the reference time.

### f

Event end (*fini*) time, in seconds relative to the reference time.

### stla

Station latitude in decimal degrees, N/S - positive/negative.

sac-format automatically enforces \f$\color{orange}\mathrm{stla}\in[-90, 90]\f$.

### stlo

Station longitude in decimal degrees, E/W - positive/negative.

sac-format automatically enforces \f$\color{orange}\mathrm{stlo}\in[-180,
180]\f$.

### evla

Event latitude in decimal degrees, N/S - positive/negative.

sac-format automatically enforces \f$\color{orange}\mathrm{evla}\in[-90, 90]\f$.

### evlo

Event longitude in decimal degrees, E/W - positive/negative.

sac-format automatically enforces \f$\color{orange}\mathrm{evlo}\in[-180,
180]\f$.

### sb

Original (*saved*) `b` value.

### sdelta

Original (*saved*) `delta` value.

## Integer (26)

32-bit (1 word, 4 bytes)

### nzyear

Reference time GMT year.

### nzjday

Reference time GMT day-of-year (often called [Julian
Date](https://en.wikipedia.org/wiki/Julian_day)) (1--366).

### nzhour

Reference time GMT hour (0--23).

### nzmin

Reference time GMT minute (0--59).

### nzsec

Reference time GMT second (0--59).

### nzmsec

Reference time GMT Millisecond (0--999).

### nvhdr

SAC-file version.

| Version | Description                       |
|---------|-----------------------------------|
| `v7`    | Footer (2020+, sac 102.0+)        |
| `v6`    | No footer (pre-2020, sac 101.6a-) |

### norid

Origin ID.

### nevid

Event ID.

### npts

*Number of points* in data.

### nsnpts

Original (*saved*) `npts`.

### nwfid

Waveform ID.

### nxsize

Spectral-only equivalent of `npts` (length of spectrum).

### nysize

Spectral-only, width of spectrum.

### iftype

File type.

| Value | Type         | Description                |
|-------|--------------|----------------------------|
| 01    | ITIME        | Time-series                |
| 02    | IRLIM        | Spectral (real/imaginary)  |
| 03    | IAMPH        | Spectral (amplitude/phase) |
| 04    | IXY          | General XY file            |
| ??    | IXYZ&lowast; | General XYZ file           |

&lowast;Value not listed in the standard.

### idep

Dependent variable type.

<!-- markdownlint-disable line-length -->
| Value | Type   | Description                                                             |
|-------|--------|-------------------------------------------------------------------------|
| 05    | IUNKN  | Unknown                                                                 |
| 06    | IDISP  | Displacement (nm)                                                       |
| 07 | IVEL | Velocity \f$\color{orange}\mathrm{\left(\frac{nm}{s}\right)}\f$ |
| 08 | IACC | Acceleration \f$\color{orange}\mathrm{\left(\frac{nm}{s^{2}}\right)}\f$ |
| 50    | IVOLTS | Velocity (volts)                                                        |
<!-- markdownlint-enable line-length -->

### iztype

Reference time equivalent.

| Value        | Type           | Description                |
|--------------|----------------|----------------------------|
| 05           | IUNKN          | Unknown                    |
| 09           | IB             | Recording start time       |
| 10           | IDAY           | Midnight reference GMT day |
| 11           | IO             | Event origin time          |
| 12           | IA             | First arrival time         |
| 13-22 | IT(0-9) | User defined time (t) pick |

### iinst

Recording instrument type.

**Not used by SAC**: free for other purposes.

### istreg

Station geographic region.

**Not used by SAC**: free for other purposes.

### ievreg

Event geographic region.

**Not used by SAC**: free for other purposes.

### ievtyp

Event type.

| Value | Type   | Description                                               |
|-------|--------|-----------------------------------------------------------|
| 05    | IUNKN  | Unknown                                                   |
| 11    | IO     | Other source of known origin                              |
| 37    | INUCL  | Nuclear                                                   |
| 38    | IPREN  | Nuclear pre-shot                                          |
| 39    | IPOSTN | Nuclear post-shot                                         |
| 40    | IQUAKE | Earthquake                                                |
| 41    | IPREQ  | Foreshock                                                 |
| 42    | IPOSTQ | Aftershock                                                |
| 43    | ICHEM  | Chemical explosion                                        |
| 44    | IOTHER | Other                                                     |
| 72    | IQB    | Quarry/mine blast: confirmed by quarry/mine         |
| 73    | IQB1   | Quarry/mine blast: designed shot info-ripple fired  |
| 74    | IQB2   | Quarry/mine blast: observed shot info-ripple fired  |
| 75    | IQBX   | Quarry/mine blast: single shot                      |
| 76    | IQMT   | Quarry/mining induced events: tremor and rockbursts |
| 77    | IEQ    | Earthquake                                                |
| 78    | IEQ1   | Earthquake in a swarm or in an aftershock sequence        |
| 79    | IEQ2   | Felt earthquake                                           |
| 80    | IME    | Marine explosion                                          |
| 81    | IEX    | Other explosion                                           |
| 82    | INU    | Nuclear explosion                                         |
| 83    | INC    | Nuclear cavity collapse                                   |
| 85    | IL     | Local event of unknown origin                             |
| 86    | IR     | Region event of unknown origin                            |
| 87    | IT     | Teleseismic event of unknown origin                       |
| 88    | IU     | Undetermined/conflicting information                      |

### iqual

Quality of data.

| Value | Type   | Description               |
|-------|--------|---------------------------|
| 44    | IOTHER | Other                     |
| 45    | IGOOD  | Good                      |
| 46    | IGLCH  | Glitches                  |
| 47    | IDROP  | Dropouts                  |
| 48    | ILOWSN | Low signal-to-noise ratio |

**Not used by SAC**: free for other purposes.

### isynth

Synthetic data flag.

| Value | Type     | Description |
|-------|----------|-------------|
| 49    | IRLDATA  | Real data   |
| XX    | &lowast; | Synthetic   |

&lowast;Values and types not listed in the standard.

### imagtyp

Magnitude type.

| Value | Type | Description                      |
|-------|------|----------------------------------|
| 52    | IMB  | Body-wave magnitude (\f$\color{orange}M_{b}\f$)    |
| 53    | IMS  | Surface-wave magnitude (\f$\color{orange}M_{s}\f$) |
| 54    | IML  | Local magnitude (\f$\color{orange}M_{l}\f$)        |
| 55    | IMW  | Moment magnitude (\f$\color{orange}M_{w}\f$)       |
| 56    | IMD  | Duration magnitude (\f$\color{orange}M_{d}\f$)     |
| 57    | IMX  | User-defined magnitude (\f$\color{orange}M_{x}\f$) |

### imagsrc

Source of magnitude information.

| Value | Type     | Description                            |
|-------|----------|----------------------------------------|
| 58    | INEIC    | National Earthquake Information Center |
| 61    | IPDE     | Preliminary Determination of Epicenter |
| 62    | IISC     | Internation Seismological Centre       |
| 63    | IREB     | Reviewed Event Bulletin                |
| 64    | IUSGS    | U.S. Geological Survey                 |
| 65    | IBRK     | UC Berkeley                            |
| 66    | ICALTECH | California Institute of Technology     |
| 67    | ILLNL    | Lawrence Livermore National Laboratory |
| 68    | IEVLOC   | Event location (computer program)      |
| 69    | IJSOP    | Joint Seismic Observation Program      |
| 70    | IUSER    | The user                               |
| 71    | IUNKNOWN | Unknown                                |

### ibody

Body/spheroid definition used to calculate distances.

| Value | Type | Name | Semi-major axis (a [m]) | Inverse Flattening (*f*) |
|--------|----------|--------------------|-------------------------|----------|
| -12345 | UNDEF | Earth (*Historic*) | 6378160.0 | 0.00335293 |
| 98 | ISUN | Sun | 696000000.0 | 8.189e-6 |
| 99 | IMERCURY | Mercury | 2439700.0 | 0.0 |
| 100 | IVENUS | Venus | 6051800.0 | 0.0 |
| 101 | IEARTH | Earth (*WGS84*) | 6378137.0 | 0.0033528106647474805 |
| 102 | IMOON | Moon | 1737400.0 | 0.0 |
| 103 | IMARS | Mars | 3396190.0 | 0.005886007555525457 |

## Boolean (4)

32-bit (1 word, 4 bytes) in-file/8-bit (1 byte) in-memory

### leven

**REQUIRED** Evenly-spaced data flag.

If true, then data is evenly spaced.

### lpspol

Station polarity flag.

If true, then station has positive-polarity; it follows the left-hand convention
(for example, North-East-Up [NEZ]).

### lovrok

File overwrite flag.

If true, then it's okay to overwrite the file.

### lcalda

Calculate geometry flag.

If true, then calculate `dist`, `az`, `baz`, and `gcarc` from `stla`, `stlo`,
`evla`, and `evlo`.

## String (23)

32/64-bit (2/4 words, 8/16 bytes, 8/16 characters)

### kstnm

Station name.

### kevnm

Event name.

&lowast;This is the **only** four word (16 character) string.

### khole

Nuclear: Hole identifier.

Other: Location identifier (LOCID).

### ko

Text for `o`.

### ka

Text for `a`.

### kt(0--9)

Text for `t(0--9)`.

### kf

Text for `f`.

### kuser(0--2)

Text for the first three of `user(0--9)`.

### kdatrd

Date the data was read onto a computer.

### kinst

Text for `iinst`.

## Data (2)

32-bit (2 words, 8 bytes) in-file/64-bit (4 words, 16 bytes) in-memory

Stored as floating-point (32-bit) values in SAC-files; stored as
double-precision in memory.

### data1

The first data vector---**always** present in a SAC-file and begins at word 158.

### data2

The second data vector---**conditionally** present and begins after `data1`.

**Required** if `leven` is false, or if `iftype` is spectral/XY/XYZ.
