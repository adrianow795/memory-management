#include <gtest/gtest.h>
#include "shared_ptr.hpp"

template class my::shared_ptr<int>;

TEST(SharedPtr, shouldCreateSharedPtrWithInitialValue) {
    my::shared_ptr<int> p{new int(5)};
    ASSERT_EQ(*p, 5);
}