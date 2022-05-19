#include "testcase.hpp"

namespace test_case {

  TestCase::TestCase() {
    this->tests = new std::map<const std::string, Test>();
    this->test_cases = new std::vector<TestCase const *>();
  }

  TestCase::~TestCase() {
    delete this->tests;
    std::for_each(
      this->test_cases->begin(),
      this->test_cases->end(),
      [&](auto test_case) {
        delete test_case;
      }
    );
    delete this->test_cases;
  }

  TestCase * TestCase::include_tests(TestCase const * test_case) {
    std::for_each(
      test_case->tests->begin(),
      test_case->tests->end(),
      [&](auto it) {
        this->register_test(it.first, it.second);
      }
    );
    return this;
  }

  TestCase * TestCase::include(TestCase const * test_case) {
    this->test_cases->push_back(test_case);
    return this;
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

    std::for_each(
      this->test_cases->begin(),
      this->test_cases->end(),
      [&](auto test_case) {
        if (test_case->run()) {
          error = 1;
        }
      }
    );

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
      } catch (std::exception & e) {
        std::cerr << "!!! " << it.first << " thrown exception \"" << e.what() << "\""<< std::endl;
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
