#include "ptk/ptk.h"

#include <iostream>

int main() {

    ptk::utils::Statistics::SetPrintFrequency(1.0);

    ptk::utils::StatsCollector stats("test");
    std::cout << "herew" << std::endl;
    PTK_INFO_MSG("yes");
    PTK_ERROR_MSG("bad");

    while (true)
    {
        stats.IncrementOne();
        std::cout << ptk::utils::Statistics::Print();
    }
    
    return 0;

}