#include "testcase.hpp"

namespace test_case {

  TestCase::TestCase() {
    this->tests = new std::map<const std::string, Test>();
  }

  TestCase::~TestCase() {
    delete this->tests;
  }

  TestCase const * TestCase::assert(bool const & value) const {
    if (!value) {
      throw AssertionException("Expected value to be true.");
    }
    return this;
  }

  TestCase * TestCase::register_test(std::string const & name, Test test) {
    this->tests->insert(std::pair<std::string const, Test>(name, test));
    return this;
  }

  int TestCase::run() const {
    int error = 0;
    auto * failed_tests = new std::map<const std::string, AssertionException>();

    std::for_each(this->tests->begin(), this->tests->end(), [&](auto it) {
      try {
        it.second();
        std::cerr << "[P] " << it.first << std::endl;
      } catch (AssertionException & e) {
        failed_tests->insert(
          std::pair<std::string const, AssertionException>(it.first, e)
        );
        std::cerr << "[F] " << it.first << " \"" << e.what() << "\""<< std::endl;
        error=1;
      }
    });

    if (failed_tests->size()) {
      std::for_each(failed_tests->begin(), failed_tests->end(), [&](auto it) {});
    }
    delete failed_tests;

    return error;
  }
}
