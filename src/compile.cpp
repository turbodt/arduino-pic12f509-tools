#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <map>

#include "pic12f509/pic12f509.interface.hpp"
#include "pic12f509/pic12f509.typing.hpp"
#include "shared/main.hpp"

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
    std::map<const std::string, const pic12f509::addr_t> * labels,
    std::vector<pic12f509::BaseException> * errors
  ) {

    for (int line_cnt = 0 ; line_cnt < lines->size() ; line_cnt++) {
      std::string line = lines->at(line_cnt);
      trim(line);

      std::stringstream ss;
      ss << line;
      std::string first_word;
      ss >> first_word;
      trim(first_word);
      if (first_word.length() == 0 || is_line_comment(first_word)) {
        continue;
      }

      try {
        pic12f509::word_t opcode = pic12f509::str_to_instruction(first_word);
      } catch (pic12f509::BaseException & exception) {
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

      if (line.length() == 0 || is_line_comment(line)) {
        continue;
      }

      pic12f509::word_t instruction;
      try {
        instruction = pic12f509::str_to_instruction(line, labels);
      } catch (pic12f509::BaseException & exception) {
        exception.update_message(
          "Line " + std::to_string(line_cnt + 1)
          + ":\t"
          + line
          + "\n" + exception.what());
        errors->push_back(exception);
      }
      instructions->push_back(instruction);
    }
    // complete
    if (instructions->size() % 2) {
      instructions->push_back(pic12f509::OPERATORS.at("NOP").opcode);
    }

    return 0;
  }

  int get_warnings(
    const std::vector<std::string> * lines,
    std::vector<pic12f509::word_t> * instructions,
    std::map<const std::string, const pic12f509::addr_t> * labels,
    std::vector<std::string> * warnings
  ) {
    // get labels beyond 0x0FF
    std::map<const std::string, const pic12f509::addr_t> * labels_beyond_0ff = new std::map<const std::string, const pic12f509::addr_t>();
    std::for_each(labels->begin(), labels->end(), [&](auto it) {
      if (it.second >= 0x0FF) {
        labels_beyond_0ff->insert(std::pair<const std::string, const pic12f509::addr_t>(it.first, it.second));
      }
    });
    if (labels_beyond_0ff->size()) {
      std::stringstream ss;
      ss << "WARNING!" << std::endl;
      ss << "There are " << labels_beyond_0ff->size() << " labels beyond 0x1FF address." << std::endl;
      std::for_each(labels_beyond_0ff->begin(), labels_beyond_0ff->end(), [&](auto it) {
        ss << "\t- \"" << it.first << "\" at address "<< it.second << std::endl;
      });
      warnings->push_back(ss.str());
    }
    delete labels_beyond_0ff;

    return 0;
  }

  std::string get_help(int argc, char * argv[]) {
    std::stringstream ss;
    ss << "Usage: " << argv[0] << " <output-file>" << " [OPTIONS]";
    ss << std::endl << std::endl << "Positional Arguments:" << std::endl;
    ss << std::endl << "  " << "output-file";
    ss << "\t" << "Output `.hex` file where the output binary data will be write.";
    ss << std::endl << std::endl << "Options:" << std::endl;
    ss << std::endl << "  " << "-h";
    ss << "\t" << "Print help";
    return ss.str();
  }

  std::vector<std::string> const * parse_pos_args(int argc, char * argv[]) {
    std::vector<std::string> * pos_args = new std::vector<std::string>(0);
    uint8_t i = 0;
    while (i < argc) {
      if (argv[i][0] != '-') {
        pos_args->push_back(std::string(argv[i]));
      }
      i++;
    }
    return (std::vector<std::string> const *) pos_args;
  }

  std::map<const std::string, const std::string> * parse_opts(int argc, char * argv[]) {
    std::map<const std::string, const std::string> * opts = new std::map<const std::string, const std::string>();
    uint8_t i = 0;
    while (i < argc) {
      std::string arg(argv[i]);
      if (arg[0] != '-') {
        // do nothing
      } else if (arg == "--help" || arg == "-h") {
        opts->insert(std::pair{"help", std::string("")});
      } else if (arg == "--verbose" || arg == "-v") {
        opts->insert(std::pair{"verbose", std::string("")});
      }
      i++;
    }
    return opts;
  }

  int main(int argc, char * argv[]) {
    auto pos_args = parse_pos_args(argc, argv);
    auto opts = parse_opts(argc, argv);

    if (opts->count("help") || pos_args->size() != 2) {
      std::cerr << get_help(argc, argv) << std::endl;
      return 1;
    }
    std::filesystem::path filename = pos_args->at(1);

    std::string line;
    std::vector<std::string> * lines = new std::vector<std::string>();
    std::vector<pic12f509::word_t> * instructions = new std::vector<pic12f509::word_t>();
    std::map<const std::string, const pic12f509::addr_t> * labels = new std::map<const std::string, const pic12f509::addr_t>();
    std::vector<pic12f509::BaseException> * errors = new std::vector<pic12f509::BaseException>();
    std::vector<std::string> * warnings = new std::vector<std::string>();

    // get code
    while(std::getline(std::cin, line)) {
      lines->push_back(line);
    }

    // process
    // first read: get labels. Ignore instructions
    int error = 0;
    error |= get_instructions(lines, instructions, labels, errors);
    if (error) {
      return error;
    }
    instructions->clear();
    errors->clear();
    // second read: now the real process
    error |= get_instructions(lines, instructions, labels, errors);
    if (error) {
      return error;
    }

    // display errors, if any
    if (errors->size()) {
      std::for_each(
        errors->begin(),
        errors->end(),
        [&](auto exception) {
          std::cerr << exception.what() << "\n" << std::endl;
        }
      );
      delete errors;
      return 1;
    }
    delete errors;

    // get warnings
    get_warnings(lines, instructions, labels, warnings);
    delete lines;
    delete labels;

    // Raise warnings
    std::for_each(warnings->begin(), warnings->end(), [&](std::string warning) {
      std::cerr << warning << std::endl;
    });

    // verbose
    if (opts->count("verbose")) {
      std::cout << "It uses " << instructions->size() << " 12-bit instructions";
      std::cout << " (" << (3*instructions->size()/2) << " Bytes)";
      std::cout << " of 1024 instructions (1536 Bytes)."<< std::endl;
      std::cout << "That represents about a " << (100 * instructions->size() / 1024);
      std::cout << "% of the program memory."<< std::endl;
    }

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

    delete opts;
    delete pos_args;
    return 0;
  }
}
