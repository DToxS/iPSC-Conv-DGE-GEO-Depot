//
//  SAMAlignmentOptionalFields.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/29/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#include <algorithm>
#include <utility>
#include <sstream>
#include <stdexcept>
#include <hts/SAMAlignmentOptionalFields.hpp>

namespace hts
{

/// Default initializer.
SAMAlignmentOptionalFields::SAMAlignmentOptionalFields() : SAMAlignmentOptionalFieldsBase() {}

/// Initialization with multiple copies of specified element.
SAMAlignmentOptionalFields::SAMAlignmentOptionalFields(std::size_t count, const SAMAlignmentOptionalField& opt_field) : SAMAlignmentOptionalFieldsBase(count, opt_field) {}

/// Initialization with multiple copies of default element.
SAMAlignmentOptionalFields::SAMAlignmentOptionalFields(std::size_t count) : SAMAlignmentOptionalFieldsBase(count) {}

/// Initialization with a range of iterators.
template<class InputIterator>
    SAMAlignmentOptionalFields::SAMAlignmentOptionalFields(InputIterator first, InputIterator last) : SAMAlignmentOptionalFieldsBase(first, last) {}

SAMAlignmentOptionalFields::SAMAlignmentOptionalFields(const SAMAlignmentOptionalFields& opt_fields) : SAMAlignmentOptionalFieldsBase(opt_fields) {}

SAMAlignmentOptionalFields::SAMAlignmentOptionalFields(SAMAlignmentOptionalFields&& opt_fields) : SAMAlignmentOptionalFieldsBase(std::move(opt_fields)) {}

SAMAlignmentOptionalFields::~SAMAlignmentOptionalFields() noexcept {}

SAMAlignmentOptionalFields& SAMAlignmentOptionalFields::operator=(const SAMAlignmentOptionalFields& opt_fields)
{
    if(this != &opt_fields)
    {
        SAMAlignmentOptionalFieldsBase::operator=(opt_fields);
    }
    return *this;
}

SAMAlignmentOptionalFields& SAMAlignmentOptionalFields::operator=(SAMAlignmentOptionalFields&& opt_fields)
{
    if(this != &opt_fields)
    {
        SAMAlignmentOptionalFieldsBase::operator=(std::move(opt_fields));
    }
    return *this;
}

SAMAlignmentOptionalFields& SAMAlignmentOptionalFields::operator=(std::initializer_list<SAMAlignmentOptionalField> opt_fields)
{
    SAMAlignmentOptionalFieldsBase::operator=(opt_fields);
    return *this;
}

/// Check if a tag exists in list.
bool SAMAlignmentOptionalFields::hasTag(const std::string& tag) const
{
    auto search = std::find_if(cbegin(), cend(), [tag](const hts::SAMAlignmentOptionalField& opt_field) -> bool {return opt_field.getTag() == tag;});
    return (search != cend());
}

/// Get the value of a tag (an inefficient version).
const std::string& SAMAlignmentOptionalFields::getValue(const std::string& tag) const
{
    auto search = std::find_if(cbegin(), cend(), [tag](const hts::SAMAlignmentOptionalField& opt_field) -> bool {return opt_field.getTag() == tag;});
    if(search == cend())
    {
        std::ostringstream err_msg;
        err_msg << "Tag " << tag << " is not found!";
        throw std::logic_error(err_msg.str());
    }
    return search->getValue();
}

/// Get the value of a tag (an efficient version).
bool SAMAlignmentOptionalFields::getValue(const std::string& tag, std::string& value) const
{
    auto search = std::find_if(cbegin(), cend(), [tag](const hts::SAMAlignmentOptionalField& opt_field) -> bool {return opt_field.getTag() == tag;});
    bool status = (search != cend());
    if(status) value = search->getValue();
    return status;
}

}
