#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
#include <unistd.h>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int);
  int Pid() const;                         
  std::string User();                      
  std::string Command();                   
  float CpuUtilization() const;            
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  


 private:
  int pid_;
};

#endif