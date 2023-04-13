#include "sac_class.hpp"

namespace SAC
{
// Constructor from file
Sac_Class::Sac_Class(const std::string& file_name)
{
  std::ifstream file(file_name, std::ifstream::binary);
  if (!file)
  {
    std::cerr << "file could not be read...\n";
  }
  // Make sure we're at the start
  file.seekg(0);
  // Possible words
  std::array<char, 2 * SAC::word_length> two_words;
  std::array<char, 4 * SAC::word_length> four_words;
  //---------------------------------------------------------------------------
  // Headers
  //---------------------------------------------------------------------------
  delta = SAC::read_next_float(&file); // 0.025
  depmin = SAC::read_next_float(&file);  // -12345
  depmax = SAC::read_next_float(&file);  // -12345
  
  // Skip 'unused'
  SAC::skip_word(&file);
  
  odelta = SAC::read_next_float(&file);  // -12345
  b = SAC::read_next_float(&file);  // 0
  e = SAC::read_next_float(&file);  // 697
  o = SAC::read_next_float(&file);  // 13
  a = SAC::read_next_float(&file);  // -12345

  // Skip 'internal'
  SAC::skip_word(&file);

  // Arrival time picking headers
  // All -12345
  t0 = SAC::read_next_float(&file); 
  t1 = SAC::read_next_float(&file); 
  t2 = SAC::read_next_float(&file); 
  t3 = SAC::read_next_float(&file); 
  t4 = SAC::read_next_float(&file); 
  t5 = SAC::read_next_float(&file); 
  t6 = SAC::read_next_float(&file); 
  t7 = SAC::read_next_float(&file); 
  t8 = SAC::read_next_float(&file); 
  t9 = SAC::read_next_float(&file); 
  f = SAC::read_next_float(&file);  // -12345

  // Resp headers
  // All -12345
  resp0 = SAC::read_next_float(&file); 
  resp1 = SAC::read_next_float(&file); 
  resp2 = SAC::read_next_float(&file); 
  resp3 = SAC::read_next_float(&file); 
  resp4 = SAC::read_next_float(&file); 
  resp5 = SAC::read_next_float(&file); 
  resp6 = SAC::read_next_float(&file); 
  resp7 = SAC::read_next_float(&file); 
  resp8 = SAC::read_next_float(&file); 
  resp9 = SAC::read_next_float(&file); 

  // Station headers
  stla = SAC::read_next_float(&file);  // 38.4328
  stlo = SAC::read_next_float(&file);  // -118.155
  stel = SAC::read_next_float(&file);  // 1509
  stdp = SAC::read_next_float(&file);  // 0

  // Event headers
  evla = SAC::read_next_float(&file);  // 36.801
  evlo = SAC::read_next_float(&file);  // -121.323
  evel = SAC::read_next_float(&file);  // -12345
  evdp = SAC::read_next_float(&file);  // 10.16
  mag = SAC::read_next_float(&file);  // -12345
 
  // User headers
  // All -12345
  user0 = SAC::read_next_float(&file); 
  user1 = SAC::read_next_float(&file); 
  user2 = SAC::read_next_float(&file); 
  user3 = SAC::read_next_float(&file); 
  user4 = SAC::read_next_float(&file); 
  user5 = SAC::read_next_float(&file); 
  user6 = SAC::read_next_float(&file); 
  user7 = SAC::read_next_float(&file); 
  user8 = SAC::read_next_float(&file); 
  user9 = SAC::read_next_float(&file); 
  dist = SAC::read_next_float(&file);  // 333.176
  az = SAC::read_next_float(&file);  // 56.1169
  baz = SAC::read_next_float(&file);  // 238.043
  gcarc = SAC::read_next_float(&file);  // 2.99645

  // Skip 'internal' 
  SAC::skip_word(&file);
  // Skip 'internal'
  SAC::skip_word(&file);

  depmen = SAC::read_next_float(&file);  // -12345
  cmpaz = SAC::read_next_float(&file);  // 0
  cmpinc = SAC::read_next_float(&file);  // 0
  xminimum = SAC::read_next_float(&file);  // -12345
  xmaximum = SAC::read_next_float(&file);  // -12345
  yminimum = SAC::read_next_float(&file);  // -12345
  ymaximum = SAC::read_next_float(&file);  // -12345

  // Skip 'unused' (x7)
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);

  // Date_time headers
  nzyear = SAC::read_next_int(&file);  // 2023
  nzjday = SAC::read_next_int(&file);  // 94
  nzhour = SAC::read_next_int(&file);  // 22 
  nzmin = SAC::read_next_int(&file);  // 23 
  nzsec = SAC::read_next_int(&file);  // 4
  nzmsec = SAC::read_next_int(&file);  // 0
  nvhdr = SAC::read_next_int(&file);  // 6 (old, 7 = new) 
  norid = SAC::read_next_int(&file);  // -12345
  nevid = SAC::read_next_int(&file);  // -12345
  npts = SAC::read_next_int(&file);  // 27881
 
  // Skip 'internal'
  SAC::skip_word(&file);

  nwfid = SAC::read_next_int(&file);  // -12345
  nxsize = SAC::read_next_int(&file);  // -12345
  nysize = SAC::read_next_int(&file);  // -12345
 
  // Skip 'unused'
  SAC::skip_word(&file);

  iftype = SAC::read_next_int(&file);  // 1
  idep = SAC::read_next_int(&file);  // -12345
  iztype = SAC::read_next_int(&file);  // -12345
  
  // Skip 'unused'
  SAC::skip_word(&file);

  iinst = SAC::read_next_int(&file);  // -12345
  istreg = SAC::read_next_int(&file);  // -12345
  ievreg = SAC::read_next_int(&file);  // 
  ievtyp = SAC::read_next_int(&file);  // 
  iqual = SAC::read_next_int(&file);  // 
  isynth = SAC::read_next_int(&file);  // 
  imagtyp = SAC::read_next_int(&file);  // 
  imagsrc = SAC::read_next_int(&file);  // 
  ibody = SAC::read_next_int(&file);  // 

  // Skip 'unused' (x7)
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);

  // Logical headers
  leven = SAC::read_next_bool(&file);  // 1
  lpspol = SAC::read_next_bool(&file);  // 1 
  lovrok = SAC::read_next_bool(&file);  // 1
  lcalda = SAC::read_next_bool(&file);  // 1

  // Skip 'unused'
  SAC::skip_word(&file);
  
  // KSTNM is 2 words long (like all other 'K' headers)
  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // NV31
  kstnm = std::string(two_words.data(), two_words.size());

  // KEVNM is 4 words long (special rule!)
  four_words = SAC::read_words<sizeof(four_words)>(&file, 4); // Central Californ [truncation]
  kevnm = std::string(four_words.data(), four_words.size());

  // All other 'K' headers are 2 words long
  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  khole = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345 
  ko = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  ka = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  kt0 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  kt1 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  kt2 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  kt3 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  kt4 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  kt5 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  kt6 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  kt7 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  kt8 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  kt9 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  kf = std::string(two_words.data(), two_words.size());
  
  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  kuser0 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  kuser1 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  kuser2 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // BHZ
  kcmpnm = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // IM
  knetwk = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // -12345
  kdatrd = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); // CMG3T_NV
  kinst = std::string(two_words.data(), two_words.size());

  //---------------------------------------------------------------------------
  // End header
  //---------------------------------------------------------------------------
  data = SAC::read_data(&file, npts); 
}
}
