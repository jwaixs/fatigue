#include <chrono>
#include <sstream>
#include <iomanip>


#include "tools.h"

string getCurrentTime() {
    /* I have no idea why it is so difficult to ask for the local time. */
    time_t t = time(nullptr);
    tm tm = *localtime(&t);

    stringstream sstime;
    sstime << put_time(&tm, "%Y-%m-%d %H:%M:%S");

    return sstime.str();
}

boost::posix_time::ptime ptimeFromString(string const &stime) {
    boost::posix_time::ptime t(boost::posix_time::time_from_string(stime));
    return t;
}

string getDayOfWeek(boost::posix_time::ptime const &ptime) {
    auto const day_of_week = ptime.date().day_of_week().as_short_string();
    return string(day_of_week);
}

string getMonth(boost::posix_time::ptime const &ptime) {
    auto const month = ptime.date().month().as_short_string();
    return string(month);
}

unsigned short getHourOfDay(boost::posix_time::ptime const &ptime) {
    return ptime.time_of_day().hours();
}

unsigned short getWeek(boost::posix_time::ptime const &ptime) {
    return ptime.date().week_number();
}

unsigned short getYear(boost::posix_time::ptime const &ptime) {
    return ptime.date().year();
}
