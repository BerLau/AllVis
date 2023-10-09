#include <gtest/gtest.h>
#include "core.h"

using namespace Core;

TEST(TestQuaternion, euler_angle)
{
    float yaw = 120.f;
    float pitch = 30.f;
    float roll = 45.f;

    Quaternion q = Quaternion::from_euler_angle(EulerAngle{yaw, pitch, roll});
    auto euler_angle = q.get_euler_angle();

    EXPECT_FLOAT_EQ(yaw, euler_angle.yaw);
    EXPECT_FLOAT_EQ(pitch, euler_angle.pitch);
    EXPECT_FLOAT_EQ(roll, euler_angle.roll);
}