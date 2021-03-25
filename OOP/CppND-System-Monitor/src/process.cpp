#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
// addition
#include "linux_parser.h"
#include <numeric>

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

float Process::CpuUtilization() const { 
    long AJ = LinuxParser::ActiveJiffies(this->pid_);
    std::vector<float> J = LinuxParser::Jiffies();  
    long total_cpu_active_jiffies = accumulate(J.begin(), J.end(), 0);
    if (total_cpu_active_jiffies == 0){
        return 0;
    }
    else {
        return AJ / total_cpu_active_jiffies; 
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
bool Process::operator<(Process const& a) const { 
    return this->CpuUtilization() < a.CpuUtilization(); 
}