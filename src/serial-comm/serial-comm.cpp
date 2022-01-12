#include "serial-comm.hpp"

namespace serial_comm {

  SerialPort::SerialPort() {
    this->is_open = true;
  };

  SerialPort::~SerialPort() {
    this->close();
  };

  int SerialPort::configure_tty() {

    // Read in existing settings, and handle any error
    if(tcgetattr(this->fh, &this->tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return errno;
    }

    this->tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    this->tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    this->tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
    this->tty.c_cflag |= CS8; // 8 bits per byte (most common)
    this->tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    this->tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    this->tty.c_lflag &= ~ICANON;
    this->tty.c_lflag &= ~ECHO; // Disable echo
    this->tty.c_lflag &= ~ECHOE; // Disable erasure
    this->tty.c_lflag &= ~ECHONL; // Disable new-line echo
    this->tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    this->tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    this->tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    this->tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    this->tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // this->tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // this->tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    this->tty.c_cc[VTIME] = 50;    // Wait for up to 5s (50 deciseconds), returning as soon as any data is received.
    this->tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&this->tty, B9600);
    cfsetospeed(&this->tty, B9600);

    // Save tty settings, also checking for error
    if (tcsetattr(this->fh, TCSANOW, &this->tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return errno;
    }

    return 0;
  }

  SerialPort * SerialPort::open(char const * filename) {
    this->fh = ::open(filename, O_RDWR);
    int err = this->configure_tty();
    if (err != 0) {
      return this;
    }

    return this;
  }

  SerialPort * SerialPort::close() {
    if (this->is_open) {
      ::close(this->fh);
      this->is_open = false;
    }
    return this;
  }

  SerialPort const * SerialPort::send_byte(uint8_t const & byte) const {
    ::write(this->fh, &byte, sizeof(uint8_t));
    return this;
  }

  uint8_t SerialPort::get_byte() const {
    uint8_t byte;
    ::read(this->fh, &byte, sizeof(uint8_t));
    return byte;
  }

}
