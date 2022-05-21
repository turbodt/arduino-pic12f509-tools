#include "pic12f509.instructions.hpp"

namespace pic12f509 {

  word_t obtain_from_mask(word_t const & mask, word_t const & value) {
    word_t result = mask & value;
    for (word_t i = mask; i > 0 && i % 2 == 0; i /= 2) {
      result /= 2;
    }
    return result;
  }

  word_t blend_with_mask(word_t const & current_value, word_t const & mask, word_t const & value) {
    uint8_t offset = 0;
    for (word_t i = mask; i > 0 && i % 2 == 0; i /= 2) {
      offset++;
    }
    if (value > mask) {
      throw ValueExceedsMaskException(value, mask);
    }
    word_t masked_value = (mask >> offset) & value;
    while (offset > 0) {
      masked_value *= 2;
      offset--;
    }
    return (current_value & (~mask)) | (masked_value & mask);
  }

  std::string const get_opcode_str(word_t const & instruction) {
    // default value
    std::string opcode_str = "UNKNOWN";
    // candidates
    auto candidates = new std::map<const word_t, const std::string>();
    std::for_each(OPERATORS.begin(), OPERATORS.end(), [&](auto pair) {
      candidates->insert(std::pair<const word_t, const std::string> (pair.second.opcode, pair.first));
    });

    // iteratively filtering
    word_t mask = 0x000;
    for (uint16_t p = 0x800; p > 0 && candidates->size() > 1 ; p = p >> 1) {
      mask += p;
      for (auto it = candidates->begin(); it != candidates->end(); ) {
        bool pass = (mask & it->first) == (mask & instruction);
        if (pass) {
          it++;
        } else {
          it = candidates->erase(it);
        }
      }
    }
    if (candidates->size() == 1) {
      opcode_str = candidates->begin()->second;
    }
    delete candidates;
    return opcode_str;
  }

  OperatorInfo const * get_operator_info(word_t const & instruction) {
    std::string opcode_str = get_opcode_str(instruction);
    return &(OPERATORS.at(opcode_str));
  }

  std::string instruction_to_str(word_t const & instruction) {
    std::string const opcode_str = get_opcode_str(instruction);
    OperatorInfo const * operator_info = get_operator_info(instruction);

    // case literal operands
    if (operator_info->masks.literal) {
      word_t literal = obtain_from_mask(operator_info->masks.literal, instruction);
      return opcode_str + " " + literal_to_str(literal);
    }

    // case not file address (nor literals) => no operands
    if (operator_info->masks.file_address == 0) {
      return opcode_str;
    }

    word_t file_address = (operator_info->masks.file_address) & instruction;
    std::string file_address_str = file_addr_to_str(file_address);

    // case file address and destination
    if (operator_info->masks.destination) {
      word_t destination = obtain_from_mask(operator_info->masks.destination, instruction);
      std::string destination_str = "W";
      if (destination) {
        destination_str = "f";
      }
      return opcode_str + " " + file_address_str + "," + destination_str;
    }

    // case file address and bit_value
    if (operator_info->masks.bit_value) {
      word_t bit_value = obtain_from_mask(operator_info->masks.bit_value, instruction);
      std::string bit_value_str = std::to_string(bit_value);
      return opcode_str + " " + file_address_str + "," + bit_value_str;
    }

    // case only file address
    return opcode_str + " " + file_address_str;
  }

  word_t str_to_instruction(
    std::string  instruction_str,
    const std::map<const std::string, const addr_t> * const addr_labels
  ) {
    to_upper(instruction_str);
    instruction_str = std::regex_replace(instruction_str, std::regex("[;,]"), " ");

    std::stringstream ss;
    ss << instruction_str;

    std::string opcode_str;
    ss >> opcode_str;

    if (OPERATORS.count(opcode_str) == 0) {
      throw CommandDoesNotExistException(opcode_str);
    }

    auto operator_info = &(OPERATORS.at(opcode_str));
    word_t instruction = operator_info->opcode;

    // case literal operands
    if (operator_info->masks.literal) {
      std::string literal_str;
      ss >> literal_str;
      word_t literal = str_to_literal(literal_str, addr_labels);
      instruction = blend_with_mask(instruction, operator_info->masks.literal, literal);
      return instruction;
    }

    // case not file address (nor literals) => no operands
    if (operator_info->masks.file_address == 0) {
      return instruction;
    }

    std::string file_address_str;
    ss >> file_address_str;
    word_t file_address = str_to_file_addr(file_address_str);
    instruction = blend_with_mask(instruction, operator_info->masks.file_address, file_address);

    // case file address and destination
    if (operator_info->masks.destination) {
      std::string destination_str;
      ss >> destination_str;
      word_t destination = 0;
      if (destination_str == "1" || destination_str == "F") {
        destination = 1;
      }
      instruction = blend_with_mask(instruction, operator_info->masks.destination, destination);
      return instruction;
    }

    // case file address and bit_value
    if (operator_info->masks.bit_value) {
      word_t bit_value;
      ss >> bit_value;
      instruction = blend_with_mask(instruction, operator_info->masks.bit_value, bit_value);
      return instruction;
    }

    // case only file address
    return instruction;
  }
}
