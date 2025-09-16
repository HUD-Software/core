#include <core/containers/vector.h>

namespace hud_test
{
    struct c
    {
        hud::i32 value;
    };

    struct d
    {
        hud::i32 value;
    };

    // Equals
    [[nodiscard]] constexpr bool operator==(const d &left, const d &right) noexcept
    {
        return left.value == right.value;
    }

    [[nodiscard]] constexpr bool operator==(const d &left, const hud::i32 &right) noexcept
    {
        return left.value == right;
    }

    [[nodiscard]] constexpr bool operator==(const hud::i32 &left, const d &right) noexcept
    {
        return left == right.value;
    }

    // Not equals
    [[nodiscard]] constexpr bool operator!=(const d &left, const d &right) noexcept
    {
        return left.value != right.value;
    }

    [[nodiscard]] constexpr bool operator!=(const d &left, const hud::i32 &right) noexcept
    {
        return left.value != right;
    }

    [[nodiscard]] constexpr bool operator!=(const hud::i32 &left, const d &right) noexcept
    {
        return left != right.value;
    }

    // Less
    [[nodiscard]] constexpr bool operator<(const c &left, const c &right) noexcept
    {
        return left.value < right.value;
    }

    [[nodiscard]] constexpr bool operator<(const c &left, const hud::i32 &right) noexcept
    {
        return left.value < right;
    }

    [[nodiscard]] constexpr bool operator<(const hud::i32 &left, const c &right) noexcept
    {
        return left < right.value;
    }

    [[nodiscard]] constexpr bool operator<(const d &left, const d &right) noexcept
    {
        return left.value < right.value;
    }

    [[nodiscard]] constexpr bool operator<(const d &left, const hud::i32 &right) noexcept
    {
        return left.value < right;
    }

    [[nodiscard]] constexpr bool operator<(const hud::i32 &left, const d &right) noexcept
    {
        return left < right.value;
    }
} // namespace hud_test

namespace hud
{
    template<>
    struct is_bitwise_comparable<hud_test::c, hud_test::c> : hud::true_type
    {
    };

    template<>
    struct is_bitwise_comparable<hud_test::c, i32> : hud::true_type
    {
    };

    template<>
    struct is_bitwise_comparable<i32, hud_test::c> : hud::true_type
    {
    };
} // namespace hud

GTEST_TEST(vector, equal_operator)
{
    /**
    *
        Testing by resolving the following truth table
        where c is bitwise comparable and d is not bitwise comparable
        And c is not comparable with d

        -----------------------------------------------------------------------------------------------------
        |              | [] | [1] | [1,2] | [1,3] | [c(1), c(2)] | c(1), c(3) | [d(1), d(2)] | [d(1), d(3)] |
        -----------------------------------------------------------------------------------------------------
        | []           |  1 |  0  |   0   |   0   |      0       |     0      |      0       |      0       |
        -----------------------------------------------------------------------------------------------------
        | [1]          |  0 |  1  |   0   |   0   |      0       |     0      |      0       |      0       |
        -----------------------------------------------------------------------------------------------------
        | [1,2]        |  0 |  0  |   1   |   0   |      1       |     0      |      1       |      0       |
        -----------------------------------------------------------------------------------------------------
        | [1,3]        |  0 |  0  |   0   |   1   |      0       |     1      |      0       |      1       |
        -----------------------------------------------------------------------------------------------------
        | [c(1), c(2)] |  0 |  0  |   1   |   0   |      1       |     0      |     ---      |     ---      |
        -----------------------------------------------------------------------------------------------------
        | [c(1), c(3)] |  0 |  0  |   0   |   1   |      0       |     1      |     ---      |     ---      |
        -----------------------------------------------------------------------------------------------------
        | [d(1), d(2)] |  0 |  0  |   1   |   0   |     ---      |    ---     |      1       |      0       |
        -----------------------------------------------------------------------------------------------------
        | [d(1), d(3)] |  0 |  0  |   0   |   1   |     ---      |    ---     |      0       |      1       |
        -----------------------------------------------------------------------------------------------------
    */

    using c = hud_test::c;
    using d = hud_test::d;

    hud::vector<i32> col_0;
    hud::vector<i32> row_0;
    hud::vector<i32> col_1({1});
    hud::vector<i32> row_1({1});
    hud::vector<i32> col_2({1, 2});
    hud::vector<i32> row_2({1, 2});
    hud::vector<i32> col_3({1, 3});
    hud::vector<i32> row_3({1, 3});
    hud::vector<c> col_4({c {1}, c {2}});
    hud::vector<c> row_4({c {1}, c {2}});
    hud::vector<c> col_5({c {1}, c {3}});
    hud::vector<c> row_5({c {1}, c {3}});
    hud::vector<d> col_6({d {1}, d {2}});
    hud::vector<d> row_6({d {1}, d {2}});
    hud::vector<d> col_7({d {1}, d {3}});
    hud::vector<d> row_7({d {1}, d {3}});

    // Verify the truth table
    hud_assert_true(col_0 == row_0);
    hud_assert_false(col_0 == row_1);
    hud_assert_false(col_0 == row_2);
    hud_assert_false(col_0 == row_3);
    hud_assert_false(col_0 == row_4);
    hud_assert_false(col_0 == row_5);
    hud_assert_false(col_0 == row_6);
    hud_assert_false(col_0 == row_7);

    hud_assert_false(col_1 == row_0);
    hud_assert_true(col_1 == row_1);
    hud_assert_false(col_1 == row_2);
    hud_assert_false(col_1 == row_3);
    hud_assert_false(col_1 == row_4);
    hud_assert_false(col_1 == row_5);
    hud_assert_false(col_1 == row_6);
    hud_assert_false(col_1 == row_7);

    hud_assert_false(col_2 == row_0);
    hud_assert_false(col_2 == row_1);
    hud_assert_true(col_2 == row_2);
    hud_assert_false(col_2 == row_3);
    hud_assert_true(col_2 == row_4);
    hud_assert_false(col_2 == row_5);
    hud_assert_true(col_2 == row_6);
    hud_assert_false(col_2 == row_7);

    hud_assert_false(col_3 == row_0);
    hud_assert_false(col_3 == row_1);
    hud_assert_false(col_3 == row_2);
    hud_assert_true(col_3 == row_3);
    hud_assert_false(col_3 == row_4);
    hud_assert_true(col_3 == row_5);
    hud_assert_false(col_3 == row_6);
    hud_assert_true(col_3 == row_7);

    hud_assert_false(col_4 == row_0);
    hud_assert_false(col_4 == row_1);
    hud_assert_true(col_4 == row_2);
    hud_assert_false(col_4 == row_3);
    hud_assert_true(col_4 == row_4);
    hud_assert_false(col_4 == row_5);
    // hud_assert_false(col_4 == row_6);  // c not comparable with d
    // hud_assert_false(col_4 == row_7);  // c not comparable with d

    hud_assert_false(col_5 == row_0);
    hud_assert_false(col_5 == row_1);
    hud_assert_false(col_5 == row_2);
    hud_assert_true(col_5 == row_3);
    hud_assert_false(col_5 == row_4);
    hud_assert_true(col_5 == row_5);
    // Assert::IsFalse(col_5 == row_6); // c not comparable with d
    // Assert::IsFalse(col_5 == row_7); // c not comparable with d

    hud_assert_false(col_6 == row_0);
    hud_assert_false(col_6 == row_1);
    hud_assert_true(col_6 == row_2);
    hud_assert_false(col_6 == row_3);
    // hud_assert_false(col_6 == row_4); // d not comparable with c
    // hud_assert_false(col_6 == row_5); // d not comparable with c
    hud_assert_true(col_6 == row_6);
    hud_assert_false(col_6 == row_7);

    hud_assert_false(col_7 == row_0);
    hud_assert_false(col_7 == row_1);
    hud_assert_false(col_7 == row_2);
    hud_assert_true(col_7 == row_3);
    // hud_assert_false(col_7 == row_4); // d not comparable with c
    // hud_assert_false(col_7 == row_5); // d not comparable with c
    hud_assert_false(col_7 == row_6);
    hud_assert_true(col_7 == row_7);
}

GTEST_TEST(vector, not_equal_operator)
{
    /**
    *
        Testing by resolving the following truth table
        where c is bitwise comparable and d is not bitwise comparable
        And c is not comparable with d

    -----------------------------------------------------------------------------------------------------
    |              | [] | [1] | [1,2] | [1,3] | [c(1), c(2)] | c(1), c(3) | [d(1), d(2)] | [d(1), d(3)] |
    -----------------------------------------------------------------------------------------------------
    | []           |  0 |  1  |   1   |   1   |      1       |     1      |      1       |      1       |
    -----------------------------------------------------------------------------------------------------
    | [1]          |  1 |  0  |   1   |   1   |      1       |     1      |      1       |      1       |
    -----------------------------------------------------------------------------------------------------
    | [1,2]        |  1 |  1  |   0   |   1   |      0       |     1      |      0       |      1       |
    -----------------------------------------------------------------------------------------------------
    | [1,3]        |  1 |  1  |   1   |   0   |      1       |     0      |      1       |      0       |
    -----------------------------------------------------------------------------------------------------
    | [c(1), c(2)] |  1 |  1  |   0   |   1   |      0       |     1      |     ---      |     ---      |
    -----------------------------------------------------------------------------------------------------
    | [c(1), c(3)] |  1 |  1  |   1   |   0   |      1       |     0      |     ---      |     ---      |
    -----------------------------------------------------------------------------------------------------
    | [d(1), d(2)] |  1 |  1  |   0   |   1   |     ---      |    ---     |      0       |      1       |
    -----------------------------------------------------------------------------------------------------
    | [d(1), d(3)] |  1 |  1  |   1   |   0   |     ---      |    ---     |      1       |      0       |
    -----------------------------------------------------------------------------------------------------
    */

    using c = hud_test::c;
    using d = hud_test::d;

    hud::vector<i32> col_0;
    hud::vector<i32> row_0;
    hud::vector<i32> col_1({1});
    hud::vector<i32> row_1({1});
    hud::vector<i32> col_2({1, 2});
    hud::vector<i32> row_2({1, 2});
    hud::vector<i32> col_3({1, 3});
    hud::vector<i32> row_3({1, 3});
    hud::vector<c> col_4({c {1}, c {2}});
    hud::vector<c> row_4({c {1}, c {2}});
    hud::vector<c> col_5({c {1}, c {3}});
    hud::vector<c> row_5({c {1}, c {3}});
    hud::vector<d> col_6({d {1}, d {2}});
    hud::vector<d> row_6({d {1}, d {2}});
    hud::vector<d> col_7({d {1}, d {3}});
    hud::vector<d> row_7({d {1}, d {3}});

    // Verify the truth table
    hud_assert_false(col_0 != row_0);
    hud_assert_true(col_0 != row_1);
    hud_assert_true(col_0 != row_2);
    hud_assert_true(col_0 != row_3);
    hud_assert_true(col_0 != row_4);
    hud_assert_true(col_0 != row_5);
    hud_assert_true(col_0 != row_6);
    hud_assert_true(col_0 != row_7);

    hud_assert_true(col_1 != row_0);
    hud_assert_false(col_1 != row_1);
    hud_assert_true(col_1 != row_2);
    hud_assert_true(col_1 != row_3);
    hud_assert_true(col_1 != row_4);
    hud_assert_true(col_1 != row_5);
    hud_assert_true(col_1 != row_6);
    hud_assert_true(col_1 != row_7);

    hud_assert_true(col_2 != row_0);
    hud_assert_true(col_2 != row_1);
    hud_assert_false(col_2 != row_2);
    hud_assert_true(col_2 != row_3);
    hud_assert_false(col_2 != row_4);
    hud_assert_true(col_2 != row_5);
    hud_assert_false(col_2 != row_6);
    hud_assert_true(col_2 != row_7);

    hud_assert_true(col_3 != row_0);
    hud_assert_true(col_3 != row_1);
    hud_assert_true(col_3 != row_2);
    hud_assert_false(col_3 != row_3);
    hud_assert_true(col_3 != row_4);
    hud_assert_false(col_3 != row_5);
    hud_assert_true(col_3 != row_6);
    hud_assert_false(col_3 != row_7);

    hud_assert_true(col_4 != row_0);
    hud_assert_true(col_4 != row_1);
    hud_assert_false(col_4 != row_2);
    hud_assert_true(col_4 != row_3);
    hud_assert_false(col_4 != row_4);
    hud_assert_true(col_4 != row_5);
    // hud_assert_false(col_4 != row_6);  // c not comparable with d
    // hud_assert_false(col_4 != row_7);  // c not comparable with d

    hud_assert_true(col_5 != row_0);
    hud_assert_true(col_5 != row_1);
    hud_assert_true(col_5 != row_2);
    hud_assert_false(col_5 != row_3);
    hud_assert_true(col_5 != row_4);
    hud_assert_false(col_5 != row_5);
    // hud_assert_false(col_5 != row_6); // c not comparable with d
    // hud_assert_false(col_5 != row_7); // c not comparable with d

    hud_assert_true(col_6 != row_0);
    hud_assert_true(col_6 != row_1);
    hud_assert_false(col_6 != row_2);
    hud_assert_true(col_6 != row_3);
    // hud_assert_false(col_6 != row_4); // d not comparable with c
    // hud_assert_false(col_6 != row_5); // d not comparable with c
    hud_assert_false(col_6 != row_6);
    hud_assert_true(col_6 != row_7);

    hud_assert_true(col_7 != row_0);
    hud_assert_true(col_7 != row_1);
    hud_assert_true(col_7 != row_2);
    hud_assert_false(col_7 != row_3);
    // hud_assert_false(col_7 != row_4); // d not comparable with c
    // hud_assert_false(col_7 != row_5); // d not comparable with c
    hud_assert_true(col_7 != row_6);
    hud_assert_false(col_7 != row_7);
}

GTEST_TEST(vector, less_operator)
{
    /**
        Testing by resolving the following truth table
        where c and d are not bitwise comparable
        And c is not comparable with d

    ---------------------------------------------------------------------------------------------------------------------------------
    |              | [] | [1] | [1,2] | [1,3] | [c(1), c(2)] | c(1), c(3) | [d(1), d(2)] | [d(1), d(3)] |  (u8) [1]  |  (u8) [1,2]  |
    ---------------------------------------------------------------------------------------------------------------------------------
    | []           |  0 |  0  |   0   |   0   |      0       |     0      |      0       |      0       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1]          |  1 |  0  |   0   |   0   |      0       |     0      |      0       |      0       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,2]        |  1 |  1  |   0   |   0   |      0       |     0      |      0       |      0       |     1      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,3]        |  1 |  1  |   1   |   0   |      1       |     0      |      1       |      0       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [c(1), c(2)] |  1 |  1  |   0   |   0   |      0       |     0      |     ---      |     ---      |     1      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [c(1), c(3)] |  1 |  1  |   1   |   0   |      1       |     0      |     ---      |     ---      |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [d(1), d(2)] |  1 |  1  |   0   |   0   |     ---      |    ---     |      0       |      0       |     1      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [d(1), d(3)] |  1 |  1  |   1   |   0   |     ---      |    ---     |      1       |      0       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |   (u8) [1]   |  1 |  0  |   0   |   0   |      0       |     0      |      0       |      0       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |  (u8) [1,2]  |  1 |  1  |   0   |   0   |      0       |     0      |      0       |      0       |     1      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    */

    using c = hud_test::c;
    using d = hud_test::d;

    hud::vector<i32> col_0;
    hud::vector<i32> row_0;
    hud::vector<i32> col_1({1});
    hud::vector<i32> row_1({1});
    hud::vector<i32> col_2({1, 2});
    hud::vector<i32> row_2({1, 2});
    hud::vector<i32> col_3({1, 3});
    hud::vector<i32> row_3({1, 3});
    hud::vector<c> col_4({c {1}, c {2}});
    hud::vector<c> row_4({c {1}, c {2}});
    hud::vector<c> col_5({c {1}, c {3}});
    hud::vector<c> row_5({c {1}, c {3}});
    hud::vector<d> col_6({d {1}, d {2}});
    hud::vector<d> row_6({d {1}, d {2}});
    hud::vector<d> col_7({d {1}, d {3}});
    hud::vector<d> row_7({d {1}, d {3}});
    hud::vector<u8> col_8({u8(1)});
    hud::vector<u8> row_8({u8(1)});
    hud::vector<u8> col_9({u8(1), u8(2)});
    hud::vector<u8> row_9({u8(1), u8(2)});

    // Verify the truth table
    hud_assert_false(col_0 < row_0);
    hud_assert_true(col_0 < row_1);
    hud_assert_true(col_0 < row_2);
    hud_assert_true(col_0 < row_3);
    hud_assert_true(col_0 < row_4);
    hud_assert_true(col_0 < row_5);
    hud_assert_true(col_0 < row_6);
    hud_assert_true(col_0 < row_7);
    hud_assert_true(col_0 < row_8);
    hud_assert_true(col_0 < row_9);

    hud_assert_false(col_1 < row_0);
    hud_assert_false(col_1 < row_1);
    hud_assert_true(col_1 < row_2);
    hud_assert_true(col_1 < row_3);
    hud_assert_true(col_1 < row_4);
    hud_assert_true(col_1 < row_5);
    hud_assert_true(col_1 < row_6);
    hud_assert_true(col_1 < row_7);
    hud_assert_false(col_1 < row_8);
    hud_assert_true(col_1 < row_9);

    hud_assert_false(col_2 < row_0);
    hud_assert_false(col_2 < row_1);
    hud_assert_false(col_2 < row_2);
    hud_assert_true(col_2 < row_3);
    hud_assert_false(col_2 < row_4);
    hud_assert_true(col_2 < row_5);
    hud_assert_false(col_2 < row_6);
    hud_assert_true(col_2 < row_7);
    hud_assert_false(col_2 < row_8);
    hud_assert_false(col_2 < row_9);

    hud_assert_false(col_3 < row_0);
    hud_assert_false(col_3 < row_1);
    hud_assert_false(col_3 < row_2);
    hud_assert_false(col_3 < row_3);
    hud_assert_false(col_3 < row_4);
    hud_assert_false(col_3 < row_5);
    hud_assert_false(col_3 < row_6);
    hud_assert_false(col_3 < row_7);
    hud_assert_false(col_3 < row_8);
    hud_assert_false(col_3 < row_9);

    hud_assert_false(col_4 < row_0);
    hud_assert_false(col_4 < row_1);
    hud_assert_false(col_4 < row_2);
    hud_assert_true(col_4 < row_3);
    hud_assert_false(col_4 < row_4);
    hud_assert_true(col_4 < row_5);
    // hud_assert_false(col_4 < row_6);  // c not comparable with d
    // hud_assert_false(col_4 < row_7);  // c not comparable with d
    hud_assert_false(col_4 < row_8);
    hud_assert_false(col_4 < row_9);

    hud_assert_false(col_5 < row_0);
    hud_assert_false(col_5 < row_1);
    hud_assert_false(col_5 < row_2);
    hud_assert_false(col_5 < row_3);
    hud_assert_false(col_5 < row_4);
    hud_assert_false(col_5 < row_5);
    // hud_assert_false(col_5 < row_6); // c not comparable with d
    // hud_assert_false(col_5 < row_7); // c not comparable with d
    hud_assert_false(col_5 < row_8);
    hud_assert_false(col_5 < row_9);

    hud_assert_false(col_6 < row_0);
    hud_assert_false(col_6 < row_1);
    hud_assert_false(col_6 < row_2);
    hud_assert_true(col_6 < row_3);
    // hud_assert_false(col_6 < row_4); // d not comparable with c
    // hud_assert_false(col_6 < row_5); // d not comparable with c
    hud_assert_false(col_6 < row_6);
    hud_assert_true(col_6 < row_7);
    hud_assert_false(col_6 < row_8);
    hud_assert_false(col_6 < row_9);

    hud_assert_false(col_7 < row_0);
    hud_assert_false(col_7 < row_1);
    hud_assert_false(col_7 < row_2);
    hud_assert_false(col_7 < row_3);
    // hud_assert_false(col_7 < row_4); // d not comparable with c
    // hud_assert_false(col_7 < row_5); // d not comparable with c
    hud_assert_false(col_7 < row_6);
    hud_assert_false(col_7 < row_7);
    hud_assert_false(col_7 < row_8);
    hud_assert_false(col_7 < row_9);

    hud_assert_false(col_8 < row_0);
    hud_assert_false(col_8 < row_1);
    hud_assert_true(col_8 < row_2);
    hud_assert_true(col_8 < row_3);
    hud_assert_true(col_8 < row_4);
    hud_assert_true(col_8 < row_5);
    hud_assert_true(col_8 < row_6);
    hud_assert_true(col_8 < row_7);
    hud_assert_false(col_8 < row_8);
    hud_assert_true(col_8 < row_9);

    hud_assert_false(col_9 < row_0);
    hud_assert_false(col_9 < row_1);
    hud_assert_false(col_9 < row_2);
    hud_assert_true(col_9 < row_3);
    hud_assert_false(col_9 < row_4);
    hud_assert_true(col_9 < row_5);
    hud_assert_false(col_9 < row_6);
    hud_assert_true(col_9 < row_7);
    hud_assert_false(col_9 < row_8);
    hud_assert_false(col_9 < row_9);
}

GTEST_TEST(vector, greater_operator)
{
    /**
        Testing by resolving the following truth table
        where c and d are not bitwise comparable
        And c is not comparable with d

    ---------------------------------------------------------------------------------------------------------------------------------
    |              | [] | [1] | [1,2] | [1,3] | [c(1), c(2)] | c(1), c(3) | [d(1), d(2)] | [d(1), d(3)] |  (u8) [1]  |  (u8) [1,2]  |
    ---------------------------------------------------------------------------------------------------------------------------------
    | []           |  0 |  1  |   1   |   1   |      1       |     1      |      1       |      1       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1]          |  0 |  0  |   1   |   1   |      1       |     1      |      1       |      1       |     0      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,2]        |  0 |  0  |   0   |   1   |      0       |     1      |      0       |      1       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,3]        |  0 |  0  |   0   |   0   |      0       |     0      |      0       |      0       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [c(1), c(2)] |  0 |  0  |   0   |   1   |      0       |     1      |     ---      |     ---      |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [c(1), c(3)] |  0 |  0  |   0   |   0   |      0       |     0      |     ---      |     ---      |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [d(1), d(2)] |  0 |  0  |   0   |   1   |     ---      |    ---     |      0       |      1       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [d(1), d(3)] |  0 |  0  |   0   |   0   |     ---      |    ---     |      0       |      0       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |   (u8) [1]   |  0 |  0  |   1   |   1   |      1       |     1      |      1       |      1       |     0      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |  (u8) [1,2]  |  0 |  0  |   0   |   1   |      0       |     1      |      0       |      1       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    */

    using c = hud_test::c;
    using d = hud_test::d;

    hud::vector<i32> col_0;
    hud::vector<i32> row_0;
    hud::vector<i32> col_1({1});
    hud::vector<i32> row_1({1});
    hud::vector<i32> col_2({1, 2});
    hud::vector<i32> row_2({1, 2});
    hud::vector<i32> col_3({1, 3});
    hud::vector<i32> row_3({1, 3});
    hud::vector<c> col_4({c {1}, c {2}});
    hud::vector<c> row_4({c {1}, c {2}});
    hud::vector<c> col_5({c {1}, c {3}});
    hud::vector<c> row_5({c {1}, c {3}});
    hud::vector<d> col_6({d {1}, d {2}});
    hud::vector<d> row_6({d {1}, d {2}});
    hud::vector<d> col_7({d {1}, d {3}});
    hud::vector<d> row_7({d {1}, d {3}});
    hud::vector<u8> col_8({u8(1)});
    hud::vector<u8> row_8({u8(1)});
    hud::vector<u8> col_9({u8(1), u8(2)});
    hud::vector<u8> row_9({u8(1), u8(2)});

    // Verify the truth table
    hud_assert_false(col_0 > row_0);
    hud_assert_false(col_0 > row_1);
    hud_assert_false(col_0 > row_2);
    hud_assert_false(col_0 > row_3);
    hud_assert_false(col_0 > row_4);
    hud_assert_false(col_0 > row_5);
    hud_assert_false(col_0 > row_6);
    hud_assert_false(col_0 > row_7);
    hud_assert_false(col_0 > row_8);
    hud_assert_false(col_0 > row_9);

    hud_assert_true(col_1 > row_0);
    hud_assert_false(col_1 > row_1);
    hud_assert_false(col_1 > row_2);
    hud_assert_false(col_1 > row_3);
    hud_assert_false(col_1 > row_4);
    hud_assert_false(col_1 > row_5);
    hud_assert_false(col_1 > row_6);
    hud_assert_false(col_1 > row_7);
    hud_assert_false(col_1 > row_8);
    hud_assert_false(col_1 > row_9);

    hud_assert_true(col_2 > row_0);
    hud_assert_true(col_2 > row_1);
    hud_assert_false(col_2 > row_2);
    hud_assert_false(col_2 > row_3);
    hud_assert_false(col_2 > row_4);
    hud_assert_false(col_2 > row_5);
    hud_assert_false(col_2 > row_6);
    hud_assert_false(col_2 > row_7);
    hud_assert_true(col_2 > row_8);
    hud_assert_false(col_2 > row_9);

    hud_assert_true(col_3 > row_0);
    hud_assert_true(col_3 > row_1);
    hud_assert_true(col_3 > row_2);
    hud_assert_false(col_3 > row_3);
    hud_assert_true(col_3 > row_4);
    hud_assert_false(col_3 > row_5);
    hud_assert_true(col_3 > row_6);
    hud_assert_false(col_3 > row_7);
    hud_assert_true(col_3 > row_8);
    hud_assert_true(col_3 > row_9);

    hud_assert_true(col_4 > row_0);
    hud_assert_true(col_4 > row_1);
    hud_assert_false(col_4 > row_2);
    hud_assert_false(col_4 > row_3);
    hud_assert_false(col_4 > row_4);
    hud_assert_false(col_4 > row_5);
    // hud_assert_false(col_4 > row_6);  // c not comparable with d
    // hud_assert_false(col_4 > row_7);  // c not comparable with d
    hud_assert_true(col_4 > row_8);
    hud_assert_false(col_4 > row_9);

    hud_assert_true(col_5 > row_0);
    hud_assert_true(col_5 > row_1);
    hud_assert_true(col_5 > row_2);
    hud_assert_false(col_5 > row_3);
    hud_assert_true(col_5 > row_4);
    hud_assert_false(col_5 > row_5);
    // hud_assert_false(col_5 > row_6); // c not comparable with d
    // hud_assert_false(col_5 > row_7); // c not comparable with d
    hud_assert_true(col_5 > row_8);
    hud_assert_true(col_5 > row_9);

    hud_assert_true(col_6 > row_0);
    hud_assert_true(col_6 > row_1);
    hud_assert_false(col_6 > row_2);
    hud_assert_false(col_6 > row_3);
    // hud_assert_false(col_6 > row_4); // d not comparable with c
    // hud_assert_false(col_6 > row_5); // d not comparable with c
    hud_assert_false(col_6 > row_6);
    hud_assert_false(col_6 > row_7);
    hud_assert_true(col_6 > row_8);
    hud_assert_false(col_6 > row_9);

    hud_assert_true(col_7 > row_0);
    hud_assert_true(col_7 > row_1);
    hud_assert_true(col_7 > row_2);
    hud_assert_false(col_7 > row_3);
    // hud_assert_false(col_7 > row_4); // d not comparable with c
    // hud_assert_false(col_7 > row_5); // d not comparable with c
    hud_assert_true(col_7 > row_6);
    hud_assert_false(col_7 > row_7);
    hud_assert_true(col_7 > row_8);
    hud_assert_true(col_7 > row_9);

    hud_assert_true(col_8 > row_0);
    hud_assert_false(col_8 > row_1);
    hud_assert_false(col_8 > row_2);
    hud_assert_false(col_8 > row_3);
    hud_assert_false(col_8 > row_4);
    hud_assert_false(col_8 > row_5);
    hud_assert_false(col_8 > row_6);
    hud_assert_false(col_8 > row_7);
    hud_assert_false(col_8 > row_8);
    hud_assert_false(col_8 > row_9);

    hud_assert_true(col_9 > row_0);
    hud_assert_true(col_9 > row_1);
    hud_assert_false(col_9 > row_2);
    hud_assert_false(col_9 > row_3);
    hud_assert_false(col_9 > row_4);
    hud_assert_false(col_9 > row_5);
    hud_assert_false(col_9 > row_6);
    hud_assert_false(col_9 > row_7);
    hud_assert_true(col_9 > row_8);
    hud_assert_false(col_9 > row_9);
}

GTEST_TEST(vector, less_equal_operator)
{
    /**
        Testing by resolving the following truth table
        where c and d are not bitwise comparable
        And c is not comparable with d

    ---------------------------------------------------------------------------------------------------------------------------------
    |              | [] | [1] | [1,2] | [1,3] | [c(1), c(2)] | c(1), c(3) | [d(1), d(2)] | [d(1), d(3)] |  (u8) [1]  |  (u8) [1,2]  |
    ---------------------------------------------------------------------------------------------------------------------------------
    | []           |  1 |  0  |   0   |   0   |      0       |     0      |      0       |      0       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1]          |  1 |  1  |   0   |   0   |      0       |     0      |      0       |      0       |     1      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,2]        |  1 |  1  |   1   |   0   |      1       |     0      |      1       |      0       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,3]        |  1 |  1  |   1   |   1   |      1       |     1      |      1       |      1       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [c(1), c(2)] |  1 |  1  |   1   |   0   |      1       |     0      |     ---      |     ---      |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [c(1), c(3)] |  1 |  1  |   1   |   1   |      1       |     1      |     ---      |     ---      |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [d(1), d(2)] |  1 |  1  |   1   |   0   |     ---      |    ---     |      1       |      0       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [d(1), d(3)] |  1 |  1  |   1   |   1   |     ---      |    ---     |      1       |      1       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |   (u8) [1]   |  1 |  1  |   0   |   0   |      0       |     0      |      0       |      0       |     1      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |  (u8) [1,2]  |  1 |  1  |   1   |   0   |      1       |     0      |      1       |      0       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    */

    using c = hud_test::c;
    using d = hud_test::d;

    hud::vector<i32> col_0;
    hud::vector<i32> row_0;
    hud::vector<i32> col_1({1});
    hud::vector<i32> row_1({1});
    hud::vector<i32> col_2({1, 2});
    hud::vector<i32> row_2({1, 2});
    hud::vector<i32> col_3({1, 3});
    hud::vector<i32> row_3({1, 3});
    hud::vector<c> col_4({c {1}, c {2}});
    hud::vector<c> row_4({c {1}, c {2}});
    hud::vector<c> col_5({c {1}, c {3}});
    hud::vector<c> row_5({c {1}, c {3}});
    hud::vector<d> col_6({d {1}, d {2}});
    hud::vector<d> row_6({d {1}, d {2}});
    hud::vector<d> col_7({d {1}, d {3}});
    hud::vector<d> row_7({d {1}, d {3}});
    hud::vector<u8> col_8({u8(1)});
    hud::vector<u8> row_8({u8(1)});
    hud::vector<u8> col_9({u8(1), u8(2)});
    hud::vector<u8> row_9({u8(1), u8(2)});

    // Verify the truth table
    hud_assert_true(col_0 <= row_0);
    hud_assert_true(col_0 <= row_1);
    hud_assert_true(col_0 <= row_2);
    hud_assert_true(col_0 <= row_3);
    hud_assert_true(col_0 <= row_4);
    hud_assert_true(col_0 <= row_5);
    hud_assert_true(col_0 <= row_6);
    hud_assert_true(col_0 <= row_7);
    hud_assert_true(col_0 <= row_8);
    hud_assert_true(col_0 <= row_9);

    hud_assert_false(col_1 <= row_0);
    hud_assert_true(col_1 <= row_1);
    hud_assert_true(col_1 <= row_2);
    hud_assert_true(col_1 <= row_3);
    hud_assert_true(col_1 <= row_4);
    hud_assert_true(col_1 <= row_5);
    hud_assert_true(col_1 <= row_6);
    hud_assert_true(col_1 <= row_7);
    hud_assert_true(col_1 <= row_8);
    hud_assert_true(col_1 <= row_9);

    hud_assert_false(col_2 <= row_0);
    hud_assert_false(col_2 <= row_1);
    hud_assert_true(col_2 <= row_2);
    hud_assert_true(col_2 <= row_3);
    hud_assert_true(col_2 <= row_4);
    hud_assert_true(col_2 <= row_5);
    hud_assert_true(col_2 <= row_6);
    hud_assert_true(col_2 <= row_7);
    hud_assert_false(col_2 <= row_8);
    hud_assert_true(col_2 <= row_9);

    hud_assert_false(col_3 <= row_0);
    hud_assert_false(col_3 <= row_1);
    hud_assert_false(col_3 <= row_2);
    hud_assert_true(col_3 <= row_3);
    hud_assert_false(col_3 <= row_4);
    hud_assert_true(col_3 <= row_5);
    hud_assert_false(col_3 <= row_6);
    hud_assert_true(col_3 <= row_7);
    hud_assert_false(col_3 <= row_8);
    hud_assert_false(col_3 <= row_9);

    hud_assert_false(col_4 <= row_0);
    hud_assert_false(col_4 <= row_1);
    hud_assert_true(col_4 <= row_2);
    hud_assert_true(col_4 <= row_3);
    hud_assert_true(col_4 <= row_4);
    hud_assert_true(col_4 <= row_5);
    // hud_assert_false(col_4 <= row_6);  // c not comparable with d
    // hud_assert_false(col_4 <= row_7);  // c not comparable with d
    hud_assert_false(col_4 <= row_8);
    hud_assert_true(col_4 <= row_9);

    hud_assert_false(col_5 <= row_0);
    hud_assert_false(col_5 <= row_1);
    hud_assert_false(col_5 <= row_2);
    hud_assert_true(col_5 <= row_3);
    hud_assert_false(col_5 <= row_4);
    hud_assert_true(col_5 <= row_5);
    // hud_assert_false(col_5 <= row_6); // c not comparable with d
    // hud_assert_false(col_5 <= row_7); // c not comparable with d
    hud_assert_false(col_5 <= row_8);
    hud_assert_false(col_5 <= row_9);

    hud_assert_false(col_6 <= row_0);
    hud_assert_false(col_6 <= row_1);
    hud_assert_true(col_6 <= row_2);
    hud_assert_true(col_6 <= row_3);
    // hud_assert_false(col_6 <= row_4); // d not comparable with c
    // hud_assert_false(col_6 <= row_5); // d not comparable with c
    hud_assert_true(col_6 <= row_6);
    hud_assert_true(col_6 <= row_7);
    hud_assert_false(col_6 <= row_8);
    hud_assert_true(col_6 <= row_9);

    hud_assert_false(col_7 <= row_0);
    hud_assert_false(col_7 <= row_1);
    hud_assert_false(col_7 <= row_2);
    hud_assert_true(col_7 <= row_3);
    // hud_assert_false(col_7 <= row_4); // d not comparable with c
    // hud_assert_false(col_7 <= row_5); // d not comparable with c
    hud_assert_false(col_7 <= row_6);
    hud_assert_true(col_7 <= row_7);
    hud_assert_false(col_7 <= row_8);
    hud_assert_false(col_7 <= row_9);

    hud_assert_false(col_8 <= row_0);
    hud_assert_true(col_8 <= row_1);
    hud_assert_true(col_8 <= row_2);
    hud_assert_true(col_8 <= row_3);
    hud_assert_true(col_8 <= row_4);
    hud_assert_true(col_8 <= row_5);
    hud_assert_true(col_8 <= row_6);
    hud_assert_true(col_8 <= row_7);
    hud_assert_true(col_8 <= row_8);
    hud_assert_true(col_8 <= row_9);

    hud_assert_false(col_9 <= row_0);
    hud_assert_false(col_9 <= row_1);
    hud_assert_true(col_9 <= row_2);
    hud_assert_true(col_9 <= row_3);
    hud_assert_true(col_9 <= row_4);
    hud_assert_true(col_9 <= row_5);
    hud_assert_true(col_9 <= row_6);
    hud_assert_true(col_9 <= row_7);
    hud_assert_false(col_9 <= row_8);
    hud_assert_true(col_9 <= row_9);
}

GTEST_TEST(vector, greater_equal_operator)
{
    /**
        Testing by resolving the following truth table
        where c and d are not bitwise comparable
        And c is not comparable with d

    ---------------------------------------------------------------------------------------------------------------------------------
    |              | [] | [1] | [1,2] | [1,3] | [c(1), c(2)] | c(1), c(3) | [d(1), d(2)] | [d(1), d(3)] |  (u8) [1]  |  (u8) [1,2]  |
    ---------------------------------------------------------------------------------------------------------------------------------
    | []           |  1 |  1  |   1   |   1   |      1       |     1      |      1       |      1       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1]          |  0 |  1  |   1   |   1   |      1       |     1      |      1       |      1       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,2]        |  0 |  0  |   1   |   1   |      1       |     1      |      1       |      1       |     0      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,3]        |  0 |  0  |   0   |   1   |      0       |     1      |      0       |      1       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [c(1), c(2)] |  0 |  0  |   1   |   1   |      1       |     1      |     ---      |     ---      |     0      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [c(1), c(3)] |  0 |  0  |   0   |   1   |      0       |     1      |     ---      |     ---      |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [d(1), d(2)] |  0 |  0  |   1   |   1   |     ---      |    ---     |      1       |      1       |     0      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [d(1), d(3)] |  0 |  0  |   0   |   1   |     ---      |    ---     |      0       |      1       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |   (u8) [1]   |  0 |  1  |   1   |   1   |      1       |     1      |      1       |      1       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |  (u8) [1,2]  |  0 |  0  |   1   |   1   |      1       |     1      |      1       |      1       |     0      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    */

    using c = hud_test::c;
    using d = hud_test::d;

    hud::vector<i32> col_0;
    hud::vector<i32> row_0;
    hud::vector<i32> col_1({1});
    hud::vector<i32> row_1({1});
    hud::vector<i32> col_2({1, 2});
    hud::vector<i32> row_2({1, 2});
    hud::vector<i32> col_3({1, 3});
    hud::vector<i32> row_3({1, 3});
    hud::vector<c> col_4({c {1}, c {2}});
    hud::vector<c> row_4({c {1}, c {2}});
    hud::vector<c> col_5({c {1}, c {3}});
    hud::vector<c> row_5({c {1}, c {3}});
    hud::vector<d> col_6({d {1}, d {2}});
    hud::vector<d> row_6({d {1}, d {2}});
    hud::vector<d> col_7({d {1}, d {3}});
    hud::vector<d> row_7({d {1}, d {3}});
    hud::vector<u8> col_8({u8(1)});
    hud::vector<u8> row_8({u8(1)});
    hud::vector<u8> col_9({u8(1), u8(2)});
    hud::vector<u8> row_9({u8(1), u8(2)});

    // Verify the truth table
    hud_assert_true(col_0 >= row_0);
    hud_assert_false(col_0 >= row_1);
    hud_assert_false(col_0 >= row_2);
    hud_assert_false(col_0 >= row_3);
    hud_assert_false(col_0 >= row_4);
    hud_assert_false(col_0 >= row_5);
    hud_assert_false(col_0 >= row_6);
    hud_assert_false(col_0 >= row_7);
    hud_assert_false(col_0 >= row_8);
    hud_assert_false(col_0 >= row_9);

    hud_assert_true(col_1 >= row_0);
    hud_assert_true(col_1 >= row_1);
    hud_assert_false(col_1 >= row_2);
    hud_assert_false(col_1 >= row_3);
    hud_assert_false(col_1 >= row_4);
    hud_assert_false(col_1 >= row_5);
    hud_assert_false(col_1 >= row_6);
    hud_assert_false(col_1 >= row_7);
    hud_assert_true(col_1 >= row_8);
    hud_assert_false(col_1 >= row_9);

    hud_assert_true(col_2 >= row_0);
    hud_assert_true(col_2 >= row_1);
    hud_assert_true(col_2 >= row_2);
    hud_assert_false(col_2 >= row_3);
    hud_assert_true(col_2 >= row_4);
    hud_assert_false(col_2 >= row_5);
    hud_assert_true(col_2 >= row_6);
    hud_assert_false(col_2 >= row_7);
    hud_assert_true(col_2 >= row_8);
    hud_assert_true(col_2 >= row_9);

    hud_assert_true(col_3 >= row_0);
    hud_assert_true(col_3 >= row_1);
    hud_assert_true(col_3 >= row_2);
    hud_assert_true(col_3 >= row_3);
    hud_assert_true(col_3 >= row_4);
    hud_assert_true(col_3 >= row_5);
    hud_assert_true(col_3 >= row_6);
    hud_assert_true(col_3 >= row_7);
    hud_assert_true(col_3 >= row_8);
    hud_assert_true(col_3 >= row_9);

    hud_assert_true(col_4 >= row_0);
    hud_assert_true(col_4 >= row_1);
    hud_assert_true(col_4 >= row_2);
    hud_assert_false(col_4 >= row_3);
    hud_assert_true(col_4 >= row_4);
    hud_assert_false(col_4 >= row_5);
    // hud_assert_false(col_4 >= row_6);  // c not comparable with d
    // hud_assert_false(col_4 >= row_7);  // c not comparable with d
    hud_assert_true(col_4 >= row_8);
    hud_assert_true(col_4 >= row_9);

    hud_assert_true(col_5 >= row_0);
    hud_assert_true(col_5 >= row_1);
    hud_assert_true(col_5 >= row_2);
    hud_assert_true(col_5 >= row_3);
    hud_assert_true(col_5 >= row_4);
    hud_assert_true(col_5 >= row_5);
    // hud_assert_false(col_5 >= row_6); // c not comparable with d
    // hud_assert_false(col_5 >= row_7); // c not comparable with d
    hud_assert_true(col_5 >= row_8);
    hud_assert_true(col_5 >= row_9);

    hud_assert_true(col_6 >= row_0);
    hud_assert_true(col_6 >= row_1);
    hud_assert_true(col_6 >= row_2);
    hud_assert_false(col_6 >= row_3);
    // hud_assert_false(col_6 >= row_4); // d not comparable with c
    // hud_assert_false(col_6 >= row_5); // d not comparable with c
    hud_assert_true(col_6 >= row_6);
    hud_assert_false(col_6 >= row_7);
    hud_assert_true(col_6 >= row_8);
    hud_assert_true(col_6 >= row_9);

    hud_assert_true(col_7 >= row_0);
    hud_assert_true(col_7 >= row_1);
    hud_assert_true(col_7 >= row_2);
    hud_assert_true(col_7 >= row_3);
    // hud_assert_false(col_7 >= row_4); // d not comparable with c
    // hud_assert_false(col_7 >= row_5); // d not comparable with c
    hud_assert_true(col_7 >= row_6);
    hud_assert_true(col_7 >= row_7);
    hud_assert_true(col_7 >= row_8);
    hud_assert_true(col_7 >= row_9);

    hud_assert_true(col_8 >= row_0);
    hud_assert_true(col_8 >= row_1);
    hud_assert_false(col_8 >= row_2);
    hud_assert_false(col_8 >= row_3);
    hud_assert_false(col_8 >= row_4);
    hud_assert_false(col_8 >= row_5);
    hud_assert_false(col_8 >= row_6);
    hud_assert_false(col_8 >= row_7);
    hud_assert_true(col_8 >= row_8);
    hud_assert_false(col_8 >= row_9);

    hud_assert_true(col_9 >= row_0);
    hud_assert_true(col_9 >= row_1);
    hud_assert_true(col_9 >= row_2);
    hud_assert_false(col_9 >= row_3);
    hud_assert_true(col_9 >= row_4);
    hud_assert_false(col_9 >= row_5);
    hud_assert_true(col_9 >= row_6);
    hud_assert_false(col_9 >= row_7);
    hud_assert_true(col_9 >= row_8);
    hud_assert_true(col_9 >= row_9);
}