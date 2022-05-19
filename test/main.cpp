#include "pic12f509/main.cpp"

int main(int argc, char * argv[]) {
  int error = 0;
  auto instructions_test_case = new InstructionsTestCase();
  error |= instructions_test_case->run();
  return error;
}
