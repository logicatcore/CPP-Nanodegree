#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
// addition
#include "linux_parser.h"
#include <numeric>
#include <cmath>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid): pid_(pid) {
    command_ = LinuxParser::Command(this->pid_);
    user_ = LinuxParser::User(this->pid_);
}

int Process::Pid() { 
    return this->pid_; 
}

float Process::CpuUtilization() { 
    float NEW_AJ = LinuxParser::ActiveJiffies(this->pid_);
    std::vector<float> NEW_J = LinuxParser::Jiffies();  
    float NEW_TOTAL = accumulate(NEW_J.begin(), NEW_J.end(), 0);
    if ((OLD_TOTAL - NEW_TOTAL) == 0){
        OLD_AJ = NEW_AJ;
        return 0.0;
    }
    else {
        CPU_UTIL = (float)abs(OLD_AJ - NEW_AJ) / abs(OLD_TOTAL - NEW_TOTAL);  
        OLD_TOTAL = NEW_TOTAL;
        OLD_AJ = NEW_AJ;
        return CPU_UTIL;
    }
}

string Process::Command() { 
    return this->command_; 
}

std::string Process::Ram() { 
    return LinuxParser::Ram(this->pid_); 
}

string Process::User() { 
    return this->user_; 
}

long int Process::UpTime() { 
    return LinuxParser::UpTime(this->pid_); 
}
bool Process::operator<(Process &a) { 
    return  a.CpuUtilization() < this->CpuUtilization(); 
}