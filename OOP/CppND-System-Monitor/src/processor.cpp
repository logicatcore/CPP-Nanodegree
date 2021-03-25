#include "processor.h"
// addition
#include "linux_parser.h"

std::vector<float> Processor::Utilization() { 
    float tmp;
    std::vector<float> percentages, NEW_AJ, NEW_J;
    
    NEW_AJ = LinuxParser::ActiveJiffies();
    NEW_J = LinuxParser::Jiffies();
    for (size_t i = 0; i < OLD_AJ.size(); i++) {
        tmp = (float)abs(OLD_AJ[i] - NEW_AJ[i]) / abs(OLD_J[i] - NEW_J[i]);
        percentages.emplace_back(tmp);
    }
    OLD_AJ = NEW_AJ;
    OLD_J = NEW_J;
    // no need to divived jiffies with sysconf(_SC_CLK_TCK), because a fraction is being calculated
    return percentages; 
}