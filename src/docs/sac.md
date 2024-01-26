# SAC-file format {#SacFileFormat}

The official and up-to-date documentation for the SAC-file format is available
from the EarthScope Consortium (formerly IRIS/UNAVCO)
[here](https://ds.iris.edu/files/sac-manual/manual/file_format.html). The
following subsections constitute my notes on the format. Below is a quick
guide&#x2014;all credit for the creation of, and documentation for, the SAC
file-format belongs to its developers and maintainers (details
[here](https://ds.iris.edu/ds/nodes/dmc/software/downloads/sac/)).

## Floating-point (39)

32-bit (1 word, 4 bytes)

1.  `depmin`

    Minimum value of the dependent variable
    (displacement/velocity/acceleration/volts/counts).

2.  `depmen`

    Mean value of the dependent variable.

3.  `depmax`

    Maximum value of the dependent variable.

4.  `odelta`

    Modified (*observational*) value of [`delta`](#%3Ddelta%3D).

5.  `resp(0--9)`

    Instrument response parameters (poles, zeros, and a constant).
    
    **Not used by SAC**&#x2014;they&rsquo;re free for other purposes.

6.  `stel`

    Station elevation in meters above sea level (*m.a.s.l*).
    
    **Not used by SAC**&#x2014;free for other purposes.

7.  `stdp`

    Station depth in meters below surface (borehole/buried vault).
    
    **Not used by SAC**&#x2014;free for other purposes.

8.  `evel`

    Event elevation *m.a.s.l.*
    
    **Not used by SAC**&#x2014;free for other purposes.

9.  `evdp`

    Event depth in kilometers (*previously meters*) below surface.

10. `mag`

    Event magnitude.

11. `user(0--9)`

    Storage for user-defined values.

12. `dist`

    Station&#x2013;Event distance in kilometers.

13. `az`

    Azimuth $\mathrm{\left(Event \to Station\right)}$, decimal degrees from
    North.

14. `baz`

    Back-azimuth $\mathrm{\left(Station \to Event\right)}$, decimal degrees from
    North.

15. `gcarc`

    Station&#x2013;Event great circle arc-length, decimal degrees.

16. `cmpaz`

    Instrument measurement azimuth, decimal degrees from North.
    
    | Value    | Direction |
    -----------|------------
    | 0&deg;   | North     |
    | 90&deg;  | East      |
    | 180&deg; | South     |
    | 270&deg; | West      |
    | Other    | 1/2/3     |

17. `cmpinc`

    Instrument measurement incident angle, decimal degrees from upward vertical
    (incident 0&deg; = dip -90&deg;).
    
    | Value    | Direction  |
    | 0&deg;   | Up         |
    | 90&deg;  | Horizontal |
    | 180&deg; | Down       |
    | 270&deg; | Horizontal |
    
    **NOTE:** SEED/MINISEED use dip angle, decimal degrees down from horizontal
    (dip 0&deg; = incident 90&deg;).

18. `xminimum`

    Spectral-only equivalent of [`depmin`](#%3Ddepmin%3D) ($f_{0}$ or
    $\omega_{0}$).

19. `xmaximum`

    Spectral-only equivalent of [`depmax`](#%3Ddepmax%3D) ($f_{max}$ or
    $\omega_{max}$).

20. `yminimum`

    Spectral-only equivalent of [`b`](#%3Db%3D).

21. `ymaximum`

    Spectral-only equivalent of [`e`](#%3De%3D).

## Double (22)

64-bit (2 words, 8 bytes)

**NOTE:** in the header section these are floats&#x2014;they&rsquo;re doubles in
the footer section of `v7` SAC-files. In memory they&rsquo;re stored as doubles
regardless of the SAC-file version.

1.  `delta`

    Increment between evenly spaced samples ($\Delta t$ for timeseries, $\Delta
    f$ or $\Delta\omega$ for spectra).

2.  `b`

    First value (*begin*) of independent variable ($t_{0}$).

3.  `e`

    Final value (*end*) of independent variable ($t_{max}$).

4.  `o`

    Event *origin* time, in seconds relative to the reference time.

5.  `a`

    Event first *arrival* time, in seconds relative to the reference time.

6.  `t(0--9)`

    User defined *time* values, in seconds relative to the reference time.

7.  `f`

    Event end (*fini*) time, in seconds relative to the reference time.

8.  `stla`

    Station latitude in decimal degrees, N/S&#x2013;positive/negative.
    
    sac-format automatically enforces $\mathrm{stla}\in[-90, 90]$.

9.  `stlo`

    Station longitude in decimal degrees, E/W&#x2013;positive/negative.
    
    sac-format automatically enforces $\mathrm{stlo}\in[-180, 180]$.

10. `evla`

    Event latitude in decimal degrees, N/S&#x2013;positive/negative.
    
    sac-format automatically enforces $\mathrm{evla}\in[-90, 90]$.

11. `evlo`

    Event longitude in decimal degrees, E/W&#x2013;positive/negative.
    
    sac-format automatically enforces $\mathrm{evlo}\in[-180, 180]$.

12. `sb`

    Original (*saved*) [`b`](#%3Db%3D) value.

13. `sdelta`

    Original (*saved*) [`delta`](#%3Ddelta%3D) value.

## Integer (26)

32-bit (1 word, 4 bytes)

1.  `nzyear`

    Reference time GMT year.

2.  `nzjday`

    Reference time GMT day-of-year (often called [Julian
    Date](https://en.wikipedia.org/wiki/Julian_day)) (1&#x2013;366).

3.  `nzhour`

    Reference time GMT hour (00&#x2013;23).

4.  `nzmin`

    Reference time GMT minute (0&#x2013;59).

5.  `nzsec`

    Reference time GMT second (0&#x2013;59).

6.  `nzmsec`

    Reference time GMT Millisecond (0&#x2013;999).

7.  `nvhdr`

    SAC-file version.
    
    | Version | Description                       |
    | `v7`    | Footer (2020+, sac 102.0+)        |
    | `v6`    | No footer (pre-2020, sac 101.6a-) |

8.  `norid`

    Origin ID.

9.  `nevid`

    Event ID.

10. `npts`

    *Number of points* in data.

11. `nsnpts`

    Original (*saved*) [`npts`](#%3Dnpts%3D).

12. `nwfid`

    Waveform ID.

13. `nxsize`

    Spectral-only equivalent of [`npts`](#%3Dnpts%3D) (length of spectrum).

14. `nysize`

    Spectral-only, width of spectrum.

15. `iftype`

    File type.
    
    | Value | Type         | Description                |
    | 01    | ITIME        | Time-series                |
    | 02    | IRLIM        | Spectral (real/imaginary)  |
    | 03    | IAMPH        | Spectral (amplitude/phase) |
    | 04    | IXY          | General XY file            |
    | ??    | IXYZ&lowast; | General XYZ file           |
    
    &lowast;Value not listed in the standard.

16. `idep`

    Dependent variable type.
    
    | Value | Type   | Description                                           |
    | 05    | IUNKN  | Unknown                                               |
    | 06    | IDISP  | Displacement (nm)                                     |
    | 07    | IVEL   | Velocity $\mathrm{\left(\frac{nm}{s}\right)}$         |
    | 08    | IACC   | Acceleration $\mathrm{\left(\frac{nm}{s^{2}}\right)}$ |
    | 50    | IVOLTS | Velocity (volts)                                      |

17. `iztype`

    Reference time equivalent.
    
    | Value        | Type           | Description                |
    | 05           | IUNKN          | Unknown                    |
    | 09           | IB             | Recording start time       |
    | 10           | IDAY           | Midnight reference GMT day |
    | 11           | IO             | Event origin time          |
    | 12           | IA             | First arrival time         |
    | 13&#x2013;22 | IT(0&#x2013;9) | User defined time (t) pick |

18. `iinst`

    Recording instrument type.
    
    **Not used by SAC**&#x2014;free for other purposes.

19. `istreg`

    Station geographic region.
    
    **Not used by SAC**&#x2014;free for other purposes.

20. `ievreg`

    Event geographic region.
    
    **Not used by SAC**&#x2014;free for other purposes.

21. `ievtyp`

    Event type.
    
    | Value | Type   | Description                                               |
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
    | 72    | IQB    | Quarry/mine blast&#x2014;confirmed by quarry/mine         |
    | 73    | IQB1   | Quarry/mine blast&#x2014;designed shot info-ripple fired  |
    | 74    | IQB2   | Quarry/mine blast&#x2014;observed shot info-ripple fired  |
    | 75    | IQBX   | Quarry/mine blast&#x2014;single shot                      |
    | 76    | IQMT   | Quarry/mining induced events&#x2014;tremor and rockbursts |
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

22. `iqual`

    Quality of data.
    
    | Value | Type   | Description               |
    | 44    | IOTHER | Other                     |
    | 45    | IGOOD  | Good                      |
    | 46    | IGLCH  | Glitches                  |
    | 47    | IDROP  | Dropouts                  |
    | 48    | ILOWSN | Low signal-to-noise ratio |
    
    **Not used by SAC**&#x2014;free for other purposes.

23. `isynth`

    Synthetic data flag.
    
    | Value | Type     | Description |
    | 49    | IRLDATA  | Real data   |
    | XX    | &lowast; | Synthetic   |
    
    &lowast;Values and types not listed in the standard.

24. `imagtyp`

    Magnitude type.
    
    | Value | Type | Description                      |
    | 52    | IMB  | Body-wave magnitude ($M_{b}$)    |
    | 53    | IMS  | Surface-wave magnitude ($M_{s}$) |
    | 54    | IML  | Local magnitude ($M_{l}$)        |
    | 55    | IMW  | Moment magnitude ($M_{w}$)       |
    | 56    | IMD  | Duration magnitude ($M_{d}$)     |
    | 57    | IMX  | User-defined magnitude ($M_{x}$) |

25. `imagsrc`

    Source of magnitude information.
    
    | Value | Type     | Description                            |
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

26. `ibody`

    Body/spheroid definition used to calculate distances.
    
    | Value  | Type     | Name               | Semi-major axis (a [m]) | Inverse Flattening (*f*) |
    | -12345 | UNDEF    | Earth (*Historic*) | 6378160.0               | 0.00335293               |
    | 98     | ISUN     | Sun                | 696000000.0             | 8.189e-6                 |
    | 99     | IMERCURY | Mercury            | 2439700.0               | 0.0                      |
    | 100    | IVENUS   | Venus              | 6051800.0               | 0.0                      |
    | 101    | IEARTH   | Earth (*WGS84*)    | 6378137.0               | 0.0033528106647474805    |
    | 102    | IMOON    | Moon               | 1737400.0               | 0.0                      |
    | 103    | IMARS    | Mars               | 3396190.0               | 0.005886007555525457     |

## Boolean (4)

32-bit (1 word, 4 bytes) in-file/8-bit (1 byte) in-memory

1.  `leven`

    **REQUIRED**
    
    Evenly-spaced data flag.
    
    If true, then data is evenly spaced.

2.  `lpspol`

    Station polarity flag.
    
    If true, then station has positive-polarity&#x2014;it follows the left-hand
    convention (for example, North-East-Up [NEZ]).

3.  `lovrok`

    File overwrite flag.
    
    If true, then it&rsquo;s okay to overwrite the file.

4.  `lcalda`

    Calculate geometry flag.
    
    If true, then calculate [`dist`](#%3Ddist%3D), [`az`](#%3Daz%3D),
    [`baz`](#%3Dbaz%3D), and [`gcarc`](#%3Dgcarc%3D) from [`stla`](#%3Dstla%3D),
    [`stlo`](#%3Dstlo%3D), [`evla`](#%3Devla%3D), and [`evlo`](#%3Devlo%3D).

## String (23)

32/64-bit (2/4 words, 8/16 bytes, 8/16 characters)

1.  `kstnm`

    Station name.

2.  `kevnm`&lowast;

    Event name.
    
    &lowast;This is the **only** four word (16 character) string.

3.  `khole`

    Nuclear&#x2014;hole identifier.
    
    Other&#x2014;Location identifier (LOCID).

4.  `ko`

    Text for [`o`](#%3Do%3D).

5.  `ka`

    Text for [`a`](#%3Da%3D).

6.  `kt(0--9)`

    Text for [`t(0--9)`](#%3Dt%280--9%29%3D).

7.  `kf`

    Text for [`f`](#%3Df%3D).

8.  `kuser(0--2)`

    Text for the first three of [`user(0--9)`](#%3Duser%280--9%29%3D).

9.  `kdatrd`

    Date the data was read onto a computer.

10. `kinst`

    Text for [`iinst`](#%3Diinst%3D).

## Data (2)

32-bit (2 words, 8 bytes) in-file/64-bit (4 words, 16 bytes) in-memory

Stored as floating-point (32-bit) values in SAC-files; stored as
double-precision in memory.

1.  `data1`

    The first data vector---**always** present in a SAC-file and begins at word
    158.

2.  `data2`

    The second data vector---**conditionally** present and begins after
    [`data1`](#%3Ddata1%3D).
    
    **Required** if [`leven`](#%3Dleven%3D) is false, or if
    [`iftype`](#%3Diftype%3D) is spectral/XY/XYZ.
