cmake_minimum_required(VERSION 3.12)

project(pic12f509 C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(
  main
  src/main.cpp
  src/pic12f509/pic12f509.utils.cpp
  src/pic12f509/pic12f509.instructions.cpp
  src/serial-comm/serial-comm.cpp
)

add_executable(
  test
  test/main.cpp
  test/testcase.cpp
  src/pic12f509/pic12f509.utils.cpp
  src/pic12f509/pic12f509.instructions.cpp
  src/serial-comm/serial-comm.cpp
)
