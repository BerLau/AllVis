#pragma once
#ifndef CORE_TEST_UTILS_H
#define CORE_TEST_UTILS_H

#include <gtest/gtest.h>
#include <core.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

std::ostream &operator<<(std::ostream &os, const glm::mat4 &m);

bool Expect_Matrix_Equal(const Core::Matrix4 &m1, const glm::mat4 &m2);
bool Expect_Matrix_Equal(const Core::Matrix3 &m1, const glm::mat3 &m2);

#endif // CORE_TEST_UTILS_H