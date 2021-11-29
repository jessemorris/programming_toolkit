#include "ptk/utilities/outputHandler.hpp"
#include "ptk/core/program.h"

#include <iostream>

namespace ptk {
namespace utils {
namespace impl {

FileHandler::FileHandler(const std::string& name_)
    : filePath(constructOutputFilePath(name_)) {
        file.open(filePath);
    }

FileHandler::~FileHandler() {
    if(file.is_open()) {
        file.close();
    }
}

const std::string FileHandler::constructOutputFilePath(const std::string& file_) const {
    const ProgramOptions* options = Program::getProgramOptions();
    //TODO: use file extensions
    // std::cout << "options " << *options << std::endl;
    return options->logging_dir + "/" + file_ + ".txt";
}


} //impl
} //utils
} //ptk