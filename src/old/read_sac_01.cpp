#include <iostream> // std::cout
#include <fstream>  // std::ifstream

int main()
{
  std::ifstream sac("./data/IM.NV31..BHZ.M.2023.094.222304.SAC", std::ifstream::binary);

  // If it failed to be read in, tell me
  if (!sac)
  {
    std::cerr << "File could not be read...\n";
    return 1;
  }
  else // so read it!
  {
    constexpr int word_length{4}; // SAC defines headers as words of 4 characters each
    // get length of file
    sac.seekg(0, sac.end);
    int length = sac.tellg();
    // Go back to the beginning!
    sac.seekg(0);
    std::cout << "The file has a length of: " << length << " chars\n";

    char kstnm[2 * word_length] = {}; // Station name
    // seekg is only needed to start with if you're not starting from where you left off
    sac.seekg(110 * word_length); // Starts as the 110-th word in the header
    sac.read((char*) &kstnm, sizeof(kstnm));
    char* tmp = kstnm;
    for (int i{0}; i < static_cast<int>(sizeof(kstnm)); ++i)
    {
      if (tmp[i] != '\0')
      {
        std::cout << tmp[i] << '\t' << static_cast<int>(tmp[i]) << '\n';
      }
    }
    std::cout << '\n';

    char kevnm[4 * word_length] = {}; // Event name is 4 words!
    // We don't use seekg because we're already where we want to be!
    // sac.seekg(112 * word_length); // Starts as the 112-th word in the header
    sac.read((char*) &kevnm, sizeof(kevnm));
    tmp = kevnm;
    for (int i{0}; i < static_cast<int>(sizeof(kevnm)); ++i)
    {
      if (tmp[i] != '\0')
      {
        std::cout << tmp[i] << '\t' << static_cast<int>(tmp[i]) << '\n';
      }
    }
    std::cout << '\n';
  }

  return 0;
}
