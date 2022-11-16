#include <core/uuid.h>


GTEST_TEST(uuid, create)
{
    hud::uuid guid;
    ASSERT_TRUE(hud::uuid::create(guid));
    ASSERT_NE(guid.a, 0u);
    ASSERT_NE(guid.b, 0u);
    ASSERT_NE(guid.c, 0u);
    ASSERT_NE(guid.d, 0u);

    hud::uuid guid2;
    ASSERT_TRUE(hud::uuid::create(guid2));
    ASSERT_NE(guid2.a, 0u);
    ASSERT_NE(guid2.b, 0u);
    ASSERT_NE(guid2.c, 0u);
    ASSERT_NE(guid2.d, 0u);
    
    // Check we do not create the same UID
    ASSERT_NE((guid.a ^ guid2.a) | (guid.b ^ guid2.b) | (guid.c ^ guid2.c) | (guid.d ^ guid2.d), 0u);
}

GTEST_TEST(uuid, default_constructor)
{
    hud::uuid guid;
    ASSERT_EQ(guid.a, 0u);
    ASSERT_EQ(guid.b, 0u);
    ASSERT_EQ(guid.c, 0u);
    ASSERT_EQ(guid.d, 0u);
}

GTEST_TEST(uuid, constructor)
{
    hud::uuid guid(1u,2u,3u,4u);
    ASSERT_EQ(guid.a, 1u);
    ASSERT_EQ(guid.b, 2u);
    ASSERT_EQ(guid.c, 3u);
    ASSERT_EQ(guid.d, 4u);
}


GTEST_TEST(uuid, is_valid)
{
    hud::uuid invalid_guid;
    ASSERT_FALSE(invalid_guid.is_valid());
    hud::uuid valid_guid(1u, 2u, 3u, 4u);
    ASSERT_TRUE(valid_guid.is_valid());
}

GTEST_TEST(uuid, reset)
{
    hud::uuid valid_guid(1u, 2u, 3u, 4u);
    ASSERT_TRUE(valid_guid.is_valid());
    valid_guid.reset();
    ASSERT_FALSE(valid_guid.is_valid());
}

GTEST_TEST(uuid, assign_operator)
{
    hud::uuid guid;
    ASSERT_EQ(guid.a, 0u);
    ASSERT_EQ(guid.b, 0u);
    ASSERT_EQ(guid.c, 0u);
    ASSERT_EQ(guid.d, 0u);

    hud::uuid guid_2(1u,2u,3u,4u);
    ASSERT_EQ(guid_2.a, 1u);
    ASSERT_EQ(guid_2.b, 2u);
    ASSERT_EQ(guid_2.c, 3u);
    ASSERT_EQ(guid_2.d, 4u);

    guid = guid_2;
    ASSERT_EQ(guid.a, 1u);
    ASSERT_EQ(guid.b, 2u);
    ASSERT_EQ(guid.c, 3u);
    ASSERT_EQ(guid.d, 4u);
    ASSERT_EQ(guid_2.a, 1u);
    ASSERT_EQ(guid_2.b, 2u);
    ASSERT_EQ(guid_2.c, 3u);
    ASSERT_EQ(guid_2.d, 4u);

}

GTEST_TEST(uuid, equal_operator)
{
    hud::uuid guid;
    ASSERT_TRUE(hud::uuid::create(guid));
    ASSERT_NE(guid.a, 0u);
    ASSERT_NE(guid.b, 0u);
    ASSERT_NE(guid.c, 0u);
    ASSERT_NE(guid.d, 0u);

    // Test the operator==
    ASSERT_EQ(guid, guid);
}

GTEST_TEST(uuid, not_equal_operator)
{
    hud::uuid guid;
    ASSERT_TRUE(hud::uuid::create(guid));
    ASSERT_NE(guid.a, 0u);
    ASSERT_NE(guid.b, 0u);
    ASSERT_NE(guid.c, 0u);
    ASSERT_NE(guid.d, 0u);

    hud::uuid guid2;
    ASSERT_TRUE(hud::uuid::create(guid2));
    ASSERT_NE(guid2.a, 0u);
    ASSERT_NE(guid2.b, 0u);
    ASSERT_NE(guid2.c, 0u);
    ASSERT_NE(guid2.d, 0u);

    // Check we do not create the same UID
    ASSERT_NE((guid.a ^ guid2.a) | (guid.b ^ guid2.b) | (guid.c ^ guid2.c) | (guid.d ^ guid2.d), 0u);

    // Test the operator!=
    ASSERT_NE(guid, guid2);
}
