#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"

//addition
#include "linux_parser.h"

using std::set;
using std::string;
using std::vector;

Processor& System::Cpu() { 
    return cpu_; 
}

int System::Cores() { 
    return LinuxParser::TotalCores(); 
}

vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids();
    processes_.clear();
    for (int &entry: pids) {
        this->processes_.emplace_back(Process(entry));
    }
    sort(processes_.begin(), processes_.end());
    return this->processes_; 
}

std::string System::Kernel() { 
    return LinuxParser::Kernel(); 
}

float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization(); 
}

std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem(); 
}

int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

long int System::UpTime() { 
    return LinuxParser::UpTime(); 
}