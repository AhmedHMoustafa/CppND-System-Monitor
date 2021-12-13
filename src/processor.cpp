#include "processor.h"

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    return stof(LinuxParser::CpuUtilization()[0]);
}