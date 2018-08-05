// Copyright 2018 Noud Aldenhoven
//
// Use non approved c++11 libraries and don't make localtime thread safe,
// because we're not planning to make the fatigue tool multithreaded. If it
// will happen this part needs to be rewritten.
//
// cpplint: --filter=-build/c++11,-runtime/threadsafe_fn

#include <chrono>
#include <iomanip>
#include <sstream>

#include "./tools.h"

std::string getCurrentTime() {
  /* I have no idea why it is so difficult to ask for the local time. */
  time_t t = time(nullptr);
  tm tm = *localtime(&t);

  std::stringstream sstime;
  sstime << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

  return sstime.str();
}

boost::posix_time::ptime ptimeFromString(std::string const &stime) {
  boost::posix_time::ptime t(boost::posix_time::time_from_string(stime));
  return t;
}

std::string getDayOfWeek(boost::posix_time::ptime const &ptime) {
  auto const day_of_week = ptime.date().day_of_week().as_short_string();
  return std::string(day_of_week);
}

std::string getMonth(boost::posix_time::ptime const &ptime) {
  auto const month = ptime.date().month().as_short_string();
  return std::string(month);
}

uint8_t getHourOfDay(boost::posix_time::ptime const &ptime) {
  return ptime.time_of_day().hours();
}

uint8_t getWeek(boost::posix_time::ptime const &ptime) {
  return ptime.date().week_number();
}

uint16_t getYear(boost::posix_time::ptime const &ptime) {
  return ptime.date().year();
}
