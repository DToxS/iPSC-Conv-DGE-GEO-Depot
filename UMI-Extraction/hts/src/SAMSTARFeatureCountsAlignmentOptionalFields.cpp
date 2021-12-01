//
//  SAMSTARFeatureCountsAlignmentOptionalFields.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/29/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#include <algorithm>
#include <utility>
#include <sstream>
#include <stdexcept>
#include <hts/SAMSTARFeatureCountsAlignmentOptionalFields.hpp>

namespace hts
{

/// Default initializer.
SAMSTARFeatureCountsAlignmentOptionalFields::SAMSTARFeatureCountsAlignmentOptionalFields() : SAMAlignmentOptionalFields() {}

/// Initialization with multiple copies of specified element.
SAMSTARFeatureCountsAlignmentOptionalFields::SAMSTARFeatureCountsAlignmentOptionalFields(std::size_t count, const SAMAlignmentOptionalField& opt_field) : SAMAlignmentOptionalFields(count, opt_field) {}

/// Initialization with multiple copies of default element.
SAMSTARFeatureCountsAlignmentOptionalFields::SAMSTARFeatureCountsAlignmentOptionalFields(std::size_t count) : SAMAlignmentOptionalFields(count) {}

/// Initialization with a range of iterators.
template<class InputIterator>
SAMSTARFeatureCountsAlignmentOptionalFields::SAMSTARFeatureCountsAlignmentOptionalFields(InputIterator first, InputIterator last) : SAMAlignmentOptionalFields(first, last) {}

SAMSTARFeatureCountsAlignmentOptionalFields::SAMSTARFeatureCountsAlignmentOptionalFields(const SAMSTARFeatureCountsAlignmentOptionalFields& opt_fields) : SAMAlignmentOptionalFields(opt_fields) {}

SAMSTARFeatureCountsAlignmentOptionalFields::SAMSTARFeatureCountsAlignmentOptionalFields(SAMSTARFeatureCountsAlignmentOptionalFields&& opt_fields) : SAMAlignmentOptionalFields(std::move(opt_fields)) {}

SAMSTARFeatureCountsAlignmentOptionalFields::~SAMSTARFeatureCountsAlignmentOptionalFields() noexcept {}

SAMSTARFeatureCountsAlignmentOptionalFields& SAMSTARFeatureCountsAlignmentOptionalFields::operator=(const SAMSTARFeatureCountsAlignmentOptionalFields& opt_fields)
{
    if(this != &opt_fields)
    {
        SAMAlignmentOptionalFields::operator=(opt_fields);
    }
    return *this;
}

SAMSTARFeatureCountsAlignmentOptionalFields& SAMSTARFeatureCountsAlignmentOptionalFields::operator=(SAMSTARFeatureCountsAlignmentOptionalFields&& opt_fields)
{
    if(this != &opt_fields)
    {
        SAMAlignmentOptionalFields::operator=(std::move(opt_fields));
    }
    return *this;
}

SAMSTARFeatureCountsAlignmentOptionalFields& SAMSTARFeatureCountsAlignmentOptionalFields::operator=(std::initializer_list<SAMAlignmentOptionalField> opt_fields)
{
    SAMAlignmentOptionalFields::operator=(opt_fields);
    return *this;
}

}
