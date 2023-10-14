#include "core.h"
#include "gtest/gtest.h"
#include "test_utils.h"

TEST(TestTransform, construct_from_base_vectors)
{
    Core::Vector3 pos = Core::Vector3(1.0f, 2.0f, 3.0f);
    Core::Vector3 front = Core::Vector3(0.0f, 0.0f, 1.0f);
    Core::Vector3 up = Core::Vector3(0.0f, 1.0f, 0.0f);
    Core::Vector3 right = Core::Vector3(1.0f, 0.0f, 0.0f);
    Core::Transform transform = Core::Transform(pos, front, up);

    Core::Vector3 pos_ = transform.get_position();
    Core::Vector3 front_ = transform.get_front();
    Core::Vector3 up_ = transform.get_up();

    std::cout << "pos: " << pos << std::endl;
    std::cout << "pos_: " << pos_ << std::endl;

    std::cout << "front: " << front << std::endl;
    std::cout << "front_: " << front_ << std::endl;

    std::cout << "up: " << up << std::endl;
    std::cout << "up_: " << up_ << std::endl;

    EXPECT_TRUE(pos == pos_);
    EXPECT_TRUE(front == front_);
    EXPECT_TRUE(up == up_);
  
}