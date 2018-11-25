// Copyright 2018 Noud Aldenhoven

#include <sqlite3.h>

#include <iostream>
#include <string>

#include <boost/filesystem.hpp>

#include "./sqlite3_helper.h"

SQLite3::SQLite3(const boost::filesystem::path &sql_path) {
  if (sqlite3_open(sql_path.c_str(), &db)) {
    std::cerr << "Failed to open database " << sql_path.string() << "."
              << std::endl;
  }
}

SQLite3::~SQLite3() {
  if (stmt) {
    sqlite3_finalize(stmt);
  }
  sqlite3_close(db);
}

void SQLite3::execute(const std::string &query) { query_str += query; }

void SQLite3::commit() {
  const auto rc = sqlite3_exec(db, query_str.c_str(), NULL, 0, &error_message);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << error_message << "." << std::endl;
    sqlite3_free(error_message);
  }
  query_str.clear();
}
