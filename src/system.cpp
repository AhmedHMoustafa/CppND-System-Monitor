#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 

    // Scan the processes
    vector<int> pids = LinuxParser::Pids();
    int size = pids.size();

    // Refresh the processes vector with the current ones
    processes_.clear();
    for(int i = 0; i < size; i++){
        Process a(pids[i]);
        processes_.push_back(a);
    }

    // Sort processes according to CPU utilization
    sort(processes_.begin(), processes_.end());

    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
    
    if(System::kernel_.empty()){
        System::kernel_ = LinuxParser::Kernel();
    }
    return System::kernel_; 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { 

    if(System::OS_.empty()){
        System::OS_ = LinuxParser::OperatingSystem();
    }
    return System::OS_; 
}    

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
