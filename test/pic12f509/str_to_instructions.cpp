#include <iostream>
#include "../testcase.hpp"
#include "../../src/pic12f509/pic12f509.instructions.hpp"

class StrToInstructionsTestCase : public test_case::TestCase {

  public:
    StrToInstructionsTestCase() : TestCase() {
      this->register_test("Test instruction to str 1", [&]() { test_instruction_to_str_001(); });
      this->register_test("Test instruction to str 2", [&]() { test_instruction_to_str_002(); });
    }

    void test_instruction_to_str_001() {
      std::string instruction_str = pic12f509::instruction_to_str(0x000);
      this->assertEqual<std::string>(instruction_str, "NOP");
    }

    void test_instruction_to_str_002() {
      std::string instruction_str = pic12f509::instruction_to_str(0x556);
      this->assertEqual<std::string>(instruction_str, "BSF 16h,2");
    }
};
