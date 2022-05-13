#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "pic12f509/pic12f509.interface.hpp"

#include "code-generation/main.hpp"
#include "serial.cpp"
#include "compile.cpp"
#include "decompile.cpp"

int main(int argc, char * argv[]) {

  if (argc < 2) {
    std::cerr << "Required one of the following commands:\n"
      << "compile, decompile, code-generation, upload"
      << std::endl;
    return 1;
  }

  if (argv[1] == std::string("decompile")) {
    return command_decompile::main(argc-1, &argv[1]);
  } else if (argv[1] == std::string("compile")) {
    return command_compile::main(argc-1, &argv[1]);
  } else if (argv[1] == std::string("code-generation")) {
    return code_generation::main(argc-1, &argv[1]);
  } else if (argv[1] == std::string("upload")) {
    return send_to_arduino(argc-1, &argv[1]);
  }

  std::cout << "Unknown command '" << argv[1] << "'." << std::endl;
  return 1;
}
