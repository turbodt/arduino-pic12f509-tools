#ifndef H_INCLUDE_PIC12f509_CONSTANTS
#define H_INCLUDE_PIC12f509_CONSTANTS

#include <iostream>
#include <map>

#include "pic12f509.typing.hpp"

namespace pic12f509 {
  const addr_t MEM_LEN = 0x800;
  const addr_t USER_MEM_LEN = 0x400;

  const command_t CMD_LOAD_DATA = 0x02;
  const command_t CMD_READ_DATA = 0x04;
  const command_t CMD_INCREMENT_ADDRESS = 0x06;
  const command_t CMD_BEGIN_PROGRAMMING = 0x08;
  const command_t CMD_END_PROGRAMMING = 0x0E;
  const command_t CMD_BULK_ERASE = 0x09;

  const addr_t LOC_RESET_VECTOR = 0x03FF;
  const addr_t LOC_OSCCAL_VALUE = 0x0404;
  const addr_t LOC_CONF_WORD = 0x07FF;

  // MCLRE
  const std::uint8_t CONF_MCLRE_MASK = 0x10;
  const std::uint8_t CONF_MCLRE_MCLR = 0x10;
  const std::uint8_t CONF_MCLRE_GP3 = 0x00;
  // Code protection
  const std::uint8_t CONF_CODE_PROTECTION_MASK = 0x08;
  const std::uint8_t CONF_CODE_PROTECTION_ON = 0x00;
  const std::uint8_t CONF_CODE_PROTECTION_OFF = 0x08;
  // Watchdog timer
  const std::uint8_t CONF_WDTE_MASK = 0x04;
  const std::uint8_t CONF_WDTE_ON = 0x04;
  const std::uint8_t CONF_WDTE_OFF = 0x00;
  // Oscilator selection
  const std::uint8_t CONF_OSC_SEL_MASK = 0x03;
  const std::uint8_t CONF_OSC_SEL_LP = 0x00;
  const std::uint8_t CONF_OSC_SEL_XP = 0x01;
  const std::uint8_t CONF_OSC_SEL_INTOSC = 0x02;
  const std::uint8_t CONF_OSC_SEL_EXTRC = 0x03;

  // File Addresses
  const std::map<const std::string, const addr_t> FILE_ADDR= {
    { "INDF", 0x000 },
    { "TMR0", 0x001 },
    { "PCL", 0x002 },
    { "STATUS", 0x003 },
    { "FSR", 0x004 },
    { "OSCCAL", 0x005 },
    { "GPIO", 0x006 }
  };


}

#endif
