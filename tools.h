#ifndef TOOLS_H
#define TOOLS_H

#include <string>
using namespace std;

#include <boost/date_time/posix_time/posix_time.hpp>

string getCurrentTime();
boost::posix_time::ptime ptimeFromString(string const &);

string getDayOfWeek(boost::posix_time::ptime const &);
unsigned short getHourOfDay(boost::posix_time::ptime const &);
string getMonth(boost::posix_time::ptime const &);
unsigned short getWeek(boost::posix_time::ptime const &);
unsigned short getYear(boost::posix_time::ptime const &);

#endif // TOOLS_H
