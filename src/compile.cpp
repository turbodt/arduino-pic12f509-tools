#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <map>

#include "pic12f509/pic12f509.interface.hpp"
#include "pic12f509/pic12f509.typing.hpp"
#include "shared/main.cpp"

namespace command_compile {
  std::vector<std::uint8_t> extract_bytes(std::vector<pic12f509::word_t>::const_iterator const & it) {
    std::vector<pic12f509::word_t>::const_iterator it2 = it;
    it2++;
    std::vector<std::uint8_t> bytes = {
      (uint8_t) (((*it) >> 4) & 0x0ff),
      (uint8_t) ((((*it) << 4) & 0x0f0) | (((*it2) >> 8) & 0x00f)),
      (uint8_t) ((*it2) & 0x0ff)
    };
    return bytes;
  }

  bool is_line_comment(std::string const & line) {
    if (line.length() < 1) {
      return false;
    }
    if (line[0] == '#' || line[0] == ';') {
      return true;
    }
    if (line.length() < 2) {
      return false;
    }

    if (line[0] == '/' && line[1] == '/') {
      return true;
    }

    return false;
  }

  int get_instructions(
    const std::vector<std::string> * lines,
    std::vector<pic12f509::word_t> * instructions,
    std::map<const std::string, const pic12f509::addr_t> * labels
  ) {

    for (int line_cnt = 0 ; line_cnt < lines->size() ; line_cnt++) {
      std::string line = lines->at(line_cnt);
      trim(line);

      std::stringstream ss;
      ss << line;
      std::string first_word;
      ss >> first_word;
      trim(first_word);
      if (first_word.length() > 0 && is_line_comment(first_word)) {
        continue;
      }
      pic12f509::word_t opcode = pic12f509::str_to_instruction(first_word);

      if (opcode == pic12f509::INSTRUCTIONS.at("UNKNOWN")) {
        // is a label
        labels->insert(
          std::pair<const std::string, const pic12f509::addr_t>(
            first_word,
            instructions->size()
          )
        );
        line.erase(0, first_word.size());
        trim(line);
      };

      if (line == "") {
        continue;
      }
      pic12f509::word_t instruction = pic12f509::str_to_instruction(line, labels);
      if (instruction == pic12f509::INSTRUCTIONS.at("UNKNOWN")) {
        std::cerr << "Unknown instruction '" << line
          << "' at line " << line_cnt + 1 << std::endl;
        return 1;
      }
      instructions->push_back(instruction);
    }
    // complete
    if (instructions->size() % 2) {
      instructions->push_back(pic12f509::INSTRUCTIONS.at("NOP"));
    }

    return 0;
  }

  int main(int argc, char * argv[]) {

    if (argc < 2) {
      std::cerr << "Required filename: " << argv[0] << " file.hex" << std::endl;
      return 1;
    }
    std::filesystem::path filename = std::string(argv[1]);

    std::string line;
    std::vector<std::string> * lines = new std::vector<std::string>();
    std::vector<pic12f509::word_t> * instructions = new std::vector<pic12f509::word_t>();
    std::map<const std::string, const pic12f509::addr_t> * labels = new std::map<const std::string, const pic12f509::addr_t>();

    // get code
    while(std::getline(std::cin, line)) {
      lines->push_back(line);
    }

    // process
    // first read: get labels. Ignore instructions
    int error = 0;
    error |= get_instructions(lines, instructions, labels);
    if (error) {
      return error;
    }
    instructions->clear();
    // second read: now the real process
    error |= get_instructions(lines, instructions, labels);
    if (error) {
      return error;
    }
    delete lines;
    delete labels;

    // output
    std::fstream fh = std::fstream(filename, std::ios::out | std::ios::binary);
    for (
      std::vector<pic12f509::word_t>::const_iterator it = instructions->begin();
      it != instructions->end();
      it += 2
    ) {
      std::vector<uint8_t> bytes = extract_bytes(it);
      std::for_each(bytes.begin(), bytes.end(), [&fh](uint8_t byte) {
        fh.write((char *) &byte,1);
      });
    }
    fh.close();
    delete instructions;

    return 0;
  }
}
