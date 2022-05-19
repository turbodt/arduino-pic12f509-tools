#ifndef H_INCLUDE_PIC12f509_TYPING
#define H_INCLUDE_PIC12f509_TYPING

#include <iostream>

namespace pic12f509 {
  typedef std::uint16_t addr_t;
  typedef std::uint16_t word_t;
  typedef std::uint8_t command_t;

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

}

#endif
