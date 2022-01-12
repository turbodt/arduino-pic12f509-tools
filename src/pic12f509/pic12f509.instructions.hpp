#ifndef H_INCLUDE_PIC12f509_INSTRUCTIONS
#define H_INCLUDE_PIC12f509_INSTRUCTIONS

#include <iostream>
#include <algorithm>
#include <sstream>
#include <map>

#include "pic12f509.typing.hpp"
#include "pic12f509.constants.hpp"
#include "pic12f509.utils.hpp"

namespace pic12f509 {
  // Instructions
  const word_t INS_DESTINATION_MASK = 0x020;
  const word_t INS_DESTINATION_W = 0x000;
  const word_t INS_DESTINATION_F = 0x020;
  const word_t INS_FILE_MASK = 0x01F;
  const word_t INS_BIT_ADDR_MASK = 0x0E0;
  const word_t INS_LITERAL_MASK = 0x0FF;
  const word_t INS_LITERAL_GOTO_MASK = 0x1FF;

  const word_t INS_BYTE_OOP_MASK = 0xFC0;
  const word_t INS_BIT_OOP_MASK = 0xF00;
  const word_t INS_LIT_CTRL_OOP_MASK = 0xF00;
  const word_t INS_LIT_CTRL_OOP_GOTO_MASK = 0xE00;

  /*
  // Byte Oriented Operations
  const word_t INS_ADDWF  = 0x1C0;
  const word_t INS_ANDWF  = 0x140;
  //const word_t INS_CLRF = 0x060;
  //const word_t INS_CLRW = 0x040;
  const word_t INS_CLR    = 0x040;
  const word_t INS_COMF   = 0x240;
  const word_t INS_DECF   = 0x0C0;
  const word_t INS_DECFSZ = 0x2C0;
  const word_t INS_INCF   = 0x280;
  const word_t INS_INCFSZ = 0x3C0;
  const word_t INS_IORWF  = 0x100;
  const word_t INS_MOVF   = 0x200;
  const word_t INS_MOVWF  = 0x020;
  const word_t INS_NOP    = 0x000;
  const word_t INS_RLF    = 0x340;
  const word_t INS_RRF    = 0x300;
  const word_t INS_SUBWF  = 0x080;
  const word_t INS_SWAPF  = 0x380;
  const word_t INS_XORWF  = 0x180;
  // Byte Oriented Operations
  const word_t INS_BCF    = 0x400;
  const word_t INS_BSF    = 0x500;
  const word_t INS_BTFSC  = 0x600;
  const word_t INS_BTFSS  = 0x700;
  // Literal and Control Oriented Operations
  const word_t INS_ANDLW  = 0xE00;
  const word_t INS_CALL   = 0x900;
  const word_t INS_CLRWDT = 0x004;
  const word_t INS_GOTO   = 0xA00;
  const word_t INS_IORLW  = 0xD00;
  const word_t INS_MOVLW  = 0xC00;
  const word_t INS_OPTION = 0x002;
  const word_t INS_RETLW  = 0x800;
  const word_t INS_SLEEP  = 0x003;
  const word_t INS_TRIS   = 0x000;
  const word_t INS_XORLW  = 0xF00;
  // CUSTOM
  const word_t INS_UNKNOWN  = 0x005;
  */

  const std::map<const std::string, const word_t> INSTRUCTIONS = {
    { "ADDWF",  0x1C0 },
    { "ANDWF",  0x140 },
  //  { "CLRF", 0x060 },
  //  { "CLRW", 0x040 },
    { "CLR",    0x040 },
    { "COMF",   0x240 },
    { "DECF",   0x0C0 },
    { "DECFSZ", 0x2C0 },
    { "INCF",   0x280 },
    { "INCFSZ", 0x3C0 },
    { "IORWF",  0x100 },
    { "MOVF",   0x200 },
    { "MOVWF",  0x020 },
    { "NOP",    0x000 },
    { "RLF",    0x340 },
    { "RRF",    0x300 },
    { "SUBWF",  0x080 },
    { "SWAPF",  0x380 },
    { "XORWF",  0x180 },
  // Byte Oriented Operations
    { "BCF",    0x400 },
    { "BSF",    0x500 },
    { "BTFSC",  0x600 },
    { "BTFSS",  0x700 },
  // Literal and Control Oriented Operations
    { "ANDLW",  0xE00 },
    { "CALL",   0x900 },
    { "CLRWDT", 0x004 },
    { "GOTO",   0xA00 },
    { "IORLW",  0xD00 },
    { "MOVLW",  0xC00 },
    { "OPTION", 0x002 },
    { "RETLW",  0x800 },
    { "SLEEP",  0x003 },
    { "TRIS",   0x000 },
    { "XORLW",  0xF00 },
  // CUSTOM
    { "UNKNOWN",  0x005 }
  };

  std::string byte_oop_to_str(word_t const &);
  std::string bit_oop_to_str(word_t const &);
  std::string literal_control_oop_to_str(word_t const &);

  bool is_instruction_byte_oop(word_t const &);
  bool is_instruction_bit_oop(word_t const &);
  bool is_instruction_literal_control_oop(word_t const &);

  std::string instruction_to_str(word_t const &);
  word_t str_to_instruction(
    std::string,
    const std::map<const std::string, const addr_t> * const = nullptr
  );

}

#endif
