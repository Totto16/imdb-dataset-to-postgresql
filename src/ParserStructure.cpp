

#include <expected.hpp>
#include <iostream>
#include <nan.h>
#include <stdexcept>
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

  // TODO the size has to be static asserted and the record.content size
  // dynamically!! (also in the original, the size has to be validated!)
  if (m_order.size() != m_declarations.size() ||
      m_order.size() != record.content.size()) {
    std::cout << "order: " << m_order.size()
              << " - m_declarations: " << m_declarations.size()
              << " - record: " << record.content.size() << "\n";

    for (const auto &field : record.content) {
      std::cout << field.content << '\n';
    }

    throw length_error("all three needed vectors have to be the same length");
  }

  size_t i = 0;
  for (const auto &field : record.content) {
    const auto &str = field.content;
    values.emplace_back(m_order.at(i), m_declarations.at(i)(str));
    ++i;
  }

  return values;
}