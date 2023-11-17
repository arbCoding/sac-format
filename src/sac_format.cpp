#include "sac_format.hpp"

// Implementation of the interface in sac_format.hpp
namespace sacfmt {
//-----------------------------------------------------------------------------
// Conversions
//-----------------------------------------------------------------------------
int word_position(const int word_number) { return (word_number * word_length); }

word_one int_to_binary(const int x) {
    word_one bits{};
    if (x >= 0) { bits = word_one(static_cast<size_t>(x)); }
    else { bits = word_one(static_cast<size_t>(std::pow(2, binary_word_size) + x)); }
    return bits;
}

int binary_to_int(word_one x) {
    int result{};
    if (x.test(binary_word_size - 1)) {
      x = ~x;
      result = static_cast<int>(x.to_ulong());
      result += 1;
      // Change sign to make it as negative
      result *= -1;
    } else { result = static_cast<int>(x.to_ulong()); }
    return result;
}

// Union makes this so easy
word_one float_to_binary(const float x) { return float_to_bits(x).bits; }

float binary_to_float(const word_one& x) { return float_to_bits(x).value; }

// Once again, union to the rescue!
word_two double_to_binary(const double x) { return double_to_bits(x).bits; }

double binary_to_double(const word_two& x) { return double_to_bits(x).value; }

// Remove leading/trailing white-space and control characters
std::string string_cleaning(const std::string& str) {
    std::string result{str};
    size_t null_position{str.find('\0')};
    if (null_position != std::string::npos) { result.erase(null_position); }
    // Remove leading and trailing white spaces
    boost::algorithm::trim(result);
    return result;
}

word_two string_to_binary(std::string x) {
    constexpr size_t string_size{2 * word_length};
    // 1 byte per character
    constexpr size_t char_size{bits_per_byte};
    // String must be of specific length
    // Remove any spurious padding
    x = string_cleaning(x);
    // Truncate if needed
    if (x.length() > string_size) { x.resize(string_size); }
    // Pad if needed
    else if (x.length() < string_size) { x = x.append(string_size - x.length(), ' '); }
    // Two words (8 characters)
    word_two bits{};
    // 1 character
    std::bitset<char_size> byte{};
    for (size_t i{0}; i < string_size; ++i) {
        size_t character{static_cast<size_t>(x[i])};
        byte = std::bitset<char_size>(character);
        for (size_t j{0}; j < char_size; ++j) { bits[(i * char_size) + j] = byte[j]; }
    }
    return bits;
}

std::string binary_to_string(const word_two& x) {
    std::string result{};
    constexpr size_t char_size{bits_per_byte};
    std::bitset<char_size> byte{};
    // Read character by character
    for (size_t i{0}; i < 2 * binary_word_size; i += char_size) {
        // Build the character
        for (size_t j{0}; j < char_size; ++j) { byte[j] = x[i + j]; }
        result += static_cast<char>(byte.to_ulong());
    }
    return string_cleaning(result);
}

word_four long_string_to_binary(std::string x) {
    constexpr size_t string_size{4 * word_length};
    constexpr size_t char_size{bits_per_byte};
    // Remove any spurious padding
    //boost::algorithm::trim(x);
    x = string_cleaning(x);
    //x = string_cleaning(x);
    // Truncate if needed
    if (x.length() > string_size) { x.resize(string_size); }
    // Pad if needed
    else if (x.length() < string_size) { x = x.append(string_size - x.length(), ' '); }
    // Four words (16 characters)
    word_four bits{};
    // One character
    std::bitset<char_size> byte{};
    for (size_t i{0}; i < string_size; ++i) {
        size_t character{static_cast<size_t>(x[i])};
        byte = std::bitset<char_size>(character);
        for (size_t j{0}; j < char_size; ++j) { bits[(i * char_size) + j] = byte[j]; }
    }
    return bits;
}

std::string binary_to_long_string(const word_four& x) {
    std::string result{};
    constexpr size_t char_size{bits_per_byte};
    std::bitset<char_size> byte{};
    // Read character by character
    for (size_t i{0}; i < 4 * binary_word_size; i += char_size) {
        // It builds character!
        for (size_t j{0}; j < char_size; ++j) { byte[j] = x[i + j]; }
        result += static_cast<char>(byte.to_ulong());
    }
    return string_cleaning(result);
}

word_one bool_to_binary(const bool x) {
    word_one result{};
    result[0] = x;
    return result;
}

bool binary_to_bool(const word_one& x) { return x[0]; }

word_two concat_words(const word_one& x, const word_one& y) {
    word_two result{};
    for (size_t i{0}; i < binary_word_size; ++i) {
        result[i] = x[i]; result[i + binary_word_size] = y[i];
    }
    return result;
}

word_four concat_words(const word_two& x, const word_two& y) {
    word_four result{};
    for (size_t i{0}; i < 2 * binary_word_size; ++i) {
        result[i] = x[i]; result[i + (2 * binary_word_size)] = y[i];
    }
    return result;
}
//-----------------------------------------------------------------------------
// Reading
//-----------------------------------------------------------------------------
word_one read_word(std::ifstream* sac) {
    word_one bits{};
    constexpr size_t char_size{bits_per_byte};
    // Where we will store the characters
    // flawfinder: ignore
    char word[word_length];
    // Read to our character array
    // flawfinder: ignore
    sac->read(word, word_length);
    // Take each character
    std::bitset<char_size> byte{};
    for (size_t i{0}; i < word_length; ++i) {
        size_t character{static_cast<size_t>(word[i])};
        byte = std::bitset<char_size>(character);
        // bit-by-bit
        for (size_t j{0}; j < char_size; ++j) {
            bits[(i * char_size) + j] = byte[j];
        }
    }
    return bits;
}

word_two read_two_words(std::ifstream* sac) {
    word_one word1{read_word(sac)};
    word_one word2{read_word(sac)};
    if constexpr (std::endian::native == std::endian::little) {
        return concat_words(word1, word2);
    } else { return concat_words(word2, word1); }
}

word_four read_four_words(std::ifstream* sac) {
    word_two word12{read_two_words(sac)};
    word_two word34{read_two_words(sac)};
    if constexpr (std::endian::native == std::endian::little) {
        return concat_words(word12, word34);
    } else { return concat_words(word34, word12); }
}

std::vector<double> read_data(std::ifstream* sac, const size_t n_words,
                              const int start) {
    sac->seekg(word_position(start));
    std::vector<double> result{};
    result.resize(n_words);
    for (size_t i{0}; i < n_words; ++i) {
        result[i] = static_cast<double>(binary_to_float(read_word(sac)));
    }
    return result;
}
//-----------------------------------------------------------------------------
// Writing
//-----------------------------------------------------------------------------
void write_words(std::ofstream* sac_file, const std::vector<char>& input) {
    std::ofstream& sac = *sac_file;
    if (sac.is_open()) { for (char c : input) { sac.write(&c, sizeof(char)); } }
}

// Template on the typename to make possible to handle float or int
template <typename T>
std::vector<char> convert_to_word(const T x) {
    // flawfinder: ignore
    char tmp[word_length];
    // Copy bytes from x into the tmp array
    // flawfinder: ignore
    std::memcpy(tmp, &x, word_length);
    std::vector<char> word{};
    word.resize(word_length);
    for (int i{0}; i < word_length; ++i) { word[static_cast<size_t>(i)] = tmp[i]; }
    return word;
}

// Explicit instantiation
template std::vector<char> convert_to_word(const float x);
template std::vector<char> convert_to_word(const int x);

std::vector<char> convert_to_word(const double x) {
    // flawfinder: ignore
    char tmp[2 * word_length];
    // Copy bytes from x into the tmp array
    // flawfinder: ignore
    std::memcpy(tmp, &x, 2 * word_length);
    std::vector<char> word{};
    word.resize(2 * word_length);
    for (int i{0}; i < 2 * word_length; ++i) { word[static_cast<size_t>(i)] = tmp[i]; }
    return word;
}

// Variable sized words for the 'K' headers
template <size_t N>
std::array<char, N> convert_to_words(const std::string& s, int n_words) {
    std::array<char, N> all_words;
    // String to null-terminated character array
    const char* c_str = s.c_str();
    for (int i{0}; i < (n_words * word_length); ++i) {
        all_words[static_cast<size_t>(i)] = c_str[i];
    }
    return all_words;
}

// Explicit instantiation
template std::array<char, word_length> convert_to_words(const std::string& s,
                                                        int n_words = 1);
template std::array<char, 2 * word_length> convert_to_words(const std::string& s,
                                                            int n_words = 2);
template std::array<char, 4 * word_length> convert_to_words(const std::string& s,
                                                            int n_words = 4);

std::vector<char> bool_to_word(const bool b) {
    std::vector<char> result;
    result.resize(word_length);
    result[0] = b;
    for (int i{1}; i < word_length; ++i) { result[i] = 0; }
    return result;
}
//-----------------------------------------------------------------------------
// Convenience methods
// -----------------------------------------------------------------------------
// Does not assume equal length, if not equal length then they're not equal
// within tolerance
bool equal_within_tolerance(const std::vector<double>& vector1,
                            const std::vector<double>& vector2,
                            const double tolerance) {
    if (vector1.size() != vector2.size()) { return false; }
    for (size_t i{0}; i < vector1.size(); ++i) {
        if (std::abs(vector1[i] - vector2[i]) > tolerance) { return false; }
    }
    return true;
}

//------------------------------------------------------------------------------
// Trace
//------------------------------------------------------------------------------
Trace::Trace() {
    std::ranges::fill(floats.begin(), floats.end(), unset_float);
    std::ranges::fill(doubles.begin(), doubles.end(), unset_double);
    std::ranges::fill(ints.begin(), ints.end(), unset_int);
    std::ranges::fill(bools.begin(), bools.end(), unset_bool);
    std::ranges::fill(strings.begin(), strings.end(), unset_word);
}

bool Trace::operator==(const Trace& other) const {
    if (floats != other.floats) { return false; }
    if (doubles != other.doubles) { return false; }
    if (ints != other.ints) { return false; }
    if (strings != other.strings) { return false; }
    if (!equal_within_tolerance(data[0], other.data[0])) { return false; }
    if (!equal_within_tolerance(data[1], other.data[1])) { return false; }
    return true;
}
// Getters
// Floats
float Trace::depmin() const { return floats[sac_map.at("depmin")]; }
float Trace::depmax() const { return floats[sac_map.at("depmax")]; }
float Trace::odelta() const { return floats[sac_map.at("odelta")]; }
float Trace::resp0() const { return floats[sac_map.at("resp0")]; }
float Trace::resp1() const { return floats[sac_map.at("resp1")]; }
float Trace::resp2() const { return floats[sac_map.at("resp2")]; }
float Trace::resp3() const { return floats[sac_map.at("resp3")]; }
float Trace::resp4() const { return floats[sac_map.at("resp4")]; }
float Trace::resp5() const { return floats[sac_map.at("resp5")]; }
float Trace::resp6() const { return floats[sac_map.at("resp6")]; }
float Trace::resp7() const { return floats[sac_map.at("resp7")]; }
float Trace::resp8() const { return floats[sac_map.at("resp8")]; }
float Trace::resp9() const { return floats[sac_map.at("resp9")]; }
float Trace::stel() const { return floats[sac_map.at("stel")]; }
float Trace::stdp() const { return floats[sac_map.at("stdp")]; }
float Trace::evel() const { return floats[sac_map.at("evel")]; }
float Trace::evdp() const { return floats[sac_map.at("evdp")]; }
float Trace::mag() const { return floats[sac_map.at("mag")]; }
float Trace::user0() const { return floats[sac_map.at("user0")]; }
float Trace::user1() const { return floats[sac_map.at("user1")]; }
float Trace::user2() const { return floats[sac_map.at("user2")]; }
float Trace::user3() const { return floats[sac_map.at("user3")]; }
float Trace::user4() const { return floats[sac_map.at("user4")]; }
float Trace::user5() const { return floats[sac_map.at("user5")]; }
float Trace::user6() const { return floats[sac_map.at("user6")]; }
float Trace::user7() const { return floats[sac_map.at("user7")]; }
float Trace::user8() const { return floats[sac_map.at("user8")]; }
float Trace::user9() const { return floats[sac_map.at("user9")]; }
float Trace::dist() const { return floats[sac_map.at("dist")]; }
float Trace::az() const { return floats[sac_map.at("az")]; }
float Trace::baz() const { return floats[sac_map.at("baz")]; }
float Trace::gcarc() const { return floats[sac_map.at("gcarc")]; }
float Trace::depmen() const { return floats[sac_map.at("depmen")]; }
float Trace::cmpaz() const { return floats[sac_map.at("cmpaz")]; }
float Trace::cmpinc() const { return floats[sac_map.at("cmpinc")]; }
float Trace::xminimum() const { return floats[sac_map.at("xminimum")]; }
float Trace::xmaximum() const { return floats[sac_map.at("xmaximum")]; }
float Trace::yminimum() const { return floats[sac_map.at("yminimum")]; }
float Trace::ymaximum() const { return floats[sac_map.at("ymaximum")]; }
// Doubles
double Trace::delta() const { return doubles[sac_map.at("delta")]; }
double Trace::b() const { return doubles[sac_map.at("b")]; }
double Trace::e() const { return doubles[sac_map.at("e")]; }
double Trace::o() const { return doubles[sac_map.at("o")]; }
double Trace::a() const { return doubles[sac_map.at("a")]; }
double Trace::t0() const { return doubles[sac_map.at("t0")]; }
double Trace::t1() const { return doubles[sac_map.at("t1")]; }
double Trace::t2() const { return doubles[sac_map.at("t2")]; }
double Trace::t3() const { return doubles[sac_map.at("t3")]; }
double Trace::t4() const { return doubles[sac_map.at("t4")]; }
double Trace::t5() const { return doubles[sac_map.at("t5")]; }
double Trace::t6() const { return doubles[sac_map.at("t6")]; }
double Trace::t7() const { return doubles[sac_map.at("t7")]; }
double Trace::t8() const { return doubles[sac_map.at("t8")]; }
double Trace::t9() const { return doubles[sac_map.at("t9")]; }
double Trace::f() const { return doubles[sac_map.at("f")]; }
double Trace::stla() const { return doubles[sac_map.at("stla")]; }
double Trace::stlo() const { return doubles[sac_map.at("stlo")]; }
double Trace::evla() const { return doubles[sac_map.at("evla")]; }
double Trace::evlo() const { return doubles[sac_map.at("evlo")]; }
double Trace::sb() const { return doubles[sac_map.at("sb")]; }
double Trace::sdelta() const { return doubles[sac_map.at("sdelta")]; }
// Ints
int Trace::nzyear() const { return ints[sac_map.at("nzyear")]; }
int Trace::nzjday() const { return ints[sac_map.at("nzjday")]; }
int Trace::nzhour() const { return ints[sac_map.at("nzhour")]; }
int Trace::nzmin() const { return ints[sac_map.at("nzmin")]; }
int Trace::nzsec() const { return ints[sac_map.at("nzsec")]; }
int Trace::nzmsec() const { return ints[sac_map.at("nzmsec")]; }
int Trace::nvhdr() const { return ints[sac_map.at("nvhdr")]; }
int Trace::norid() const { return ints[sac_map.at("norid")]; }
int Trace::nevid() const { return ints[sac_map.at("nevid")]; }
int Trace::npts() const { return ints[sac_map.at("npts")]; }
int Trace::nsnpts() const { return ints[sac_map.at("nsnpts")]; }
int Trace::nwfid() const { return ints[sac_map.at("nwfid")]; }
int Trace::nxsize() const { return ints[sac_map.at("nxsize")]; }
int Trace::nysize() const { return ints[sac_map.at("nysize")]; }
int Trace::iftype() const { return ints[sac_map.at("iftype")]; }
int Trace::idep() const { return ints[sac_map.at("idep")]; }
int Trace::iztype() const { return ints[sac_map.at("iztype")]; }
int Trace::iinst() const { return ints[sac_map.at("iinst")]; }
int Trace::istreg() const { return ints[sac_map.at("istreg")]; }
int Trace::ievreg() const { return ints[sac_map.at("ievreg")]; }
int Trace::ievtyp() const { return ints[sac_map.at("ievtyp")]; }
int Trace::iqual() const { return ints[sac_map.at("iqual")]; }
int Trace::isynth() const { return ints[sac_map.at("isynth")]; }
int Trace::imagtyp() const { return ints[sac_map.at("imagtyp")]; }
int Trace::imagsrc() const { return ints[sac_map.at("imagsrc")]; }
int Trace::ibody() const { return ints[sac_map.at("ibody")]; }
// Bools
bool Trace::leven() const { return bools[sac_map.at("leven")]; }
bool Trace::lpspol() const { return bools[sac_map.at("lpspol")]; }
bool Trace::lovrok() const { return bools[sac_map.at("lovrok")]; }
bool Trace::lcalda() const { return bools[sac_map.at("lcalda")]; }
// Strings
std::string Trace::kstnm() const { return strings[sac_map.at("kstnm")]; }
std::string Trace::kevnm() const { return strings[sac_map.at("kevnm")]; }
std::string Trace::khole() const { return strings[sac_map.at("khole")]; }
std::string Trace::ko() const { return strings[sac_map.at("ko")]; }
std::string Trace::ka() const { return strings[sac_map.at("ka")]; }
std::string Trace::kt0() const { return strings[sac_map.at("kt0")]; }
std::string Trace::kt1() const { return strings[sac_map.at("kt1")]; }
std::string Trace::kt2() const { return strings[sac_map.at("kt2")]; }
std::string Trace::kt3() const { return strings[sac_map.at("kt3")]; }
std::string Trace::kt4() const { return strings[sac_map.at("kt4")]; }
std::string Trace::kt5() const { return strings[sac_map.at("kt5")]; }
std::string Trace::kt6() const { return strings[sac_map.at("kt6")]; }
std::string Trace::kt7() const { return strings[sac_map.at("kt7")]; }
std::string Trace::kt8() const { return strings[sac_map.at("kt8")]; }
std::string Trace::kt9() const { return strings[sac_map.at("kt9")]; }
std::string Trace::kf() const { return strings[sac_map.at("kf")]; }
std::string Trace::kuser0() const { return strings[sac_map.at("kuser0")]; }
std::string Trace::kuser1() const { return strings[sac_map.at("kuser1")]; }
std::string Trace::kuser2() const { return strings[sac_map.at("kuser2")]; }
std::string Trace::kcmpnm() const { return strings[sac_map.at("kcmpnm")]; }
std::string Trace::knetwk() const { return strings[sac_map.at("knetwk")]; }
std::string Trace::kdatrd() const { return strings[sac_map.at("kdatrd")]; }
std::string Trace::kinst() const { return strings[sac_map.at("kinst")]; }
// Data
std::vector<double> Trace::data1() const { return data[sac_map.at("data1")]; }
std::vector<double> Trace::data2() const { return data[sac_map.at("data2")]; }
// Setters
// Floats
void Trace::depmin(const float x) { floats[sac_map.at("depmin")] = x; }
void Trace::depmax(const float x) { floats[sac_map.at("depmax")] = x; }
void Trace::odelta(const float x) { floats[sac_map.at("odelta")] = x; }
void Trace::resp0(const float x) { floats[sac_map.at("resp0")] = x; }
void Trace::resp1(const float x) { floats[sac_map.at("resp1")] = x; }
void Trace::resp2(const float x) { floats[sac_map.at("resp2")] = x; }
void Trace::resp3(const float x) { floats[sac_map.at("resp3")] = x; }
void Trace::resp4(const float x) { floats[sac_map.at("resp4")] = x; }
void Trace::resp5(const float x) { floats[sac_map.at("resp5")] = x; }
void Trace::resp6(const float x) { floats[sac_map.at("resp6")] = x; }
void Trace::resp7(const float x) { floats[sac_map.at("resp7")] = x; }
void Trace::resp8(const float x) { floats[sac_map.at("resp8")] = x; }
void Trace::resp9(const float x) { floats[sac_map.at("resp9")] = x; }
void Trace::stel(const float x) { floats[sac_map.at("stel")] = x; }
void Trace::stdp(const float x) { floats[sac_map.at("stdp")] = x; }
void Trace::evel(const float x) { floats[sac_map.at("evel")] = x; }
void Trace::evdp(const float x) { floats[sac_map.at("evdp")] = x; }
void Trace::mag(const float x) { floats[sac_map.at("mag")] = x; }
void Trace::user0(const float x) { floats[sac_map.at("user0")] = x; }
void Trace::user1(const float x) { floats[sac_map.at("user1")] = x; }
void Trace::user2(const float x) { floats[sac_map.at("user2")] = x; }
void Trace::user3(const float x) { floats[sac_map.at("user3")] = x; }
void Trace::user4(const float x) { floats[sac_map.at("user4")] = x; }
void Trace::user5(const float x) { floats[sac_map.at("user5")] = x; }
void Trace::user6(const float x) { floats[sac_map.at("user6")] = x; }
void Trace::user7(const float x) { floats[sac_map.at("user7")] = x; }
void Trace::user8(const float x) { floats[sac_map.at("user8")] = x; }
void Trace::user9(const float x) { floats[sac_map.at("user9")] = x; }
void Trace::dist(const float x) { floats[sac_map.at("dist")] = x; }
void Trace::az(const float x) { floats[sac_map.at("az")] = x; }
void Trace::baz(const float x) { floats[sac_map.at("baz")] = x; }
void Trace::gcarc(const float x) { floats[sac_map.at("gcarc")] = x; }
void Trace::depmen(const float x) { floats[sac_map.at("depmen")] = x; }
void Trace::cmpaz(const float x) { floats[sac_map.at("cmpaz")] = x; }
void Trace::cmpinc(const float x) { floats[sac_map.at("cmpinc")] = x; }
void Trace::xminimum(const float x) { floats[sac_map.at("xminimum")] = x; }
void Trace::xmaximum(const float x) { floats[sac_map.at("xmaximum")] = x; }
void Trace::yminimum(const float x) { floats[sac_map.at("yminimum")] = x; }
void Trace::ymaximum(const float x) { floats[sac_map.at("ymaximum")] = x; }
// Doubles
// Doubles
void Trace::delta(const double x) { doubles[sac_map.at("delta")] = x; }
void Trace::b(const double x) { doubles[sac_map.at("b")] = x; }
void Trace::e(const double x) { doubles[sac_map.at("e")] = x; }
void Trace::o(const double x) { doubles[sac_map.at("o")] = x; }
void Trace::a(const double x) { doubles[sac_map.at("a")] = x; }
void Trace::t0(const double x) { doubles[sac_map.at("t0")] = x; }
void Trace::t1(const double x) { doubles[sac_map.at("t1")] = x; }
void Trace::t2(const double x) { doubles[sac_map.at("t2")] = x; }
void Trace::t3(const double x) { doubles[sac_map.at("t3")] = x; }
void Trace::t4(const double x) { doubles[sac_map.at("t4")] = x; }
void Trace::t5(const double x) { doubles[sac_map.at("t5")] = x; }
void Trace::t6(const double x) { doubles[sac_map.at("t6")] = x; }
void Trace::t7(const double x) { doubles[sac_map.at("t7")] = x; }
void Trace::t8(const double x) { doubles[sac_map.at("t8")] = x; }
void Trace::t9(const double x) { doubles[sac_map.at("t9")] = x; }
void Trace::f(const double x) { doubles[sac_map.at("f")] = x; }
void Trace::stla(const double x) { doubles[sac_map.at("stla")] = x; }
void Trace::stlo(const double x) { doubles[sac_map.at("stlo")] = x; }
void Trace::evla(const double x) { doubles[sac_map.at("evla")] = x; }
void Trace::evlo(const double x) { doubles[sac_map.at("evlo")] = x; }
void Trace::sb(const double x) { doubles[sac_map.at("sb")] = x; }
void Trace::sdelta(const double x) { doubles[sac_map.at("sdelta")] = x; }
// Ints
void Trace::nzyear(const int x) { ints[sac_map.at("nzyear")] = x; }
void Trace::nzjday(const int x) { ints[sac_map.at("nzjday")] = x; }
void Trace::nzhour(const int x) { ints[sac_map.at("nzhour")] = x; }
void Trace::nzmin(const int x) { ints[sac_map.at("nzmin")] = x; }
void Trace::nzsec(const int x) { ints[sac_map.at("nzsec")] = x; }
void Trace::nzmsec(const int x) { ints[sac_map.at("nzmsec")] = x; }
void Trace::nvhdr(const int x) { ints[sac_map.at("nvhdr")] = x; }
void Trace::norid(const int x) { ints[sac_map.at("norid")] = x; }
void Trace::nevid(const int x) { ints[sac_map.at("nevid")] = x; }
void Trace::npts(const int x) { ints[sac_map.at("npts")] = x; }
void Trace::nsnpts(const int x) { ints[sac_map.at("nsnpts")] = x; }
void Trace::nwfid(const int x) { ints[sac_map.at("nwfid")] = x; }
void Trace::nxsize(const int x) { ints[sac_map.at("nxsize")] = x; }
void Trace::nysize(const int x) { ints[sac_map.at("nysize")] = x; }
void Trace::iftype(const int x) { ints[sac_map.at("iftype")] = x; }
void Trace::idep(const int x) { ints[sac_map.at("idep")] = x; }
void Trace::iztype(const int x) { ints[sac_map.at("iztype")] = x; }
void Trace::iinst(const int x) { ints[sac_map.at("iinst")] = x; }
void Trace::istreg(const int x) { ints[sac_map.at("istreg")] = x; }
void Trace::ievreg(const int x) { ints[sac_map.at("ievreg")] = x; }
void Trace::ievtyp(const int x) { ints[sac_map.at("ievtyp")] = x; }
void Trace::iqual(const int x) { ints[sac_map.at("iqual")] = x; }
void Trace::isynth(const int x) { ints[sac_map.at("isynth")] = x; }
void Trace::imagtyp(const int x) { ints[sac_map.at("imagtyp")] = x; }
void Trace::imagsrc(const int x) { ints[sac_map.at("imagsrc")] = x; }
void Trace::ibody(const int x) { ints[sac_map.at("ibody")] = x; }
// Bools
void Trace::leven(const bool x) { bools[sac_map.at("leven")] = x; }
void Trace::lpspol(const bool x) { bools[sac_map.at("lpspol")] = x; }
void Trace::lovrok(const bool x) { bools[sac_map.at("lovrok")] = x; }
void Trace::lcalda(const bool x) { bools[sac_map.at("lcalda")] = x; }
// Strings
void Trace::kstnm(const std::string& x) { strings[sac_map.at("kstnm")] = x; }
void Trace::kevnm(const std::string& x) { strings[sac_map.at("kevnm")] = x; }
void Trace::khole(const std::string& x) { strings[sac_map.at("khole")] = x; }
void Trace::ko(const std::string& x) { strings[sac_map.at("ko")] = x; }
void Trace::ka(const std::string& x) { strings[sac_map.at("ka")] = x; }
void Trace::kt0(const std::string& x) { strings[sac_map.at("kt0")] = x; }
void Trace::kt1(const std::string& x) { strings[sac_map.at("kt1")] = x; }
void Trace::kt2(const std::string& x) { strings[sac_map.at("kt2")] = x; }
void Trace::kt3(const std::string& x) { strings[sac_map.at("kt3")] = x; }
void Trace::kt4(const std::string& x) { strings[sac_map.at("kt4")] = x; }
void Trace::kt5(const std::string& x) { strings[sac_map.at("kt5")] = x; }
void Trace::kt6(const std::string& x) { strings[sac_map.at("kt6")] = x; }
void Trace::kt7(const std::string& x) { strings[sac_map.at("kt7")] = x; }
void Trace::kt8(const std::string& x) { strings[sac_map.at("kt8")] = x; }
void Trace::kt9(const std::string& x) { strings[sac_map.at("kt9")] = x; }
void Trace::kf(const std::string& x) { strings[sac_map.at("kf")] = x; }
void Trace::kuser0(const std::string& x) { strings[sac_map.at("kuser0")] = x; }
void Trace::kuser1(const std::string& x) { strings[sac_map.at("kuser1")] = x; }
void Trace::kuser2(const std::string& x) { strings[sac_map.at("kuser2")] = x; }
void Trace::kcmpnm(const std::string& x) { strings[sac_map.at("kcmpnm")] = x; }
void Trace::knetwk(const std::string& x) { strings[sac_map.at("knetwk")] = x; }
void Trace::kdatrd(const std::string& x) { strings[sac_map.at("kdatrd")] = x; }
void Trace::kinst(const std::string& x) { strings[sac_map.at("kinst")] = x; }
// Data
void Trace::data1(const std::vector<double>& x) { data[sac_map.at("data1")] = x; }
void Trace::data2(const std::vector<double>& x) { data[sac_map.at("data2")] = x; }
//------------------------------------------------------------------------------
// Read
Trace::Trace(const std::filesystem::path& path) {
    std::ifstream file(path, std::ifstream::binary);
    if (!file) { std::cerr << path.string() << " could not be read.\n"; return; }
    file.seekg(0);
    //--------------------------------------------------------------------------
    // Header
    delta(binary_to_float(read_word(&file)));
    depmin(binary_to_float(read_word(&file)));
    depmax(binary_to_float(read_word(&file)));
    // Skip 'unused'
    read_word(&file);
    odelta(binary_to_float(read_word(&file)));
    b(binary_to_float(read_word(&file)));
    e(binary_to_float(read_word(&file)));
    o(binary_to_float(read_word(&file)));
    a(binary_to_float(read_word(&file)));
    // Skip 'internal'
    read_word(&file);
    // T# pick headers
    t0(binary_to_float(read_word(&file)));
    t1(binary_to_float(read_word(&file)));
    t2(binary_to_float(read_word(&file)));
    t3(binary_to_float(read_word(&file)));
    t4(binary_to_float(read_word(&file)));
    t5(binary_to_float(read_word(&file)));
    t6(binary_to_float(read_word(&file)));
    t7(binary_to_float(read_word(&file)));
    t8(binary_to_float(read_word(&file)));
    t9(binary_to_float(read_word(&file)));
    f(binary_to_float(read_word(&file)));
    // Response headers
    resp0(binary_to_float(read_word(&file)));
    resp1(binary_to_float(read_word(&file)));
    resp2(binary_to_float(read_word(&file)));
    resp3(binary_to_float(read_word(&file)));
    resp4(binary_to_float(read_word(&file)));
    resp5(binary_to_float(read_word(&file)));
    resp6(binary_to_float(read_word(&file)));
    resp7(binary_to_float(read_word(&file)));
    resp8(binary_to_float(read_word(&file)));
    resp9(binary_to_float(read_word(&file)));
    // Station headers
    stla(binary_to_float(read_word(&file)));
    stlo(binary_to_float(read_word(&file)));
    stel(binary_to_float(read_word(&file)));
    stdp(binary_to_float(read_word(&file)));
    // Event headers
    evla(binary_to_float(read_word(&file)));
    evlo(binary_to_float(read_word(&file)));
    evel(binary_to_float(read_word(&file)));
    evdp(binary_to_float(read_word(&file)));
    mag(binary_to_float(read_word(&file)));
    // User misc headers
    user0(binary_to_float(read_word(&file)));
    user1(binary_to_float(read_word(&file)));
    user2(binary_to_float(read_word(&file)));
    user3(binary_to_float(read_word(&file)));
    user4(binary_to_float(read_word(&file)));
    user5(binary_to_float(read_word(&file)));
    user6(binary_to_float(read_word(&file)));
    user7(binary_to_float(read_word(&file)));
    user8(binary_to_float(read_word(&file)));
    user9(binary_to_float(read_word(&file)));
    // Geometry headers
    dist(binary_to_float(read_word(&file)));
    az(binary_to_float(read_word(&file)));
    baz(binary_to_float(read_word(&file)));
    gcarc(binary_to_float(read_word(&file)));
    // Metadata headers
    sb(binary_to_float(read_word(&file)));
    sdelta(binary_to_float(read_word(&file)));
    depmen(binary_to_float(read_word(&file)));
    cmpaz(binary_to_float(read_word(&file)));
    cmpinc(binary_to_float(read_word(&file)));
    xminimum(binary_to_float(read_word(&file)));
    xmaximum(binary_to_float(read_word(&file)));
    yminimum(binary_to_float(read_word(&file)));
    ymaximum(binary_to_float(read_word(&file)));
    // Skip 'unused' (x7)
    for (int i{0}; i < 7; ++i ) { read_word(&file); }
    // Date/time headers
    nzyear(binary_to_int(read_word(&file)));
    nzjday(binary_to_int(read_word(&file)));
    nzhour(binary_to_int(read_word(&file)));
    nzmin(binary_to_int(read_word(&file)));
    nzsec(binary_to_int(read_word(&file)));
    nzmsec(binary_to_int(read_word(&file)));
    // More metadata headers
    nvhdr(binary_to_int(read_word(&file)));
    norid(binary_to_int(read_word(&file)));
    nevid(binary_to_int(read_word(&file)));
    npts(binary_to_int(read_word(&file)));
    nsnpts(binary_to_int(read_word(&file)));
    nwfid(binary_to_int(read_word(&file)));
    nxsize(binary_to_int(read_word(&file)));
    nysize(binary_to_int(read_word(&file)));
    // Skip 'unused'
    read_word(&file);
    iftype(binary_to_int(read_word(&file)));
    idep(binary_to_int(read_word(&file)));
    iztype(binary_to_int(read_word(&file)));
    // Skip 'unused'
    read_word(&file);
    iinst(binary_to_int(read_word(&file)));
    istreg(binary_to_int(read_word(&file)));
    ievreg(binary_to_int(read_word(&file)));
    ievtyp(binary_to_int(read_word(&file)));
    iqual(binary_to_int(read_word(&file)));
    isynth(binary_to_int(read_word(&file)));
    imagtyp(binary_to_int(read_word(&file)));
    imagsrc(binary_to_int(read_word(&file)));
    ibody(binary_to_int(read_word(&file)));
    // Skip 'unused' (x7)
    for (int i{0}; i < 7; ++i) { read_word(&file); }
    // Logical headers
    leven(binary_to_bool(read_word(&file)));
    lpspol(binary_to_bool(read_word(&file)));
    lovrok(binary_to_bool(read_word(&file)));
    lcalda(binary_to_bool(read_word(&file)));
    // Skip 'unused'
    read_word(&file);
    // KSTNM is 2 words (normal)
    kstnm(binary_to_string(read_two_words(&file)));
    // KEVNM is 4 words long (unique!)
    kevnm(binary_to_long_string(read_four_words(&file)));
    // All other 'K' headers are 2 words
    khole(binary_to_string(read_two_words(&file)));
    ko(binary_to_string(read_two_words(&file)));
    ka(binary_to_string(read_two_words(&file)));
    kt0(binary_to_string(read_two_words(&file)));
    kt1(binary_to_string(read_two_words(&file)));
    kt2(binary_to_string(read_two_words(&file)));
    kt3(binary_to_string(read_two_words(&file)));
    kt4(binary_to_string(read_two_words(&file)));
    kt5(binary_to_string(read_two_words(&file)));
    kt6(binary_to_string(read_two_words(&file)));
    kt7(binary_to_string(read_two_words(&file)));
    kt8(binary_to_string(read_two_words(&file)));
    kt9(binary_to_string(read_two_words(&file)));
    kf(binary_to_string(read_two_words(&file)));
    kuser0(binary_to_string(read_two_words(&file)));
    kuser1(binary_to_string(read_two_words(&file)));
    kuser2(binary_to_string(read_two_words(&file)));
    kcmpnm(binary_to_string(read_two_words(&file)));
    knetwk(binary_to_string(read_two_words(&file)));
    kdatrd(binary_to_string(read_two_words(&file)));
    kinst(binary_to_string(read_two_words(&file)));
    //--------------------------------------------------------------------------
    // DATA
    if (npts() != unset_int) {
        // Originally floats, read as doubles
        data1(read_data(&file, static_cast<size_t>(npts()), data_word));
        // Uneven or spectral data
        if ((leven() == false) || (iftype() > 1)) {
            data2(read_data(&file, static_cast<size_t>(npts()),
                            data_word + npts()));
        }
    }
    //--------------------------------------------------------------------------
    // Footer
    if (nvhdr() == 7) {
        delta(binary_to_double(read_two_words(&file)));
        b(binary_to_double(read_two_words(&file)));
        e(binary_to_double(read_two_words(&file)));
        o(binary_to_double(read_two_words(&file)));
        a(binary_to_double(read_two_words(&file)));
        t0(binary_to_double(read_two_words(&file)));
        t1(binary_to_double(read_two_words(&file)));
        t2(binary_to_double(read_two_words(&file)));
        t3(binary_to_double(read_two_words(&file)));
        t4(binary_to_double(read_two_words(&file)));
        t5(binary_to_double(read_two_words(&file)));
        t6(binary_to_double(read_two_words(&file)));
        t7(binary_to_double(read_two_words(&file)));
        t8(binary_to_double(read_two_words(&file)));
        t9(binary_to_double(read_two_words(&file)));
        f(binary_to_double(read_two_words(&file)));
        evlo(binary_to_double(read_two_words(&file)));
        evla(binary_to_double(read_two_words(&file)));
        stlo(binary_to_double(read_two_words(&file)));
        stla(binary_to_double(read_two_words(&file)));
        sb(binary_to_double(read_two_words(&file)));
        sdelta(binary_to_double(read_two_words(&file)));
    } else { nvhdr(7); }
    file.close();
}
//------------------------------------------------------------------------------
// Write
void Trace::write(const std::filesystem::path& path) {
    std::ofstream file(path, std::ios::binary
                       | std::ios::out | std::ios::trunc);
    if (!file) { std::cerr << path.string() << "cannot be written.\n"; return; }
    write_words(&file, convert_to_word(static_cast<float>(delta())));
    write_words(&file, convert_to_word(depmin()));
    write_words(&file, convert_to_word(depmax()));
    // Fill 'unused'
    write_words(&file, convert_to_word(depmax()));
    write_words(&file, convert_to_word(odelta()));
    write_words(&file, convert_to_word(static_cast<float>(b())));
    write_words(&file, convert_to_word(static_cast<float>(e())));
    write_words(&file, convert_to_word(static_cast<float>(o())));
    write_words(&file, convert_to_word(static_cast<float>(a())));
    // Fill 'internal'
    write_words(&file, convert_to_word(depmin()));
    write_words(&file, convert_to_word(static_cast<float>(t0())));
    write_words(&file, convert_to_word(static_cast<float>(t1())));
    write_words(&file, convert_to_word(static_cast<float>(t2())));
    write_words(&file, convert_to_word(static_cast<float>(t3())));
    write_words(&file, convert_to_word(static_cast<float>(t4())));
    write_words(&file, convert_to_word(static_cast<float>(t5())));
    write_words(&file, convert_to_word(static_cast<float>(t6())));
    write_words(&file, convert_to_word(static_cast<float>(t7())));
    write_words(&file, convert_to_word(static_cast<float>(t8())));
    write_words(&file, convert_to_word(static_cast<float>(t9())));
    write_words(&file, convert_to_word(static_cast<float>(f())));
    write_words(&file, convert_to_word(resp0()));
    write_words(&file, convert_to_word(resp1()));
    write_words(&file, convert_to_word(resp2()));
    write_words(&file, convert_to_word(resp3()));
    write_words(&file, convert_to_word(resp4()));
    write_words(&file, convert_to_word(resp5()));
    write_words(&file, convert_to_word(resp6()));
    write_words(&file, convert_to_word(resp7()));
    write_words(&file, convert_to_word(resp8()));
    write_words(&file, convert_to_word(resp9()));
    write_words(&file, convert_to_word(static_cast<float>(stla())));
    write_words(&file, convert_to_word(static_cast<float>(stlo())));
    write_words(&file, convert_to_word(stel()));
    write_words(&file, convert_to_word(stdp()));
    write_words(&file, convert_to_word(static_cast<float>(evla())));
    write_words(&file, convert_to_word(static_cast<float>(evlo())));
    write_words(&file, convert_to_word(evel()));
    write_words(&file, convert_to_word(evdp()));
    write_words(&file, convert_to_word(mag()));
    write_words(&file, convert_to_word(user0()));
    write_words(&file, convert_to_word(user1()));
    write_words(&file, convert_to_word(user2()));
    write_words(&file, convert_to_word(user3()));
    write_words(&file, convert_to_word(user4()));
    write_words(&file, convert_to_word(user5()));
    write_words(&file, convert_to_word(user6()));
    write_words(&file, convert_to_word(user7()));
    write_words(&file, convert_to_word(user8()));
    write_words(&file, convert_to_word(user9()));
    write_words(&file, convert_to_word(dist()));
    write_words(&file, convert_to_word(az()));
    write_words(&file, convert_to_word(baz()));
    write_words(&file, convert_to_word(gcarc()));
    write_words(&file, convert_to_word(static_cast<float>(sb())));
    write_words(&file, convert_to_word(static_cast<float>(sdelta())));
    write_words(&file, convert_to_word(depmen()));
    write_words(&file, convert_to_word(cmpaz()));
    write_words(&file, convert_to_word(cmpinc()));
    write_words(&file, convert_to_word(xminimum()));
    write_words(&file, convert_to_word(xmaximum()));
    write_words(&file, convert_to_word(yminimum()));
    write_words(&file, convert_to_word(ymaximum()));
    // Fill 'unused' (x7)
    for (int i{0}; i < 7; ++i) { write_words(&file, convert_to_word(az())); }
    write_words(&file, convert_to_word(nzyear()));
    write_words(&file, convert_to_word(nzjday()));
    write_words(&file, convert_to_word(nzhour()));
    write_words(&file, convert_to_word(nzmin()));
    write_words(&file, convert_to_word(nzsec()));
    write_words(&file, convert_to_word(nzmsec()));
    write_words(&file, convert_to_word(nvhdr()));
    write_words(&file, convert_to_word(norid()));
    write_words(&file, convert_to_word(nevid()));
    write_words(&file, convert_to_word(npts()));
    write_words(&file, convert_to_word(nsnpts()));
    write_words(&file, convert_to_word(nwfid()));
    write_words(&file, convert_to_word(nxsize()));
    write_words(&file, convert_to_word(nysize()));
    // Fill 'unused'
    write_words(&file, convert_to_word(nysize()));
    write_words(&file, convert_to_word(iftype()));
    write_words(&file, convert_to_word(idep()));
    write_words(&file, convert_to_word(iztype()));
    // Fill 'unused'
    write_words(&file, convert_to_word(iztype()));
    write_words(&file, convert_to_word(iinst()));
    write_words(&file, convert_to_word(istreg()));
    write_words(&file, convert_to_word(ievreg()));
    write_words(&file, convert_to_word(ievtyp()));
    write_words(&file, convert_to_word(iqual()));
    write_words(&file, convert_to_word(isynth()));
    write_words(&file, convert_to_word(imagtyp()));
    write_words(&file, convert_to_word(imagsrc()));
    write_words(&file, convert_to_word(ibody()));
    // Fill 'unused' (x7)
    for (int i{0}; i < 7; ++i) { write_words(&file, convert_to_word(ibody())); }
    write_words(&file, bool_to_word(leven()));
    write_words(&file, bool_to_word(lpspol()));
    write_words(&file, bool_to_word(lovrok()));
    write_words(&file, bool_to_word(lcalda()));
    // Fill 'unused'
    write_words(&file, bool_to_word(lcalda()));
    // Strings are special
    std::array<char, 2 * word_length> two_words{
        convert_to_words<sizeof(two_words)>(kstnm(), 2)};
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    std::array<char, 4 * word_length> four_words{
        convert_to_words<sizeof(four_words)>(kevnm(), 4)};
    write_words(&file, std::vector<char>(four_words.begin(), four_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(khole(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(ko(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(ka(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt0(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt1(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt2(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt3(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt4(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt5(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt6(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt7(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt8(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kt9(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kf(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kuser0(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kuser1(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kuser2(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kcmpnm(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(knetwk(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kdatrd(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));

    two_words = convert_to_words<sizeof(two_words)>(kinst(), 2);
    write_words(&file, std::vector<char>(two_words.begin(), two_words.end()));
    // Data
    for (double x : data1()) {
        write_words(&file, convert_to_word(static_cast<float>(x)));
    }
    if ((leven() == false) || (iftype() > 1)) {
        for (double x : data2()) {
            write_words(&file, convert_to_word(static_cast<float>(x)));
        }
    }
    // Footer
    if (nvhdr() == 7) {
        write_words(&file, convert_to_word(delta()));
        write_words(&file, convert_to_word(b()));
        write_words(&file, convert_to_word(e()));
        write_words(&file, convert_to_word(o()));
        write_words(&file, convert_to_word(a()));
        write_words(&file, convert_to_word(t0()));
        write_words(&file, convert_to_word(t1()));
        write_words(&file, convert_to_word(t2()));
        write_words(&file, convert_to_word(t3()));
        write_words(&file, convert_to_word(t4()));
        write_words(&file, convert_to_word(t5()));
        write_words(&file, convert_to_word(t6()));
        write_words(&file, convert_to_word(t7()));
        write_words(&file, convert_to_word(t8()));
        write_words(&file, convert_to_word(t9()));
        write_words(&file, convert_to_word(f()));
        write_words(&file, convert_to_word(evlo()));
        write_words(&file, convert_to_word(evla()));
        write_words(&file, convert_to_word(stlo()));
        write_words(&file, convert_to_word(stla()));
        write_words(&file, convert_to_word(sb()));
        write_words(&file, convert_to_word(sdelta()));
    }
    file.close();
}

void Trace::legacy_write(const std::filesystem::path& path) {
    nvhdr(6);
    write(path);
}
};
