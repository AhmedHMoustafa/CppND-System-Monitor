#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor initializing PID
Process::Process(int p) :  pid_{p} {}

// Return this process's ID
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const {
    long int totaltime = LinuxParser::ActiveJiffies(Pid());
    long seconds = LinuxParser::UpTime();
    float percent = (((double)totaltime/(double)sysconf(_SC_CLK_TCK))/seconds);
    return percent;
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid());  }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// TOverload the "less than" comparison operator for Process objects
// Compares according to CPU utilization
bool Process::operator<(Process const& a) const {

    if(a.CpuUtilization() < CpuUtilization()){
     return true;
    } else{
        return false;
    }
    
}

