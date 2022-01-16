#ifndef H_INCLUDE_CODING_GENERATION
#define H_INCLUDE_CODING_GENERATION

#include "delay-loop.cpp"

namespace coding_generation {

  int main(int argc, char * argv[]) {

    if (argc < 2) {
      std::cerr << "Required one of the following commands:\n"
        << "delay"
        << std::endl;
      return 1;
    }

    if (argv[1] == std::string("delay")) {
      return coding_generation::delay_loop::main(argc-1, &argv[1]);
    }
    std::cout << "Unknown subcommand '" << argv[1] << "'." << std::endl;
    return 1;
  }



}

#endif
