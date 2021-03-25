#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>
#include <cmath>

class Processor {
 public:
  std::vector<float> Utilization();
 private:
  // size and value intialisation is not working for some reason
  // hardcoded to work for 4 cores here
  std::vector<float> OLD_AJ{0, 0, 0, 0}, OLD_J{0, 0, 0, 0};
};

#endif