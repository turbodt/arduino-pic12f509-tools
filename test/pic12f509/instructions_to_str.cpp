#include <iostream>
#include "testcase.hpp"
#include "../../src/pic12f509/pic12f509.interface.hpp"

class InstructionsToStrTestCase : public TestCase::TestCase {

  public:
    InstructionsToStrTestCase() : TestCase() {
      this->register_test("Test string to instruction 1", [&]() { test_str_to_instruction_001(); });
      this->register_test("Test string to instruction 2", [&]() { test_str_to_instruction_002(); });
      this->register_test("Test string to instruction 3", [&]() { test_str_to_instruction_003(); });
      this->register_test("Test string to instruction 4", [&]() { test_str_to_instruction_004(); });
      this->register_test("Test string to instruction 5", [&]() { test_str_to_instruction_005(); });
      this->register_test("Test string to instruction 6", [&]() { test_str_to_instruction_006(); });
      this->register_test("Test string to instruction 7", [&]() { test_str_to_instruction_007(); });
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
      pic12f509::word_t instruction = pic12f509::str_to_instruction("TRIS GPIO");
      this->assertEqual(instruction, (pic12f509::word_t) 0x006);
    }

    void test_str_to_instruction_006() {
      pic12f509::word_t instruction = pic12f509::str_to_instruction("BSF 16h,2");
      this->assertEqual(instruction, (pic12f509::word_t) 0x556);
    }

    void test_str_to_instruction_007() {
      pic12f509::word_t instruction;
      instruction = pic12f509::str_to_instruction("GOTO 0x023");
      this->assertEqual(instruction, (pic12f509::word_t) 0xA23);
      instruction = pic12f509::str_to_instruction("GOTO 0x123");
      this->assertEqual(instruction, (pic12f509::word_t) 0xB23);
    }
};
