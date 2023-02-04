#include <core/uuid.h>

GTEST_TEST(uuid, create)
{
    hud::uuid guid;
    hud_assert_true(hud::uuid::create(guid));
    hud_assert_ne(guid.a, 0u);
    hud_assert_ne(guid.b, 0u);
    hud_assert_ne(guid.c, 0u);
    hud_assert_ne(guid.d, 0u);

    hud::uuid guid2;
    hud_assert_true(hud::uuid::create(guid2));
    hud_assert_ne(guid2.a, 0u);
    hud_assert_ne(guid2.b, 0u);
    hud_assert_ne(guid2.c, 0u);
    hud_assert_ne(guid2.d, 0u);

    // Check we do not create the same UID
    hud_assert_ne((guid.a ^ guid2.a) | (guid.b ^ guid2.b) | (guid.c ^ guid2.c) | (guid.d ^ guid2.d), 0u);
}

GTEST_TEST(uuid, default_constructor)
{
    hud::uuid guid;
    hud_assert_eq(guid.a, 0u);
    hud_assert_eq(guid.b, 0u);
    hud_assert_eq(guid.c, 0u);
    hud_assert_eq(guid.d, 0u);
}

GTEST_TEST(uuid, constructor)
{
    hud::uuid guid(1u, 2u, 3u, 4u);
    hud_assert_eq(guid.a, 1u);
    hud_assert_eq(guid.b, 2u);
    hud_assert_eq(guid.c, 3u);
    hud_assert_eq(guid.d, 4u);
}

GTEST_TEST(uuid, is_valid)
{
    hud::uuid invalid_guid;
    hud_assert_false(invalid_guid.is_valid());
    hud::uuid valid_guid(1u, 2u, 3u, 4u);
    hud_assert_true(valid_guid.is_valid());
}

GTEST_TEST(uuid, reset)
{
    hud::uuid valid_guid(1u, 2u, 3u, 4u);
    hud_assert_true(valid_guid.is_valid());
    valid_guid.reset();
    hud_assert_false(valid_guid.is_valid());
}

GTEST_TEST(uuid, assign_operator)
{
    hud::uuid guid;
    hud_assert_eq(guid.a, 0u);
    hud_assert_eq(guid.b, 0u);
    hud_assert_eq(guid.c, 0u);
    hud_assert_eq(guid.d, 0u);

    hud::uuid guid_2(1u, 2u, 3u, 4u);
    hud_assert_eq(guid_2.a, 1u);
    hud_assert_eq(guid_2.b, 2u);
    hud_assert_eq(guid_2.c, 3u);
    hud_assert_eq(guid_2.d, 4u);

    guid = guid_2;
    hud_assert_eq(guid.a, 1u);
    hud_assert_eq(guid.b, 2u);
    hud_assert_eq(guid.c, 3u);
    hud_assert_eq(guid.d, 4u);
    hud_assert_eq(guid_2.a, 1u);
    hud_assert_eq(guid_2.b, 2u);
    hud_assert_eq(guid_2.c, 3u);
    hud_assert_eq(guid_2.d, 4u);
}

GTEST_TEST(uuid, equal_operator)
{
    hud::uuid guid;
    hud_assert_true(hud::uuid::create(guid));
    hud_assert_ne(guid.a, 0u);
    hud_assert_ne(guid.b, 0u);
    hud_assert_ne(guid.c, 0u);
    hud_assert_ne(guid.d, 0u);

    // Test the operator==
    hud_assert_eq(guid, guid);
}

GTEST_TEST(uuid, not_equal_operator)
{
    hud::uuid guid;
    hud_assert_true(hud::uuid::create(guid));
    hud_assert_ne(guid.a, 0u);
    hud_assert_ne(guid.b, 0u);
    hud_assert_ne(guid.c, 0u);
    hud_assert_ne(guid.d, 0u);

    hud::uuid guid2;
    hud_assert_true(hud::uuid::create(guid2));
    hud_assert_ne(guid2.a, 0u);
    hud_assert_ne(guid2.b, 0u);
    hud_assert_ne(guid2.c, 0u);
    hud_assert_ne(guid2.d, 0u);

    // Check we do not create the same UID
    hud_assert_ne((guid.a ^ guid2.a) | (guid.b ^ guid2.b) | (guid.c ^ guid2.c) | (guid.d ^ guid2.d), 0u);

    // Test the operator!=
    hud_assert_ne(guid, guid2);
}
