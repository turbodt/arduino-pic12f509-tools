#include <iostream>
#include "instructions_to_str.cpp"
#include "str_to_instructions.cpp"
#include "testcase.hpp"
#include "../../src/pic12f509/pic12f509.instructions.hpp"

class InstructionsTestCase : public TestCase::TestCase {

  public:
    InstructionsTestCase() : TestCase() {
      this->include(new InstructionsToStrTestCase());
      this->include(new StrToInstructionsTestCase());

      this->register_test("Test obtain from mask 1", [&]() { test_obtain_from_mask_001(); });
      this->register_test("Test obtain from mask 2", [&]() { test_obtain_from_mask_002(); });
      this->register_test("Test blend with mask 1", [&]() { test_blend_with_mask_001(); });
      this->register_test("Test blend with mask 2", [&]() { test_blend_with_mask_002(); });
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

    void test_blend_with_mask_002() {
      pic12f509::word_t result = pic12f509::blend_with_mask(
        0x006,  // 0b 0000 0000 0110
        0x007,  // 0b 0000 0000 0111
        0x006   // 0b 0000 0000 0110
      );
      // expected 0b 0000 0000 0110
      this->assertEqual(result, (pic12f509::word_t) 0x006);
    }
};
