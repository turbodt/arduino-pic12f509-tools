#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "pic12f509/pic12f509.interface.hpp"

namespace command_decompile {
  pic12f509::word_t decompile_extract_first_instruction(uint8_t const * const buff) {
    return (((uint16_t)buff[0]) << 4) & 0xff0 | (((uint16_t)buff[1]) >> 4) & 0x00f;
  }

  pic12f509::word_t decompile_extract_second_instruction(uint8_t const * const buff) {
    return (((uint16_t)buff[1]) << 8) & 0xf00 | (((uint16_t)buff[2])) & 0x0ff;
  }

  int main(int argc, char * argv[]) {

    if (argc < 2) {
      std::cerr << "Required filename: " << argv[0] << " file.hex" << std::endl;
      return 1;
    }
    std::filesystem::path filename = std::string(argv[1]);

    uint16_t num_bytes = std::filesystem::file_size(filename);
    if (num_bytes%3 == 1) {
      std::cerr << "Invalid number of bytes" << std::endl;
      return 1;
    }
    uint16_t num_instructions = 2*(uint16_t)(num_bytes /3) + (num_bytes%3 == 2 ? 1 : 0);

    pic12f509::word_t instructions[num_instructions];
    uint8_t pair_of_instructions_buff[3];

    std::fstream fh = std::fstream(filename, std::ios::in | std::ios::binary);
    for (uint16_t i=0; i < num_bytes; i++) {
      fh.read((char *)&pair_of_instructions_buff[i % 3], 1);
      if (i%3 == 1) {
        instructions[2*(i-1)/3] = decompile_extract_first_instruction(pair_of_instructions_buff);
      } else if (i%3 == 2) {
        instructions[2*(i-2)/3 + 1] = decompile_extract_second_instruction(pair_of_instructions_buff);
      }
    }
    fh.close();

    for (uint16_t i = 0; i < num_instructions; i++) {
      pic12f509::word_t instruction = instructions[i];
      // out
      std::cout << ""
        << pic12f509::word_to_str(i)
        << " "
        << pic12f509::instruction_to_str(instruction)
        << std::endl;
    }

    return 0;
  }
}
