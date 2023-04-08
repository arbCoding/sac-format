#include <iostream>
#include <fstream>

// SAC uses words that are 4 characters long
static constexpr int word_length{4};

void read_next_word(std::ifstream &sac)
{
  char word[word_length] = {};
  sac.read((char*) &word, word_length);
  char* tmp = word;
  for (int i{0}; i < static_cast<int>(sizeof(word)); ++i)
  {
    std::cout << tmp[i] << '\t' << static_cast<int>(tmp[i]) << '\n';
  }
}

int word_position(int word_number)
{
  return (word_number * word_length);
}

// argc is the argument count, 
// argv[] is the argument vector
// it is an array of pointers to strings
int main(int argc, char* argv[])
{
  if (argc == 2)
  {
    std::cout << argv[1] << '\n';
  }
  else
  {
    std::cerr << "You must provide a single input file to be read.\n";
    return 1;
  }
  std::ifstream sac_file(argv[1], std::ifstream::binary);
  //std::ifstream sac_file("./data/IM.NV31..BHZ.M.2023.094.222304.SAC", std::ifstream::binary);
  if (!sac_file)
  {
    std::cerr << "File could not be read...\n";
    return 1;
  }
  sac_file.seekg(word_position(110));

  read_next_word(sac_file);

  return 0;
}
