#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

// additions
#include <cmath>
#include "../include/format.h"
#include "../include/linux_parser.h"
#include "../include/process.h"
#include "../include/processor.h"
#include "../include/system.h"

void decorate_start(){
    std::cout << "\n############################################################" << std::endl;
}
void decorate_end(){
    std::cout << "############################################################\n" << std::endl;
}

TEST(LinuxParserTests, CheckPIDs){
    decorate_start();
    std::cout << "Checking PIDs" << std::endl;
    std::vector<int> pids = LinuxParser::Pids();
    for (int pid: pids){
        std::cout << pid << std::endl;
    }
    decorate_end();
}

TEST(LinuxParserTests, CalculateMemoryUtilisation){
    decorate_start();
    std::cout << "Calculating Memory Utilisation" << std::endl;
    float mem_used_percentage = LinuxParser::MemoryUtilization();
    std::cout << "Memory being used if: " << mem_used_percentage*100 << "%" << std::endl;
    decorate_end();
}

TEST(LinuxParserTests, QueryUpTime){
    decorate_start();
    std::cout << "Querying Up-Time" << std::endl;
    long active_time = LinuxParser::UpTime();

    std::cout << "Total active time of the system is: " << active_time << " seconds" << std::endl;
    printf("Total active time of the system is: %01.0f days, %02.0f:%02.0f:%02.0f \n",  floor(active_time/86400.0), floor(fmod(active_time,86400.0)/3600.0), floor(fmod(fmod(active_time,86400.0), 3600.0)/60.0), fmod(fmod(fmod(active_time,86400.0), 3600.0), 60.0));
    decorate_end();
}

TEST(LinuxParserTests, QueryUID){
    decorate_start();
    std::string uid = LinuxParser::Uid(1); // Process 1 has 0 as UID which is root
    std::cout << "Querying UID of Process with PID 1: " << uid << std::endl;
    EXPECT_EQ(uid, "0"); 
    decorate_end();
}

TEST(LinuxParserTests, QueryRunningPrcoesses){
    decorate_start();
    int processes = LinuxParser::RunningProcesses();
    std::cout << "Querying Running Processes: " << processes << std::endl;
    decorate_end();
}

TEST(LinuxParserTests, QueryTotalJiffies){
    decorate_start();
    short cpu_no{0};
    std::vector<float> total_jiffies = LinuxParser::Jiffies();
    for (float &cpu_jiffies: total_jiffies) {
        std::cout << "Total Jiffies of CPU" << cpu_no << ": " << cpu_jiffies << std::endl;
        cpu_no++;
    }
    decorate_end();
}

TEST(LinuxParserTests, QueryPIDJiffies){
    decorate_start();
    long PID_jiffies = LinuxParser::ActiveJiffies(1);
    std::cout << "Querying PID 1 Jiffies: " << PID_jiffies << std::endl;
    decorate_end();
}

TEST(LinuxParserTests, QueryTotalCores){
    decorate_start();
    int cores = LinuxParser::TotalCores();
    std::cout << "Total cores in CPU are: " << cores << std::endl;
    decorate_end();
}

TEST(LinuxParserTests, QueryActiveJiffies){
    decorate_start();
    short cpu_no{0};
    std::vector<float> active_jiffies = LinuxParser::ActiveJiffies();
    for (float &cpu_jiffies: active_jiffies) {
        std::cout << "Querying Active Jiffies of CPU" << cpu_no << ": " << cpu_jiffies << std::endl;
        cpu_no++;
    }
    decorate_end();
}

TEST(LinuxParserTests, QueryIdleJiffies){
    decorate_start();
    short cpu_no{0};
    std::vector<float> idle_jiffies = LinuxParser::IdleJiffies();
    for (float &cpu_jiffies: idle_jiffies) {
        std::cout << "Querying Idle Jiffies: " << cpu_jiffies << std::endl;
        cpu_no++;
    }
    decorate_end();
}

TEST(LinuxParserTests, QueryCPUutil){
    decorate_start();
    short cpu_no{0};
    std::vector<std::vector<std::string>> CPUs = LinuxParser::CpuUtilization();
    for (std::vector<std::string> &aCPU: CPUs) {
        std::cout << "Querying CPU utilisation of CPU" << cpu_no << ": ";
        for (std::string &value: aCPU) {
            std::cout << value << " " ;
        }
        cpu_no++;
        std::cout << "\n";
    }
    std::cout << std::endl;
    decorate_end();
}

TEST(LinuxParserTests, QueryTotalPrcoesses){
    decorate_start();
    int processes = LinuxParser::TotalProcesses();
    std::cout << "Querying Total Processes: " << processes << std::endl;
    decorate_end();
}

TEST(LinuxParserTests, QueryUserName){
    decorate_start();
    std::string name = LinuxParser::User(1); // Process 1 has 0 as UID which is root
    std::cout << "Querying User name: " << name << std::endl;
    EXPECT_EQ(name, "root"); 
    decorate_end();
}

TEST(LinuxParserTests, QueryProcessUpTime){
    decorate_start();
    long uptime = LinuxParser::UpTime(1);
    std::cout << "Querying Process 1 Up-Time: " << uptime << std::endl;
    decorate_end();
}

TEST(LinuxParserTests, QueryProcessRamSize){
    decorate_start();
    std::string ram_used = LinuxParser::Ram(1);
    std::cout << "Querying Process 1 Ram Size: " << ram_used << std::endl;
    decorate_end();
}

TEST(LinuxParserTests, QueryProcessCMD){
    decorate_start();
    std::string cmd = LinuxParser::Command(1);
    std::cout << "Querying Process 1 Command: " << cmd << std::endl;
    decorate_end();
}

TEST(FormatTests, ElapsedTimeTest){
    decorate_start();
    std::string formated_time = Format::ElapsedTime(8542);
    std::cout << "Formatted time output: " << formated_time << std::endl;
    EXPECT_EQ(formated_time, "02:22:22");
    decorate_end();
}

int main(int argc, char* argv[]){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}