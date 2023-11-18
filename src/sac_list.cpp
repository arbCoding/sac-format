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
  std::string ext{file.extension()};
  // Issue a warning if the extension is not sac or SAC
  if (ext.empty() || ((ext != ".SAC") && (ext != ".sac"))) {
    std::cout << "WARNING: " << file << " does not have a SAC extension.\n";
  }
  // Okay, now we assume it is a SAC-file, let's go.
  Trace trace = Trace(file);
  // Time to spit out the information
  std::cout << "\033[1;36;40m========================================\n";
  std::cout << "SAC List\n";
  std::cout << "========================================\033[0m\n";
  std::cout << "File: " << file << '\n';
  std::cout << "\033[1;32;40m==============================\n";
  std::cout << "STATION\n";
  std::cout << "==============================\033[0m\n";
  std::cout << "KNetwk:\t\t" << trace.knetwk() << '\n';
  std::cout << "KStNm:\t\t" << trace.kstnm() << '\n';
  std::cout << "IInst:\t\t" << trace.iinst() << '\n';
  std::cout << "KInst:\t\t" << trace.kinst() << '\n';
  std::cout << "KCmpNm:\t\t" << trace.kcmpnm() << '\n';
  std::cout << "CmpAz:\t\t" << trace.cmpaz() << '\n';
  std::cout << "CmpInc:\t\t" << trace.cmpinc() << '\n';
  std::cout << "KHole:\t\t" << trace.khole() << '\n';
  std::cout << "StLa:\t\t" << trace.stla() << '\n';
  std::cout << "StLo:\t\t" << trace.stlo() << '\n';
  std::cout << "StEl:\t\t" << trace.stel() << '\n';
  std::cout << "StDp:\t\t" << trace.stdp() << '\n';
  std::cout << "IStReg:\t\t" << trace.istreg() << '\n';
  std::cout << "Resp0:\t\t" << trace.resp0() << '\n';
  std::cout << "Resp1:\t\t" << trace.resp1() << '\n';
  std::cout << "Resp2:\t\t" << trace.resp2() << '\n';
  std::cout << "Resp3:\t\t" << trace.resp3() << '\n';
  std::cout << "Resp4:\t\t" << trace.resp4() << '\n';
  std::cout << "Resp5:\t\t" << trace.resp5() << '\n';
  std::cout << "Resp6:\t\t" << trace.resp6() << '\n';
  std::cout << "Resp7:\t\t" << trace.resp7() << '\n';
  std::cout << "Resp8:\t\t" << trace.resp8() << '\n';
  std::cout << "Resp9:\t\t" << trace.resp9() << '\n';
  std::cout << "\033[1;32;40m==============================\n";
  std::cout << "EVENT\n";
  std::cout << "==============================\033[0m\n";
  std::cout << "NEvID:\t\t" << trace.nevid() << '\n';
  std::cout << "KEvNm:\t\t" << trace.kevnm() << '\n';
  std::cout << "EvLa:\t\t" << trace.evla() << '\n';
  std::cout << "EvLo:\t\t" << trace.evlo() << '\n';
  std::cout << "EvEl:\t\t" << trace.evel() << '\n';
  std::cout << "EvDp:\t\t" << trace.evdp() << '\n';
  std::cout << "IEvReg:\t\t" << trace.ievreg() << '\n';
  std::cout << "IEvTyp:\t\t" << trace.ievtyp() << '\n';
  std::cout << "Mag:\t\t" << trace.mag() << '\n';
  std::cout << "IMagTyp:\t" << trace.imagtyp() << '\n';
  std::cout << "IMagSrc:\t" << trace.imagsrc() << '\n';
  std::cout << "\033[1;32;40m==============================\n";
  std::cout << "USER\n";
  std::cout << "==============================\033[0m\n";
  std::cout << "KUser0:\t\t" << trace.kuser0() << '\n';
  std::cout << "User0:\t\t" << trace.user0() << '\n';
  std::cout << "KUser1:\t\t" << trace.kuser1() << '\n';
  std::cout << "User1:\t\t" << trace.user1() << '\n';
  std::cout << "KUser2:\t\t" << trace.kuser2() << '\n';
  std::cout << "User2:\t\t" << trace.user2() << '\n';
  std::cout << "User3:\t\t" << trace.user3() << '\n';
  std::cout << "User4:\t\t" << trace.user4() << '\n';
  std::cout << "User5:\t\t" << trace.user5() << '\n';
  std::cout << "User6:\t\t" << trace.user6() << '\n';
  std::cout << "User7:\t\t" << trace.user7() << '\n';
  std::cout << "User8:\t\t" << trace.user8() << '\n';
  std::cout << "User9:\t\t" << trace.user9() << '\n';
  std::cout << "\033[1;32;40m==============================\n";
  std::cout << "GEOMETRY\n";
  std::cout << "==============================\033[0m\n";
  std::cout << "IBody:\t\t" << trace.ibody() << '\n';
  std::cout << "GCArc:\t\t" << trace.gcarc() << '\n';
  std::cout << "Dist:\t\t" << trace.dist() << '\n';
  std::cout << "Az:\t\t" << trace.az() << '\n';
  std::cout << "BAz:\t\t" << trace.baz() << '\n';
  std::cout << "\033[1;32;40m==============================\n";
  std::cout << "REFERENCE TIME\n";
  std::cout << "==============================\033[0m\n";
  std::cout << "IZType:\t\t" << trace.iztype() << '\n';
  std::cout << "NzYear:\t\t" << trace.nzyear() << '\n';
  std::cout << "NzJDay:\t\t" << trace.nzjday() << '\n';
  std::cout << "NzHour:\t\t" << trace.nzhour() << '\n';
  std::cout << "NzMin:\t\t" << trace.nzmin() << '\n';
  std::cout << "NzSec:\t\t" << trace.nzsec() << '\n';
  std::cout << "NzMSec:\t\t" << trace.nzmsec() << '\n';
  std::cout << "\033[1;32;40m==============================\n";
  std::cout << "RELATIVE TiME\n";
  std::cout << "==============================\033[0m\n";
  std::cout << "B:\t\t" << trace.b() << '\n';
  std::cout << "SB:\t\t" << trace.sb() << '\n';
  std::cout << "E:\t\t" << trace.e() << '\n';
  std::cout << "NOrID:\t\t" << trace.norid() << '\n';
  std::cout << "KO:\t\t" << trace.ko() << '\n';
  std::cout << "O:\t\t" << trace.o() << '\n';
  std::cout << "KA:\t\t" << trace.ka() << '\n';
  std::cout << "A:\t\t" << trace.a() << '\n';
  std::cout << "KF:\t\t" << trace.kf() << '\n';
  std::cout << "F:\t\t" << trace.f() << '\n';
  std::cout << "KT0:\t\t" << trace.kt0() << '\n';
  std::cout << "T0:\t\t" << trace.t0() << '\n';
  std::cout << "KT1:\t\t" << trace.kt1() << '\n';
  std::cout << "T1:\t\t" << trace.t1() << '\n';
  std::cout << "KT2:\t\t" << trace.kt2() << '\n';
  std::cout << "T2:\t\t" << trace.t2() << '\n';
  std::cout << "KT3:\t\t" << trace.kt3() << '\n';
  std::cout << "T3:\t\t" << trace.t3() << '\n';
  std::cout << "KT4:\t\t" << trace.kt4() << '\n';
  std::cout << "T4:\t\t" << trace.t4() << '\n';
  std::cout << "KT5:\t\t" << trace.kt5() << '\n';
  std::cout << "T5:\t\t" << trace.t5() << '\n';
  std::cout << "KT6:\t\t" << trace.kt6() << '\n';
  std::cout << "T6:\t\t" << trace.t6() << '\n';
  std::cout << "KT7:\t\t" << trace.kt7() << '\n';
  std::cout << "T7:\t\t" << trace.t7() << '\n';
  std::cout << "KT8:\t\t" << trace.kt8() << '\n';
  std::cout << "T8:\t\t" << trace.t8() << '\n';
  std::cout << "KT9:\t\t" << trace.kt9() << '\n';
  std::cout << "T9:\t\t" << trace.t9() << '\n';
  std::cout << "\033[1;32;40m==============================\n";
  std::cout << "META-DATA\n";
  std::cout << "==============================\033[0m\n";
  std::cout << "NvHdr:\t\t" << trace.nvhdr() << '\n';
  std::cout << "Npts:\t\t" << trace.npts() << '\n';
  std::cout << "NsNpts:\t\t" << trace.nsnpts() << '\n';
  std::cout << "NWfID:\t\t" << trace.nwfid() << '\n';
  std::cout << "IFType:\t\t" << trace.iftype() << '\n';
  std::cout << "IQual:\t\t" << trace.iqual() << '\n';
  std::cout << "ISynth:\t\t" << trace.isynth() << '\n';
  std::cout << "LEven:\t\t" << trace.leven() << '\n';
  std::cout << "LPsPol:\t\t" << trace.lpspol() << '\n';
  std::cout << "LOvrOK:\t\t" << trace.lovrok() << '\n';
  std::cout << "LCalDA:\t\t" << trace.lcalda() << '\n';
  std::cout << "YMinimum:\t" << trace.yminimum() << '\n';
  std::cout << "YMaximum:\t" << trace.ymaximum() << '\n';
  std::cout << "Delta:\t\t" << trace.delta() << '\n';
  std::cout << "ODelta:\t\t" << trace.odelta() << '\n';
  std::cout << "SDelta:\t\t" << trace.sdelta() << '\n';
  std::cout << "KDatRd:\t\t" << trace.kdatrd() << '\n';
  std::cout << "\033[1;34;40m------------------------------\n";
  std::cout << "DEPENDENT VARIABLE\n";
  std::cout << "------------------------------\033[0m\n";
  std::cout << "IDep:\t\t" << trace.idep() << '\n';
  std::cout << "DepMin:\t\t" << trace.depmin() << '\n';
  std::cout << "DepMen:\t\t" << trace.depmen() << '\n';
  std::cout << "DepMax:\t\t" << trace.depmax() << '\n';
  std::cout << "NXSize:\t\t" << trace.nxsize() << '\n';
  std::cout << "NYSize:\t\t" << trace.nysize() << '\n';
  std::cout << "XMinimum:\t" << trace.xminimum() << '\n';
  std::cout << "XMaximum:\t" << trace.xmaximum() << '\n';
  std::cout << "\033[1;32;40m==============================\n";
  std::cout << "DATA\n";
  std::cout << "==============================\033[0m\n";
  std::cout << "Data1:\t\t" << trace.data1().size() << '\n';
  std::cout << "Data2:\t\t" << trace.data2().size() << '\n';
  return EXIT_SUCCESS;
}
