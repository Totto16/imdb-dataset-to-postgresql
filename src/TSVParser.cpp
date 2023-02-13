
#include <csv/datasource/utf8/DataSource.hpp>
#include <csv/parser.hpp>
#include <expected.hpp>
#include <iostream>
#include <nan.h>
#include <string>
#include <vector>

#include "TSVParser.hpp"

using namespace std;
using namespace v8;

template <typename T>
TSVParser<T>::TSVParser(string path, string type)
    : m_path{move(path)}, m_type{move(type)} {
  cout << "inner 1" << endl;
};

template <typename T> ParseResult TSVParser<T>::parseData() {
  cout << "test here" << endl;

  csv::utf8::FileDataSource input;

  if (!input.open(m_path.c_str())) {
    return tl::make_unexpected("Filepath was invalid: '" + m_path + "'!");
  }

  vector<int> vec = vector<int>{};
  /*   csv::parse(
        input,
        [](const csv::field &field) -> bool {
          // Do something with 'field'
          return true;
        },
        [](const csv::record &record, double progress) -> bool {
          // Do something with 'record'
          return true;
        }); */

  vec.push_back(2);

  v8::Local<v8::Array> results = Nan::New<v8::Array>(vec.size());
  int i = 0;
  for_each(vec.begin(), vec.end(), [&](int value) {
    Nan::Set(results, i, Nan::New<v8::Number>(value));
    i++;
  });

  return results;
}
