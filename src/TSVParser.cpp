
#include <string>
#include <vector>
#include <csv/parser.hpp>
#include <expected.hpp>

#include "TSVParser.hpp"

using namespace std;

ParseResult parse_data(std::string path) {
  auto vec = std::vector<int>{};
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(4);
  return vec;
}
