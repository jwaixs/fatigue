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

