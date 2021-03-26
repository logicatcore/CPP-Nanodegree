#include "processor.h"
// addition
#include "linux_parser.h"

std::vector<float> Processor::Utilization() { 
    float tmp, denominator;
    std::vector<float> percentages, NEW_AJ, NEW_J;
    
    NEW_AJ = LinuxParser::ActiveJiffies();
    NEW_J = LinuxParser::Jiffies();
    for (size_t i = 0; i < NEW_AJ.size(); i++) {
        denominator = abs(OLD_J[i] - NEW_J[i]);
        if (denominator != 0) {
            tmp = (float)abs(OLD_AJ[i] - NEW_AJ[i]) / denominator;
        }
        else tmp = 0;
        percentages.emplace_back(tmp);
    }
    OLD_AJ = NEW_AJ;
    OLD_J = NEW_J;
    // no need to divived jiffies with sysconf(_SC_CLK_TCK), because a fraction is being calculated
    return percentages; 
}