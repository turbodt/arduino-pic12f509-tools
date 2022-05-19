#ifndef H_INCLUDE_TEST_CASE
#define H_INCLUDE_TEST_CASE

#include <iostream>
#include <algorithm>
#include <functional>
#include <exception>
#include <map>

namespace test_case {

  typedef std::function<void()> Test;

  class AssertionException : public std::exception {
    protected:
      std::string message;
    public:
      explicit AssertionException() {};
      explicit AssertionException(const std::string& message) : message(message) {};
      virtual const char* what() const noexcept {
        return this->message.c_str();
      }
  };

  class TestCase {

    protected:
      std::map<const std::string, Test> * tests;
      std::vector<TestCase const *> * test_cases;

      TestCase * include_tests(TestCase const *);
      TestCase * include(TestCase const *);

      TestCase const * assert(bool const &) const;
      template<typename T>
      TestCase const * assertEqual(T const &, T const &) const;
      template<typename T>
      T assertThrow(std::function<void()>) const;

    public:
      TestCase();
      ~TestCase();
      int run() const;

      TestCase * register_test(std::string const &, Test);

  };
}

namespace test_case {

  template<typename T>
  TestCase const * TestCase::assertEqual(T const & a, T const & b) const {
    if (a != b) {
      throw AssertionException("Expected values to be equal.");
    }
    return this;
  }

  template<typename T>
  T TestCase::assertThrow(std::function<void()> f) const {
    try {
      f();
    } catch (T & e) {
      return e;
    }
    throw AssertionException("Expression did not thrown expected exception.");
  }
}

#endif
