#include "pic12f509.utils.hpp"


namespace pic12f509 {

  std::regex regex_file_addr_hex = std::regex("([0-9A-Fa-f]+)[hH]");
  std::regex regex_literal_hex = std::regex("0[xX]([0-9A-Fa-f]+)|[hH]'([0-9A-Fa-f]+)'|([0-9A-Fa-f]+)");
  std::regex regex_literal_bin = std::regex("[bB]'([01]+)'");
  std::regex regex_literal_dec = std::regex("[dD]'([0-9]+)'");

  std::string get_first_not_empty_submatch(std::smatch const & sm) {
    for (uint8_t i = 1 ; i < sm.size() ; i ++) {
      if ( sm[i].length() > 0) {
        return sm[i];
      }
    }
    return "";
  }

  std::string word_to_str(word_t const & instruction) {
    std::stringstream ss;
    ss << "0x"
      << std::setfill('0')
      << std::setw(3)
      << std::hex
      << std::uppercase
      << instruction;
    return ss.str();
  }

  std::string file_addr_to_str(addr_t const & file_addr) {

    for (
      std::map<const std::string, const word_t>::const_iterator it = FILE_ADDR.begin();
      it != FILE_ADDR.end();
      it++
    ) {
      if (file_addr == it->second) {
        return it->first;
      }
    }

    std::stringstream ss;
    ss << ""
      << std::setfill('0')
      << std::setw(2)
      << std::hex
      << std::uppercase
      << file_addr
      << "h";
    return ss.str();
  }

  addr_t str_to_file_addr(
    std::string file_addr_str,
    const std::map<const std::string, const addr_t> * const labels
  ) {

    if (file_addr_str.size() == 0) {
      return 0;
    }

    to_upper(file_addr_str);

    for (
      std::map<const std::string, const word_t>::const_iterator it = FILE_ADDR.begin();
      it != FILE_ADDR.end();
      it++
    ) {
      if (file_addr_str == it->first) {
        return it->second;
      }
    }

    if (labels != nullptr) {
      for (
        std::map<const std::string, const word_t>::const_iterator it = labels->begin();
        it != labels->end();
        it++
      ) {
        if (file_addr_str == it->first) {
          return it->second;
        }
      }
    }

    addr_t file_addr = 0;
    std::smatch sm_hex;
    std::regex_match(file_addr_str,sm_hex,regex_file_addr_hex);
    if (sm_hex.size() > 1) {
      std::stringstream ss;
      ss << std::hex << sm_hex[1];
      ss >> file_addr;
    }

    return file_addr;
  }

  addr_t str_to_literal(
    std::string literal_str,
    const std::map<const std::string, const addr_t> * const labels
  ) {

    if (literal_str.size() == 0) {
      return 0;
    }

    to_upper(literal_str);

    for (
      std::map<const std::string, const word_t>::const_iterator it = FILE_ADDR.begin();
      it != FILE_ADDR.end();
      it++
    ) {
      if (literal_str == it->first) {
        return it->second;
      }
    }

    if (labels != nullptr) {
      for (
        std::map<const std::string, const word_t>::const_iterator it = labels->begin();
        it != labels->end();
        it++
      ) {
        if (literal_str == to_upper_copy(it->first)) {
          return it->second;
        }
      }
    }

    addr_t literal = 0;
    std::smatch sm;
    if (std::regex_match(literal_str,sm,regex_literal_hex)) {
      std::stringstream ss;
      ss << std::hex << get_first_not_empty_submatch(sm);
      ss >> literal;
    } else if (std::regex_match(literal_str,sm,regex_literal_dec)) {
      std::stringstream ss;
      ss << get_first_not_empty_submatch(sm);
      ss >> literal;
    } else if (std::regex_match(literal_str,sm,regex_literal_bin)) {
      literal = std::stoi( get_first_not_empty_submatch(sm), nullptr, 2);
    }

    return literal;
  }

  void to_upper(std::string & str) {
    std::for_each(
      str.begin(),
      str.end(),
      [](char & c) {
        c = ::toupper(c);
      }
    );
  }

  std::string to_upper_copy(std::string const & str) {
    std::string new_str = str;
    std::for_each(
      new_str.begin(),
      new_str.end(),
      [](char & c) {
        c = ::toupper(c);
      }
    );
    return new_str;
  }

}
