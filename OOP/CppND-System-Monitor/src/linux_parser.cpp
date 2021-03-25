#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

// Additions
#include <boost/filesystem.hpp>
#include <unordered_map>
#include <unistd.h>

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
/*
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
*/

// Port to boost filesystem
string get_stem(const boost::filesystem::path &p) { 
  return (p.stem().string()); 
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  string filename;
  // create a directories iterator
  boost::filesystem::directory_iterator proc_iterator(kProcDirectory.c_str());
  // iterate through directories ONLY!
  for(const auto& d: proc_iterator){
    // Is every character of the name a digit?
    filename = get_stem(d.path());
    if (std::all_of(filename.begin(), filename.end(), isdigit)) {
      int pid = stoi(filename);
      pids.push_back(pid);
    }
  }
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  std::unordered_map<string, float> mem_util;
  string line, key, unit;
  float value{0};
  std::ifstream meminfo(kProcDirectory+kMeminfoFilename);
  
  if (meminfo.is_open()){
      while (std::getline(meminfo, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        for (int i = 0; i < 4 ; i++){
          linestream >> key >> value >> unit;
          mem_util[key] = value;
        }
    } 
  }
  return (mem_util["MemTotal"] - mem_util["MemFree"]) / mem_util["MemTotal"];;
}

long LinuxParser::UpTime() { 
  std::ifstream uptime_info(kProcDirectory+kUptimeFilename);
  long active{0}, active_and_idle{0};
  string line;

  if (uptime_info.is_open()){
    std::getline(uptime_info, line);
    std::istringstream linestream(line);
    linestream >> active >> active_and_idle;
  }
  return active; 
}

std::vector<float> LinuxParser::Jiffies() { 
  std::vector<float> AJ = LinuxParser::ActiveJiffies();
  std::vector<float> IJ = LinuxParser::IdleJiffies();
  std::vector<float> SUM;
  for (size_t i = 0; i < AJ.size(); i++) {
    SUM.emplace_back(AJ[i] + IJ[i]);
  }
  return SUM; 
}

long LinuxParser::ActiveJiffies(int pid) { 
  string pid_s = "/" + std::to_string(pid);
  std::ifstream pid_info(kProcDirectory + pid_s + kStatFilename);
  string line, block ;
  long sum{0};
  short count{0};

  if (pid_info.is_open()){
    std::getline(pid_info, line);
    std::istringstream linestream(line);
    while (count < 15) {
      linestream >> block;
      if (std::all_of(block.begin(), block.end(), isdigit) || block == "-1") {
        count++;
      }
      if (count >= 12){
        sum += stol(block);
      }
    }
  }
  return sum; 
}

std::vector<float> LinuxParser::ActiveJiffies() { 
  std::vector<float> sum;
  long tmp{0};
  short index{0};
  std::vector<std::vector<string>> CPUs = LinuxParser::CpuUtilization();
  // user nice system idle iowait irq softirq steal guest guest_nice
  //   0    1     2     3    4     5     6      7     8       9
  // NonIdle = user + nice + system + irq + softirq + steal
  for (std::vector<string> &aCPU: CPUs) {
    index = 0;
    for (string &jiffies: aCPU) {
      if (index != 3 && index != 4 && index < 8){
        tmp += stof(jiffies);
      }
      index++;
    }
    sum.emplace_back(tmp);
  }
  return sum; 
}

std::vector<float> LinuxParser::IdleJiffies() { 
  std::vector<float> sum;
  long tmp{0};
  short index{0};
  std::vector<std::vector<string>> CPUs = LinuxParser::CpuUtilization();
  // user nice system idle iowait irq softirq steal guest guest_nice
  //   0    1     2     3    4     5     6      7     8       9
  // Idle = idle + iowait
  for (std::vector<string> &aCPU: CPUs) {
    index = 0;
    for (string &jiffies: aCPU) {
      if (index == 3 || index == 4){
        tmp += stof(jiffies);
      }
      index++;
    }
    sum.emplace_back(tmp);
  }
  return sum;  
}

vector<vector<string>> LinuxParser::CpuUtilization() { 
  vector<vector<string>> CPUs; // aggregate CPU usage
  vector<string> tmp;
  std::ifstream cpu_info(kProcDirectory + kStatFilename);
  string line, value;

  if (cpu_info.is_open()){
    while (std::getline(cpu_info, line)) {
      tmp.clear();
      std::istringstream linestream(line);
      linestream >> value;
      if (value.size() > 3 && value.substr(0, 3) == "cpu") {
        while (linestream >> value) { 
          tmp.push_back(value);
        }
        CPUs.emplace_back(tmp);
      }
      if (value == "intr") {
        break;
      }
    }
  }
  return CPUs;
}

int LinuxParser::TotalProcesses() { 
  std::ifstream cpu_info(kProcDirectory + kStatFilename);
  string line, key;
  int processes{0};

  if (cpu_info.is_open()){
    while (std::getline(cpu_info, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "processes") {
        linestream >> processes;
        return processes;
      }
    }
  }
  return processes; 
}

int LinuxParser::RunningProcesses() { 
  std::ifstream cpu_info(kProcDirectory + kStatFilename);
  string line, key;
  int processes{0};

  if (cpu_info.is_open()){
    while (std::getline(cpu_info, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running") {
        linestream >> processes;
        return processes;
      }
    }
  }
  return processes; 
}

int LinuxParser::TotalCores() { 
  std::ifstream cpu_info(kProcDirectory + kCpuinfoFilename);
  string line, key;
  int cores{0};

  if (cpu_info.is_open()){
    while (std::getline(cpu_info, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu") {
        linestream >> key;
        if (key == "cores") {
          linestream >> key >> cores;
          break;
        }
      }
    }
  }
  return cores; 
}

string LinuxParser::Command(int pid) { 
  string pid_s = "/" + std::to_string(pid);
  std::ifstream pid_info(kProcDirectory + pid_s + kCmdlineFilename);
  string line;

  if (pid_info.is_open()){
    std::getline(pid_info, line);
    std::istringstream linestream(line);
    return linestream.str(); 
  }

  return "";
}

string LinuxParser::Ram(int pid) { 
  string pid_s = "/" + std::to_string(pid);
  std::ifstream pid_info(kProcDirectory + pid_s + kStatusFilename);
  string line, _ ;
  float vmsize;

  if (pid_info.is_open()){
    // throw away 17 lines
    for (int i = 0; i < 17; i++){
        std::getline(pid_info, line);
    }
    std::getline(pid_info, line);
    std::istringstream linestream(line);
    linestream >> _ >> vmsize >> _ ;
  }
  vmsize /= 1024;
  // kB to MB
  return to_string(vmsize); 
}

string LinuxParser::Uid(int pid) { 
  string pid_s = "/" + std::to_string(pid);
  std::ifstream pid_info(kProcDirectory + pid_s + kStatusFilename);
  string line, uid_no, _ ;

  if (pid_info.is_open()){
    // throw away 8 lines
    for (int i = 0; i < 8; i++){
        std::getline(pid_info, line);
    }
    std::getline(pid_info, line);
    std::istringstream linestream(line);
    linestream >> _ >> uid_no;
  }

  return uid_no; 
}

string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  std::ifstream users_info(kPasswordPath);
  string line, _, user, tmp;

  if (users_info.is_open()){
    while (std::getline(users_info, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> _ >> tmp;
      if (tmp == uid){
        break;
      }
    }    
  }

  return user; 
}

long LinuxParser::UpTime(int pid) { 
  std::ifstream pid_info(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  string line, block ;
  long uptime{0};
  short count{0};

  if (pid_info.is_open()){
    std::getline(pid_info, line);
    std::istringstream linestream(line);
    // overwrite the first 21 blocks
    while (count != 20){
      linestream >> block ;
      if (std::all_of(block.begin(), block.end(), isdigit) || block == "-1") {
        count++;
      }
    }
  }
  uptime = stol(block);
  return uptime / sysconf(_SC_CLK_TCK); 
}
