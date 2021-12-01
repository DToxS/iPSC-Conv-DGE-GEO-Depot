//
//  ProgramArguments.hpp
//  Universal-Toolkit
//
//  Created by Yuguang Xiong on 9/6/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef ProgramArguments_hpp
#define ProgramArguments_hpp

#include <string>

namespace utk
{

/// \brief ProgramArguments checks and stores input arguments of main function
/// This class takes input arguments of main function, checks their validity,
/// and store them in corresponding member variables.
class ProgramArguments
{
protected:

    /// \brief Number of input arguments
    const int argc;

    /// \brief Number of input arguments
    const char** argv;

    /// \brief Minimum number of input arguments required
    const int min_argc;

    /// \brief Maximum number of input arguments allowed
    const int max_argc;

protected:

    /// \brief Check the number of input arguments
    void checkNumberOfArguments();

    /// \brief Help messages on program usage
    virtual void helpMessage() {}

    /// \brief Assign mandatory input arguments
    virtual void assignMandatoryArguments() {}

    /// \brief Assign optional input arguments
    virtual void assignOptionalArguments() {}

    /// \brief Validate input arguments
    virtual void validateArguments() {}

public:

    /// \brief Program name
    std::string prog_name;

public:
    
    /// \brief Retrieve input arguments
    ProgramArguments(int argc, const char** argv, int min_argc, int max_argc);

    /// \brief Check input arguments
    /// Note: this function needs to be called by main function.
    void check();
};

}

#endif /* ProgramArguments_hpp */
