#include "pic12f509.utils.hpp"


namespace pic12f509 {
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

    if (file_addr_str.substr(0,2) == "0X") {
      file_addr_str.erase(0,2);
    }
    if (file_addr_str.at(file_addr_str.size()-1) == 'H') {
      file_addr_str.erase(file_addr_str.size()-1);
    }
    std::stringstream ss;
    ss << std::hex << file_addr_str;
    addr_t file_addr;
    ss >> file_addr;
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

    if (literal_str.substr(0,2) == "0X") {
      literal_str.erase(0,2);
    }

    std::stringstream ss;
    ss << std::hex << literal_str;
    addr_t literal;
    ss >> literal;
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
