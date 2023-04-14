#ifndef SAC_CLASS_H
#define SAC_CLASS_H

#include "sac_io.hpp" // my sac-format io functions

#include <string> // std::string
#include <vector> // std::vector

// Define a SAC class to make it easier to read sac files
// Much of the header/footer values are generally SAC-exclusive
// But in the interest of full compatibility with the file-format
// They are included
//
// Because SAC is moving to the new version (NVHDR = 7) where it prefers
// to use the double-precision footer values over the header values
// I'm going to preface the single-precision version f_ (only if there is a
// footer equivalent!)
//
// Footer values will have the regular name (instead of prefixed with d_ for double)
// And will be used by default
// If an old version is written in, the floats will be converted to doubles and nvhdr updated

namespace SAC
{
  class Sac_Class
  {
  public:
    //-------------------------------------------------------------------------
    // Header
    //-------------------------------------------------------------------------
    // Increment between evenly spaced samples [required]
    float f_delta{SAC::unset_float};
    // Minimum value of dependent variable
    // So Amplitude for time series
    // Is spectra then amplitude or real component
    float depmin{SAC::unset_float};
    // Maximum value of dependent variable
    float depmax{SAC::unset_float};
    // Observed increment (if differs from delta)
    float odelta{SAC::unset_float};
    // Beginning value of independent variable [required]
    // First time value for time-series
    // What is this for spectral?
    float f_b{SAC::unset_float};
    // End value of independent variable [requipred]
    // Final time value for time-series
    // What is this for spectral?
    float f_e{SAC::unset_float};
    // Event origin time, seconds relative to reference time
    float f_o{SAC::unset_float};
    // First arrival time, seconds relative to reference time
    float f_a{SAC::unset_float};
    // User defined time picks for markers 0-9
    float f_t0{SAC::unset_float};
    float f_t1{SAC::unset_float};
    float f_t2{SAC::unset_float};
    float f_t3{SAC::unset_float};
    float f_t4{SAC::unset_float};
    float f_t5{SAC::unset_float};
    float f_t6{SAC::unset_float};
    float f_t7{SAC::unset_float};
    float f_t8{SAC::unset_float};
    float f_t9{SAC::unset_float};
    // End of event time, seconds relative to reference time
    float f_f{SAC::unset_float};
    // Instrument response parameters 0-9 [not used by SAC]
    float resp0{SAC::unset_float};
    float resp1{SAC::unset_float};
    float resp2{SAC::unset_float};
    float resp3{SAC::unset_float};
    float resp4{SAC::unset_float};
    float resp5{SAC::unset_float};
    float resp6{SAC::unset_float};
    float resp7{SAC::unset_float};
    float resp8{SAC::unset_float};
    float resp9{SAC::unset_float};
    // Station latitude, degrees, north positive
    float f_stla{SAC::unset_float};
    // Station longitude, degrees, east positive
    float f_stlo{SAC::unset_float};
    // Station elevation above sea level, meters [not used by SAC]
    float stel{SAC::unset_float};
    // Station depth below surface, meters [not used by SAC]
    float stdp{SAC::unset_float};
    // Event latitude, degrees, north positive
    float f_evla{SAC::unset_float};
    // Event longitude, degrees, east positive
    float f_evlo{SAC::unset_float};
    // Event elevation above sea level, meters [not used by SAC]
    float evel{SAC::unset_float};
    // Event depth below surface, kilometers [previously meters]
    float evdp{SAC::unset_float};
    // Event magnitude
    float mag{SAC::unset_float};
    // User defined variable storage
    float user0{SAC::unset_float};
    float user1{SAC::unset_float};
    float user2{SAC::unset_float};
    float user3{SAC::unset_float};
    float user4{SAC::unset_float};
    float user5{SAC::unset_float};
    float user6{SAC::unset_float};
    float user7{SAC::unset_float};
    float user8{SAC::unset_float};
    float user9{SAC::unset_float};
    // Station to event distance, kilometers
    float dist{SAC::unset_float};
    // Event to station azimuth, degrees
    float az{SAC::unset_float};
    // Station to event azimuth, degrees
    float baz{SAC::unset_float};
    // Station to event great circle arc length, degrees
    float gcarc{SAC::unset_float};
    // Original b (file), not same as in memory (SAC internal)
    float f_sb{SAC::unset_float};
    // Origin delta (file) not same as in memory (SAC internal)
    float f_sdelta{SAC::unset_float};
    // Mean value of dependent variable
    float depmen{SAC::unset_float};
    // Component azimuth, degrees clockwise from north
    float cmpaz{SAC::unset_float};
    // Component incident angle, degrees from upward vertical
    // SEED/MINISEED use dip, degrees from horizontal down
    float cmpinc{SAC::unset_float};
    // Minimum value of X (spectral only)
    float xminimum{SAC::unset_float};
    // Maximum value of X (spectral only)
    float xmaximum{SAC::unset_float};
    // Minimum value of Y (spectral only)
    float yminimum{SAC::unset_float};
    // Maximum value of Y (spectral only)
    float ymaximum{SAC::unset_float};
    // GMT year corresponding to reference (zero) time
    int nzyear{SAC::unset_int};
    // GMT Day of the Year
    int nzjday{SAC::unset_int};
    // GMT Hour
    int nzhour{SAC::unset_int};
    // GMT Minute
    int nzmin{SAC::unset_int};
    // GMT Second
    int nzsec{SAC::unset_int};
    // GMT MilliSecond
    int nzmsec{SAC::unset_int};
    // Header version number
    // SAC 102.0+ auto-updates from <=6 to 7 upon reading nowadays
    int nvhdr{SAC::unset_int};
    // Origin ID
    int norid{SAC::unset_int};
    // Event ID
    int nevid{SAC::unset_int};
    // Number of points per data component [required]
    int npts{SAC::unset_int};
    // Original NPTS (file), not same as in memory (SAC internal)
    int nsnpts{SAC::unset_int};
    // Waveform ID
    int nwfid{SAC::unset_int};
    // Spectral Length (Spectral only)
    int nxsize{SAC::unset_int};
    // Spectral Width (Spectral Only)
    int nysize{SAC::unset_int};
    // Type of file [required]
    // 01 = ITIME = Time-series
    // 02 = IRLIM = Spectral (real and imaginary)
    // 03 = IAMPH = Spectral (amplitude and phase)
    // 04 = IXY   = General X vs Y
    // UNTIL I HAVE THE CODE FOR IXYZ IT WILL NOT BE IMPLEMENTED!
    // ?? = IXYZ  = General XYZ (3D) file <- integer value not listed in the data format specification
    int iftype{SAC::unset_int};
    // Type of dependent variable
    // 05 = IUNKN  = Unknown
    // 06 = IDSIP  = Displacement (nm)
    // 07 = IVEL   = Velocity (nm/sec)
    // 08 = IACC   = Acceleration (nm/sec/sec)
    // 50 = IVOLTS = Velocity (volts)
    // Note other units may be used (like SI units)
    int idep{SAC::unset_int};
    // Reference time equivalent
    // 05 = IUNKN = Unknown
    // 09 = IB    = Begin time
    // 10 = IDAY  = Midnight of reference GMT day
    // 11 = IO    = Event origin time
    // 12 = IA    = First arrival time
    // XX = ITn   = User defined time pick (n = 0-9), (13-22)
    int iztype{SAC::unset_int};
    // Type of recording instrument [not used by SAC]
    int iinst{SAC::unset_int};
    // Station geographic region [not used by SAC]
    int istreg{SAC::unset_int};
    // Event geographic region [not used by SAC]
    int ievreg{SAC::unset_int};
    // Type of event
    // 05 = IUNKN  = Unknown
    // 37 = INUCL  = Nuclear
    // 38 = IPREN  = Nuclear pre-shot
    // 39 = IPOSTN = Nuclear post-shot
    // 40 = IQUAKE = Earthquake
    // 41 = IPREQ  = Foreshock
    // 42 = IPOSTQ = Aftershock
    // 43 = ICHEM  = Chemical explosion
    // 72 = IQB    = Quarry/mine blast, confirmed by quarry/mine
    // 73 = IQB1   = Quarry/mine blast with designed shot info-ripple fired
    // 74 = IQB2   = Quarry/mine blast with observed shot info-ripple fired
    // 75 = IQBX   = Quarry/mine blast, single shot
    // 76 = IQMT   = Quarry/mining induced events: tremor and rockbursts
    // 77 = IEQ    = Earthquake
    // 78 = IEQ1   = Earthquakes in a swarm or an aftershock sequence
    // 79 = IEQ2   = Felt earthquake
    // 80 = IME    = Marine explosion
    // 81 = IEX    = Other explosion
    // 82 = INU    = Nuclear explosion
    // 83 = INC    = Nuclear cavity collapse
    // 11 = IO     = Other source of known origin
    // 85 = IL     = Local event of unknown origin
    // 86 = IR     = Regional event of unknown origin
    // 87 = IT     = Teleseismic event of unknown origin
    // 88 = IU     = Undetermined or conflicting information
    // 44 = IOTHER = Other
    int ievtyp{SAC::unset_int};
    // Quality of data [not used by SAC]
    // 45 = IGOOD  = Good data
    // 46 = IGLCH  = Glithces
    // 47 = IDROP  = Dropouts
    // 48 = ILOWSN = Low signal to noise ratio
    // 44 = IOTHER = Other
    int iqual{SAC::unset_int};
    // Synthetic data flag [not used by SAC]
    // 49 = IRLDATA = Real data
    // ?? = Flags for various synthetic seismogram codes... (standardized or per person/group?)
    int isynth{SAC::unset_int};
    // Magnitude type
    // 52 = IMB = Body-wave magnitude
    // 53 = IMS = Surface-wave magnitude
    // 54 = IML = Local magnitude
    // 55 = IMW = Moment magnitude
    // 56 = IMD = Duration magnitude
    // 57 = IMX = User defined magnitude
    int imagtyp{SAC::unset_int};
    // Source of magnitude information
    // 58 = INEIC    = National Earthquake Information Center
    // 61 = IPDE     = Preliminary Determination of Epicenter
    // 62 = IISC     = Internation Seismological Centre
    // 63 = IREB     = Reviewed Event Bulletin
    // 64 = IUSGS    = US Geological Survey
    // 65 = IBRK     = UC Berkeley
    // 66 = ICALTECH = California Institute of Technology
    // 67 = ILLNL    = Lawrence Livermore National Laboratory
    // 68 = IEVLOC   = Event location (computer program)
    // 69 = IJSOP    = Joint Seismic Observation Program
    // 70 = IUSER    = The user (person running SAC)
    // 71 = IUNKNOWN = Unknown
    int imagsrc{SAC::unset_int};
    // Body/spheroid definition used in distance calculations
    // undef/-12345  = SAC historical spheroid version: a = 6378160.0m, f = 0.00335293 (this is NOT WGS84)
    //  98 = ISUN     = Sun: a = 696000000.0 m, f = 8.189e-6
    //  99 = IMERCURY = Mercury: a = 2439700.0 m, f = 0.0
    // 100 = IVENUS   = Venus: a = 6051800.0 m, f = 0.0
    //      Note: Looking into WGS84 on Wikipedia: a is semi-major axis (equatorial), f is 'inverse flattening' 
    //       ((a-c) / a), it is the square of the eccentricity
    // 101 = IEARTH   = Earth: a = 6378137.0 m, f = 1.0 / 298.257223563 ( = 0.0033528106647474805) (this IS WGS84)
    // 102 = IMOON    = Moon: a = 1737400.0 m, f = 0.0
    // 103 = IMARS    = Mars: a = 3396190.0 m, f = 1.0 / 169.89444722361179 ( = 0.005886007555525457)
    int ibody{SAC::unset_int};
    // True if data is evenly spaced [required]
    bool leven{SAC::unset_bool};
    // True if station have positive-polarity (left-hand rule [e.g. NEZ])
    bool lpspol{SAC::unset_bool};
    // True if okay to overwrite file
    bool lovrok{SAC::unset_bool};
    // True if dist, az, baz, gcarc are to be calculated from station and event coordinates
    bool lcalda{SAC::unset_bool};
    // Station name
    std::string kstnm{SAC::unset_two_words};
    // Event name
    std::string kevnm{SAC::unset_four_words};
    // Nuclear: hole identifier; Other: Location identifier (LOCID)
    std::string khole{SAC::unset_two_words};
    // Event origin time identification (test for o)
    std::string ko{SAC::unset_two_words};
    // First arrival time identification (text for a)
    std::string ka{SAC::unset_two_words};
    // User defined pick identifiers (test for t0-t9)
    std::string kt0{SAC::unset_two_words};
    std::string kt1{SAC::unset_two_words};
    std::string kt2{SAC::unset_two_words};
    std::string kt3{SAC::unset_two_words};
    std::string kt4{SAC::unset_two_words};
    std::string kt5{SAC::unset_two_words};
    std::string kt6{SAC::unset_two_words};
    std::string kt7{SAC::unset_two_words};
    std::string kt8{SAC::unset_two_words};
    std::string kt9{SAC::unset_two_words};
    // Fini identification
    std::string kf{SAC::unset_two_words};
    // User defined variable storage area (text, n=0-2)
    std::string kuser0{SAC::unset_two_words};
    std::string kuser1{SAC::unset_two_words};
    std::string kuser2{SAC::unset_two_words};
    // Channel Name
    // SEED volumes use 3 coded characters (e.g. BHZ)
    // Third if direction (Z = up)
    // Current trend is to prefer 1/2 over N/E
    std::string kcmpnm{SAC::unset_two_words};
    // Name of seismic network
    std::string knetwk{SAC::unset_two_words};
    // Date data was read onto computer
    std::string kdatrd{SAC::unset_two_words};
    // Generic name of recording instrument
    std::string kinst{SAC::unset_two_words};
    //-------------------------------------------------------------------------
    // End header
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // Data
    //-------------------------------------------------------------------------
    // data1 is the first data chunk in the SAC-file
    // It is ALWAYS present
    std::vector<float> data1{};
    // data2 is the second data chunk in the SAC-file
    // It is CONDITIONALLY present
    std::vector<float> data2{};
    //-------------------------------------------------------------------------
    // End data
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // Footer (if nvhdr = 7)
    //-------------------------------------------------------------------------
    // These are double precision version of float headers
    // Giving them default unset_double value to be consistent with all default values
    // double-precision version of delta
    double delta{SAC::unset_double};
    // double-precision version of b
    double b{SAC::unset_double};
    // double-precision version of e
    double e{SAC::unset_double};
    // double-precision version of o
    double o{SAC::unset_double};
    // double-precision version of a
    double a{SAC::unset_double};
    // double-precision version of t0-t9
    double t0{SAC::unset_double};
    double t1{SAC::unset_double};
    double t2{SAC::unset_double};
    double t3{SAC::unset_double};
    double t4{SAC::unset_double};
    double t5{SAC::unset_double};
    double t6{SAC::unset_double};
    double t7{SAC::unset_double};
    double t8{SAC::unset_double};
    double t9{SAC::unset_double};
    // double-precision version of f
    double f{SAC::unset_double};
    // double-precision version of evlo
    double evlo{SAC::unset_double};
    // double-precision version of evla
    double evla{SAC::unset_double};
    // double-precision version of stlo
    double stlo{SAC::unset_double};
    // double-precision version of stla
    double stla{SAC::unset_double};
    // double-precision version of sb
    double sb{SAC::unset_double};
    // double-precision version of sdelta
    double sdelta{SAC::unset_double};
    //-------------------------------------------------------------------------
    // End footer
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // Constructors
    //-------------------------------------------------------------------------
    // Parameterized constructor (reader)
    Sac_Class(const std::string& file_name);
    //-------------------------------------------------------------------------
    // End constructors
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // Writing
    //-------------------------------------------------------------------------
    // Allows writing out to binary sac file format
    void write(const std::string& file_name);
    //-------------------------------------------------------------------------
    // End writing
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // Misc
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // End misc
    //-------------------------------------------------------------------------
  };
}

#endif
