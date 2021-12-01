//
//  DSVReader.hpp
//  Universal-Toolkit
//
//  Created by Yuguang Xiong on 9/7/17.
//  Copyright © 2017 Granville Xiong. All rights reserved.
//

#ifndef DSVReader_hpp
#define DSVReader_hpp

#include <vector>
#include <utility>
#include <stdexcept>
#include "LineReader.hpp"
#include "StringUtils.hpp"

namespace utk
{

/// \brief Delimiter-separated value reader for text file
/// This class splits a line from a text file into multiple values according to
/// specified delimiter.
class DSVReader : public LineReader
{
private:

    using StringsType = std::vector<std::string>;
    using StringsIteratorType = StringsType::iterator;
    using StringsConstIteratorType = StringsType::const_iterator;

private:

    /// \brief Value delimiter of FASTQ file
    /// Value delimiter can include multiple characters as required by the
    /// std::regex that uses ECMAScript regular expressions pattern syntax.
    std::string value_delim;

    /// \brief Number of delimiter-separated values.
    /// The number of delimiter-separated values must be determined by either
    /// header line (preferred) or explicitly specified.
    std::size_t n_values = 0;
    
    /// \brief Presence of header line
    bool header_line = true;

private:

#if defined (__GNUC__) || defined (__GNUG__) || defined (__clang__)

    // GCC and Clang supports the fold expression feature of C++17.

    /// \brief Convert strings and assign them to input arguments
    /// Convert all strings to corresponding data types of input arguments and
    /// then assign converted values to corresponding arguments.
    template<typename... ArgTypes>
    void assignValuesToArguments(StringsType& data_values, ArgTypes&&... args)
    {
        StringsConstIteratorType val_iter = data_values.cbegin();
        (convert(*(val_iter++), std::forward<ArgTypes>(args)), ...);
    }

#elif defined (_MSC_VER) || defined (__INTEL_COMPILER)

    // VC++ doesn't support the fold expression feature of C++17.

    template<typename ArgType, typename... ArgTypes>
    void assignValueToArgument(StringsConstIteratorType& val_iter, ArgType&& arg, ArgTypes&&... args)
    {
        convert(*(val_iter++), std::forward<ArgType>(arg));
        assignValueToArgument(val_iter, args...);
    }

    template<typename ArgType>
    void assignValueToArgument(StringsConstIteratorType& val_iter, ArgType&& arg)
    {
        convert(*(val_iter++), std::forward<ArgType>(arg));
    }

    /// \brief Convert strings and assign them to input arguments
    /// Convert all strings to corresponding data types of input arguments and
    /// then assign converted values to corresponding arguments.
    template<typename... ArgTypes>
    void assignValuesToArguments(StringsType& data_values, ArgTypes&&... args)
    {
        StringsConstIteratorType val_iter = data_values.cbegin();
        assignValueToArgument(val_iter, args...);
    }

#else

    template<typename... ArgTypes>
    void assignValuesToArguments(StringsType& data_values, ArgTypes&&... args) {}

#endif

private:

    /// \brief Check the validity of header line
    void checkHeaderLine();

    /// \brief Check the number of delimiter-separated values
    void checkNumberOfValues()
    {
        if(n_values == 0) throw std::runtime_error("The number of values must be greater than zero");
    }

    /// \brief Check the validity of header line and initialized number of values
    void checkHeaderValues()
    {
        if(header_line) checkHeaderLine();
        checkNumberOfValues();
    }

public:

    DSVReader();

    DSVReader(const std::string& file_name_arg, const std::string& val_delim_arg, bool header_line_arg=true, std::size_t n_vals_arg=0, const std::string& line_delim_type_arg="unix");

    /// Forbid copy construction behavior.
    DSVReader(const DSVReader& file) = delete;

    /// Allow copy construction behavior.
    DSVReader(DSVReader&& file);

    virtual ~DSVReader() noexcept;

    /// Forbid copy assignment behavior.
    DSVReader& operator=(const DSVReader& file) = delete;

    /// Allow move assignment behavior.
    DSVReader& operator=(DSVReader&& file);

    /// \brief Open file and initialize parameters
    void open(const std::string& file_name_arg, const std::string& val_delim_arg, bool header_line_arg=true, std::size_t n_vals_arg=0, const std::string& line_delim_type_arg="unix");

    /// \brief Read in specified value fields from a text line
    /// Note: the number of specified value fields must be no more than that of
    /// splitted values of each text line.
    template<typename... ArgTypes>
    bool readValue(ArgTypes&&... args)
    {
        // Check the number of arguments to read.
        constexpr auto n_args = sizeof...(ArgTypes);
        if(n_args != n_values) throw std::runtime_error("The number of data fields to read is different from the preset value");

        // Read in a text line.
        std::string line;
        bool status = readLine(line);

        // Split text line into multiple values and assign them to input arguments.
        if(status && !line.empty())
        {
            // Split text line into multiple values using delimiter.
            StringsType data_values = splitString(line, value_delim);

            // Convert and assign splitted values to corresponding input arguments.
            if(data_values.size() == n_values) assignValuesToArguments(data_values, std::forward<ArgTypes>(args)...);
            else throw std::runtime_error("The number of data fields available in file is different from the preset value");
        }
        
        return status;
    }
};

}

#endif /* DSVReader_hpp */
