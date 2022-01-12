#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "serial-comm/serial-comm.hpp"

int send_to_arduino(int argc, char * argv[]) {

  if (argc < 3) {
    std::cerr << "Required filename and comm: parse <file> <COM>" << std::endl;
    return 1;
  }
  std::filesystem::path filename = std::string(argv[1]);
  std::string serial_path = std::string(argv[2]);

  unsigned int num_bytes = std::filesystem::file_size(filename);

  char ack = 0;
  const int CHUNK_SIZE = 64;

  serial_comm::SerialPort * serial = new serial_comm::SerialPort();
  serial->open(serial_path.c_str());

  std::fstream fh = std::fstream(filename, std::ios::in | std::ios::binary);
  std::cout << "Send bytes: " << num_bytes << std::endl;
  serial->send(num_bytes);
  ack |= serial->get_byte();
  if (ack) {
    std::cerr << "Bad ACK received: " << (int)ack << std::endl;
    return 1;
  }
  std::cout << "ACK received" << std::endl;
  char byte;
  for (unsigned int i = 0 ; i < num_bytes ; i++) {
    fh.read(&byte, 1);
    serial->send((uint8_t)byte);
    if (
      (i + 1) % CHUNK_SIZE == 0
      || (i+1) == num_bytes
    ) {
      std::cout << CHUNK_SIZE<< "bytes sent." << std::endl;
      ack |= serial->get_byte();
      if (ack) {
        std::cerr << "Bad ACK received: " << (uint8_t)ack << std::endl;
        return 1;
      }
      std::cout << "ACK received" << std::endl;
    }
  }
  fh.close();

  serial->close();
  delete serial;

  return 0;
}
