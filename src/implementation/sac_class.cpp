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
  delta  = SAC::read_next_float(&file);
  depmin = SAC::read_next_float(&file);
  depmax = SAC::read_next_float(&file);
  
  // Skip 'unused'
  SAC::skip_word(&file);
  
  odelta = SAC::read_next_float(&file); 
  b = SAC::read_next_float(&file);
  e = SAC::read_next_float(&file);
  o = SAC::read_next_float(&file);
  a = SAC::read_next_float(&file); 

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
  f = SAC::read_next_float(&file); 

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
  stla = SAC::read_next_float(&file);
  stlo = SAC::read_next_float(&file);
  stel = SAC::read_next_float(&file);
  stdp = SAC::read_next_float(&file);

  // Event headers
  evla = SAC::read_next_float(&file);
  evlo = SAC::read_next_float(&file);
  evel = SAC::read_next_float(&file);
  evdp = SAC::read_next_float(&file);
  mag = SAC::read_next_float(&file);
 
  // User headers
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
  dist = SAC::read_next_float(&file);
  az = SAC::read_next_float(&file);
  baz = SAC::read_next_float(&file);
  gcarc = SAC::read_next_float(&file);

  // Skip 'internal' 
  SAC::skip_word(&file);
  // Skip 'internal'
  SAC::skip_word(&file);

  depmen = SAC::read_next_float(&file); 
  cmpaz = SAC::read_next_float(&file);
  cmpinc = SAC::read_next_float(&file);
  xminimum = SAC::read_next_float(&file); 
  xmaximum = SAC::read_next_float(&file); 
  yminimum = SAC::read_next_float(&file); 
  ymaximum = SAC::read_next_float(&file); 

  // Skip 'unused' (x7)
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);

  // Date_time headers
  nzyear = SAC::read_next_int(&file);
  nzjday = SAC::read_next_int(&file);
  nzhour = SAC::read_next_int(&file);
  nzmin = SAC::read_next_int(&file);
  nzsec = SAC::read_next_int(&file);
  nzmsec = SAC::read_next_int(&file);
  nvhdr = SAC::read_next_int(&file);
  norid = SAC::read_next_int(&file); 
  nevid = SAC::read_next_int(&file); 
  npts = SAC::read_next_int(&file);
 
  // Skip 'internal'
  SAC::skip_word(&file);

  nwfid = SAC::read_next_int(&file); 
  nxsize = SAC::read_next_int(&file); 
  nysize = SAC::read_next_int(&file); 
 
  // Skip 'unused'
  SAC::skip_word(&file);

  iftype = SAC::read_next_int(&file);
  idep = SAC::read_next_int(&file); 
  iztype = SAC::read_next_int(&file); 
  
  // Skip 'unused'
  SAC::skip_word(&file);

  iinst = SAC::read_next_int(&file); 
  istreg = SAC::read_next_int(&file); 
  ievreg = SAC::read_next_int(&file);
  ievtyp = SAC::read_next_int(&file);
  iqual = SAC::read_next_int(&file);
  isynth = SAC::read_next_int(&file);
  imagtyp = SAC::read_next_int(&file);
  imagsrc = SAC::read_next_int(&file); 
  ibody = SAC::read_next_int(&file); 

  // Skip 'unused' (x7)
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);
  SAC::skip_word(&file);

  // Logical headers
  leven = SAC::read_next_bool(&file);
  lpspol = SAC::read_next_bool(&file); 
  lovrok = SAC::read_next_bool(&file);
  lcalda = SAC::read_next_bool(&file);

  // Skip 'unused'
  SAC::skip_word(&file);
  
  // KSTNM is 2 words long (like all other 'K' headers)
  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kstnm = std::string(two_words.data(), two_words.size());

  // KEVNM is 4 words long (special rule!)
  four_words = SAC::read_words<sizeof(four_words)>(&file, 4);
  kevnm = std::string(four_words.data(), four_words.size());

  // All other 'K' headers are 2 words long
  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  khole = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2); 
  ko = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  ka = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kt0 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kt1 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kt2 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kt3 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kt4 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kt5 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kt6 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kt7 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kt8 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kt9 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kf = std::string(two_words.data(), two_words.size());
  
  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kuser0 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kuser1 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kuser2 = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kcmpnm = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  knetwk = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kdatrd = std::string(two_words.data(), two_words.size());

  two_words = SAC::read_words<sizeof(two_words)>(&file, 2);
  kinst = std::string(two_words.data(), two_words.size());

  //---------------------------------------------------------------------------
  // End header
  //---------------------------------------------------------------------------
  data = SAC::read_data(&file, npts); 
}
}
