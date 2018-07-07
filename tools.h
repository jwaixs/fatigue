// Copyright 2018 Noud Aldenhoven

#ifndef TOOLS_H_
#define TOOLS_H_

#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>

std::string getCurrentTime();
boost::posix_time::ptime ptimeFromString(std::string const &);

std::string getDayOfWeek(boost::posix_time::ptime const &);
uint8_t getHourOfDay(boost::posix_time::ptime const &);
std::string getMonth(boost::posix_time::ptime const &);
uint8_t getWeek(boost::posix_time::ptime const &);
uint16_t getYear(boost::posix_time::ptime const &);

#endif  // TOOLS_H_
