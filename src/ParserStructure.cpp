

#include <expected.hpp>
#include <nan.h>
#include <string>
#include <vector>

#include "InternalParsers.hpp"
#include "ParserStructure.hpp"
#include "constructable.hpp"
#include "types.hpp"

using namespace std;
using namespace v8;

ParserStructure::ParserStructure(vector<string> order,
                                 vector<ParserFunction> declarations)
    : m_order(order), m_declarations(declarations) {}

ObjectValues ParserStructure::parse(const csv::record &record) {
  ObjectValues values = ObjectValues{};
  // TODO: here
  return values;
}