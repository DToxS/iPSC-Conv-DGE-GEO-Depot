//
//  SAMAlignmentOptionalFields.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/29/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#ifndef SAMAlignmentOptionalFields_hpp
#define SAMAlignmentOptionalFields_hpp

#include <vector>
#include "SAMAlignmentOptionalField.hpp"

namespace hts
{

using SAMAlignmentOptionalFieldsBase = std::vector<SAMAlignmentOptionalField>;

/// \brief A class of a list of general optional fields of a SAM file.
/// This class defines some functions to search specified tag of optional field
/// and retrieve its value.
class SAMAlignmentOptionalFields : public SAMAlignmentOptionalFieldsBase
{
public:

    /// Default initializer.
    SAMAlignmentOptionalFields();

    /// \brief Initialization with multiple copies of specified element.
    SAMAlignmentOptionalFields(std::size_t count, const SAMAlignmentOptionalField& opt_field);

    /// \brief Initialization with multiple copies of default element.
    SAMAlignmentOptionalFields(std::size_t count);

    /// \brief Initialization with a range of iterators.
    template<class InputIterator>
    SAMAlignmentOptionalFields(InputIterator first, InputIterator last);

    SAMAlignmentOptionalFields(const SAMAlignmentOptionalFields& opt_fields);

    SAMAlignmentOptionalFields(SAMAlignmentOptionalFields&& opt_fields);

    virtual ~SAMAlignmentOptionalFields() noexcept;

    SAMAlignmentOptionalFields& operator=(const SAMAlignmentOptionalFields& opt_fields);

    SAMAlignmentOptionalFields& operator=(SAMAlignmentOptionalFields&& opt_fields);

    SAMAlignmentOptionalFields& operator=(std::initializer_list<SAMAlignmentOptionalField> opt_fields);

    /// Check if a tag exists in list.
    bool hasTag(const std::string& tag) const;

    /// Get the value of a tag (an inefficient version).
    /// Note: This function will throw an exception if the specified tag cannot
    /// be found. So, to avoid triggering exception, hasTag needs to be called
    /// before this function. However such call sequence will cause duplicate
    /// search operation, which is inefficient.
    const std::string& getValue(const std::string& tag) const;

    /// Get the value of a tag (an efficient version).
    /// Note: This function can be used directly to obtain the value of the
    /// specified tag without any overheads. The retrieved value is stored in
    /// the input argument. If the tag cannot be found, a false status will be
    /// returned.
    bool getValue(const std::string& tag, std::string& value) const;
};

}

#endif /* SAMAlignmentOptionalFields_hpp */
