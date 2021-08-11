#include "ptk/ptk.h"

#include <iostream>

int main(int argc, char **argv) {

    ptk::Program::init(argc, argv);

    const ptk::ProgramOptions* options = ptk::Program::getProgramOptions();
    PTK_INFO_MSG(*options);

    // ptk::utils::Statistics::SetPrintFrequency(1.0);

    // ptk::utils::StatsCollector stats("test");
    // std::cout << "herew" << std::endl;
    // PTK_INFO_MSG("yes");
    // PTK_ERROR_MSG("bad");

    // while (true)
    // {
    //     stats.IncrementOne();
    //     std::cout << ptk::utils::Statistics::Print();
    // }
    
    // return 0;

}