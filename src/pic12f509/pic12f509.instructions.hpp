#ifndef H_INCLUDE_PIC12f509_INSTRUCTIONS
#define H_INCLUDE_PIC12f509_INSTRUCTIONS

#include <iostream>
#include <algorithm>
#include <sstream>
#include <map>
#include <regex>

#include "pic12f509.typing.hpp"
#include "pic12f509.exceptions.hpp"
#include "pic12f509.constants.hpp"
#include "pic12f509.utils.hpp"

namespace pic12f509 {

  struct OperatorInfo {
    struct Masks {
      word_t destination;
      word_t file_address;
      word_t literal;
      word_t byte;
      word_t bit_value;
    };
    word_t opcode;
    Masks masks;
  };

  const std::map<const std::string, const OperatorInfo> OPERATORS = {
    { "ADDWF", OperatorInfo{.opcode=0x1C0, .masks=OperatorInfo::Masks{.destination=0x020, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "ANDWF", OperatorInfo{.opcode=0x140, .masks=OperatorInfo::Masks{.destination=0x020, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "CLRF",  OperatorInfo{.opcode=0x060, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "CLRW",  OperatorInfo{.opcode=0x040, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x000, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "COMF",  OperatorInfo{.opcode=0x240, .masks=OperatorInfo::Masks{.destination=0x020, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "DECF",  OperatorInfo{.opcode=0x0C0, .masks=OperatorInfo::Masks{.destination=0x020, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "DECFSZ",OperatorInfo{.opcode=0x2C0, .masks=OperatorInfo::Masks{.destination=0x020, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "INCF",  OperatorInfo{.opcode=0x280, .masks=OperatorInfo::Masks{.destination=0x020, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "INCFSZ",OperatorInfo{.opcode=0x3C0, .masks=OperatorInfo::Masks{.destination=0x020, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "IORWF", OperatorInfo{.opcode=0x100, .masks=OperatorInfo::Masks{.destination=0x020, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "MOVF",  OperatorInfo{.opcode=0x200, .masks=OperatorInfo::Masks{.destination=0x020, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "MOVWF", OperatorInfo{.opcode=0x020, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "NOP",   OperatorInfo{.opcode=0x000, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x000, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "RLF",   OperatorInfo{.opcode=0x340, .masks=OperatorInfo::Masks{.destination=0x020, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "RRF",   OperatorInfo{.opcode=0x300, .masks=OperatorInfo::Masks{.destination=0x020, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "SUBWF", OperatorInfo{.opcode=0x080, .masks=OperatorInfo::Masks{.destination=0x020, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "SWAPF", OperatorInfo{.opcode=0x380, .masks=OperatorInfo::Masks{.destination=0x020, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "XORWF", OperatorInfo{.opcode=0x180, .masks=OperatorInfo::Masks{.destination=0x020, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
  // Byte Oriented Operations
    { "BCF",   OperatorInfo{.opcode=0x400, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x0E0} }},
    { "BSF",   OperatorInfo{.opcode=0x500, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x0E0} }},
    { "BTFSC", OperatorInfo{.opcode=0x600, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x0E0} }},
    { "BTFSS", OperatorInfo{.opcode=0x700, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x01F, .literal=0x000, .byte=0x000, .bit_value=0x0E0} }},
  // Literal and Control Oriented Operations
    { "ANDLW", OperatorInfo{.opcode=0xE00, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x000, .literal=0x0FF, .byte=0x000, .bit_value=0x000} }},
    { "CALL",  OperatorInfo{.opcode=0x900, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x000, .literal=0x0FF, .byte=0x000, .bit_value=0x000} }},
    { "CLRWDT",OperatorInfo{.opcode=0x004, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x000, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "GOTO",  OperatorInfo{.opcode=0xA00, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x000, .literal=0x1FF, .byte=0x000, .bit_value=0x000} }},
    { "IORLW", OperatorInfo{.opcode=0xD00, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x000, .literal=0x0FF, .byte=0x000, .bit_value=0x000} }},
    { "MOVLW", OperatorInfo{.opcode=0xC00, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x000, .literal=0x0FF, .byte=0x000, .bit_value=0x000} }},
    { "OPTION",OperatorInfo{.opcode=0x002, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x000, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "RETLW", OperatorInfo{.opcode=0x800, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x000, .literal=0x0FF, .byte=0x000, .bit_value=0x000} }},
    { "SLEEP", OperatorInfo{.opcode=0x003, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x000, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "TRIS",  OperatorInfo{.opcode=0x006, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x007, .literal=0x000, .byte=0x000, .bit_value=0x000} }},
    { "XORLW", OperatorInfo{.opcode=0xF00, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x000, .literal=0x0FF, .byte=0x000, .bit_value=0x000} }},
  // CUSTOM
    { "UNKNOWN", OperatorInfo{.opcode= 0x005, .masks=OperatorInfo::Masks{.destination=0x000, .file_address=0x000, .literal=0x000, .byte=0x000, .bit_value=0x000} }}
  };

  word_t obtain_from_mask(word_t const &, word_t const &);
  word_t blend_with_mask(word_t const &, word_t const &, word_t const &);

  std::string const get_opcode_str(word_t const &);
  OperatorInfo const * get_operator_info(word_t const &);

  std::string instruction_to_str(word_t const &);
  word_t str_to_instruction(
    std::string,
    const std::map<const std::string, const addr_t> * const = nullptr
  );

}

#endif
