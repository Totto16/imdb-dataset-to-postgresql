
#pragma once

#include <string>
#include <vector>

using namespace expected;

typedef tl::expected<vector<int>, string> ParseResult

ParseResult parse_data(string path);

