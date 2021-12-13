#include <string>

#include "format.h"

using std::string;
using std::to_string;

// Helper function to format total seconds to HH:MM:SS time format
string Format::ElapsedTime(long seconds) {
    int h,m,s;
    string HH,MM,SS;
    h = seconds/3600;
    m = (seconds-(h*3600))/60;
    s = seconds - (h*3600) - (m*60);
    HH = (h < 10) ? "0" + to_string(h): to_string(h);
    MM = (m < 10) ? "0" + to_string(m): to_string(m);
    SS = (s < 10) ? "0" + to_string(s): to_string(s);
    return HH + ":" + MM + ":" + SS;
}