#include "sac_class.hpp"

namespace SAC
{
//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------
// Constructor from file
Sac_Class::Sac_Class(const std::string& file_name)
{
  //---------------------------------------------------------------------------
  // Open binary file
  //---------------------------------------------------------------------------
  std::ifstream file(file_name, std::ifstream::binary);
  if (!file)
  {
    std::cerr << "file could not be read...\n";
  }
  // Make sure we're at the start
  file.seekg(0);
  //---------------------------------------------------------------------------
  // End open binary file
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  // Header
  //---------------------------------------------------------------------------
  f_delta  = binary_to_float(read_word(&file));
  depmin = binary_to_float(read_word(&file));
  depmax = binary_to_float(read_word(&file));
  
  // Skip 'unused'
  read_word(&file);
  
  odelta = binary_to_float(read_word(&file)); 
  f_b = binary_to_float(read_word(&file));
  f_e = binary_to_float(read_word(&file));
  f_o = binary_to_float(read_word(&file));
  f_a = binary_to_float(read_word(&file)); 

  // Skip 'internal'
  read_word(&file);

  // Arrival time picking headers
  // All -12345
  f_t0 = binary_to_float(read_word(&file)); 
  f_t1 = binary_to_float(read_word(&file)); 
  f_t2 = binary_to_float(read_word(&file)); 
  f_t3 = binary_to_float(read_word(&file)); 
  f_t4 = binary_to_float(read_word(&file)); 
  f_t5 = binary_to_float(read_word(&file)); 
  f_t6 = binary_to_float(read_word(&file)); 
  f_t7 = binary_to_float(read_word(&file)); 
  f_t8 = binary_to_float(read_word(&file)); 
  f_t9 = binary_to_float(read_word(&file)); 
  f_f = binary_to_float(read_word(&file)); 

  // Resp headers
  // All -12345
  resp0 = binary_to_float(read_word(&file)); 
  resp1 = binary_to_float(read_word(&file)); 
  resp2 = binary_to_float(read_word(&file)); 
  resp3 = binary_to_float(read_word(&file)); 
  resp4 = binary_to_float(read_word(&file));
  resp5 = binary_to_float(read_word(&file)); 
  resp6 = binary_to_float(read_word(&file)); 
  resp7 = binary_to_float(read_word(&file)); 
  resp8 = binary_to_float(read_word(&file)); 
  resp9 = binary_to_float(read_word(&file)); 

  // Station headers
  f_stla = binary_to_float(read_word(&file));
  f_stlo = binary_to_float(read_word(&file));
  stel = binary_to_float(read_word(&file));
  stdp = binary_to_float(read_word(&file));

  // Event headers
  f_evla = binary_to_float(read_word(&file));
  f_evlo = binary_to_float(read_word(&file));
  evel = binary_to_float(read_word(&file));
  evdp = binary_to_float(read_word(&file));
  mag = binary_to_float(read_word(&file));
 
  // User headers
  user0 = binary_to_float(read_word(&file)); 
  user1 = binary_to_float(read_word(&file)); 
  user2 = binary_to_float(read_word(&file)); 
  user3 = binary_to_float(read_word(&file)); 
  user4 = binary_to_float(read_word(&file)); 
  user5 = binary_to_float(read_word(&file)); 
  user6 = binary_to_float(read_word(&file)); 
  user7 = binary_to_float(read_word(&file)); 
  user8 = binary_to_float(read_word(&file)); 
  user9 = binary_to_float(read_word(&file)); 
  dist = binary_to_float(read_word(&file));
  az = binary_to_float(read_word(&file));
  baz = binary_to_float(read_word(&file));
  gcarc = binary_to_float(read_word(&file));
  f_sb = binary_to_float(read_word(&file));
  f_sdelta = binary_to_float(read_word(&file));

  depmen = binary_to_float(read_word(&file)); 
  cmpaz = binary_to_float(read_word(&file));
  cmpinc = binary_to_float(read_word(&file));
  xminimum = binary_to_float(read_word(&file)); 
  xmaximum = binary_to_float(read_word(&file)); 
  yminimum = binary_to_float(read_word(&file)); 
  ymaximum = binary_to_float(read_word(&file)); 

  // Skip 'unused' (x7)
  read_word(&file);
  read_word(&file);
  read_word(&file);
  read_word(&file);
  read_word(&file);
  read_word(&file);
  read_word(&file);

  // Date_time headers
  nzyear = binary_to_int(read_word(&file));
  nzjday = binary_to_int(read_word(&file));
  nzhour = binary_to_int(read_word(&file));
  nzmin = binary_to_int(read_word(&file));
  nzsec = binary_to_int(read_word(&file));
  nzmsec = binary_to_int(read_word(&file));
  nvhdr = binary_to_int(read_word(&file));
  norid = binary_to_int(read_word(&file)); 
  nevid = binary_to_int(read_word(&file)); 
  npts = binary_to_int(read_word(&file));
  nsnpts = binary_to_int(read_word(&file));
  nwfid = binary_to_int(read_word(&file)); 
  nxsize = binary_to_int(read_word(&file)); 
  nysize = binary_to_int(read_word(&file)); 
 
  // Skip 'unused'
  read_word(&file);

  iftype = binary_to_int(read_word(&file));
  idep = binary_to_int(read_word(&file)); 
  iztype = binary_to_int(read_word(&file)); 
  
  // Skip 'unused'
  read_word(&file);

  iinst = binary_to_int(read_word(&file)); 
  istreg = binary_to_int(read_word(&file)); 
  ievreg = binary_to_int(read_word(&file));
  ievtyp = binary_to_int(read_word(&file));
  iqual = binary_to_int(read_word(&file));
  isynth = binary_to_int(read_word(&file));
  imagtyp = binary_to_int(read_word(&file));
  imagsrc = binary_to_int(read_word(&file)); 
  ibody = binary_to_int(read_word(&file)); 

  // Skip 'unused' (x7)
  read_word(&file);
  read_word(&file);
  read_word(&file);
  read_word(&file);
  read_word(&file);
  read_word(&file);
  read_word(&file);

  // Logical headers
  leven = binary_to_bool(read_word(&file));
  lpspol = binary_to_bool(read_word(&file)); 
  lovrok = binary_to_bool(read_word(&file));
  lcalda = binary_to_bool(read_word(&file));

  // Skip 'unused'
  read_word(&file);
  
  // KSTNM is 2 words long (like all other 'K' headers)
  kstnm = binary_to_string(read_two_words(&file));

  // KEVNM is 4 words long (special rule!)
  kevnm = binary_to_long_string(read_four_words(&file));

  // All other 'K' headers are 2 words long
  khole = binary_to_string(read_two_words(&file));
  ko = binary_to_string(read_two_words(&file));
  ka = binary_to_string(read_two_words(&file));
  kt0 = binary_to_string(read_two_words(&file));
  kt1 = binary_to_string(read_two_words(&file));
  kt2 = binary_to_string(read_two_words(&file));
  kt3 = binary_to_string(read_two_words(&file));
  kt4 = binary_to_string(read_two_words(&file));
  kt5 = binary_to_string(read_two_words(&file));
  kt6 = binary_to_string(read_two_words(&file));
  kt7 = binary_to_string(read_two_words(&file));
  kt8 = binary_to_string(read_two_words(&file));
  kt9 = binary_to_string(read_two_words(&file));
  kf = binary_to_string(read_two_words(&file));
  kuser0 = binary_to_string(read_two_words(&file));
  kuser1 = binary_to_string(read_two_words(&file));
  kuser2 = binary_to_string(read_two_words(&file));
  kcmpnm = binary_to_string(read_two_words(&file));
  knetwk = binary_to_string(read_two_words(&file));
  kdatrd = binary_to_string(read_two_words(&file));
  kinst = binary_to_string(read_two_words(&file));
  //---------------------------------------------------------------------------
  // End header
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  // Data
  //---------------------------------------------------------------------------
  data1 = read_data(&file, static_cast<size_t>(npts));
  // (Unevenly sampled-data) or (spectral or xy)
  // Same size as data1
  if ((leven == 0) || (iftype > 1))
  {
    data2 = read_data(&file, static_cast<size_t>(npts), SAC::data_word + npts);
  }

  //---------------------------------------------------------------------------
  // End data
  //---------------------------------------------------------------------------
  
  //---------------------------------------------------------------------------
  // Footer (if nvhdr = 7)
  //---------------------------------------------------------------------------
  // NOTE: NVHDR = 6 was the format for SEVERAL DECADES, NVHDR = 7 is from 2020
  //   and beyond
  // New version of format, load in the footer values after data section(s)
  if (nvhdr == 7)
  {
    delta = binary_to_double(read_two_words(&file));
    b = binary_to_double(read_two_words(&file));
    e = binary_to_double(read_two_words(&file));
    o = binary_to_double(read_two_words(&file));
    a = binary_to_double(read_two_words(&file));
    t0 = binary_to_double(read_two_words(&file));
    t1 = binary_to_double(read_two_words(&file));
    t2 = binary_to_double(read_two_words(&file));
    t3 = binary_to_double(read_two_words(&file));
    t4 = binary_to_double(read_two_words(&file));
    t5 = binary_to_double(read_two_words(&file));
    t6 = binary_to_double(read_two_words(&file));
    t7 = binary_to_double(read_two_words(&file));
    t8 = binary_to_double(read_two_words(&file));
    t9 = binary_to_double(read_two_words(&file));
    f = binary_to_double(read_two_words(&file));
    evlo = binary_to_double(read_two_words(&file));
    evla = binary_to_double(read_two_words(&file));
    stlo = binary_to_double(read_two_words(&file));
    stla = binary_to_double(read_two_words(&file));
    sb = binary_to_double(read_two_words(&file));
    sdelta = binary_to_double(read_two_words(&file));
  }
  else
  {
    // Convert to NVHDR = 7
    delta = static_cast<double>(f_delta);
    b = static_cast<double>(f_b);
    e = static_cast<double>(f_e);
    o = static_cast<double>(f_o);
    a = static_cast<double>(f_a);
    t0 = static_cast<double>(f_t0);
    t1 = static_cast<double>(f_t1);
    t2 = static_cast<double>(f_t2);
    t3 = static_cast<double>(f_t3);
    t4 = static_cast<double>(f_t4);
    t5 = static_cast<double>(f_t5);
    t6 = static_cast<double>(f_t6);
    t7 = static_cast<double>(f_t7);
    t8 = static_cast<double>(f_t8);
    t9 = static_cast<double>(f_t9);
    f = static_cast<double>(f_f);
    evlo = static_cast<double>(f_evlo);
    evla = static_cast<double>(f_evla);
    stlo = static_cast<double>(f_stlo);
    stla = static_cast<double>(f_stla);
    sb = static_cast<double>(f_sb);
    sdelta = static_cast<double>(f_sdelta);
    nvhdr = 7;
  }
  //---------------------------------------------------------------------------
  // End footer 
  //---------------------------------------------------------------------------
  file.close();
}
//-----------------------------------------------------------------------------
// End constructors
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Writing
//-----------------------------------------------------------------------------
void Sac_Class::write(const std::string& file_name)
{
  std::ofstream file(file_name, std::ios::binary | std::ios::out | std::ios::trunc);
  if (!file)
  {
    std::cerr << "Unable to write file...\n";
  }
  //---------------------------------------------------------------------------
  // Header
  //---------------------------------------------------------------------------
  //write_word(&file, float_to_binary(f_delta));
  //write_word(&file, float_to_binary(depmin));
  /*
  SAC::write_words(&file, convert_to_word(depmax));
  // Fill 'unused'
  SAC::write_words(&file, convert_to_word(depmax));

  SAC::write_words(&file, convert_to_word(odelta));
  SAC::write_words(&file, convert_to_word(f_b));
  SAC::write_words(&file, convert_to_word(f_e));
  SAC::write_words(&file, convert_to_word(f_o));
  SAC::write_words(&file, convert_to_word(f_a));
  // Fill 'internal'
  SAC::write_words(&file, convert_to_word(f_a)); // Could give this a name and a value..

  SAC::write_words(&file, convert_to_word(f_t0));
  SAC::write_words(&file, convert_to_word(f_t1));
  SAC::write_words(&file, convert_to_word(f_t2));
  SAC::write_words(&file, convert_to_word(f_t3));
  SAC::write_words(&file, convert_to_word(f_t4));
  SAC::write_words(&file, convert_to_word(f_t5));
  SAC::write_words(&file, convert_to_word(f_t6));
  SAC::write_words(&file, convert_to_word(f_t7));
  SAC::write_words(&file, convert_to_word(f_t8));
  SAC::write_words(&file, convert_to_word(f_t9));
  SAC::write_words(&file, convert_to_word(f_f));
  SAC::write_words(&file, convert_to_word(resp0));
  SAC::write_words(&file, convert_to_word(resp1));
  SAC::write_words(&file, convert_to_word(resp2));
  SAC::write_words(&file, convert_to_word(resp3));
  SAC::write_words(&file, convert_to_word(resp4));
  SAC::write_words(&file, convert_to_word(resp5));
  SAC::write_words(&file, convert_to_word(resp6));
  SAC::write_words(&file, convert_to_word(resp7));
  SAC::write_words(&file, convert_to_word(resp8));
  SAC::write_words(&file, convert_to_word(resp9));
  SAC::write_words(&file, convert_to_word(f_stla));
  SAC::write_words(&file, convert_to_word(f_stlo));
  SAC::write_words(&file, convert_to_word(stel));
  SAC::write_words(&file, convert_to_word(stdp));
  SAC::write_words(&file, convert_to_word(f_evla));
  SAC::write_words(&file, convert_to_word(f_evlo));
  SAC::write_words(&file, convert_to_word(evel));
  SAC::write_words(&file, convert_to_word(evdp));
  SAC::write_words(&file, convert_to_word(mag));
  SAC::write_words(&file, convert_to_word(user0));
  SAC::write_words(&file, convert_to_word(user1));
  SAC::write_words(&file, convert_to_word(user2));
  SAC::write_words(&file, convert_to_word(user3));
  SAC::write_words(&file, convert_to_word(user4));
  SAC::write_words(&file, convert_to_word(user5));
  SAC::write_words(&file, convert_to_word(user6));
  SAC::write_words(&file, convert_to_word(user7));
  SAC::write_words(&file, convert_to_word(user8));
  SAC::write_words(&file, convert_to_word(user9));
  SAC::write_words(&file, convert_to_word(dist));
  SAC::write_words(&file, convert_to_word(az));
  SAC::write_words(&file, convert_to_word(baz));
  SAC::write_words(&file, convert_to_word(gcarc));
  SAC::write_words(&file, convert_to_word(f_sb));
  SAC::write_words(&file, convert_to_word(f_sdelta));

  SAC::write_words(&file, convert_to_word(depmen));
  SAC::write_words(&file, convert_to_word(cmpaz));
  SAC::write_words(&file, convert_to_word(cmpinc));
  SAC::write_words(&file, convert_to_word(xminimum));
  SAC::write_words(&file, convert_to_word(xmaximum));
  SAC::write_words(&file, convert_to_word(yminimum));
  SAC::write_words(&file, convert_to_word(ymaximum));
  // Fill 'unused' (x7)
  // Could give these names and values
  SAC::write_words(&file, convert_to_word(ymaximum));
  SAC::write_words(&file, convert_to_word(ymaximum));
  SAC::write_words(&file, convert_to_word(ymaximum));
  SAC::write_words(&file, convert_to_word(ymaximum));
  SAC::write_words(&file, convert_to_word(ymaximum));
  SAC::write_words(&file, convert_to_word(ymaximum));
  SAC::write_words(&file, convert_to_word(ymaximum));

  SAC::write_words(&file, convert_to_word(nzyear));
  SAC::write_words(&file, convert_to_word(nzjday));
  SAC::write_words(&file, convert_to_word(nzhour));
  SAC::write_words(&file, convert_to_word(nzmin));
  SAC::write_words(&file, convert_to_word(nzsec));
  SAC::write_words(&file, convert_to_word(nzmsec));
  SAC::write_words(&file, convert_to_word(nvhdr));
  SAC::write_words(&file, convert_to_word(norid));
  SAC::write_words(&file, convert_to_word(nevid));
  SAC::write_words(&file, convert_to_word(npts));
  SAC::write_words(&file, convert_to_word(nsnpts));

  SAC::write_words(&file, convert_to_word(nwfid));
  SAC::write_words(&file, convert_to_word(nxsize));
  SAC::write_words(&file, convert_to_word(nysize));
  // Fill 'unused'
  // could give a name and a value
  SAC::write_words(&file, convert_to_word(nysize));

  SAC::write_words(&file, convert_to_word(iftype));
  SAC::write_words(&file, convert_to_word(idep));
  SAC::write_words(&file, convert_to_word(iztype));
  // Fill 'unused'
  // could give a name and a vlue
  SAC::write_words(&file, convert_to_word(iztype));

  SAC::write_words(&file, convert_to_word(iinst));
  SAC::write_words(&file, convert_to_word(istreg));
  SAC::write_words(&file, convert_to_word(ievreg));
  SAC::write_words(&file, convert_to_word(ievtyp));
  SAC::write_words(&file, convert_to_word(iqual));
  SAC::write_words(&file, convert_to_word(isynth));
  SAC::write_words(&file, convert_to_word(imagtyp));
  SAC::write_words(&file, convert_to_word(imagsrc));
  SAC::write_words(&file, convert_to_word(ibody));
  // Fill 'unused' (x7)
  // could give these names and values
  SAC::write_words(&file, convert_to_word(ibody));
  SAC::write_words(&file, convert_to_word(ibody));
  SAC::write_words(&file, convert_to_word(ibody));
  SAC::write_words(&file, convert_to_word(ibody));
  SAC::write_words(&file, convert_to_word(ibody));
  SAC::write_words(&file, convert_to_word(ibody));
  SAC::write_words(&file, convert_to_word(ibody));

  SAC::write_words(&file, bool_to_word(leven));
  SAC::write_words(&file, bool_to_word(lpspol));
  SAC::write_words(&file, bool_to_word(lovrok));
  SAC::write_words(&file, bool_to_word(lcalda));
  // Fill 'unused'
  // could give this a name and a value
  SAC::write_words(&file, bool_to_word(lcalda));

  std::array<char, 2 * word_length> two_words;
  two_words = convert_to_words<sizeof(two_words)>(kstnm, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  std::array<char, 4 * word_length> four_words;
  four_words = convert_to_words<sizeof(four_words)>(kevnm, 4);
  SAC::write_words(&file, std::vector<char>(four_words.begin(), four_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(khole, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(ko, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(ka, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt0, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt1, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt2, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt3, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt4, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt5, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt6, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt7, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt8, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kt9, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kf, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kuser0, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kuser1, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kuser2, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kcmpnm, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(knetwk, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kdatrd, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

  two_words = convert_to_words<sizeof(two_words)>(kinst, 2);
  SAC::write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));
  //---------------------------------------------------------------------------
  // End header
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  // Data
  //---------------------------------------------------------------------------
  for (float x : data1)
  {
    SAC::write_words(&file, convert_to_word(x));
  }
  // (Unevenly sampled-data) or (spectral or xy)
  if ((leven == 0) || (iftype > 1))
  {
    for (float x : data2)
    {
      SAC::write_words(&file, convert_to_word(x));
    }
  }
  //---------------------------------------------------------------------------
  // End data
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  // Footer
  //---------------------------------------------------------------------------
  // Because upon reading we convert to NVHDR = 7 this should be automatically ran
  // But incase you specifically wanted to make some NVHDR = 6 I wouldn't want
  // the write-out to be borked (say, generating synthetics using the old standard)
  if (nvhdr == 7)
  {
    SAC::write_words(&file, convert_to_word(delta));
    SAC::write_words(&file, convert_to_word(b));
    SAC::write_words(&file, convert_to_word(e));
    SAC::write_words(&file, convert_to_word(o));
    SAC::write_words(&file, convert_to_word(a));
    SAC::write_words(&file, convert_to_word(t0));
    SAC::write_words(&file, convert_to_word(t1));
    SAC::write_words(&file, convert_to_word(t2));
    SAC::write_words(&file, convert_to_word(t3));
    SAC::write_words(&file, convert_to_word(t4));
    SAC::write_words(&file, convert_to_word(t5));
    SAC::write_words(&file, convert_to_word(t6));
    SAC::write_words(&file, convert_to_word(t7));
    SAC::write_words(&file, convert_to_word(t8));
    SAC::write_words(&file, convert_to_word(t9));
    SAC::write_words(&file, convert_to_word(f));
    SAC::write_words(&file, convert_to_word(evlo));
    SAC::write_words(&file, convert_to_word(evla));
    SAC::write_words(&file, convert_to_word(stlo));
    SAC::write_words(&file, convert_to_word(stla));
    SAC::write_words(&file, convert_to_word(sb));
    SAC::write_words(&file, convert_to_word(sdelta));
  }
  */
  //---------------------------------------------------------------------------
  // End footer
  //---------------------------------------------------------------------------
  file.close();
}
//-----------------------------------------------------------------------------
// End writing
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Misc
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// End misc
//-----------------------------------------------------------------------------

}
