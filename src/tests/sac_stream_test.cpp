// Preferred for handling sac data
#include "sac_stream.hpp"
// Temporary for testing
//#include "sac_io.hpp"

#include <iostream>
#include <string>
#include <vector>

int main()
{
  //std::string file_name = "../../data/IM.NV31..BHZ.M.2023.094.222304.SAC";
  std::string file_name = "../../data/test.SAC";
  // Something fails when writing/reading test.SAC with the double-precision footer values...
  // They come back as zero (except delta, 1.32525e-314)
  // I know they get converted to double correctly from a v6 file
  // Problem is in writing out the new format?
  // ObsPy cannot handle v7 data yet (thinks file is wrong size lol)
  //std::string file_name = "../data/test.SAC";
  std::cout << "File:\t\t" << file_name << '\n';
  SAC::SacStream sac(file_name);

  //---------------------------------------------------------------------------
  // Header values
  //---------------------------------------------------------------------------
  std::cout << "f_Delta:\t" << sac.f_delta << " s\n"; // 0.025
  std::cout << "Delta:\t\t" << sac.delta << " s \n"; // 0.025
  std::cout << "DepMin:\t\t" << sac.depmin << '\n'; // -12345
  std::cout << "DepMax:\t\t" << sac.depmax << '\n'; // -12345
  std::cout << "ODelta:\t\t" << sac.odelta << '\n'; // -12345
  std::cout << "f_B:\t\t" << sac.f_b << '\n'; // 0
  std::cout << "B:\t\t" << sac.b << '\n'; // 0
  std::cout << "f_E:\t\t" << sac.f_e << '\n'; // 697
  std::cout << "E:\t\t" << sac.e << '\n'; // 697
  std::cout << "f_O:\t\t" << sac.f_o << '\n'; // 13
  std::cout << "O:\t\t" << sac.o << '\n'; // 13
  std::cout << "f_A:\t\t" << sac.f_a << '\n'; // -12345
  std::cout << "A:\t\t" << sac.a << '\n'; // -12345
  std::cout << "f_T0:\t\t" << sac.f_t0 << '\n'; // -12345
  std::cout << "T0:\t\t" << sac.t0 << '\n'; // -12345
  std::cout << "f_T1:\t\t" << sac.f_t1 << '\n'; // -12345
  std::cout << "T1:\t\t" << sac.t1 << '\n'; // -12345
  std::cout << "f_T2:\t\t" << sac.f_t2 << '\n'; // -12345
  std::cout << "T2:\t\t" << sac.t2 << '\n'; // -12345
  std::cout << "f_T3:\t\t" << sac.f_t3 << '\n'; // -12345
  std::cout << "T3:\t\t" << sac.t3 << '\n'; // -12345
  std::cout << "f_T4:\t\t" << sac.f_t4 << '\n'; // -12345
  std::cout << "T4:\t\t" << sac.t4 << '\n'; // -12345
  std::cout << "f_T5:\t\t" << sac.f_t5 << '\n'; // -12345
  std::cout << "T5:\t\t" << sac.t5 << '\n'; // -12345
  std::cout << "f_T6:\t\t" << sac.f_t6 << '\n'; // -12345
  std::cout << "T6:\t\t" << sac.t6 << '\n'; // -12345
  std::cout << "f_T7:\t\t" << sac.f_t7 << '\n'; // -12345
  std::cout << "T7:\t\t" << sac.t7 << '\n'; // -12345
  std::cout << "f_T8:\t\t" << sac.f_t8 << '\n'; // -12345
  std::cout << "T8:\t\t" << sac.t8 << '\n'; // -12345
  std::cout << "f_T9:\t\t" << sac.f_t9 << '\n'; // -12345
  std::cout << "T9:\t\t" << sac.t9 << '\n'; // -12345
  std::cout << "f_f:\t\t" << sac.f_f << '\n'; // -12345
  std::cout << "f:\t\t" << sac.f << '\n'; // -12345
  std::cout << "Resp0:\t\t" << sac.resp0 << '\n'; // -12345
  std::cout << "Resp1:\t\t" << sac.resp1 << '\n'; // -12345
  std::cout << "Resp2:\t\t" << sac.resp2 << '\n'; // -12345
  std::cout << "Resp3:\t\t" << sac.resp3 << '\n'; // -12345
  std::cout << "Resp4:\t\t" << sac.resp4 << '\n'; // -12345
  std::cout << "Resp5:\t\t" << sac.resp5 << '\n'; // -12345
  std::cout << "Resp6:\t\t" << sac.resp6 << '\n'; // -12345
  std::cout << "Resp7:\t\t" << sac.resp7 << '\n'; // -12345
  std::cout << "Resp8:\t\t" << sac.resp8 << '\n'; // -12345
  std::cout << "Resp9:\t\t" << sac.resp9 << '\n'; // -12345
  std::cout << "f_Stla:\t\t" << sac.f_stla << '\n'; // 38.4328
  std::cout << "Stla:\t\t" << sac.stla << '\n'; // 38.4328
  std::cout << "f_Stlo:\t\t" << sac.f_stlo << '\n'; // -118.155
  std::cout << "Stlo:\t\t" << sac.stlo << '\n'; // -118.155
  std::cout << "Stel\t\t" << sac.stel << '\n'; // 1509
  std::cout << "Stdp:\t\t" << sac.stdp << '\n'; // 0
  std::cout << "f_Evla:\t\t" << sac.f_evla << '\n'; // 36.801
  std::cout << "Evla:\t\t" << sac.evla << '\n'; // 36.801
  std::cout << "f_Evlo:\t\t" << sac.f_evlo << '\n'; // -121.323
  std::cout << "Evlo:\t\t" << sac.evlo << '\n'; // -121.323
  std::cout << "Evel:\t\t" << sac.evel << '\n'; // -12345
  std::cout << "Evdp:\t\t" << sac.evdp << '\n'; // 10.16
  std::cout << "Mag:\t\t" << sac.mag << '\n'; // -12345
  std::cout << "User0:\t\t" << sac.user0 << '\n'; // -12345
  std::cout << "User1:\t\t" << sac.user1 << '\n'; // -12345
  std::cout << "User2:\t\t" << sac.user2 << '\n'; // -12345
  std::cout << "User3:\t\t" << sac.user3 << '\n'; // -12345
  std::cout << "User4:\t\t" << sac.user4 << '\n'; // -12345
  std::cout << "User5:\t\t" << sac.user5 << '\n'; // -12345
  std::cout << "User6:\t\t" << sac.user6 << '\n'; // -12345
  std::cout << "User7:\t\t" << sac.user7 << '\n'; // -12345
  std::cout << "User8:\t\t" << sac.user8 << '\n'; // -12345
  std::cout << "User9:\t\t" << sac.user9 << '\n'; // -12345
  std::cout << "Dist:\t\t" << sac.dist << '\n'; // 333.176
  std::cout << "Az:\t\t" << sac.az << '\n'; // 56.1169
  std::cout << "Baz:\t\t" << sac.baz << '\n'; // 238.043
  std::cout << "Gcarc:\t\t" << sac.gcarc << '\n'; // 2.99645
  std::cout << "f_sb:\t\t" << sac.f_sb << '\n'; // -12345
  std::cout << "sb:\t\t" << sac.sb << '\n'; // -12345
  std::cout << "f_sdelta:\t" << sac.f_sdelta << '\n'; // -12345
  std::cout << "sdelta:\t\t" << sac.sdelta << '\n'; // -12345 
  std::cout << "DepMen:\t\t" << sac.depmen << '\n'; // -12345
  std::cout << "CmpAz:\t\t" << sac.cmpaz << '\n'; // 0
  std::cout << "CmpInc:\t\t" << sac.cmpinc << '\n'; // 0
  std::cout << "XMinimum:\t" << sac.xminimum << '\n'; // -12345
  std::cout << "XMaximum:\t" << sac.xmaximum << '\n'; // -12345
  std::cout << "YMinimum:\t" << sac.yminimum << '\n'; // -12345
  std::cout << "YMaximum:\t" << sac.ymaximum << '\n'; // -12345
  std::cout << "NzYear:\t\t" << sac.nzyear << '\n'; // 2023
  std::cout << "NzJDay:\t\t" << sac.nzjday << '\n'; // 94
  std::cout << "NzHour:\t\t" << sac.nzhour << '\n'; // 22
  std::cout << "NzMin:\t\t" << sac.nzmin << '\n'; // 23
  std::cout << "NzSec:\t\t" << sac.nzsec << '\n'; // 4
  std::cout << "NzMSec:\t\t" << sac.nzmsec << '\n'; // 0
  std::cout << "NvHdr:\t\t" << sac.nvhdr << '\n'; // 7
  std::cout << "NorID:\t\t" << sac.norid << '\n'; // -12345
  std::cout << "NEvID:\t\t" << sac.nevid << '\n'; // -12345
  std::cout << "Npts:\t\t" << sac.npts << '\n'; // 27881
  std::cout << "NwfID:\t\t" << sac.nwfid << '\n'; // -12345
  std::cout << "NXSize:\t\t" << sac.nxsize << '\n'; // -12345
  std::cout << "NYSize:\t\t" << sac.nysize << '\n'; // -12345
  std::cout << "IfType:\t\t" << sac.iftype << '\n'; // 1
  std::cout << "Idep:\t\t" << sac.idep << '\n'; // -12345
  std::cout << "IzType:\t\t" << sac.iztype << '\n'; // -12345
  std::cout << "IInst:\t\t" << sac.iinst << '\n'; // -12345
  std::cout << "Istreg:\t\t" << sac.istreg << '\n'; // -12345
  std::cout << "IevReg:\t\t" << sac.ievreg << '\n'; // -12345
  std::cout << "IevTyp:\t\t" << sac.ievtyp << '\n'; // -12345
  std::cout << "IQual:\t\t" << sac.iqual << '\n'; // -12345
  std::cout << "ISynth:\t\t" << sac.isynth << '\n'; // -12345
  std::cout << "IMagTyp:\t" << sac.imagtyp << '\n'; // -12345
  std::cout << "IMagSrc:\t" << sac.imagsrc << '\n'; // -12345
  std::cout << "IBody:\t\t" << sac.ibody << '\n'; // -12345
  std::cout << "LEven:\t\t" << sac.leven << '\n'; // 1
  std::cout << "LpsPol:\t\t" << sac.lpspol << '\n'; // 1
  std::cout << "LovRok:\t\t" << sac.lovrok << '\n'; // 1
  std::cout << "LCalda:\t\t" << sac.lcalda << '\n'; // 1
  std::cout << "KStNm:\t\t" << sac.kstnm << '\n'; // NV31
  std::cout << "KEvNm:\t\t" << sac.kevnm << '\n'; // Central Californ [truncation]
  std::cout << "KHole:\t\t" << sac.khole << '\n'; // -12345
  std::cout << "KO:\t\t" << sac.ko << '\n'; // -12345
  std::cout << "KA:\t\t" << sac.ka << '\n'; // -12345
  std::cout << "KT0:\t\t" << sac.kt0 << '\n'; // -12345
  std::cout << "KT1:\t\t" << sac.kt1 << '\n'; // -12345
  std::cout << "KT2:\t\t" << sac.kt2 << '\n'; // -12345
  std::cout << "KT3:\t\t" << sac.kt3 << '\n'; // -12345
  std::cout << "KT4:\t\t" << sac.kt4 << '\n'; // -12345
  std::cout << "KT5:\t\t" << sac.kt5 << '\n'; // -12345
  std::cout << "KT6:\t\t" << sac.kt6 << '\n'; // -12345
  std::cout << "KT7:\t\t" << sac.kt7 << '\n'; // -12345
  std::cout << "KT8:\t\t" << sac.kt8 << '\n'; // -12345
  std::cout << "KT9:\t\t" << sac.kt9 << '\n'; // -12345
  std::cout << "KF:\t\t" << sac.kf << '\n'; // -12345
  std::cout << "KUser0:\t\t" << sac.kuser0 << '\n'; // -12345
  std::cout << "KUser1:\t\t" << sac.kuser1 << '\n'; // -12345
  std::cout << "KUser2:\t\t" << sac.kuser2 << '\n'; // -12345
  std::cout << "KCmpNm:\t\t" << sac.kcmpnm << '\n'; // BHZ
  std::cout << "KNetwk:\t\t" << sac.knetwk << '\n'; // IM
  std::cout << "KDatRd:\t\t" << sac.kdatrd << '\n'; // -12345
  std::cout << "KInst:\t\t" << sac.kinst << '\n'; // CMG3T_NV
  //---------------------------------------------------------------------------
  // Additional information
  //---------------------------------------------------------------------------
  std::cout << "Data size:\t" << sac.data1.size() << " points\n";
  std::cout << "Class size:\t" << sizeof(sac) << " bytes\n";
  //---------------------------------------------------------------------------
  // Test Writing sac data
  //---------------------------------------------------------------------------
  // Leave a marker
  std::cout << "\n\nTesting writing to binary SAC...\n";
  sac.f_delta = 123.0f;
  std::cout << "f_Delta:\t" << sac.f_delta << '\n';
  sac.delta = 12345.0;
  std::cout << "Delta:\t\t" << sac.delta << '\n';
  std::string new_file = "../../data/test.SAC";
  sac.write(new_file);
  std::cout << "Successfully written out to: " << new_file << '\n';

  std::cout << "\nTesting reading new file...\n";
  SAC::SacStream new_sac(new_file);
  std::cout << "f_Delta:\t" << new_sac.f_delta << '\n';
  std::cout << "Delta:\t\t" << new_sac.delta << '\n';

  std::cout << "\nWriting legacy file...\n";
  std::string legacy_file = "../../data/test2.SAC";
  new_sac.legacy_write(legacy_file);

  return 0;
}
