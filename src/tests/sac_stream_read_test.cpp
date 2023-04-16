// Preferred for handling sac data
#include "sac_stream.hpp"
// Temporary for testing
//#include "sac_io.hpp"

#include <iostream>
#include <string>
#include <vector>

int main(int arg_count, char *arg_array[])
{
  if (arg_count != 2)
  {
    std::printf("Usage: `%s <file>`\n", arg_array[0]);
    return 1;
  }
  std::string file_name = arg_array[1];
  // ObsPy cannot handle v7 data yet (thinks file is wrong size lol)
  std::cout << "File:\t\t" << file_name << '\n';
  SAC::SacStream sac(file_name);

  //---------------------------------------------------------------------------
  // Header values
  //---------------------------------------------------------------------------
  std::cout << "f_Delta:\t" << sac.f_delta << " s\n";
  std::cout << "Delta:\t\t" << sac.delta << " s \n";
  std::cout << "DepMin:\t\t" << sac.depmin << '\n';
  std::cout << "DepMax:\t\t" << sac.depmax << '\n';
  std::cout << "ODelta:\t\t" << sac.odelta << '\n';
  std::cout << "f_B:\t\t" << sac.f_b << '\n';
  std::cout << "B:\t\t" << sac.b << '\n';
  std::cout << "f_E:\t\t" << sac.f_e << '\n';
  std::cout << "E:\t\t" << sac.e << '\n';
  std::cout << "f_O:\t\t" << sac.f_o << '\n';
  std::cout << "O:\t\t" << sac.o << '\n';
  std::cout << "f_A:\t\t" << sac.f_a << '\n';
  std::cout << "A:\t\t" << sac.a << '\n';
  std::cout << "f_T0:\t\t" << sac.f_t0 << '\n';
  std::cout << "T0:\t\t" << sac.t0 << '\n';
  std::cout << "f_T1:\t\t" << sac.f_t1 << '\n';
  std::cout << "T1:\t\t" << sac.t1 << '\n';
  std::cout << "f_T2:\t\t" << sac.f_t2 << '\n';
  std::cout << "T2:\t\t" << sac.t2 << '\n';
  std::cout << "f_T3:\t\t" << sac.f_t3 << '\n';
  std::cout << "T3:\t\t" << sac.t3 << '\n';
  std::cout << "f_T4:\t\t" << sac.f_t4 << '\n';
  std::cout << "T4:\t\t" << sac.t4 << '\n';
  std::cout << "f_T5:\t\t" << sac.f_t5 << '\n';
  std::cout << "T5:\t\t" << sac.t5 << '\n';
  std::cout << "f_T6:\t\t" << sac.f_t6 << '\n';
  std::cout << "T6:\t\t" << sac.t6 << '\n';
  std::cout << "f_T7:\t\t" << sac.f_t7 << '\n';
  std::cout << "T7:\t\t" << sac.t7 << '\n';
  std::cout << "f_T8:\t\t" << sac.f_t8 << '\n';
  std::cout << "T8:\t\t" << sac.t8 << '\n';
  std::cout << "f_T9:\t\t" << sac.f_t9 << '\n';
  std::cout << "T9:\t\t" << sac.t9 << '\n';
  std::cout << "f_f:\t\t" << sac.f_f << '\n';
  std::cout << "f:\t\t" << sac.f << '\n';
  std::cout << "Resp0:\t\t" << sac.resp0 << '\n';
  std::cout << "Resp1:\t\t" << sac.resp1 << '\n';
  std::cout << "Resp2:\t\t" << sac.resp2 << '\n';
  std::cout << "Resp3:\t\t" << sac.resp3 << '\n';
  std::cout << "Resp4:\t\t" << sac.resp4 << '\n';
  std::cout << "Resp5:\t\t" << sac.resp5 << '\n';
  std::cout << "Resp6:\t\t" << sac.resp6 << '\n';
  std::cout << "Resp7:\t\t" << sac.resp7 << '\n';
  std::cout << "Resp8:\t\t" << sac.resp8 << '\n';
  std::cout << "Resp9:\t\t" << sac.resp9 << '\n';
  std::cout << "f_Stla:\t\t" << sac.f_stla << '\n';
  std::cout << "Stla:\t\t" << sac.stla << '\n';
  std::cout << "f_Stlo:\t\t" << sac.f_stlo << '\n';
  std::cout << "Stlo:\t\t" << sac.stlo << '\n';
  std::cout << "Stel\t\t" << sac.stel << '\n';
  std::cout << "Stdp:\t\t" << sac.stdp << '\n';
  std::cout << "f_Evla:\t\t" << sac.f_evla << '\n';
  std::cout << "Evla:\t\t" << sac.evla << '\n';
  std::cout << "f_Evlo:\t\t" << sac.f_evlo << '\n';
  std::cout << "Evlo:\t\t" << sac.evlo << '\n';
  std::cout << "Evel:\t\t" << sac.evel << '\n';
  std::cout << "Evdp:\t\t" << sac.evdp << '\n';
  std::cout << "Mag:\t\t" << sac.mag << '\n';
  std::cout << "User0:\t\t" << sac.user0 << '\n';
  std::cout << "User1:\t\t" << sac.user1 << '\n';
  std::cout << "User2:\t\t" << sac.user2 << '\n';
  std::cout << "User3:\t\t" << sac.user3 << '\n';
  std::cout << "User4:\t\t" << sac.user4 << '\n';
  std::cout << "User5:\t\t" << sac.user5 << '\n';
  std::cout << "User6:\t\t" << sac.user6 << '\n';
  std::cout << "User7:\t\t" << sac.user7 << '\n';
  std::cout << "User8:\t\t" << sac.user8 << '\n';
  std::cout << "User9:\t\t" << sac.user9 << '\n';
  std::cout << "Dist:\t\t" << sac.dist << '\n';
  std::cout << "Az:\t\t" << sac.az << '\n';
  std::cout << "Baz:\t\t" << sac.baz << '\n';
  std::cout << "Gcarc:\t\t" << sac.gcarc << '\n';
  std::cout << "f_sb:\t\t" << sac.f_sb << '\n';
  std::cout << "sb:\t\t" << sac.sb << '\n';
  std::cout << "f_sdelta:\t" << sac.f_sdelta << '\n';
  std::cout << "sdelta:\t\t" << sac.sdelta << '\n';
  std::cout << "DepMen:\t\t" << sac.depmen << '\n';
  std::cout << "CmpAz:\t\t" << sac.cmpaz << '\n';
  std::cout << "CmpInc:\t\t" << sac.cmpinc << '\n';
  std::cout << "XMinimum:\t" << sac.xminimum << '\n';
  std::cout << "XMaximum:\t" << sac.xmaximum << '\n';
  std::cout << "YMinimum:\t" << sac.yminimum << '\n';
  std::cout << "YMaximum:\t" << sac.ymaximum << '\n';
  std::cout << "NzYear:\t\t" << sac.nzyear << '\n';
  std::cout << "NzJDay:\t\t" << sac.nzjday << '\n';
  std::cout << "NzHour:\t\t" << sac.nzhour << '\n';
  std::cout << "NzMin:\t\t" << sac.nzmin << '\n';
  std::cout << "NzSec:\t\t" << sac.nzsec << '\n';
  std::cout << "NzMSec:\t\t" << sac.nzmsec << '\n';
  std::cout << "NvHdr:\t\t" << sac.nvhdr << '\n';
  std::cout << "NorID:\t\t" << sac.norid << '\n';
  std::cout << "NEvID:\t\t" << sac.nevid << '\n';
  std::cout << "Npts:\t\t" << sac.npts << '\n';
  std::cout << "NwfID:\t\t" << sac.nwfid << '\n';
  std::cout << "NXSize:\t\t" << sac.nxsize << '\n';
  std::cout << "NYSize:\t\t" << sac.nysize << '\n';
  std::cout << "IfType:\t\t" << sac.iftype << '\n';
  std::cout << "Idep:\t\t" << sac.idep << '\n';
  std::cout << "IzType:\t\t" << sac.iztype << '\n';
  std::cout << "IInst:\t\t" << sac.iinst << '\n';
  std::cout << "Istreg:\t\t" << sac.istreg << '\n';
  std::cout << "IevReg:\t\t" << sac.ievreg << '\n';
  std::cout << "IevTyp:\t\t" << sac.ievtyp << '\n';
  std::cout << "IQual:\t\t" << sac.iqual << '\n';
  std::cout << "ISynth:\t\t" << sac.isynth << '\n';
  std::cout << "IMagTyp:\t" << sac.imagtyp << '\n';
  std::cout << "IMagSrc:\t" << sac.imagsrc << '\n';
  std::cout << "IBody:\t\t" << sac.ibody << '\n';
  std::cout << "LEven:\t\t" << sac.leven << '\n';
  std::cout << "LpsPol:\t\t" << sac.lpspol << '\n';
  std::cout << "LovRok:\t\t" << sac.lovrok << '\n';
  std::cout << "LCalda:\t\t" << sac.lcalda << '\n';
  std::cout << "KStNm:\t\t" << sac.kstnm << '\n';
  std::cout << "KEvNm:\t\t" << sac.kevnm << '\n';
  std::cout << "KHole:\t\t" << sac.khole << '\n';
  std::cout << "KO:\t\t" << sac.ko << '\n';
  std::cout << "KA:\t\t" << sac.ka << '\n';
  std::cout << "KT0:\t\t" << sac.kt0 << '\n';
  std::cout << "KT1:\t\t" << sac.kt1 << '\n';
  std::cout << "KT2:\t\t" << sac.kt2 << '\n';
  std::cout << "KT3:\t\t" << sac.kt3 << '\n';
  std::cout << "KT4:\t\t" << sac.kt4 << '\n';
  std::cout << "KT5:\t\t" << sac.kt5 << '\n';
  std::cout << "KT6:\t\t" << sac.kt6 << '\n';
  std::cout << "KT7:\t\t" << sac.kt7 << '\n';
  std::cout << "KT8:\t\t" << sac.kt8 << '\n';
  std::cout << "KT9:\t\t" << sac.kt9 << '\n';
  std::cout << "KF:\t\t" << sac.kf << '\n';
  std::cout << "KUser0:\t\t" << sac.kuser0 << '\n';
  std::cout << "KUser1:\t\t" << sac.kuser1 << '\n';
  std::cout << "KUser2:\t\t" << sac.kuser2 << '\n';
  std::cout << "KCmpNm:\t\t" << sac.kcmpnm << '\n';
  std::cout << "KNetwk:\t\t" << sac.knetwk << '\n';
  std::cout << "KDatRd:\t\t" << sac.kdatrd << '\n';
  std::cout << "KInst:\t\t" << sac.kinst << '\n';
  //---------------------------------------------------------------------------
  // Additional information
  //---------------------------------------------------------------------------
  std::cout << "Data1 size:\t" << sac.data1.size() << " points\n";
  std::cout << "Data2 size:\t" << sac.data2.size() << " points\n";
  std::cout << "Class size:\t" << sizeof(sac) << " bytes\n";

  return 0;
}
