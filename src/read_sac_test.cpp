#include "read_sac.hpp"

#include <iostream>
#include <fstream>
#include <array>
// note string_views are more efficient because read-only
#include <string_view> // std::string_view (faster, read-only)
// #include <string> // std::string (slower)

// TODO
// Read in entire header
// Parse values to see if set (not = -12345)
// Convert to a sac object?
// Handle writing out a sac file?

int main()
{
  std::ifstream sac_file("./data/IM.NV31..BHZ.M.2023.094.222304.SAC", std::ifstream::binary);
  if (!sac_file)
  {
    std::cerr << "File could not be read...\n";
    return 1;
  }
  // Make sure at beginning of file
  sac_file.seekg(0);
  // Possible words
  std::array<char, 2 * SAC::word_length> two_words;
  std::array<char, 4 * SAC::word_length> four_words;
  //---------------------------------------------------------------------------
  // Headers
  //---------------------------------------------------------------------------
  std::cout << "Begin reading header...\n\n";

  float delta{SAC::read_next_float(&sac_file)}; // 0.025
  std::cout << "Delta:\t\t" << delta << " s\n";
  
  float depmin{SAC::read_next_float(&sac_file)}; // -12345
  std::cout << "DepMin:\t\t" << depmin << '\n';
  
  float depmax{SAC::read_next_float(&sac_file)}; // -12345
  std::cout << "DepMax:\t\t" << depmax << '\n';

  // Skip 'unused'
  SAC::skip_word(&sac_file);

  float odelta{SAC::read_next_float(&sac_file)}; // -12345
  std::cout << "ODelta:\t\t" << odelta << '\n';
  
  float b{SAC::read_next_float(&sac_file)}; // 0
  std::cout << "B:\t\t" << b << '\n';
  
  float e{SAC::read_next_float(&sac_file)}; // 697
  std::cout << "E:\t\t" << e << '\n';
  
  float o{SAC::read_next_float(&sac_file)}; // 13
  std::cout << "O:\t\t" << o << '\n';
  
  float a{SAC::read_next_float(&sac_file)}; // -12345
  std::cout << "A:\t\t" << a << '\n';

  // Skip 'internal'
  SAC::skip_word(&sac_file);

  // Arrival time picking headers
  // All -12345
  float t0{SAC::read_next_float(&sac_file)};
  std::cout << "T0:\t\t" << t0 << '\n';
  
  float t1{SAC::read_next_float(&sac_file)};
  std::cout << "T1:\t\t" << t1 << '\n';
  
  float t2{SAC::read_next_float(&sac_file)};
  std::cout << "T2:\t\t" << t2 << '\n';
  
  float t3{SAC::read_next_float(&sac_file)};
  std::cout << "T3:\t\t" << t3 << '\n';
  
  float t4{SAC::read_next_float(&sac_file)};
  std::cout << "T4:\t\t" << t4 << '\n';
  
  float t5{SAC::read_next_float(&sac_file)};
  std::cout << "T5:\t\t" << t5 << '\n';
  
  float t6{SAC::read_next_float(&sac_file)};
  std::cout << "T6:\t\t" << t6 << '\n';
  
  float t7{SAC::read_next_float(&sac_file)};
  std::cout << "T7:\t\t" << t7 << '\n';
  
  float t8{SAC::read_next_float(&sac_file)};
  std::cout << "T8:\t\t" << t8 << '\n';
  
  float t9{SAC::read_next_float(&sac_file)};
  std::cout << "T9:\t\t" << t9 << '\n';

  float f{SAC::read_next_float(&sac_file)}; // -12345
  std::cout << "F:\t\t" << f << '\n';

  // Resp headers
  // All -12345
  float resp0{SAC::read_next_float(&sac_file)};
  std::cout << "Resp0:\t\t" << resp0 << '\n';
  
  float resp1{SAC::read_next_float(&sac_file)};
  std::cout << "Resp1:\t\t" << resp1 << '\n';
  
  float resp2{SAC::read_next_float(&sac_file)};
  std::cout << "Resp2:\t\t" << resp2 << '\n';
  
  float resp3{SAC::read_next_float(&sac_file)};
  std::cout << "Resp3:\t\t" << resp3 << '\n';
  
  float resp4{SAC::read_next_float(&sac_file)};
  std::cout << "Resp4:\t\t" << resp4 << '\n';
  
  float resp5{SAC::read_next_float(&sac_file)};
  std::cout << "Resp5:\t\t" << resp5 << '\n';
  
  float resp6{SAC::read_next_float(&sac_file)};
  std::cout << "Resp6:\t\t" << resp6 << '\n';
  
  float resp7{SAC::read_next_float(&sac_file)};
  std::cout << "Resp7:\t\t" << resp7 << '\n';
  
  float resp8{SAC::read_next_float(&sac_file)};
  std::cout << "Resp8:\t\t" << resp8 << '\n';
  
  float resp9{SAC::read_next_float(&sac_file)};
  std::cout << "Resp9:\t\t" << resp9 << '\n';

  // Station headers
  float stla{SAC::read_next_float(&sac_file)}; // 38.4328
  std::cout << "Stla:\t\t" << stla << '\n';
  
  float stlo{SAC::read_next_float(&sac_file)}; // -118.155
  std::cout << "Stlo:\t\t" << stlo << '\n';
  
  float stel{SAC::read_next_float(&sac_file)}; // 1509
  std::cout << "Stel:\t\t" << stel << '\n';
  
  float stdp{SAC::read_next_float(&sac_file)}; // 0
  std::cout << "Stdp:\t\t" << stdp << '\n';

  // Event headers
  float evla{SAC::read_next_float(&sac_file)}; // 36.801
  std::cout << "Evla:\t\t" << evla << '\n';
  
  float evlo{SAC::read_next_float(&sac_file)}; // -121.323
  std::cout << "Evlo:\t\t" << evlo << '\n';
  
  float evel{SAC::read_next_float(&sac_file)}; // -12345
  std::cout << "Evel:\t\t" << evel << '\n';
  
  float evdp{SAC::read_next_float(&sac_file)}; // 10.16
  std::cout << "Evdp:\t\t" << evdp << '\n';
  
  float mag{SAC::read_next_float(&sac_file)}; // -12345
  std::cout << "Mag:\t\t" << mag << '\n';
  
  // User headers
  // All -12345
  float user0{SAC::read_next_float(&sac_file)};
  std::cout << "User0:\t\t" << user0 << '\n';
  
  float user1{SAC::read_next_float(&sac_file)};
  std::cout << "User1:\t\t" << user1 << '\n';
  
  float user2{SAC::read_next_float(&sac_file)};
  std::cout << "User2:\t\t" << user2 << '\n';
  
  float user3{SAC::read_next_float(&sac_file)};
  std::cout << "User3:\t\t" << user3 << '\n';
  
  float user4{SAC::read_next_float(&sac_file)};
  std::cout << "User4:\t\t" << user4 << '\n';
  
  float user5{SAC::read_next_float(&sac_file)};
  std::cout << "User5:\t\t" << user5 << '\n';
  
  float user6{SAC::read_next_float(&sac_file)};
  std::cout << "User6:\t\t" << user6 << '\n';
  
  float user7{SAC::read_next_float(&sac_file)};
  std::cout << "User7:\t\t" << user7 << '\n';
  
  float user8{SAC::read_next_float(&sac_file)};
  std::cout << "User8:\t\t" << user8 << '\n';
  
  float user9{SAC::read_next_float(&sac_file)};
  std::cout << "User9:\t\t" << user9 << '\n';

  float dist{SAC::read_next_float(&sac_file)}; // 333.176
  std::cout << "Dist:\t\t" << dist << '\n';
  
  float az{SAC::read_next_float(&sac_file)}; // 56.1169
  std::cout << "Az:\t\t" << az << '\n';
  
  float baz{SAC::read_next_float(&sac_file)}; // 238.043
  std::cout << "Baz:\t\t" << baz << '\n';
  
  float gcarc{SAC::read_next_float(&sac_file)}; // 2.99645
  std::cout << "Gcarc:\t\t" << gcarc << '\n';

  // Skip 'internal' 
  SAC::skip_word(&sac_file);
  // Skip 'internal'
  SAC::skip_word(&sac_file);

  float depmen{SAC::read_next_float(&sac_file)}; // -12345
  std::cout << "DepMen:\t\t" << depmen << '\n';
  
  float cmpaz{SAC::read_next_float(&sac_file)}; // 0
  std::cout << "Cmpaz:\t\t" << cmpaz << '\n';
  
  float cmpinc{SAC::read_next_float(&sac_file)}; // 0
  std::cout << "Cmpinc:\t\t" << cmpinc << '\n';
  
  float xminimum{SAC::read_next_float(&sac_file)}; // -12345
  std::cout << "Xminimum:\t" << xminimum << '\n';
  
  float xmaximum{SAC::read_next_float(&sac_file)}; // -12345
  std::cout << "Xmaximum:\t" << xmaximum << '\n';
  
  float yminimum{SAC::read_next_float(&sac_file)}; // -12345
  std::cout << "Yminimum:\t" << yminimum << '\n';
  
  float ymaximum{SAC::read_next_float(&sac_file)}; // -12345
  std::cout << "Ymaximum:\t" << ymaximum << '\n';

  // Skip 'unused' (x7)
  SAC::skip_word(&sac_file);
  SAC::skip_word(&sac_file);
  SAC::skip_word(&sac_file);
  SAC::skip_word(&sac_file);
  SAC::skip_word(&sac_file);
  SAC::skip_word(&sac_file);
  SAC::skip_word(&sac_file);

  // Date_time headers
  int nzyear{SAC::read_next_int(&sac_file)}; // 2023
  std::cout << "Nzyear:\t\t" << nzyear << '\n';
  
  int nzjday{SAC::read_next_int(&sac_file)}; // 94
  std::cout << "Nzjday:\t\t" << nzjday << '\n';
  
  int nzhour{SAC::read_next_int(&sac_file)}; // 22 
  std::cout << "Nzhour:\t\t" << nzhour << '\n';
  
  int nzmin{SAC::read_next_int(&sac_file)}; // 23 
  std::cout << "Nzmin:\t\t" << nzmin << '\n';
  
  int nzsec{SAC::read_next_int(&sac_file)}; // 4
  std::cout << "Nzsec:\t\t" << nzsec << '\n';
  
  int nzmsec{SAC::read_next_int(&sac_file)}; // 0
  std::cout << "Nzmsec:\t\t" << nzmsec << '\n';
  
  int nvhdr{SAC::read_next_int(&sac_file)}; // 6 (old, 7 = new) 
  std::cout << "NvHdr:\t\t" << nvhdr << '\n';
  
  int norid{SAC::read_next_int(&sac_file)}; // -12345
  std::cout << "Norid:\t\t" << norid << '\n';
  
  int nevid{SAC::read_next_int(&sac_file)}; // -12345
  std::cout << "Nevid:\t\t" << nevid << '\n';
  
  int npts{SAC::read_next_int(&sac_file)}; // 27881
  std::cout << "Npts:\t\t" << npts << '\n';
  
  // Skip 'internal'
  SAC::skip_word(&sac_file);

  int nwfid{SAC::read_next_int(&sac_file)}; // -12345
  std::cout << "Nwfid:\t\t" << nwfid << '\n';
  
  int nxsize{SAC::read_next_int(&sac_file)}; // -12345
  std::cout << "Nxsize:\t\t" << nxsize << '\n';
  
  int nysize{SAC::read_next_int(&sac_file)}; // -12345
  std::cout << "Nysize:\t\t" << nysize << '\n';
  
  // Skip 'unused'
  SAC::skip_word(&sac_file);

  int iftype{SAC::read_next_int(&sac_file)}; // 1
  std::cout << "Iftype:\t\t" << iftype << '\n';
  
  int idep{SAC::read_next_int(&sac_file)}; // -12345
  std::cout << "Idep:\t\t" << idep << '\n';
  
  int iztype{SAC::read_next_int(&sac_file)}; // -12345
  std::cout << "Iztype:\t\t" << iztype << '\n';
  
  // Skip 'unused'
  SAC::skip_word(&sac_file);

  int iinst{SAC::read_next_int(&sac_file)}; // -12345
  std::cout << "Iinst:\t\t" << iinst << '\n';
  
  int istreg{SAC::read_next_int(&sac_file)}; // -12345
  std::cout << "Istreg:\t\t" << istreg << '\n';
  
  int ievreg{SAC::read_next_int(&sac_file)}; // 
  std::cout << "Ievreg:\t\t" << ievreg << '\n';
  
  int ievtyp{SAC::read_next_int(&sac_file)}; // 
  std::cout << "Ievtyp:\t\t" << ievtyp << '\n';
  
  int iqual{SAC::read_next_int(&sac_file)}; // 
  std::cout << "Iqual:\t\t" << iqual << '\n';
  
  int isynth{SAC::read_next_int(&sac_file)}; // 
  std::cout << "Isynth:\t\t" << isynth << '\n';
  
  int imagtyp{SAC::read_next_int(&sac_file)}; // 
  std::cout << "Imagtyp:\t" << imagtyp << '\n';
  
  int imagsrc{SAC::read_next_int(&sac_file)}; // 
  std::cout << "Imagsrc:\t" << imagsrc << '\n';
  
  int ibody{SAC::read_next_int(&sac_file)}; // 
  std::cout << "Ibody:\t\t" << ibody << '\n';

  // Skip 'unused' (x7)
  SAC::skip_word(&sac_file);
  SAC::skip_word(&sac_file);
  SAC::skip_word(&sac_file);
  SAC::skip_word(&sac_file);
  SAC::skip_word(&sac_file);
  SAC::skip_word(&sac_file);
  SAC::skip_word(&sac_file);

  // Logical headers
  bool leven{SAC::read_next_bool(&sac_file)}; // 1
  std::cout << "Leven:\t\t" << leven << '\n';
  
  bool lpspol{SAC::read_next_bool(&sac_file)}; // 1 
  std::cout << "Lpspol:\t\t" << lpspol << '\n';
  
  bool lovrok{SAC::read_next_bool(&sac_file)}; // 1
  std::cout << "Lovrok:\t\t" << lovrok << '\n';
  
  bool lcalda{SAC::read_next_bool(&sac_file)}; // 1
  std::cout << "lcalda:\t\t" << lcalda << '\n';

  // Skip 'unused'
  SAC::skip_word(&sac_file);
  
  // KSTNM is 2 words long (like all other 'K' headers)
  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // NV31
  std::string_view kstnm(two_words.data(), two_words.size());
  std::cout << "Kstnm:\t\t" << kstnm << '\n';

  // KEVNM is 4 words long (special rule!)
  four_words = SAC::read_words<sizeof(four_words)>(&sac_file, 4); // Central Californ [truncation]
  std::string_view kevnm(four_words.data(), four_words.size());
  std::cout << "Kevnm:\t\t" << kevnm << '\n';

  // All other 'K' headers are 2 words long
  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view khole(two_words.data(), two_words.size());
  std::cout << "Khole:\t\t" << khole << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345 
  std::string_view ko(two_words.data(), two_words.size());
  std::cout << "Ko:\t\t" << ko << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view ka(two_words.data(), two_words.size());
  std::cout << "Ka:\t\t" << ka << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view kt0(two_words.data(), two_words.size());
  std::cout << "Kt0:\t\t" << kt0 << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view kt1(two_words.data(), two_words.size());
  std::cout << "Kt1:\t\t" << kt1 << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view kt2(two_words.data(), two_words.size());
  std::cout << "Kt2:\t\t" << kt2 << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view kt3(two_words.data(), two_words.size());
  std::cout << "Kt3:\t\t" << kt3 << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view kt4(two_words.data(), two_words.size());
  std::cout << "Kt4:\t\t" << kt4 << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view kt5(two_words.data(), two_words.size());
  std::cout << "Kt5:\t\t" << kt5 << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view kt6(two_words.data(), two_words.size());
  std::cout << "Kt6:\t\t" << kt6 << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view kt7(two_words.data(), two_words.size());
  std::cout << "Kt7:\t\t" << kt7 << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view kt8(two_words.data(), two_words.size());
  std::cout << "Kt8:\t\t" << kt8 << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view kt9(two_words.data(), two_words.size());
  std::cout << "Kt9:\t\t" << kt9 << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view kf(two_words.data(), two_words.size());
  std::cout << "Kf:\t\t" << kf << '\n';
  
  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view kuser0(two_words.data(), two_words.size());
  std::cout << "Kuser0:\t\t" << kuser0 << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view kuser1(two_words.data(), two_words.size());
  std::cout << "Kuser1:\t\t" << kuser1 << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view kuser2(two_words.data(), two_words.size());
  std::cout << "Kuser2:\t\t" << kuser2 << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // BHZ
  std::string_view kcmpnm(two_words.data(), two_words.size());
  std::cout << "Kcmpnm:\t\t" << kcmpnm << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // IM
  std::string_view knetwk(two_words.data(), two_words.size());
  std::cout << "Knetwk:\t\t" << knetwk << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // -12345
  std::string_view kdatrd(two_words.data(), two_words.size());
  std::cout << "Kdatrd:\t\t" << kdatrd << '\n';

  two_words = SAC::read_words<sizeof(two_words)>(&sac_file, 2); // CMG3T_NV
  std::string_view kinst(two_words.data(), two_words.size());
  std::cout << "Kinst:\t\t" << kinst << '\n';

  //---------------------------------------------------------------------------
  // End header
  // --------------------------------------------------------------------------

  std::cout << "\nEnd header.\n Now reading data...\n\n";
  // Read all the data
  std::cout << "Data vector:\n";
  std::vector data{SAC::read_data(&sac_file)};
  for (long unsigned int i{0}; i < 25; ++i)
  {
    std::cout << '\t' << data[i] << '\n';
  }
  std::cout << "\t...\n";

  std::cout << "\nEnd data.\n Successful run!\n";

  return 0;
}
