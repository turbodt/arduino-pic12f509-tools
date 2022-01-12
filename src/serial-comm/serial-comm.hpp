#ifndef H_INCLUDE_SERIAL_COMM
#define H_INCLUDE_SERIAL_COMM

// source: https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/

// C library headers
#include <stdio.h>
#include <string.h>
#include <cstdint>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

namespace serial_comm {

  class SerialPort {
    private:
    protected:
      struct termios tty;
      int fh;
      bool is_open;

      int configure_tty();
    public:
      SerialPort();
      ~SerialPort();

      SerialPort * open(char const *);
      SerialPort * close();

      SerialPort const * send_byte(uint8_t const &) const;
      uint8_t get_byte() const;
      template<typename T> SerialPort const * send(T const & data) const {
        ::write(this->fh, &data, sizeof(T));
        return this;
      }
  };
}

#endif
