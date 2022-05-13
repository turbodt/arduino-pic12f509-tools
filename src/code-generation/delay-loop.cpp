#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include "../shared/main.hpp"
#include "../pic12f509/pic12f509.utils.hpp"

namespace code_generation {
namespace delay_loop {

  typedef unsigned long long int ullint;

  struct delay_seq_info {
    uint16_t value;
    uint8_t extra_ops;
    uint8_t accumulated_extra_ops;
    uint8_t deep;
    bool is_null = false;
  };

  delay_seq_info null_seq{0,0,0,0, true};

  bool is_null(const delay_seq_info & a) {
    return a.is_null;
  }

  uint16_t num_instructions(const delay_seq_info & a) {
    return a.accumulated_extra_ops + 4 * a.deep;
  }

  bool is_similar_to(const delay_seq_info & a, const delay_seq_info & b) {
    return a.deep == b.deep && a.accumulated_extra_ops == b.accumulated_extra_ops;
  }

  bool is_better_than(const delay_seq_info & a, const delay_seq_info & b) {
    if (is_null(a)) {
      return false;
    } else if (is_null(b)) {
      return true;
    }
    if (num_instructions(a) < num_instructions(b)) {
      return true;
    } else if (num_instructions(b) < num_instructions(a)) {
      return false;
    }
    if (a.deep < b.deep) {
      return true;
    } else if (b.deep < a.deep) {
      return false;
    }
    if (a.accumulated_extra_ops < b.accumulated_extra_ops) {
      return true;
    } else if (b.accumulated_extra_ops < a.accumulated_extra_ops) {
      return false;
    }
    return false;
  }

  bool is_better_or_equal_to(const delay_seq_info & a, const delay_seq_info & b) {
    return is_similar_to(a, b) || is_better_than(a, b);
  }

  std::vector<uint16_t> primes_until_256{
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
    157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
    239, 241, 251
  };

  std::vector<uint16_t> get_prime_factors_until_256(ullint n) {
    std::vector<uint16_t> v = std::vector<uint16_t>();
    for (uint8_t i = 0 ; i < primes_until_256.size() && primes_until_256[i] <= n; i++) {
      uint16_t p = primes_until_256[i];
      while ( n % p == 0) {
        v.push_back(p);
        n /= p;
      }
    }
    return v;
  }

  std::set<uint16_t> get_factors_until(
    std::vector<uint16_t>::const_iterator begin,
    std::vector<uint16_t>::const_iterator end,
    uint16_t const & until
  ) {
    std::set<uint16_t> factors{1};
    for (; begin != end && *begin <= until; begin++) {
      uint16_t new_until = until / (*begin);
      if (new_until) {
        std::set<uint16_t> new_factors = get_factors_until(begin+1, end, new_until);
        for (
          std::set<uint16_t>::const_iterator it = new_factors.begin();
          it != new_factors.end();
          it++
        ) {
          factors.insert((*begin) * (*it));
        }
      }
    }
    return factors;
  }

  std::vector<uint16_t> get_factors_until_256(const ullint & n) {
    std::vector<uint16_t> primes = get_prime_factors_until_256(n);
    std::set<uint16_t> factors = get_factors_until(primes.begin(),primes.end(),256);
    return std::vector<uint16_t>(factors.begin(), factors.end());
  }

  std::map<ullint, const delay_seq_info> dyn_delay_mapping = std::map<ullint, const delay_seq_info>();

  std::string repeat_str(const std::string & str, unsigned int n) {
    std::string new_str = "";
    for (;n>0;n--) {
      new_str += str;
    }
    return new_str;
  }

  delay_seq_info const generate_delay(const ullint & num_operations, unsigned int rec_lvl = 0) {
    if (rec_lvl > 4) {
      return null_seq;
    }
//DEBUG:    std::cerr << repeat_str("  ", rec_lvl)
//DEBUG:      << "(" << dyn_delay_mapping.size() << ")"
//DEBUG:      << " Case " << num_operations << std::endl;
    if (dyn_delay_mapping.find(num_operations) != dyn_delay_mapping.end()) {
      return dyn_delay_mapping.at(num_operations);
    }
    if (num_operations <= 4) {
      dyn_delay_mapping.erase(num_operations);
      dyn_delay_mapping.insert(
        std::pair<ullint, const delay_seq_info>(
          num_operations,
          {0,(uint8_t)num_operations,(uint8_t)num_operations,0}
        )
      );
    } else {
      dyn_delay_mapping.erase(num_operations);
      dyn_delay_mapping.insert(
        std::pair<ullint, const delay_seq_info>(num_operations, null_seq)
      );
//DEBUG:      std::cerr << std::endl << std::endl;
//DEBUG:      std::cerr << num_operations << " = 1 + ( 3 + d_{n-1}) X_n + A_n" << std::endl;
      for (uint8_t extra_ops = 0 ; extra_ops < 4 ; extra_ops++) {
        ullint n = num_operations -1 -extra_ops;
//DEBUG:        std::cerr << "Choose A_n = " << (int) extra_ops << std::endl;
//DEBUG:        std::cerr << num_operations
//DEBUG:          << " -" << (int)extra_ops << " -1"
//DEBUG:          << " = " << n << " = "
//DEBUG:          << "( 3 + d_{n-1} ) X_n"
//DEBUG:          << std::endl;
        const std::vector<uint16_t> factors = get_factors_until_256(n);
        for (uint16_t factor : factors) {
          // keep in mind the case
          if (3*factor <= n) {
//DEBUG:            std::cerr << "Choose X_n = " << (int) factor << std::endl;
//DEBUG:            std::cerr << n << " = "
//DEBUG:              << n/factor << " * " << (int)factor << " = "
//DEBUG:              << "( 3 + d_{n-1} ) * " << (int)factor
//DEBUG:              << std::endl;
            auto current_best_seq = dyn_delay_mapping.at(num_operations);
            delay_seq_info previous_seq = generate_delay(n/factor -3, rec_lvl + 1);
            if (is_null(previous_seq)) {
              continue;
            }
            delay_seq_info candidate_seq;
            candidate_seq.value = factor;
            candidate_seq.extra_ops = extra_ops;
            candidate_seq.accumulated_extra_ops = extra_ops + previous_seq.accumulated_extra_ops;
            candidate_seq.deep = previous_seq.deep +1;

            if (is_better_than(candidate_seq, current_best_seq)) {
              dyn_delay_mapping.erase(num_operations);
              dyn_delay_mapping.insert(
                std::pair<ullint, const delay_seq_info>(
                  num_operations,
                  candidate_seq
                )
              );
            }
          }
        }
      }
    }
//DEBUG:    std::cerr << repeat_str("  ", rec_lvl);
//DEBUG:    if (is_null(dyn_delay_mapping.at(num_operations))) {
//DEBUG:      std::cerr << "is null";
//DEBUG:    } else {
//DEBUG:      auto seq = dyn_delay_mapping.at(num_operations);
//DEBUG:      std::cerr << "X_n = " << (int)seq.value
//DEBUG:        << "\tA_n = " << (int)seq.extra_ops
//DEBUG:        << "\tS(A_n) = " << (int)seq.accumulated_extra_ops
//DEBUG:        << "\tn = " << (int)seq.deep;
//DEBUG:    }
//DEBUG:    std::cerr << std::endl;
    return dyn_delay_mapping.at(num_operations);
  }


  void generate_code(
      std::string const & label,
      pic12f509::addr_t const & file_addr,
      std::vector<std::string> & code,
      const ullint & num_operations,
      uint8_t rec_lvl = 0
  ) {
    std::string t = repeat_str("  ", rec_lvl);

    if (rec_lvl == 0) {
      delay_seq_info const seq = generate_delay(num_operations - 4);
      std::string used_file_addresses = pic12f509::file_addr_to_str(file_addr);
      for (pic12f509::addr_t i = 1 ; i < seq.deep; i++) {
        used_file_addresses += ", " + pic12f509::file_addr_to_str(file_addr + i);
      }
      code.push_back(t + label);
      code.push_back(t + "  ;  To call and to return from this subroutine takes");
      code.push_back(t + "  ;  takes exactly " + std::to_string(num_operations) + " operations in " + std::to_string(num_instructions(seq) + 1) + " instructions.");
      code.push_back(t + "  ;  Uses file addresses: " + used_file_addresses);
      code.push_back("");
      code.push_back(t + "  ; 2 operations from CALL");
      code.push_back("");
      generate_code(label, file_addr, code, num_operations - 4, rec_lvl+1);
      code.push_back("");
      code.push_back(t + "  ; 2 operations from RETLW");
      code.push_back(t + "  RETLW 0X00");
      return;
    }

    std::string label_deep = label + "_deep_" + std::to_string(rec_lvl);

    delay_seq_info const seq = generate_delay(num_operations);
    if (is_null(seq)) {
      code.clear();
      code.push_back("; is imposible to generate that delay");
    }

    if (seq.deep > 0) {
      code.push_back(t + "MOVLW d'" + std::to_string(seq.value) + "'");
      code.push_back(t + "MOVWF " + pic12f509::file_addr_to_str(file_addr));
    }
    for (uint8_t i = 0 ; i < seq.extra_ops; i++) {
      code.push_back(t + "NOP");
    }
    if (seq.deep > 0) {
      code.push_back(t + label_deep);
      ullint prev_num_operations = (num_operations -1 -seq.extra_ops) /seq.value -3;
      generate_code(label, file_addr + 1, code, prev_num_operations, rec_lvl+1);
      code.push_back(t + "  DECFSZ " + pic12f509::file_addr_to_str(file_addr) + ",f");
      code.push_back(t + "  GOTO " + label_deep);
    }
  }

  void print_options() {
    std::cerr << "Options:\n"
      << "  -l, --label        Defines the name of the subroutine.\n"
      << "  -f, --address      Defines the starting file address which counters\n"
      << "                     of the delay will use.\n"
      << std::endl;
  }

  int main(int argc, char * argv[]) {

    if (argc < 2) {
      std::cerr << "Required filename and comm: " << argv[0] << " <num_ops>" << std::endl;
      print_options();
      return 1;
    }
    ullint num_operations = std::strtoull(argv[1], nullptr, 10);
    std::string label = "delay";
    pic12f509::addr_t file_addr = 7;
    for (uint8_t i = 2 ; i < argc ; i++ ) {
      std::string flag = argv[i];
      if (flag == "-l" || flag == "--label") {
        i++;
        label = std::string(argv[i]);
      } else if (flag == "-f" || flag == "--address") {
        i++;
        file_addr = pic12f509::str_to_file_addr(argv[i]);
      } else {
        print_options();
        return 1;
      }
    }

    std::vector<std::string> code = std::vector<std::string>();
    generate_code(label, file_addr,code, num_operations);
    for (std::string line : code) {
      std::cout << line << std::endl;
    }
    return 0;
  }

}
}
