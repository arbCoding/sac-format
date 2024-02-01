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

Pre-data word 001.

Minimum value of the dependent variable
(displacement/velocity/acceleration/volts/counts).

### depmen

Pre-data word 057.

Mean value of the dependent variable.

### depmax

Pre-data word 002.

Maximum value of the dependent variable.

### odelta

Pre-data word 004.

Modified (*observational*) value of `delta`.

### resp(0--9)

Pre-data words 021--030.

Instrument response parameters (poles, zeros, and a constant).

**Not used by SAC** they're free for other purposes.

### stel

Pre-data word 033.

Station elevation in meters above sea level (*m.a.s.l*).

**Not used by SAC** free for other purposes.

### stdp

Pre-data word 034.

Station depth in meters below surface (borehole/buried vault).

**Not used by SAC** free for other purposes.

### evel

Pre-data word 037.

Event elevation *m.a.s.l.*

**Not used by SAC** free for other purposes.

### evdp

Pre-data word 038.

Event depth in kilometers (*previously meters*) below surface.

### mag

Pre-data word 039.

Event magnitude.

### user(0--9)

Pre-data words 040--049.

Storage for user-defined values.

### dist

Pre-data word 050.

Station-Event distance in kilometers.

### az

Pre-data word 051.

Azimuth \f$\color{orange}\mathrm{\left(Event \to Station\right)}\f$, decimal
degrees from North.

### baz

Pre-data word 052.

Back-azimuth \f$\color{orange}\mathrm{\left(Station \to Event\right)}\f$,
decimal degrees from North.

### gcarc

Pre-data word 053.

Station-Event great circle arc-length, decimal degrees.

### cmpaz

Pre-data word 057.

Instrument measurement azimuth, decimal degrees from North.

| Value    | Direction |
|----------|-----------|
| 0&deg;   | North     |
| 90&deg;  | East      |
| 180&deg; | South     |
| 270&deg; | West      |
| Other    | 1/2/3     |

### cmpinc

Pre-data word 058.

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

Pre-data word 059.

Spectral-only equivalent of `depmin` (\f$\color{orange}f_{0}\f$ or
\f$\color{orange}\omega_{0}\f$).

### xmaximum

Pre-data word 060.

Spectral-only equivalent of `depmax` (\f$\color{orange}f_{max}\f$ or
\f$\color{orange}\omega_{max}\f$).

### yminimum

Pre-data word 061.

Spectral-only equivalent of `b`.

### ymaximum

Pre-data word 062.

Spectral-only equivalent of `e`.

## Double (22)

64-bit (2 words, 8 bytes)

**NOTE:** in the header section these are floats; they're doubles in
the footer section of `v7` SAC-files. In memory they're stored as doubles
regardless of the SAC-file version.

### delta

Pre-data word 000, post-data words 00-01.

Increment between evenly spaced samples (\f$\color{orange}\Delta t\f$ for
timeseries, \f$\color{orange}\Delta f\f$ or \f$\color{orange}\Delta\omega\f$ for
spectra).

### b

Pre-data word 005, post-data words 02-03.

First value (*begin*) of independent variable (\f$\color{orange}t_{0}\f$).

### e

Pre-data word 006, post-data words 04-05.

Final value (*end*) of independent variable (\f$\color{orange}t_{max}\f$).

### o

Pre-data word 007, post-data words 06-07.

Event *origin* time, in seconds relative to the reference time.

### a

Pre-data word 008, post-data words 08-09.

Event first *arrival* time, in seconds relative to the reference time.

### t(0--9)

Pre-data words 010--019, post-data words 10--29.

User defined *time* values, in seconds relative to the reference time.

### f

Pre-data word 020, post-data words 30-31.

Event end (*fini*) time, in seconds relative to the reference time.

### stla

Pre-data word 031, post-data words 36-37.

Station latitude in decimal degrees, N/S - positive/negative.

sac-format automatically enforces \f$\color{orange}\mathrm{stla}\in[-90, 90]\f$.

### stlo

Pre-data word 032, post-data words 38-39.

Station longitude in decimal degrees, E/W - positive/negative.

sac-format automatically enforces \f$\color{orange}\mathrm{stlo}\in[-180,
180]\f$.

### evla

Pre-data word 035, post-data words 32-33.

Event latitude in decimal degrees, N/S - positive/negative.

sac-format automatically enforces \f$\color{orange}\mathrm{evla}\in[-90, 90]\f$.

### evlo

Pre-data word 036, post-data words 34-35.

Event longitude in decimal degrees, E/W - positive/negative.

sac-format automatically enforces \f$\color{orange}\mathrm{evlo}\in[-180,
180]\f$.

### sb

Pre-data word 054, post-data words 40-41.

Original (*saved*) `b` value.

### sdelta

Pre-data word 055, post-data words 42-43.

Original (*saved*) `delta` value.

## Integer (26)

32-bit (1 word, 4 bytes)

### nzyear

Pre-data word 070.

Reference time GMT year.

### nzjday

Pre-data word 071.

Reference time GMT day-of-year (often called [Julian
Date](https://en.wikipedia.org/wiki/Julian_day)) (1--366).

### nzhour

Pre-data word 072.

Reference time GMT hour (0--23).

### nzmin

Pre-data word 073.

Reference time GMT minute (0--59).

### nzsec

Pre-data word 074.

Reference time GMT second (0--59).

### nzmsec

Pre-data word 075.

Reference time GMT Millisecond (0--999).

### nvhdr

Pre-data word 076.

SAC-file version.

| Version | Description                       |
|---------|-----------------------------------|
| `v7`    | Footer (2020+, sac 102.0+)        |
| `v6`    | No footer (pre-2020, sac 101.6a-) |

### norid

Pre-data word 077.

Origin ID.

### nevid

Pre-data word 078.

Event ID.

### npts

Pre-data word 079.

*Number of points* in data.

### nsnpts

Pre-data word 080.

Original (*saved*) `npts`.

### nwfid

Pre-data word 081.

Waveform ID.

### nxsize

Pre-data word 082.

Spectral-only equivalent of `npts` (length of spectrum).

### nysize

Pre-data word 083.

Spectral-only, width of spectrum.

### iftype

Pre-data word 085.

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

Pre-data word 086.

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

Pre-data word 087.

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

Pre-data word 089.

Recording instrument type.

**Not used by SAC**: free for other purposes.

### istreg

Pre-data word 090.

Station geographic region.

**Not used by SAC**: free for other purposes.

### ievreg

Pre-data word 091.

Event geographic region.

**Not used by SAC**: free for other purposes.

### ievtyp

Pre-data word 092.

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

Pre-data word 093.

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

Pre-data word 094.

Synthetic data flag.

| Value | Type     | Description |
|-------|----------|-------------|
| 49    | IRLDATA  | Real data   |
| XX    | &lowast; | Synthetic   |

&lowast;Values and types not listed in the standard.

### imagtyp

Pre-data word 095.

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

Pre-data word 096.

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

Pre-data word 097.

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

Pre-data word 105.

32-bit (1 word, 4 bytes) in-file/8-bit (1 byte) in-memory

### leven

Pre-data word 106.

**REQUIRED** Evenly-spaced data flag.

If true, then data is evenly spaced.

### lpspol

Pre-data word 107.

Station polarity flag.

If true, then station has positive-polarity; it follows the left-hand convention
(for example, North-East-Up [NEZ]).

### lovrok

Pre-data word 108.

File overwrite flag.

If true, then it's okay to overwrite the file.

### lcalda

Pre-data word 109.

Calculate geometry flag.

If true, then calculate `dist`, `az`, `baz`, and `gcarc` from `stla`, `stlo`,
`evla`, and `evlo`.

## String (23)

32/64-bit (2/4 words, 8/16 bytes, 8/16 characters)

### kstnm

Pre-data words 110--111.

Station name.

### kevnm

Pre-data words 112--115.

Event name.

&lowast;This is the **only** four word (16 character) string.

### khole

Pre-data words 116--117.

Nuclear: Hole identifier.

Other: Location identifier (LOCID).

### ko

Pre-data words 118--119.

Text for `o`.

### ka

Pre-data words 120--121.

Text for `a`.

### kt(0--9)

Pre-data words 112--141.

Text for `t(0--9)`.

### kf

Pre-data words 142--143.

Text for `f`.

### kuser(0--2)

Pre-data words 144--149.

Text for the first three of `user(0--9)`.

### kcmpnm

Pre-data words 150--151.

Component name.

### knetwk

Pre-data words 152--153.

Network name.

### kdatrd

Pre-data words 154--155.

Date the data was read onto a computer.

### kinst

Pre-data words 156--157.

Text for `iinst`.

## Data (2)

32-bit (2 words, 8 bytes) in-file/64-bit (4 words, 16 bytes) in-memory

Stored as floating-point (32-bit) values in SAC-files; stored as
double-precision in memory.

### data1

Words 158--(158 + npts)

The first data vector---**always** present in a SAC-file and begins at word 158.

### data2

Words (158 + 1 + npts)--(159 + (2 * npts))

The second data vector---**conditionally** present and begins after `data1`.

**Required** if `leven` is false, or if `iftype` is spectral/XY/XYZ.
