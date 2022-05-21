#include <iostream>
#include "testcase.hpp"
#include "../../src/pic12f509/pic12f509.instructions.hpp"

class StrToInstructionsTestCase : public TestCase::TestCase {

  public:
    StrToInstructionsTestCase() : TestCase() {
      this->register_test("Test instruction to str 1", [&]() { test_instruction_to_str_001(); });
      this->register_test("Test instruction to str 2", [&]() { test_instruction_to_str_002(); });
      this->register_test("Test instruction to str 3", [&]() { test_instruction_to_str_003(); });
      this->register_test("Test instruction to str 4", [&]() { test_instruction_to_str_004(); });
    }

    void test_instruction_to_str_001() {
      std::string instruction_str = pic12f509::instruction_to_str(0x000);
      this->assertEqual<std::string>(instruction_str, "NOP");
    }

    void test_instruction_to_str_002() {
      std::string instruction_str = pic12f509::instruction_to_str(0x556);
      this->assertEqual<std::string>(instruction_str, "BSF 16h,2");
    }

    void test_instruction_to_str_003() {
      std::string instruction_str = pic12f509::instruction_to_str(0xA23);
      this->assertEqual<std::string>(instruction_str, "GOTO 0x23");
    }

    void test_instruction_to_str_004() {
      std::string instruction_str = pic12f509::instruction_to_str(0xB23);
      this->assertEqual<std::string>(instruction_str, "GOTO 0x123");
    }
};
