#ifndef H_INCLUDE_PIC12f509_UTILS
#define H_INCLUDE_PIC12f509_UTILS

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <map>
#include <regex>

#include "pic12f509.typing.hpp"
#include "pic12f509.constants.hpp"

namespace pic12f509 {

  std::string word_to_str(word_t const &);
  std::string file_addr_to_str(addr_t const &);
  addr_t str_to_file_addr(
    std::string,
    const std::map<const std::string, const addr_t> * const = nullptr
  );
  addr_t str_to_literal(
    std::string,
    const std::map<const std::string, const addr_t> * const = nullptr
  );
  std::string to_upper_copy(std::string const &);
  void to_upper(std::string &);
}

#endif
