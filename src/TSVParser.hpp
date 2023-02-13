
#pragma once

#include <string>
#include <vector>
#include <csv/parser.hpp>
#include <expected.hpp>

using namespace std;

typedef tl::expected<vector<int>, string> ParseResult;

ParseResult parse_data(string path);
