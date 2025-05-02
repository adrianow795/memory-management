#include <gtest/gtest.h>
#include "shared_ptr.hpp"

template class my::shared_ptr<int>;

TEST(SharedPtr, shouldCreateSharedPtrWithInitialValue) {
    my::shared_ptr<int> p{new int(5)};
    ASSERT_EQ(*p, 5);
}

TEST(SharedPtr, shouldCreateWithCopyConstructor) {
    my::shared_ptr<int> p {new int(5)};
    my::shared_ptr<int> p2 (p);

    ASSERT_EQ(p.get(), p2.get());
    ASSERT_EQ(p.operator->(), p2.get());
    ASSERT_EQ(p.operator->(), p2.operator->());
    ASSERT_EQ(p.use_count(), 2);
}

TEST(SharedPtr, shouldCreateWithMoveConstructor) {
    my::shared_ptr<int> p {new int(5)};
    my::shared_ptr<int> p2 (std::move(p));

    ASSERT_EQ(p.get(), nullptr);
    ASSERT_EQ(p2.use_count(), 1);
    p2.reset();
    ASSERT_EQ(p2.get(), nullptr);
    p2.reset(new int(6));
    ASSERT_EQ(*p2, 6);
}