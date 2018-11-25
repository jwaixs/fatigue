// Copyright 2018 Noud Aldenhoven

#include <sqlite3.h>

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
