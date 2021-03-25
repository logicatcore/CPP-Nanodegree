#include <string>

#include "format.h"

// addition
#include <cmath>

using std::string;

string Format::ElapsedTime(long seconds) { 
    char buff[10];
    sprintf(buff, "%02.0f:%02.0f:%02.0f", floor(fmod(seconds,86400.0)/3600.0), floor(fmod(fmod(seconds,86400.0), 3600.0)/60.0), fmod(fmod(fmod(seconds,86400.0), 3600.0), 60.0));
    return buff; 
}