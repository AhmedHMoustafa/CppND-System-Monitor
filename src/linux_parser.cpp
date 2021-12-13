#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string line1,line2;
  string key1,key2;
  long memT, memF;

  if(stream.is_open()){
    std::getline(stream, line1);
    std::istringstream linestream1(line1);
    linestream1 >> key1 >> memT;

    std::getline(stream, line2);
    std::istringstream linestream2(line2);
    linestream2 >> key2 >> memF;
  }
  return (float)(memT-memF)/(float)memT;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  float uptime;
  string line;
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    
 }
 return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  string key;
  long user,nice,sys,idle,iow,irq,sirq,steal,guest,gnice;
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> sys >> idle >> iow >> irq >> sirq >> steal >> guest >> gnice;
  }
  return user + nice + sys + idle + iow + irq + sirq + steal + guest + gnice;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  string line, buffer;
  long int totaltime, utime, stime, cutime, cstime;

  if(stream.is_open()){

    std::getline(stream, line);
    std::istringstream linestream(line);

    for(int i = 0; i < 13; i++){
      linestream >> buffer;
    }

    linestream >> utime >> stime >> cutime >> cstime;
    totaltime =  utime + stime + cutime + cstime;
  }

  return totaltime;
  }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return LinuxParser::Jiffies() - LinuxParser::IdleJiffies(); }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  string key;
  long user,nice,sys,idle;

  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> sys >> idle;
  }
  return idle;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  double percent = (double)ActiveJiffies()/(double)Jiffies();
  vector<string> cpuUtil{to_string(percent)};
  return cpuUtil; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  string key;
  int num;

  if(stream.is_open()){
    
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> num;

      if(key == "processes"){
        return num;
      }
    }  
  }
  return num;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  string key1,key2;
  int num;
  if(stream.is_open()){
    
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), '_', ' ');
      std::istringstream linestream(line);

      linestream >> key1 >> key2 >> num;
      if(key1 == "procs" && key2 == "running"){
        return num;
      }
    }  
  }
  return num;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line;
  if(stream.is_open()){
    std::getline(stream, line);
  }

  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;
  string key,ram;
  int vm;

  if(stream.is_open()){

    while (std::getline(stream, line)) {
      std::istringstream linestream(line);

      linestream >> key >> vm;
      if(key == "VmSize:"){
        ram = to_string(vm/1000);
        return ram;
      }
    }
  }
  return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;
  string key, uid;

  if(stream.is_open()){

    while (std::getline(stream, line)) {
      std::istringstream linestream(line);

      linestream >> key >> uid;
      if(key == "Uid:"){
        return uid;
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  string line,user,pswd;
  long puid;

  if(stream.is_open()){

    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      linestream >> user >> pswd >> puid;

      if(to_string(puid) == uid){
        return user;
      }
    }
  }

  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  string line, starttime;
  long int time;

  if(stream.is_open()){

    std::getline(stream, line);
    std::istringstream linestream(line);

    for(int i = 0; i < 22; i++){
      linestream >> starttime;
    }

    time =  LinuxParser::UpTime() - (double)stoi(starttime)/(double)sysconf(_SC_CLK_TCK);
  }

  return time;
}
