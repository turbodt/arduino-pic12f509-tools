#include "pic12f509.instructions.hpp"

namespace pic12f509 {

  std::string byte_oop_to_str(word_t const & instruction) {
    word_t masked_instruction = instruction & INS_BYTE_OOP_MASK;

    std::string opcode_str = "UNKNOWN";
    if (instruction == 0) {
      return "NOP";
    } else if (! masked_instruction ) {
      masked_instruction = INSTRUCTIONS.at("MOVWF");
      opcode_str = "MOVWF";
    }

    std::string destination = "";
    std::string file_addr = "";

    for (
      std::map<const std::string, const word_t>::const_iterator it = INSTRUCTIONS.begin();
      it != INSTRUCTIONS.end() && opcode_str == "UNKNOWN";
      it++
    ) {
      if (masked_instruction == it->second) {
        opcode_str = it->first;
      }
    }

    switch (instruction & INS_DESTINATION_MASK) {
      case INS_DESTINATION_F:
        destination = "f";
        break;
      case INS_DESTINATION_W:
        destination = "W";
        break;
    }

    file_addr = file_addr_to_str(instruction & INS_FILE_MASK);
    if (masked_instruction == INSTRUCTIONS.at("MOVWF")) {
      return opcode_str + " " + file_addr;
    } else {
      return opcode_str + " " + destination + " " + file_addr;
    }
  }

  std::string bit_oop_to_str(word_t const & instruction) {
    word_t masked_instruction = instruction & INS_BIT_OOP_MASK;
    std::string opcode_str = "UNKNOWN";
    for (
      std::map<const std::string, const word_t>::const_iterator it = INSTRUCTIONS.begin();
      it != INSTRUCTIONS.end() && opcode_str == "UNKNOWN";
      it++
    ) {
      if (masked_instruction == it->second) {
        opcode_str = it->first;
      }
    }

    std::string bit = std::to_string((instruction & INS_BIT_ADDR_MASK) >> 5);
    std::string file_addr = file_addr_to_str(instruction & INS_FILE_MASK);
    return opcode_str + " " + bit + " " + file_addr;
  }

  std::string literal_control_oop_to_str(word_t const & instruction) {
    word_t masked_instruction = instruction & INS_LIT_CTRL_OOP_MASK;

    if (instruction == INSTRUCTIONS.at("CLRWDT")) {
      return "CLRWDT";
    } else if (instruction == INSTRUCTIONS.at("OPTION")) {
      return "OPTION";
    } else if (instruction == INSTRUCTIONS.at("SLEEP")) {
      return "SLEEP";
    } else if (instruction < 8) {
      return "TRIS " + word_to_str(instruction & 0x007);
    }

    std::string opcode_str = "UNKNOWN";
    for (
      std::map<const std::string, const word_t>::const_iterator it = INSTRUCTIONS.begin();
      it != INSTRUCTIONS.end() && opcode_str == "UNKNOWN";
      it++
    ) {
      if (masked_instruction == it->second) {
        opcode_str = it->first;
      }
    }

    std::string literal = word_to_str(instruction & INS_LITERAL_MASK);
    return opcode_str + " " + literal;
  }


  bool is_instruction_byte_oop(word_t const & instruction) {
    return instruction < 0x400
      && instruction != INSTRUCTIONS.at("OPTION")
      && instruction != INSTRUCTIONS.at("CLRWDT")
      && instruction != INSTRUCTIONS.at("SLEEP")
      && !(instruction > 0 && instruction < 8); //TRIS instruction
  }

  bool is_instruction_bit_oop(word_t const & instruction) {
    return instruction < 0x800 && instruction >= 0x400;
  }

  bool is_instruction_literal_control_oop(word_t const & instruction) {
    return instruction >= 0x800
      || instruction == INSTRUCTIONS.at("OPTION")
      || instruction == INSTRUCTIONS.at("CLRWDT")
      || instruction == INSTRUCTIONS.at("SLEEP")
      || (instruction > 0 && instruction < 8); //TRIS instruction
  }

  std::string instruction_to_str(word_t const & instruction) {
    if (is_instruction_byte_oop(instruction)) {
      return byte_oop_to_str(instruction);
    } else if (is_instruction_bit_oop(instruction)) {
      return bit_oop_to_str(instruction);
    } else if (is_instruction_literal_control_oop(instruction)) {
      return literal_control_oop_to_str(instruction);
    }
    return "Unknown";
  }

  word_t str_to_instruction(
    std::string  instruction_str,
    const std::map<const std::string, const addr_t> * const addr_labels
  ) {

    to_upper(instruction_str);

    std::stringstream ss;
    ss << instruction_str;

    std::string opcode_str;
    ss >> opcode_str;
    word_t instruction = INSTRUCTIONS.at("UNKNOWN");

    if (opcode_str == "NOP") {
      return INSTRUCTIONS.at("NOP");
    } else if (opcode_str == "CLRWDT") {
      return INSTRUCTIONS.at("CLRWDT");
    } else if (opcode_str == "OPTION") {
      return INSTRUCTIONS.at("OPTION");
    } else if (opcode_str == "SLEEP") {
      return INSTRUCTIONS.at("SLEEP");
    } else if (opcode_str == "TRIS") {
      instruction = INSTRUCTIONS.at("TRIS");
    }

    for (
      std::map<const std::string, const word_t>::const_iterator it = INSTRUCTIONS.begin();
      it != INSTRUCTIONS.end() && instruction == INSTRUCTIONS.at("UNKNOWN");
      it++
    ) {
      if (opcode_str == it->first) {
        instruction = it->second;
      }
    }

    if (instruction == INSTRUCTIONS.at("UNKNOWN")) {
      return instruction;
    }

    if (
      instruction == INSTRUCTIONS.at("TRIS")
      || instruction == INSTRUCTIONS.at("MOVWF")
    ) {
      std::string file_addr_str;
      ss >> file_addr_str;
      word_t file_addr = str_to_file_addr(file_addr_str);
      return instruction | (file_addr & INS_FILE_MASK);
    } else if (is_instruction_byte_oop(instruction)) {
      std::string destination_str, file_addr_str;
      word_t destination = 0;
      ss >> destination_str;
      if (destination_str == "F") {
        destination = INS_DESTINATION_F;
      } else {
        destination = INS_DESTINATION_W;
      }
      ss >> file_addr_str;
      word_t file_addr = str_to_file_addr(file_addr_str);
      return instruction | destination | (file_addr & INS_FILE_MASK);
    } else if (is_instruction_bit_oop(instruction)) {
      uint8_t bit;
      std::string file_addr_str;
      ss >> bit >> file_addr_str;
      word_t file_addr = str_to_file_addr(file_addr_str);
      return instruction | ((bit <<5) & INS_BIT_ADDR_MASK) | (file_addr & INS_FILE_MASK);
    } else if (is_instruction_literal_control_oop(instruction)) {
      std::string literal_str;
      ss >> literal_str;
      word_t literal;
      if (
        instruction == INSTRUCTIONS.at("GOTO")
        || instruction == INSTRUCTIONS.at("CALL")
      ) {
        literal = str_to_literal(literal_str, addr_labels);
      } else {
        literal = str_to_literal(literal_str);
      }

      if (instruction == INSTRUCTIONS.at("GOTO")) {
        return instruction | (literal & INS_LITERAL_GOTO_MASK);
      } else {
        return instruction | (literal & INS_LITERAL_MASK);
      }
    }

    return instruction;
  }
}
