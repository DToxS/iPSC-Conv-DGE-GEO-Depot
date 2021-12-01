//
//  ProgramArguments.cpp
//  Universal-Toolkit
//
//  Created by Yuguang Xiong on 9/6/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#include <sstream>
#include <stdexcept>
#include <utk/ProgramArguments.hpp>

namespace utk
{

/// Retrieve input arguments.
ProgramArguments::ProgramArguments(int argc, const char** argv, int min_argc, int max_argc) :
    argc{argc},
    argv{argv},
    min_argc{min_argc},
    max_argc{max_argc},
    prog_name{argv[0]} {}

/// Check input arguments
void ProgramArguments::check()
{
    // Check the number of input arguments.
    checkNumberOfArguments();

    // Assign mandatory input arguments.
    assignMandatoryArguments();

    // Assign optional input arguments.
    assignOptionalArguments();

    // Validate input arguments.
    validateArguments();
}

/// Check the number of input arguments
void ProgramArguments::checkNumberOfArguments()
{
    if(argc < min_argc || argc > max_argc)
    {
        helpMessage();
        throw std::runtime_error("Wrong number of input arguments");
    }
}

///// Assign mandatory input arguments
//void ProgramArguments::assignMandatoryArguments()
//{
//    // Assign mandatory arguments.
//    arg_1 = argv[1];
//    arg_2 = argv[2];
//}
//
///// Assign optional input arguments
//void ProgramArguments::assignOptionalArguments()
//{
//    // Assign optional arguments.
//    // 3th argument.
//    if(argc > 3) arg_3 = convert<std::size_t>(toLowerString(argv[3]));
//    // 4th argument.
//    if(argc > 4) arg_4 = convert<std::size_t>(toLowerString(argv[4]));
//}
//
///// Print help messages on program usage.
//void ProgramArguments::helpMessage()
//{
//    std::cerr << "Usage: " << prog_name << " [Argument 1] [Argument 2] [Argument 3] [Argument 4]" << '\n';
//    std::cerr << "where:" << '\n';
//    std::cerr << "       " << "[Argument 1]: Description of argument 1" << '\n';
//    std::cerr << "       " << "[Argument 2]: Description of argument 2" << '\n';
//    std::cerr << "       " << "[Argument 3]: Description of argument 3" << '\n';
//    std::cerr << "       " << "[Argument 4]: Description of argument 4" << '\n';
//}

}
