#ifndef SAC_CLASS_H
#define SAC_CLASS_H

#include "sac_io.hpp" // my sac format reading sac functions

#include <string> // std::string
#include <vector> // std::vector

// Define a SAC class to make it easier to read sac files

namespace SAC
{
  class Sac_Class
  {
  public:
    //-------------------------------------------------------------------------
    // Header
    //-------------------------------------------------------------------------
    float delta{};
    float depmin{};
    float depmax{};
    float odelta{};
    float b{};
    float e{};
    float o{};
    float a{};
    float t0{};
    float t1{};
    float t2{};
    float t3{};
    float t4{};
    float t5{};
    float t6{};
    float t7{};
    float t8{};
    float t9{};
    float f{};
    float resp0{};
    float resp1{};
    float resp2{};
    float resp3{};
    float resp4{};
    float resp5{};
    float resp6{};
    float resp7{};
    float resp8{};
    float resp9{};
    float stla{};
    float stlo{};
    float stel{};
    float stdp{};
    float evla{};
    float evlo{};
    float evel{};
    float evdp{};
    float mag{};
    float user0{};
    float user1{};
    float user2{};
    float user3{};
    float user4{};
    float user5{};
    float user6{};
    float user7{};
    float user8{};
    float user9{};
    float dist{};
    float az{};
    float baz{};
    float gcarc{};
    float depmen{};
    float cmpaz{};
    float cmpinc{};
    float xminimum{};
    float xmaximum{};
    float yminimum{};
    float ymaximum{};
    int nzyear{};
    int nzjday{};
    int nzhour{};
    int nzmin{};
    int nzsec{};
    int nzmsec{};
    int nvhdr{};
    int norid{};
    int nevid{};
    int npts{};
    int nwfid{};
    int nxsize{};
    int nysize{};
    int iftype{};
    int idep{};
    int iztype{};
    int iinst{};
    int istreg{};
    int ievreg{};
    int ievtyp{};
    int iqual{};
    int isynth{};
    int imagtyp{};
    int imagsrc{};
    int ibody{};
    bool leven{};
    bool lpspol{};
    bool lovrok{};
    bool lcalda{};
    std::string kstnm{};
    std::string kevnm{};
    std::string khole{};
    std::string ko{};
    std::string ka{};
    std::string kt0{};
    std::string kt1{};
    std::string kt2{};
    std::string kt3{};
    std::string kt4{};
    std::string kt5{};
    std::string kt6{};
    std::string kt7{};
    std::string kt8{};
    std::string kt9{};
    std::string kf{};
    std::string kuser0{};
    std::string kuser1{};
    std::string kuser2{};
    std::string kcmpnm{};
    std::string knetwk{};
    std::string kdatrd{};
    std::string kinst{};
    //-------------------------------------------------------------------------
    // End header
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // Data
    //-------------------------------------------------------------------------
    std::vector<float> data{};
    //-------------------------------------------------------------------------
    // End data
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // Footer (if nvhdr = 7)
    //-------------------------------------------------------------------------

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
    // Calculate standard mean of the data vector
    float mean();
    //-------------------------------------------------------------------------
    // End misc
    //-------------------------------------------------------------------------
  };
}

#endif
