#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include "../shared/main.hpp"
#include "../pic12f509/pic12f509.utils.hpp"

namespace coding_generation {
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

  bool is_similar_to(const delay_seq_info & a, const delay_seq_info & b) {
    return a.deep == b.deep && a.accumulated_extra_ops == b.accumulated_extra_ops;
  }

  bool is_better_than(const delay_seq_info & a, const delay_seq_info & b) {
    if (is_null(a)) {
      return false;
    } else if (is_null(b)) {
      return true;
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
      std::vector<std::string> & code,
      const ullint & num_operations,
      uint8_t rec_lvl = 0
  ) {
    std::string t = repeat_str("  ", rec_lvl);
    delay_seq_info const seq = generate_delay(num_operations);
    if (is_null(seq)) {
      code.clear();
      code.push_back("; is imposible to generate that delay");
    }
//DEBUG:    std::cerr << "X_n = " << (int)seq.value
//DEBUG:      << "\tA_n = " << (int)seq.extra_ops
//DEBUG:      << "\tS(A_n) = " << (int)seq.accumulated_extra_ops
//DEBUG:      << "\tn = " << (int)seq.deep
//DEBUG:      << std::endl;
    if (seq.deep > 0) {
      code.push_back(t + "movlw " + pic12f509::word_to_str(seq.value));
      code.push_back(t + "movfw " + pic12f509::file_addr_to_str(7 + rec_lvl));
    }
    for (uint8_t i = 0 ; i < seq.extra_ops; i++) {
      code.push_back(t + "nop");
    }
    if (seq.deep > 0) {
      code.push_back(t + "deep_" + std::to_string(rec_lvl));
      ullint prev_num_operations = (num_operations -1 -seq.extra_ops) /seq.value -3;
      generate_code(code, prev_num_operations, rec_lvl+1);
      code.push_back(t + "  decfsz " + pic12f509::file_addr_to_str(7 + rec_lvl) + ",f");
      code.push_back(t + "  goto deep_" + std::to_string(rec_lvl));
    }
  }

  int main(int argc, char * argv[]) {

    if (argc < 2) {
      std::cerr << "Required filename and comm: " << argv[0] << " <num_ops>" << std::endl;
      return 1;
    }
    ullint num_operations = std::strtoull(argv[1], nullptr, 10);
    std::cout << "; subrutine a delay using " << num_operations
      << " operations" << std::endl;

    std::vector<std::string> code = std::vector<std::string>();
    generate_code(code, num_operations);
    for (std::string line : code) {
      std::cout << line << std::endl;
    }
    return 0;
  }

}
}
