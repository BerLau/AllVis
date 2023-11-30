#pragma once
#ifndef TESTS_CORE_CASES_VECTOR_H
#define TESTS_CORE_CASES_VECTOR_H

#include <gtest/gtest.h>
#include "vector.h"
class VectorTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
    }
    virtual void TearDown() {}

public:
    Core::Vector TestCopyAssignment(Core::Vector &v)
    {
        auto rslt = v;
        return rslt;
    }

    Core::Vector TestMoveAssignment(Core::Vector &&v)
    {
        auto rslt = std::move(v);
        return rslt;
    }
};

#endif // TESTS_CORE_CASES_VECTOR_H