
#pragma once

#include "helper/expected.hpp"

#include <csv/parser.hpp>
#include <functional>
#include <string>
#include <vector>

#include "InternalParsers.hpp"
#include "helper/postgres.hpp"

template <typename T>
using AdvancedParserFunction =
    std::function<void(T &value, const std::string &input)>;

template <typename T>
using AdvancedParser = std::pair<std::string, AdvancedParserFunction<T>>;

#include "postgres.hpp"
#include <csv/parser.hpp>

struct Parseable {

  virtual void setup_prepared_statement(postgres::Connection &connection) = 0;

  virtual void insert_record(postgres::Connection &connection,
                             const csv::record &record) = 0;

  [[nodiscard]] virtual const std::vector<std::string> names() const = 0;
};

template <typename T> class ParserStructure : public Parseable {
private:
  std::vector<AdvancedParser<T>> m_parser;
  std::string m_prepared_command_name;

public:
  ParserStructure(std::vector<AdvancedParser<T>> &&parser)
      : m_parser{std::move(parser)} {

    // TODO:REPLACE . with _
    m_prepared_command_name =
        postgres::Statement<T>::table() + "_prepared_insert";
  }

  virtual ~ParserStructure() = default;

  void setup_prepared_statement(postgres::Connection &connection) override {

    connection.exec(postgres::PrepareData{
        m_prepared_command_name, postgres::Statement<T>::insert(),
        helper::PreparedStatement<T>::types()});
  }

  void insert_record(postgres::Connection &connection,
                     const csv::record &record) override {
    T value = {};

    // dynamically!! (also in the original, the size has to be validated!)
    if (m_parser.size() != record.content.size()) {

      throw std::length_error("parsers and records have to be the same size");
    }

    size_t i = 0;
    for (const auto &field : record.content) {
      m_parser.at(i).second(value, field.content);
      ++i;
    }

    // TODO: RangeStatement and Statement
    (void)connection;
    /*  connection.exec(postgres::PreparedCommand{
         m_prepared_command_name, postgres::Statement<T>::fields()}); */
  }

  [[nodiscard]] virtual const std::vector<std::string> names() const override {
    std::vector<std::string> vec = {};
    vec.reserve(m_parser.size());
    for (const auto &[name, _] : m_parser) {
      vec.emplace_back(name);
    }

    return vec;
  }
};
