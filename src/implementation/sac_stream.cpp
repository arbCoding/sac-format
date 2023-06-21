#include "sac_stream.hpp"

namespace SAC
{
//-----------------------------------------------------------------------------
// Convenience methods
//-----------------------------------------------------------------------------
void SacStream::header_to_footer()
{
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
}

void SacStream::footer_to_header()
{
    f_delta = static_cast<float>(delta);
    f_b = static_cast<float>(b);
    f_e = static_cast<float>(e);
    f_o = static_cast<float>(o);
    f_a = static_cast<float>(a);
    f_t0 = static_cast<float>(t0);
    f_t1 = static_cast<float>(t1);
    f_t2 = static_cast<float>(t2);
    f_t3 = static_cast<float>(t3);
    f_t4 = static_cast<float>(t4);
    f_t5 = static_cast<float>(t5);
    f_t6 = static_cast<float>(t6);
    f_t7 = static_cast<float>(t7);
    f_t8 = static_cast<float>(t8);
    f_t9 = static_cast<float>(t9);
    f_f = static_cast<float>(f);
    f_evlo = static_cast<float>(evlo);
    f_evla = static_cast<float>(evla);
    f_stlo = static_cast<float>(stlo);
    f_stla = static_cast<float>(stla);
    f_sb = static_cast<float>(sb);
    f_sdelta = static_cast<float>(sdelta);
}
//-----------------------------------------------------------------------------
// End convenience functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------
// Parameterized constructor (reader)
SacStream::SacStream(const std::string& file_name)
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
    for (std::size_t i{0}; i < 7; ++i) { read_word(&file); }
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
    for (std::size_t i{0}; i < 7; ++i) { read_word(&file); }
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
    if (npts != SAC::unset_int)
    {
        // These are read in and converted to doubles (originally single-precision)
        data1 = read_data(&file, static_cast<size_t>(npts), data_word);
        // (Unevenly sampled-data) or (spectral or xy)
        // Same size as data1
        if ((leven == 0) || (iftype > 1))
        { data2 = read_data(&file, static_cast<size_t>(npts), data_word + npts); }
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
        // Make sure things are consistent
        footer_to_header();
    }
    // Convert to NVHDR = 7
    else { header_to_footer(); nvhdr = 7; }
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
void SacStream::write(const std::string& file_name)
{
    std::ofstream file(file_name, std::ios::binary | std::ios::out | std::ios::trunc);
    if (!file) { std::cerr << "Unable to write file...\n"; }
    // Make sure things are consistent just in case footer values changed,
    // but header values did not
    footer_to_header();
    //---------------------------------------------------------------------------
    // Header
    //---------------------------------------------------------------------------
    write_words(&file, convert_to_word(f_delta));
    write_words(&file, convert_to_word(depmin));
    write_words(&file, convert_to_word(depmax));
    // Fill 'unused'
    write_words(&file, convert_to_word(depmax));

    write_words(&file, convert_to_word(odelta));
    write_words(&file, convert_to_word(f_b));
    write_words(&file, convert_to_word(f_e));
    write_words(&file, convert_to_word(f_o));
    write_words(&file, convert_to_word(f_a));
    // Fill 'internal'
    write_words(&file, convert_to_word(f_a)); // Could give this a name and a value...

    write_words(&file, convert_to_word(f_t0));
    write_words(&file, convert_to_word(f_t1));
    write_words(&file, convert_to_word(f_t2));
    write_words(&file, convert_to_word(f_t3));
    write_words(&file, convert_to_word(f_t4));
    write_words(&file, convert_to_word(f_t5));
    write_words(&file, convert_to_word(f_t6));
    write_words(&file, convert_to_word(f_t7));
    write_words(&file, convert_to_word(f_t8));
    write_words(&file, convert_to_word(f_t9));
    write_words(&file, convert_to_word(f_f));
    write_words(&file, convert_to_word(resp0));
    write_words(&file, convert_to_word(resp1));
    write_words(&file, convert_to_word(resp2));
    write_words(&file, convert_to_word(resp3));
    write_words(&file, convert_to_word(resp4));
    write_words(&file, convert_to_word(resp5));
    write_words(&file, convert_to_word(resp6));
    write_words(&file, convert_to_word(resp7));
    write_words(&file, convert_to_word(resp8));
    write_words(&file, convert_to_word(resp9));
    write_words(&file, convert_to_word(f_stla));
    write_words(&file, convert_to_word(f_stlo));
    write_words(&file, convert_to_word(stel));
    write_words(&file, convert_to_word(stdp));
    write_words(&file, convert_to_word(f_evla));
    write_words(&file, convert_to_word(f_evlo));
    write_words(&file, convert_to_word(evel));
    write_words(&file, convert_to_word(evdp));
    write_words(&file, convert_to_word(mag));
    write_words(&file, convert_to_word(user0));
    write_words(&file, convert_to_word(user1));
    write_words(&file, convert_to_word(user2));
    write_words(&file, convert_to_word(user3));
    write_words(&file, convert_to_word(user4));
    write_words(&file, convert_to_word(user5));
    write_words(&file, convert_to_word(user6));
    write_words(&file, convert_to_word(user7));
    write_words(&file, convert_to_word(user8));
    write_words(&file, convert_to_word(user9));
    write_words(&file, convert_to_word(dist));
    write_words(&file, convert_to_word(az));
    write_words(&file, convert_to_word(baz));
    write_words(&file, convert_to_word(gcarc));
    write_words(&file, convert_to_word(f_sb));
    write_words(&file, convert_to_word(f_sdelta));
    write_words(&file, convert_to_word(depmen));
    write_words(&file, convert_to_word(cmpaz));
    write_words(&file, convert_to_word(cmpinc));
    write_words(&file, convert_to_word(xminimum));
    write_words(&file, convert_to_word(xmaximum));
    write_words(&file, convert_to_word(yminimum));
    write_words(&file, convert_to_word(ymaximum));
    // Fill 'unused' (x7)
    // Could give these names and values
    for (std::size_t i{0}; i < 7; ++i) { write_words(&file, convert_to_word(ymaximum)); }

    write_words(&file, convert_to_word(nzyear));
    write_words(&file, convert_to_word(nzjday));
    write_words(&file, convert_to_word(nzhour));
    write_words(&file, convert_to_word(nzmin));
    write_words(&file, convert_to_word(nzsec));
    write_words(&file, convert_to_word(nzmsec));
    write_words(&file, convert_to_word(nvhdr));
    write_words(&file, convert_to_word(norid));
    write_words(&file, convert_to_word(nevid));
    write_words(&file, convert_to_word(npts));
    write_words(&file, convert_to_word(nsnpts));
    write_words(&file, convert_to_word(nwfid));
    write_words(&file, convert_to_word(nxsize));
    write_words(&file, convert_to_word(nysize));
    // Fill 'unused'
    // could give a name and a value
    write_words(&file, convert_to_word(nysize));

    write_words(&file, convert_to_word(iftype));
    write_words(&file, convert_to_word(idep));
    write_words(&file, convert_to_word(iztype));
    // Fill 'unused'
    // could give a name and a vlue
    write_words(&file, convert_to_word(iztype));

    write_words(&file, convert_to_word(iinst));
    write_words(&file, convert_to_word(istreg));
    write_words(&file, convert_to_word(ievreg));
    write_words(&file, convert_to_word(ievtyp));
    write_words(&file, convert_to_word(iqual));
    write_words(&file, convert_to_word(isynth));
    write_words(&file, convert_to_word(imagtyp));
    write_words(&file, convert_to_word(imagsrc));
    write_words(&file, convert_to_word(ibody));
    // Fill 'unused' (x7)
    // could give these names and values
    for (std::size_t i{0}; i < 7; ++i) { write_words(&file, convert_to_word(ibody)); }

    write_words(&file, bool_to_word(leven));
    write_words(&file, bool_to_word(lpspol));
    write_words(&file, bool_to_word(lovrok));
    write_words(&file, bool_to_word(lcalda));
    // Fill 'unused'
    // could give this a name and a value
    write_words(&file, bool_to_word(lcalda));

    // Strings are handled differently
    std::array<char, 2 * word_length> two_words;
    two_words = convert_to_words<sizeof(two_words)>(kstnm, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    std::array<char, 4 * word_length> four_words;
    four_words = convert_to_words<sizeof(four_words)>(kevnm, 4);
    write_words(&file, std::vector<char>(four_words.begin(), four_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(khole, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(ko, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(ka, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt0, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt1, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt2, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt3, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt4, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt5, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt6, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt7, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt8, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt9, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kf, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kuser0, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kuser1, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kuser2, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kcmpnm, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(knetwk, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kdatrd, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kinst, 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));
    //---------------------------------------------------------------------------
    // End header
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    // Data
    //---------------------------------------------------------------------------
    // We go from double to float (loss in precision, but matches the data standard)
    // This way we do double-precision in all interal calculations, only lose precision
    // on outputting data to SAC file.
    for (double x : data1) { write_words(&file, convert_to_word(static_cast<float>(x))); }
    // (Unevenly sampled-data) or (spectral or xy)
    if ((leven == 0) || (iftype > 1))
    { for (double x : data2) { write_words(&file, convert_to_word(static_cast<float>(x))); } }
    //---------------------------------------------------------------------------
    // End data
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    // Footer
    //---------------------------------------------------------------------------
    // Because upon reading we convert to NVHDR = 7 this should be automatically ran
    // But incase you specifically wanted to make NVHDR = 6 for legacy writing I wouldn't want
    // the write-out to be borked (say, generating synthetics using the old standard)
    if (nvhdr == 7)
    {
        write_words(&file, convert_to_word(delta));
        write_words(&file, convert_to_word(b));
        write_words(&file, convert_to_word(e));
        write_words(&file, convert_to_word(o));
        write_words(&file, convert_to_word(a));
        write_words(&file, convert_to_word(t0));
        write_words(&file, convert_to_word(t1));
        write_words(&file, convert_to_word(t2));
        write_words(&file, convert_to_word(t3));
        write_words(&file, convert_to_word(t4));
        write_words(&file, convert_to_word(t5));
        write_words(&file, convert_to_word(t6));
        write_words(&file, convert_to_word(t7));
        write_words(&file, convert_to_word(t8));
        write_words(&file, convert_to_word(t9));
        write_words(&file, convert_to_word(f));
        write_words(&file, convert_to_word(evlo));
        write_words(&file, convert_to_word(evla));
        write_words(&file, convert_to_word(stlo));
        write_words(&file, convert_to_word(stla));
        write_words(&file, convert_to_word(sb));
        write_words(&file, convert_to_word(sdelta));
    }
    //---------------------------------------------------------------------------
    // End footer
    //---------------------------------------------------------------------------
    file.close();
}

// Since nvhdr is checked before writing to determine if a footer is appropriate
void SacStream::legacy_write(const std::string& file_name) { nvhdr = 6; write(file_name); }
//-----------------------------------------------------------------------------
// End writing
//-----------------------------------------------------------------------------
bool SacStream::operator==(const SacStream& other) const
{
    // Run through all the possible checks
    // As soon as we fail once, we're done with a false
    // Check the headers
    if (f_delta != other.f_delta) { return false; }
    if (depmin != other.depmin) { return false; }
    if (depmax != other.depmax) { return false; }
    if (odelta != other.odelta) { return false; }
    if (f_b != other.f_b) { return false; }
    if (f_e != other.f_e) { return false; }
    if (f_o != other.f_o) { return false; }
    if (f_a != other.f_a) { return false; }
    if (f_t0 != other.f_t0) { return false; }
    if (f_t1 != other.f_t1) { return false; }
    if (f_t2 != other.f_t2) { return false; }
    if (f_t3 != other.f_t3) { return false; }
    if (f_t4 != other.f_t4) { return false; }
    if (f_t5 != other.f_t5) { return false; }
    if (f_t6 != other.f_t6) { return false; }
    if (f_t7 != other.f_t7) { return false; }
    if (f_t8 != other.f_t8) { return false; }
    if (f_t9 != other.f_t9) { return false; }
    if (f_f != other.f_f) { return false; }
    if (resp0 != other.resp0) { return false; }
    if (resp1 != other.resp1) { return false; }
    if (resp2 != other.resp2) { return false; }
    if (resp3 != other.resp3) { return false; }
    if (resp4 != other.resp4) { return false; }
    if (resp5 != other.resp5) { return false; }
    if (resp6 != other.resp6) { return false; }
    if (resp7 != other.resp7) { return false; }
    if (resp8 != other.resp8) { return false; }
    if (resp9 != other.resp9) { return false; }
    if (f_stla != other.f_stla) { return false; }
    if (f_stlo != other.f_stlo) { return false; }
    // Good down to here
    if (stel != other.stel) { return false; }
    if (stdp != other.stdp) { return false; }
    if (f_evla != other.f_evla) { return false; }
    if (f_evlo != other.f_evlo) { return false; }
    if (evel != other.evel) { return false; }
    if (evdp != other.evdp) { return false; }
    if (mag != other.mag) { return false; }
    if (user0 != other.user0) { return false; }
    if (user1 != other.user1) { return false; }
    if (user2 != other.user2) { return false; }
    if (user3 != other.user3) { return false; }
    if (user4 != other.user4) { return false; }
    if (user5 != other.user5) { return false; }
    if (user6 != other.user6) { return false; }
    if (user7 != other.user7) { return false; }
    if (user8 != other.user8) { return false; }
    if (user9 != other.user9) { return false; }
    if (dist != other.dist) { return false; }
    if (az != other.az) { return false; }
    if (baz != other.baz) { return false; }
    if (gcarc != other.gcarc) { return false; }
    if (f_sb != other.f_sb) { return false; }
    if (f_sdelta != other.f_sdelta) { return false; }
    if (depmen != other.depmen) { return false; }
    if (cmpaz != other.cmpaz) { return false; }
    if (cmpinc != other.cmpinc) { return false; }
    if (xminimum != other.xminimum) { return false; }
    if (xmaximum != other.xmaximum) { return false; }
    if (yminimum != other.yminimum) { return false; }
    if (ymaximum != other.ymaximum) { return false; }
    if (nzyear != other.nzyear) { return false; }
    if (nzjday != other.nzjday) { return false; }
    if (nzhour != other.nzhour) { return false; }
    if (nzmin != other.nzmin) { return false; }
    if (nzsec != other.nzsec) { return false; }
    if (nzmsec != other.nzmsec) { return false; }
    if (nvhdr != other.nvhdr) { return false; }
    if (norid != other.norid) { return false; }
    if (nevid != other.nevid) { return false; }
    if (npts != other.npts) { return false; }
    if (nsnpts != other.nsnpts) { return false; }
    if (nwfid != other.nwfid) { return false; }
    if (nxsize != other.nxsize) { return false; }
    if (nysize != other.nysize) { return false; }
    if (iftype != other.iftype) { return false; }
    if (idep != other.idep) { return false; }
    if (iztype != other.iztype) { return false; }
    if (iinst != other.iinst) { return false; }
    if (istreg != other.istreg) { return false; }
    if (ievreg != other.ievreg) { return false; }
    if (ievtyp != other.ievtyp) { return false; }
    if (iqual != other.iqual) { return false; }
    if (isynth != other.isynth) { return false; }
    if (imagtyp != other.imagtyp) { return false; }
    if (imagsrc != other.imagsrc) { return false; }
    if (ibody != other.ibody) { return false; }
    if (leven != other.leven) { return false; }
    if (lpspol != other.lpspol) { return false; }
    if (lovrok != other.lovrok) { return false; }
    if (lcalda != other.lcalda) { return false; }
    if (kstnm != other.kstnm) { return false; }
    if (kevnm != other.kevnm) { return false; }
    if (khole != other.khole) { return false; }
    if (ko != other.ko) { return false; }
    if (ka != other.ka) { return false; }
    if (kt0 != other.kt0) { return false; }
    if (kt1 != other.kt1) { return false; }
    if (kt2 != other.kt2) { return false; }
    if (kt3 != other.kt3) { return false; }
    if (kt4 != other.kt4) { return false; }
    if (kt5 != other.kt5) { return false; }
    if (kt6 != other.kt6) { return false; }
    if (kt7 != other.kt7) { return false; }
    if (kt8 != other.kt8) { return false; }
    if (kt9 != other.kt9) { return false; }
    if (kf != other.kf) { return false; }
    if (kuser0 != other.kuser0) { return false; }
    if (kuser1 != other.kuser1) { return false; }
    if (kuser2 != other.kuser2) { return false; }
    if (kcmpnm != other.kcmpnm) { return false; }
    if (knetwk != other.knetwk) { return false; }
    if (kdatrd != other.kdatrd) { return false; }
    if (kinst != other.kinst) { return false; }
    // Check the footers (data section is slowest and therefore last)
    if (delta != other.delta) { return false; }
    if (b != other.b) { return false; }
    if (e != other.e) { return false; }
    if (o != other.o) { return false; }
    if (a != other.a) { return false; }
    if (t0 != other.t0) { return false; }
    if (t1 != other.t1) { return false; }
    if (t2 != other.t2) { return false; }
    if (t3 != other.t3) { return false; }
    if (t4 != other.t4) { return false; }
    if (t5 != other.t5) { return false; }
    if (t6 != other.t6) { return false; }
    if (t7 != other.t7) { return false; }
    if (t8 != other.t8) { return false; }
    if (t9 != other.t9) { return false; }
    if (f != other.f) { return false; }
    if (evlo != other.evlo) { return false; }
    if (evla != other.evla) { return false; }
    if (stlo != other.stlo) { return false; }
    if (stla != other.stla) { return false; }
    if (sb != other.sb) { return false; }
    if (sdelta != other.sdelta) { return false; }
    // Check the data vectors by size first as it is faster
    if (data1.size() != other.data1.size()) { return false; }
    if (data2.size() != other.data2.size()) { return false; }
    // Ignoring FlawFinder CWE-126 because we first check that
    // the sizes are equal
    if (!std::ranges::equal(data1, other.data1)) { return false; }
    if (!std::ranges::equal(data2, other.data2)) { return false; }
    // We failed to fail!
    return true;
}
}
