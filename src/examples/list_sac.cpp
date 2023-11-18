#include <filesystem>
#include <iostream>

#include "sac_format.hpp"

namespace fs = std::filesystem;
using namespace sacfmt;

int main(const int arg_count, const char* arg_array[]) {
  // Correct number of arguments
  if (arg_count != 2) {
    std::printf("Usage: `%s <sac-file>`\n", arg_array[0]);
    return EXIT_FAILURE;
  }
  fs::path file{arg_array[1]};
  fs::file_status status(fs::status(file));
  // It exists
  if (!fs::exists(status)) {
    std::cout << file << " does not exist.\n";
    return EXIT_FAILURE;
  }
  // It's a regular file
  if (!fs::is_regular_file(status)) {
    std::cout << file << " is not a readable file.\n";
  }
  // Now we check the extension
  std::string ext = file.extension();
  // Issue a warning if the extension is not sac or SAC
  if (ext.empty() || ((ext != ".SAC") && (ext != ".sac"))) {
    std::cout << "WARNING: " << file << " does not have a SAC extension.\n";
  }
  // Okay, now we assume it is a SAC-file, let's go.
  Trace trace = Trace(file);
  // Time to spit out the information
  std::cout << "\033[1;36;40m=======================================================\n";
  std::cout << "\t\tSAC List\n";
  std::cout << "=======================================================\033[0m\n";
  std::cout << "\tFile: " << file << '\n';
  std::cout << "\033[1;32;40m=============================================\n";
  std::cout << "\t\tSTATION\n";
  std::cout << "=============================================\033[0m\n";
  std::cout << "\tKNetwk:\t\t" << trace.knetwk() << '\n';
  std::cout << "\tKStNm:\t\t" << trace.kstnm() << '\n';
  std::cout << "\tIInst:\t\t" << trace.iinst() << '\n';
  std::cout << "\tKInst:\t\t" << trace.kinst() << '\n';
  std::cout << "\tKCmpNm:\t\t" << trace.kcmpnm() << '\n';
  std::cout << "\tCmpAz:\t\t" << trace.cmpaz() << '\n';
  std::cout << "\tCmpInc:\t\t" << trace.cmpinc() << '\n';
  std::cout << "\tKHole:\t\t" << trace.khole() << '\n';
  std::cout << "\tStLa:\t\t" << trace.stla() << '\n';
  std::cout << "\tStLo:\t\t" << trace.stlo() << '\n';
  std::cout << "\tStEl:\t\t" << trace.stel() << '\n';
  std::cout << "\tStDp:\t\t" << trace.stdp() << '\n';
  std::cout << "\tIStReg:\t\t" << trace.istreg() << '\n';
  std::cout << "\tResp0:\t\t" << trace.resp0() << '\n';
  std::cout << "\tResp1:\t\t" << trace.resp1() << '\n';
  std::cout << "\tResp2:\t\t" << trace.resp2() << '\n';
  std::cout << "\tResp3:\t\t" << trace.resp3() << '\n';
  std::cout << "\tResp4:\t\t" << trace.resp4() << '\n';
  std::cout << "\tResp5:\t\t" << trace.resp5() << '\n';
  std::cout << "\tResp6:\t\t" << trace.resp6() << '\n';
  std::cout << "\tResp7:\t\t" << trace.resp7() << '\n';
  std::cout << "\tResp8:\t\t" << trace.resp8() << '\n';
  std::cout << "\tResp9:\t\t" << trace.resp9() << '\n';
  std::cout << "\033[1;32;40m=============================================\n";
  std::cout << "\t\tEVENT\n";
  std::cout << "=============================================\033[0m\n";
  std::cout << "\tNEvID:\t\t" << trace.nevid() << '\n';
  std::cout << "\tKEvNm:\t\t" << trace.kevnm() << '\n';
  std::cout << "\tEvLa:\t\t" << trace.evla() << '\n';
  std::cout << "\tEvLo:\t\t" << trace.evlo() << '\n';
  std::cout << "\tEvEl:\t\t" << trace.evel() << '\n';
  std::cout << "\tEvDp:\t\t" << trace.evdp() << '\n';
  std::cout << "\tIEvReg:\t\t" << trace.ievreg() << '\n';
  std::cout << "\tIEvTyp:\t\t" << trace.ievtyp() << '\n';
  std::cout << "\tMag:\t\t" << trace.mag() << '\n';
  std::cout << "\tIMagTyp:\t" << trace.imagtyp() << '\n';
  std::cout << "\tIMagSrc:\t" << trace.imagsrc() << '\n';
  std::cout << "\033[1;32;40m=============================================\n";
  std::cout << "\t\tUSER\n";
  std::cout << "=============================================\033[0m\n";
  std::cout << "\tKUser0:\t\t" << trace.kuser0() << '\n';
  std::cout << "\tUser0:\t\t" << trace.user0() << '\n';
  std::cout << "\tKUser1:\t\t" << trace.kuser1() << '\n';
  std::cout << "\tUser1:\t\t" << trace.user1() << '\n';
  std::cout << "\tKUser2:\t\t" << trace.kuser2() << '\n';
  std::cout << "\tUser2:\t\t" << trace.user2() << '\n';
  std::cout << "\tUser3:\t\t" << trace.user3() << '\n';
  std::cout << "\tUser4:\t\t" << trace.user4() << '\n';
  std::cout << "\tUser5:\t\t" << trace.user5() << '\n';
  std::cout << "\tUser6:\t\t" << trace.user6() << '\n';
  std::cout << "\tUser7:\t\t" << trace.user7() << '\n';
  std::cout << "\tUser8:\t\t" << trace.user8() << '\n';
  std::cout << "\tUser9:\t\t" << trace.user9() << '\n';
  std::cout << "\033[1;32;40m=============================================\n";
  std::cout << "\t\tGEOMETRY\n";
  std::cout << "=============================================\033[0m\n";
  std::cout << "\tIBody:\t\t" << trace.ibody() << '\n';
  std::cout << "\tGCArc:\t\t" << trace.gcarc() << '\n';
  std::cout << "\tDist:\t\t" << trace.dist() << '\n';
  std::cout << "\tAz:\t\t" << trace.az() << '\n';
  std::cout << "\tBAz:\t\t" << trace.baz() << '\n';
  std::cout << "\033[1;32;40m=============================================\n";
  std::cout << "\t\tREFERENCE TIME\n";
  std::cout << "=============================================\033[0m\n";
  std::cout << "\tIZType:\t\t" << trace.iztype() << '\n';
  std::cout << "\tNzYear:\t\t" << trace.nzyear() << '\n';
  std::cout << "\tNzJDay:\t\t" << trace.nzjday() << '\n';
  std::cout << "\tNzHour:\t\t" << trace.nzhour() << '\n';
  std::cout << "\tNzMin:\t\t" << trace.nzmin() << '\n';
  std::cout << "\tNzSec:\t\t" << trace.nzsec() << '\n';
  std::cout << "\tNzMSec:\t\t" << trace.nzmsec() << '\n';
  std::cout << "\033[1;32;40m=============================================\n";
  std::cout << "\t\tRELATIVE TIME\n";
  std::cout << "=============================================\033[0m\n";
  std::cout << "\tB:\t\t" << trace.b() << '\n';
  std::cout << "\tSB:\t\t" << trace.sb() << '\n';
  std::cout << "\tE:\t\t" << trace.e() << '\n';
  std::cout << "\tNOrID:\t\t" << trace.norid() << '\n';
  std::cout << "\tKO:\t\t" << trace.ko() << '\n';
  std::cout << "\tO:\t\t" << trace.o() << '\n';
  std::cout << "\tKA:\t\t" << trace.ka() << '\n';
  std::cout << "\tA:\t\t" << trace.a() << '\n';
  std::cout << "\tKF:\t\t" << trace.kf() << '\n';
  std::cout << "\tF:\t\t" << trace.f() << '\n';
  std::cout << "\tKT0:\t\t" << trace.kt0() << '\n';
  std::cout << "\tT0:\t\t" << trace.t0() << '\n';
  std::cout << "\tKT1:\t\t" << trace.kt1() << '\n';
  std::cout << "\tT1:\t\t" << trace.t1() << '\n';
  std::cout << "\tKT2:\t\t" << trace.kt2() << '\n';
  std::cout << "\tT2:\t\t" << trace.t2() << '\n';
  std::cout << "\tKT3:\t\t" << trace.kt3() << '\n';
  std::cout << "\tT3:\t\t" << trace.t3() << '\n';
  std::cout << "\tKT4:\t\t" << trace.kt4() << '\n';
  std::cout << "\tT4:\t\t" << trace.t4() << '\n';
  std::cout << "\tKT5:\t\t" << trace.kt5() << '\n';
  std::cout << "\tT5:\t\t" << trace.t5() << '\n';
  std::cout << "\tKT6:\t\t" << trace.kt6() << '\n';
  std::cout << "\tT6:\t\t" << trace.t6() << '\n';
  std::cout << "\tKT7:\t\t" << trace.kt7() << '\n';
  std::cout << "\tT7:\t\t" << trace.t7() << '\n';
  std::cout << "\tKT8:\t\t" << trace.kt8() << '\n';
  std::cout << "\tT8:\t\t" << trace.t8() << '\n';
  std::cout << "\tKT9:\t\t" << trace.kt9() << '\n';
  std::cout << "\tT9:\t\t" << trace.t9() << '\n';
  std::cout << "\033[1;32;40m=============================================\n";
  std::cout << "\t\tMETA-DATA\n";
  std::cout << "=============================================\033[0m\n";
  std::cout << "\tNvHdr:\t\t" << trace.nvhdr() << '\n';
  std::cout << "\tNpts:\t\t" << trace.npts() << '\n';
  std::cout << "\tNsNpts:\t\t" << trace.nsnpts() << '\n';
  std::cout << "\tNWfID:\t\t" << trace.nwfid() << '\n';
  std::cout << "\tIFType:\t\t" << trace.iftype() << '\n';
  std::cout << "\tIQual:\t\t" << trace.iqual() << '\n';
  std::cout << "\tISynth:\t\t" << trace.isynth() << '\n';
  std::cout << "\tLEven:\t\t" << trace.leven() << '\n';
  std::cout << "\tLPsPol:\t\t" << trace.lpspol() << '\n';
  std::cout << "\tLOvrOK:\t\t" << trace.lovrok() << '\n';
  std::cout << "\tLCalDA:\t\t" << trace.lcalda() << '\n';
  std::cout << "\tYMinimum:\t" << trace.yminimum() << '\n';
  std::cout << "\tYMaximum:\t" << trace.ymaximum() << '\n';
  std::cout << "\tDelta:\t\t" << trace.delta() << '\n';
  std::cout << "\tODelta:\t\t" << trace.odelta() << '\n';
  std::cout << "\tSDelta:\t\t" << trace.sdelta() << '\n';
  std::cout << "\tKDatRd:\t\t" << trace.kdatrd() << '\n';
  std::cout << "\033[1;34;40m----------------------------------------\n";
  std::cout << "\tDEPENDENT VARIABLE\n";
  std::cout << "----------------------------------------\033[0m\n";
  std::cout << "\tIDep:\t\t" << trace.idep() << '\n';
  std::cout << "\tDepMin:\t\t" << trace.depmin() << '\n';
  std::cout << "\tDepMen:\t\t" << trace.depmen() << '\n';
  std::cout << "\tDepMax:\t\t" << trace.depmax() << '\n';
  std::cout << "\tNXSize:\t\t" << trace.nxsize() << '\n';
  std::cout << "\tNYSize:\t\t" << trace.nysize() << '\n';
  std::cout << "\tXMinimum:\t" << trace.xminimum() << '\n';
  std::cout << "\tXMaximum:\t" << trace.xmaximum() << '\n';
  std::cout << "\033[1;32;40m=============================================\n";
  std::cout << "\t\tDATA\n";
  std::cout << "=============================================\033[0m\n";
  std::cout << "\tData1:\t\t" << trace.data1().size() << '\n';
  std::cout << "\tData2:\t\t" << trace.data2().size() << '\n';
  return EXIT_SUCCESS;
}
