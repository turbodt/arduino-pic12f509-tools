#ifndef H_INCLUDE_PIC12f509_EXCEPTIONS
#define H_INCLUDE_PIC12f509_EXCEPTIONS

#include <iostream>
#include "pic12f509.typing.hpp"
#include "pic12f509.utils.hpp"

namespace pic12f509 {
  class BaseException : public std::exception {
    protected:
      std::string message;
    public:
      explicit BaseException() {};
      explicit BaseException(const std::string& message) : message(message) {};
      virtual const char* what() const noexcept {
        return this->message.c_str();
      }
      void update_message(const std::string & message) {
        this->message = message;
      }
  };

  class CommandDoesNotExistException : public BaseException {
    public:
      std::string command;
      explicit CommandDoesNotExistException(const std::string& command): command(command) {
        this->message = "Command `" + this->command + "` does not exist.";
      };
  };

  class ValueExceedsMaskException : public BaseException {
    public:
      word_t value;
      word_t mask;
      explicit ValueExceedsMaskException(word_t const & value, word_t const & mask): value(value), mask(mask) {
        this->message = "Value `" + word_to_str(this->value)
          + "` exceeds the number of bits reserved by the mask "
          + word_to_str(this->mask) + ".";
      };
  };

}

#endif
