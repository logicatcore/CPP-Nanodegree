#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                             
  std::string User();                    
  std::string Command();                 
  float CpuUtilization();                
  std::string Ram();                     
  long int UpTime();                     
  bool operator<(Process &a);

 private:
    int pid_;
    std::string user_, command_;
    float OLD_AJ{0}, OLD_TOTAL{0}, CPU_UTIL{0};
    std::vector<float> NEW_J{0, 0, 0, 0};

};

#endif