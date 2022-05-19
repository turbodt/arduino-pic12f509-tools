#include <iostream>
#include "../testcase.hpp"
#include "../../src/pic12f509/pic12f509.instructions.hpp"

class InstructionsTestCase : public test_case::TestCase {

  public:
    InstructionsTestCase() : TestCase() {
      this->register_test("Test obtain from mask 1", [&]() { test_obtain_from_mask_001(); });
      this->register_test("Test obtain from mask 2", [&]() { test_obtain_from_mask_002(); });
      this->register_test("Test blend with mask 1", [&]() { test_blend_with_mask_001(); });
      this->register_test("Test string to instruction 1", [&]() { test_str_to_instruction_001(); });
      this->register_test("Test string to instruction 2", [&]() { test_str_to_instruction_002(); });
      this->register_test("Test string to instruction 3", [&]() { test_str_to_instruction_003(); });
      this->register_test("Test string to instruction 4", [&]() { test_str_to_instruction_004(); });
      this->register_test("Test string to instruction 5", [&]() { test_str_to_instruction_005(); });
    }

    void test_obtain_from_mask_001() {
      pic12f509::word_t result = pic12f509::obtain_from_mask(
        0x0F0,  // 0b 0000 1111 0000
        0x014   // 0b 0000 0001 0100
      );
      // expected 0b 0000 0000 0001
      this->assertEqual(result, (pic12f509::word_t) 0x001);
    }

    void test_obtain_from_mask_002() {
      pic12f509::word_t result = pic12f509::obtain_from_mask(
        0x01F,  // 0b 0000 0001 1111
        0x1CE   // 0b 0001 1100 1110
      );
      // expected 0b 0000 0000 1110
      this->assertEqual(result, (pic12f509::word_t) 0x00E);
    }

    void test_blend_with_mask_001() {
      pic12f509::word_t result = pic12f509::blend_with_mask(
        0x1C0,  // 0b 0001 1100 0000
        0x01F,  // 0b 0000 0001 1111
        0x00E   // 0b 0000 0000 1110
      );
      // expected 0b 0001 1100 1110
      this->assertEqual(result, (pic12f509::word_t) 0x1CE);
    }

    void test_str_to_instruction_001() {
      pic12f509::word_t instruction = pic12f509::str_to_instruction("NOP");
      this->assertEqual(instruction, (pic12f509::word_t) 0x000);
    }

    void test_str_to_instruction_002() {
      auto exception = this->assertThrow<pic12f509::CommandDoesNotExistException>([&]() {
        pic12f509::word_t instruction = pic12f509::str_to_instruction("INVALID COMMAND");
      });
      this->assertEqual<std::string>(exception.command, "INVALID");
    }

    void test_str_to_instruction_003() {
      pic12f509::word_t instruction;

      instruction = pic12f509::str_to_instruction("ADDWF 14h,W");
      this->assertEqual(instruction, (pic12f509::word_t) 0x1D4);

      instruction = pic12f509::str_to_instruction("ADDWF 14h,f");
      this->assertEqual(instruction, (pic12f509::word_t) 0x1F4);
    }

    void test_str_to_instruction_004() {
      pic12f509::word_t instruction;

      instruction = pic12f509::str_to_instruction("MOVWF 13h");
      this->assertEqual(instruction, (pic12f509::word_t) 0x033);

      instruction = pic12f509::str_to_instruction("MOVWF GPIO");
      this->assertEqual(instruction, (pic12f509::word_t) 0x026);
    }

    void test_str_to_instruction_005() {
      pic12f509::word_t instruction = pic12f509::str_to_instruction("NOP");
      this->assertEqual(instruction, (pic12f509::word_t) 0x000);
    }
};
