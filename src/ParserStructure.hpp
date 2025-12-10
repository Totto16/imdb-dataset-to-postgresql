
#pragma once

#include <csv/parser.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "InternalParsers.hpp"
#include "helper/postgres.hpp"
#include "postgres/Transaction.h"

template <typename T>
using AdvancedParserFunction =
    std::function<void(T &value, const std::string &input)>;

template <typename T>
using AdvancedParser = std::pair<std::string, AdvancedParserFunction<T>>;

#include <csv/parser.hpp>
#include <postgres/Postgres.h>

struct Parseable {

  virtual void setup_prepared_statement(postgres::Connection &connection) = 0;

  virtual void insert_record(postgres::Connection &connection,
                             const csv::record &record) = 0;

  [[nodiscard]] virtual const std::vector<std::string> names() const = 0;

  virtual void finish() = 0;

  inline void setTransactionSize(std::optional<std::uint32_t> transactionSize) {
    m_transaction_size =
        transactionSize.has_value() ? transactionSize.value() : 0;
  }

protected:
  std::uint32_t m_transaction_size{0};
  std::uint32_t m_execs{0};
  std::shared_ptr<postgres::Transaction> m_trans{};
};

template <typename T>
concept IsTable = postgres::internal::isVisitable<T>();

template <typename T>
  requires IsTable<T>
class ParserStructure : public Parseable {
private:
  std::vector<AdvancedParser<T>> m_parser;
  std::string m_prepared_command_name;

public:
  ParserStructure(std::vector<AdvancedParser<T>> &&parser)
      : m_parser{std::move(parser)} {

    m_prepared_command_name =
        postgres::Statement<T>::table() + "_prepared_insert";
  }

  virtual ~ParserStructure() = default;

  void setup_prepared_statement(postgres::Connection &connection) override {

    const auto result = connection.exec(postgres::PrepareData{
        m_prepared_command_name, postgres::Statement<T>::insert(),
        postgres::PreparedStatement<T>::types()});

    if (!result.isOk()) {
      throw std::runtime_error{result.message()};
    }
  }

  void insert_record(postgres::Connection &connection,
                     const csv::record &record) override {
    T value = {};

    if (m_parser.size() != record.content.size()) {
      throw std::length_error(
          "parsers and records have to be the same size, expected " +
          std::to_string(m_parser.size()) + " but got " +
          std::to_string(record.content.size()) +
          " row:" + std::to_string(record.row));
    }

    size_t i = 0;
    for (const auto &field : record.content) {
      m_parser.at(i).second(value, field.content);
      ++i;
    }

    if (m_transaction_size != 0) {
      if (m_trans == nullptr) {
        m_trans = std::make_shared<postgres::Transaction>(connection.begin());
      } else if (m_execs >= m_transaction_size) {
        auto _ = m_trans->commit();
        m_trans = std::make_shared<postgres::Transaction>(connection.begin());
      }
    }

    const auto result = connection.exec(
        postgres::PreparedCommand{m_prepared_command_name, value});
    if (!result.isOk()) {
      throw std::runtime_error{result.message()};
    }
    m_execs++;
  }

  void finish() override {
    if (m_trans != nullptr) {
      auto _ = m_trans->commit();
      m_trans = nullptr;
    }
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
