

#include <expected.hpp>
#include <nan.h>
#include <string>
#include <vector>

#include "ParserStructure.hpp"
#include "types.hpp"

using namespace std;
using namespace v8;

ParserStructure::ParserStructure(
    vector<string> order,
    vector<function<Local<Value>(const string &str)>> declarations)
    : m_order(order), m_declarations(declarations) {}