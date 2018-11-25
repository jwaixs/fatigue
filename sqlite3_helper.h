// Copyright 2018 Noud Aldenhoven

#ifndef SQLITE3_HELPER_H_
#define SQLITE3_HELPER_H_

#include <sqlite3.h>

#include <string>

#include <boost/filesystem.hpp>

class SQLite3 {
 public:
  explicit SQLite3(const boost::filesystem::path &);
  ~SQLite3();
  void execute(const std::string &);
  void commit();

 private:
  sqlite3 *db;
  char *error_message = 0;
  sqlite3_stmt *stmt = NULL;
  std::string query_str;
};

#endif  // SQLITE3_HELPER_H_
