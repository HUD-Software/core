#include <core/containers/array.h>


namespace hud::test {
    struct c {
        hud::i32 value;
    };

    struct D {
        hud::i32 value;
    };

    // Equals
    [[nodiscard]]
    constexpr bool operator==(const D& left, const D& right) noexcept {
        return left.value == right.value;
    }
    [[nodiscard]]
    constexpr bool operator==(const D& left, const hud::i32& right) noexcept {
        return left.value == right;
    }
    [[nodiscard]]
    constexpr bool operator==(const hud::i32& left, const D& right) noexcept {
        return left == right.value;
    }

    // Not equals
    [[nodiscard]]
    constexpr bool operator!=(const D& left, const D& right) noexcept {
        return left.value != right.value;
    }
    [[nodiscard]]
    constexpr bool operator!=(const D& left, const hud::i32& right) noexcept {
        return left.value != right;
    }
    [[nodiscard]]
    constexpr bool operator!=(const hud::i32& left, const D& right) noexcept {
        return left != right.value;
    }

    // Less
    [[nodiscard]]
    constexpr bool operator<(const C& left, const C& right) noexcept {
        return left.value < right.value;
    }
    [[nodiscard]]
    constexpr bool operator<(const C& left, const hud::i32& right) noexcept {
        return left.value < right;
    }
    [[nodiscard]]
    constexpr bool operator<(const hud::i32& left, const C& right) noexcept {
        return left < right.value;
    }

    [[nodiscard]]
    constexpr bool operator<(const D& left, const D& right) noexcept {
        return left.value < right.value;
    }
    [[nodiscard]]
    constexpr bool operator<(const D& left, const hud::i32& right) noexcept {
        return left.value < right;
    }
    [[nodiscard]]
    constexpr bool operator<(const hud::i32& left, const D& right) noexcept {
        return left < right.value;
    }
}

namespace hud {
    template<>
    struct IsBitwiseComparable<hud::test::C,hud::test::C> : hud::true_type
    {};
    template<>
    struct IsBitwiseComparable<hud::test::C, i32> : hud::true_type
    {};
    template<>
    struct IsBitwiseComparable<i32, hud::test::C> : hud::true_type
    {};
}

TEST(Array, equal_operator)
{
    /**
    *
        Testing by resolving the following truth table
        where C is bitwise comparable and D is not bitwise comparable
        And C is not comparable with D

        -----------------------------------------------------------------------------------------------------
        |              | [] | [1] | [1,2] | [1,3] | [C(1), C(2)] | C(1), C(3) | [D(1), D(2)] | [D(1), D(3)] |
        -----------------------------------------------------------------------------------------------------
        | []           |  1 |  0  |   0   |   0   |      0       |     0      |      0       |      0       |
        -----------------------------------------------------------------------------------------------------
        | [1]          |  0 |  1  |   0   |   0   |      0       |     0      |      0       |      0       |
        -----------------------------------------------------------------------------------------------------
        | [1,2]        |  0 |  0  |   1   |   0   |      1       |     0      |      1       |      0       |
        -----------------------------------------------------------------------------------------------------
        | [1,3]        |  0 |  0  |   0   |   1   |      0       |     1      |      0       |      1       |
        -----------------------------------------------------------------------------------------------------
        | [C(1), C(2)] |  0 |  0  |   1   |   0   |      1       |     0      |     ---      |     ---      |
        -----------------------------------------------------------------------------------------------------
        | [C(1), C(3)] |  0 |  0  |   0   |   1   |      0       |     1      |     ---      |     ---      |
        -----------------------------------------------------------------------------------------------------
        | [D(1), D(2)] |  0 |  0  |   1   |   0   |     ---      |    ---     |      1       |      0       |
        -----------------------------------------------------------------------------------------------------
        | [D(1), D(3)] |  0 |  0  |   0   |   1   |     ---      |    ---     |      0       |      1       |
        -----------------------------------------------------------------------------------------------------
    */

    using C = hud::test::C;
    using D = hud::test::D;

    hud::array<i32> col_0;
    hud::array<i32> row_0;
    hud::array<i32> col_1({ 1 });
    hud::array<i32> row_1({ 1 });
    hud::array<i32> col_2({ 1,2 });
    hud::array<i32> row_2({ 1,2 });
    hud::array<i32> col_3({ 1,3 });
    hud::array<i32> row_3({ 1,3 });
    hud::array<c> col_4({ C{1},C{2} });
    hud::array<c> row_4({ C{1},C{2} });
    hud::array<c> col_5({ C{1},C{3} });
    hud::array<c> row_5({ C{1},C{3} });
    hud::array<D> col_6({ D{1},D{2} });
    hud::array<D> row_6({ D{1},D{2} });
    hud::array<D> col_7({ D{1},D{3} });
    hud::array<D> row_7({ D{1},D{3} });

    // Verify the truth table
    ASSERT_TRUE(col_0 == row_0);
    ASSERT_FALSE(col_0 == row_1);
    ASSERT_FALSE(col_0 == row_2);
    ASSERT_FALSE(col_0 == row_3);
    ASSERT_FALSE(col_0 == row_4);
    ASSERT_FALSE(col_0 == row_5);
    ASSERT_FALSE(col_0 == row_6);
    ASSERT_FALSE(col_0 == row_7);

    ASSERT_FALSE(col_1 == row_0);
    ASSERT_TRUE(col_1 == row_1);
    ASSERT_FALSE(col_1 == row_2);
    ASSERT_FALSE(col_1 == row_3);
    ASSERT_FALSE(col_1 == row_4);
    ASSERT_FALSE(col_1 == row_5);
    ASSERT_FALSE(col_1 == row_6);
    ASSERT_FALSE(col_1 == row_7);

    ASSERT_FALSE(col_2 == row_0);
    ASSERT_FALSE(col_2 == row_1);
    ASSERT_TRUE(col_2 == row_2);
    ASSERT_FALSE(col_2 == row_3);
    ASSERT_TRUE(col_2 == row_4);
    ASSERT_FALSE(col_2 == row_5);
    ASSERT_TRUE(col_2 == row_6);
    ASSERT_FALSE(col_2 == row_7);

    ASSERT_FALSE(col_3 == row_0);
    ASSERT_FALSE(col_3 == row_1);
    ASSERT_FALSE(col_3 == row_2);
    ASSERT_TRUE(col_3 == row_3);
    ASSERT_FALSE(col_3 == row_4);
    ASSERT_TRUE(col_3 == row_5);
    ASSERT_FALSE(col_3 == row_6);
    ASSERT_TRUE(col_3 == row_7);

    ASSERT_FALSE(col_4 == row_0);
    ASSERT_FALSE(col_4 == row_1);
    ASSERT_TRUE(col_4 == row_2);
    ASSERT_FALSE(col_4 == row_3);
    ASSERT_TRUE(col_4 == row_4);
    ASSERT_FALSE(col_4 == row_5);
    //ASSERT_FALSE(col_4 == row_6);  // C not comparable with D
    //ASSERT_FALSE(col_4 == row_7);  // C not comparable with D

    ASSERT_FALSE(col_5 == row_0);
    ASSERT_FALSE(col_5 == row_1);
    ASSERT_FALSE(col_5 == row_2);
    ASSERT_TRUE(col_5 == row_3);
    ASSERT_FALSE(col_5 == row_4);
    ASSERT_TRUE(col_5 == row_5);
    //Assert::IsFalse(col_5 == row_6); // C not comparable with D
    //Assert::IsFalse(col_5 == row_7); // C not comparable with D

    ASSERT_FALSE(col_6 == row_0);
    ASSERT_FALSE(col_6 == row_1);
    ASSERT_TRUE(col_6 == row_2);
    ASSERT_FALSE(col_6 == row_3);
    //ASSERT_FALSE(col_6 == row_4); // D not comparable with C
    //ASSERT_FALSE(col_6 == row_5); // D not comparable with C
    ASSERT_TRUE(col_6 == row_6);
    ASSERT_FALSE(col_6 == row_7);

    ASSERT_FALSE(col_7 == row_0);
    ASSERT_FALSE(col_7 == row_1);
    ASSERT_FALSE(col_7 == row_2);
    ASSERT_TRUE(col_7 == row_3);
    //ASSERT_FALSE(col_7 == row_4); // D not comparable with C
    //ASSERT_FALSE(col_7 == row_5); // D not comparable with C
    ASSERT_FALSE(col_7 == row_6);
    ASSERT_TRUE(col_7 == row_7);
}

TEST(Array, not_equal_operator)
{
    /**
    *
        Testing by resolving the following truth table
        where C is bitwise comparable and D is not bitwise comparable
        And C is not comparable with D

    -----------------------------------------------------------------------------------------------------
    |              | [] | [1] | [1,2] | [1,3] | [C(1), C(2)] | C(1), C(3) | [D(1), D(2)] | [D(1), D(3)] |
    -----------------------------------------------------------------------------------------------------
    | []           |  0 |  1  |   1   |   1   |      1       |     1      |      1       |      1       |
    -----------------------------------------------------------------------------------------------------
    | [1]          |  1 |  0  |   1   |   1   |      1       |     1      |      1       |      1       |
    -----------------------------------------------------------------------------------------------------
    | [1,2]        |  1 |  1  |   0   |   1   |      0       |     1      |      0       |      1       |
    -----------------------------------------------------------------------------------------------------
    | [1,3]        |  1 |  1  |   1   |   0   |      1       |     0      |      1       |      0       |
    -----------------------------------------------------------------------------------------------------
    | [C(1), C(2)] |  1 |  1  |   0   |   1   |      0       |     1      |     ---      |     ---      |
    -----------------------------------------------------------------------------------------------------
    | [C(1), C(3)] |  1 |  1  |   1   |   0   |      1       |     0      |     ---      |     ---      |
    -----------------------------------------------------------------------------------------------------
    | [D(1), D(2)] |  1 |  1  |   0   |   1   |     ---      |    ---     |      0       |      1       |
    -----------------------------------------------------------------------------------------------------
    | [D(1), D(3)] |  1 |  1  |   1   |   0   |     ---      |    ---     |      1       |      0       |
    -----------------------------------------------------------------------------------------------------
    */

    using C = hud::test::C;
    using D = hud::test::D;

    hud::array<i32> col_0;
    hud::array<i32> row_0;
    hud::array<i32> col_1({ 1 });
    hud::array<i32> row_1({ 1 });
    hud::array<i32> col_2({ 1,2 });
    hud::array<i32> row_2({ 1,2 });
    hud::array<i32> col_3({ 1,3 });
    hud::array<i32> row_3({ 1,3 });
    hud::array<c> col_4({ C{1},C{2} });
    hud::array<c> row_4({ C{1},C{2} });
    hud::array<c> col_5({ C{1},C{3} });
    hud::array<c> row_5({ C{1},C{3} });
    hud::array<D> col_6({ D{1},D{2} });
    hud::array<D> row_6({ D{1},D{2} });
    hud::array<D> col_7({ D{1},D{3} });
    hud::array<D> row_7({ D{1},D{3} });

    // Verify the truth table
    ASSERT_FALSE(col_0 != row_0);
    ASSERT_TRUE(col_0 != row_1);
    ASSERT_TRUE(col_0 != row_2);
    ASSERT_TRUE(col_0 != row_3);
    ASSERT_TRUE(col_0 != row_4);
    ASSERT_TRUE(col_0 != row_5);
    ASSERT_TRUE(col_0 != row_6);
    ASSERT_TRUE(col_0 != row_7);

    ASSERT_TRUE(col_1 != row_0);
    ASSERT_FALSE(col_1 != row_1);
    ASSERT_TRUE(col_1 != row_2);
    ASSERT_TRUE(col_1 != row_3);
    ASSERT_TRUE(col_1 != row_4);
    ASSERT_TRUE(col_1 != row_5);
    ASSERT_TRUE(col_1 != row_6);
    ASSERT_TRUE(col_1 != row_7);

    ASSERT_TRUE(col_2 != row_0);
    ASSERT_TRUE(col_2 != row_1);
    ASSERT_FALSE(col_2 != row_2);
    ASSERT_TRUE(col_2 != row_3);
    ASSERT_FALSE(col_2 != row_4);
    ASSERT_TRUE(col_2 != row_5);
    ASSERT_FALSE(col_2 != row_6);
    ASSERT_TRUE(col_2 != row_7);

    ASSERT_TRUE(col_3 != row_0);
    ASSERT_TRUE(col_3 != row_1);
    ASSERT_TRUE(col_3 != row_2);
    ASSERT_FALSE(col_3 != row_3);
    ASSERT_TRUE(col_3 != row_4);
    ASSERT_FALSE(col_3 != row_5);
    ASSERT_TRUE(col_3 != row_6);
    ASSERT_FALSE(col_3 != row_7);

    ASSERT_TRUE(col_4 != row_0);
    ASSERT_TRUE(col_4 != row_1);
    ASSERT_FALSE(col_4 != row_2);
    ASSERT_TRUE(col_4 != row_3);
    ASSERT_FALSE(col_4 != row_4);
    ASSERT_TRUE(col_4 != row_5);
    //ASSERT_FALSE(col_4 != row_6);  // C not comparable with D
    //ASSERT_FALSE(col_4 != row_7);  // C not comparable with D

    ASSERT_TRUE(col_5 != row_0);
    ASSERT_TRUE(col_5 != row_1);
    ASSERT_TRUE(col_5 != row_2);
    ASSERT_FALSE(col_5 != row_3);
    ASSERT_TRUE(col_5 != row_4);
    ASSERT_FALSE(col_5 != row_5);
    //ASSERT_FALSE(col_5 != row_6); // C not comparable with D
    //ASSERT_FALSE(col_5 != row_7); // C not comparable with D

    ASSERT_TRUE(col_6 != row_0);
    ASSERT_TRUE(col_6 != row_1);
    ASSERT_FALSE(col_6 != row_2);
    ASSERT_TRUE(col_6 != row_3);
    //ASSERT_FALSE(col_6 != row_4); // D not comparable with C
    //ASSERT_FALSE(col_6 != row_5); // D not comparable with C
    ASSERT_FALSE(col_6 != row_6);
    ASSERT_TRUE(col_6 != row_7);

    ASSERT_TRUE(col_7 != row_0);
    ASSERT_TRUE(col_7 != row_1);
    ASSERT_TRUE(col_7 != row_2);
    ASSERT_FALSE(col_7 != row_3);
    //ASSERT_FALSE(col_7 != row_4); // D not comparable with C
    //ASSERT_FALSE(col_7 != row_5); // D not comparable with C
    ASSERT_TRUE(col_7 != row_6);
    ASSERT_FALSE(col_7 != row_7);
}

TEST(Array, less_operator)
{
    /**
        Testing by resolving the following truth table
        where C and D are not bitwise comparable
        And C is not comparable with D

    ---------------------------------------------------------------------------------------------------------------------------------
    |              | [] | [1] | [1,2] | [1,3] | [C(1), C(2)] | C(1), C(3) | [D(1), D(2)] | [D(1), D(3)] |  (u8) [1]  |  (u8) [1,2]  |
    ---------------------------------------------------------------------------------------------------------------------------------
    | []           |  0 |  0  |   0   |   0   |      0       |     0      |      0       |      0       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1]          |  1 |  0  |   0   |   0   |      0       |     0      |      0       |      0       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,2]        |  1 |  1  |   0   |   0   |      0       |     0      |      0       |      0       |     1      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,3]        |  1 |  1  |   1   |   0   |      1       |     0      |      1       |      0       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [C(1), C(2)] |  1 |  1  |   0   |   0   |      0       |     0      |     ---      |     ---      |     1      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [C(1), C(3)] |  1 |  1  |   1   |   0   |      1       |     0      |     ---      |     ---      |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [D(1), D(2)] |  1 |  1  |   0   |   0   |     ---      |    ---     |      0       |      0       |     1      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [D(1), D(3)] |  1 |  1  |   1   |   0   |     ---      |    ---     |      1       |      0       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |   (u8) [1]   |  1 |  0  |   0   |   0   |      0       |     0      |      0       |      0       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |  (u8) [1,2]  |  1 |  1  |   0   |   0   |      0       |     0      |      0       |      0       |     1      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    */

    using C = hud::test::C;
    using D = hud::test::D;

    hud::array<i32> col_0;
    hud::array<i32> row_0;
    hud::array<i32> col_1({ 1 });
    hud::array<i32> row_1({ 1 });
    hud::array<i32> col_2({ 1,2 });
    hud::array<i32> row_2({ 1,2 });
    hud::array<i32> col_3({ 1,3 });
    hud::array<i32> row_3({ 1,3 });
    hud::array<c> col_4({ C{1}, C{2} });
    hud::array<c> row_4({ C{1}, C{2} });
    hud::array<c> col_5({ C{1}, C{3} });
    hud::array<c> row_5({ C{1}, C{3} });
    hud::array<D> col_6({ D{1}, D{2} });
    hud::array<D> row_6({ D{1}, D{2} });
    hud::array<D> col_7({ D{1}, D{3} });
    hud::array<D> row_7({ D{1}, D{3} });
    hud::array<u8> col_8({ u8(1) });
    hud::array<u8> row_8({ u8(1) });
    hud::array<u8> col_9({ u8(1), u8(2) });
    hud::array<u8> row_9({ u8(1), u8(2) });

    // Verify the truth table
    ASSERT_FALSE(col_0 < row_0);
    ASSERT_TRUE(col_0 < row_1);
    ASSERT_TRUE(col_0 < row_2);
    ASSERT_TRUE(col_0 < row_3);
    ASSERT_TRUE(col_0 < row_4);
    ASSERT_TRUE(col_0 < row_5);
    ASSERT_TRUE(col_0 < row_6);
    ASSERT_TRUE(col_0 < row_7);
    ASSERT_TRUE(col_0 < row_8);
    ASSERT_TRUE(col_0 < row_9);

    ASSERT_FALSE(col_1 < row_0);
    ASSERT_FALSE(col_1 < row_1);
    ASSERT_TRUE(col_1 < row_2);
    ASSERT_TRUE(col_1 < row_3);
    ASSERT_TRUE(col_1 < row_4);
    ASSERT_TRUE(col_1 < row_5);
    ASSERT_TRUE(col_1 < row_6);
    ASSERT_TRUE(col_1 < row_7);
    ASSERT_FALSE(col_1 < row_8);
    ASSERT_TRUE(col_1 < row_9);

    ASSERT_FALSE(col_2 < row_0);
    ASSERT_FALSE(col_2 < row_1);
    ASSERT_FALSE(col_2 < row_2);
    ASSERT_TRUE(col_2 < row_3);
    ASSERT_FALSE(col_2 < row_4);
    ASSERT_TRUE(col_2 < row_5);
    ASSERT_FALSE(col_2 < row_6);
    ASSERT_TRUE(col_2 < row_7);
    ASSERT_FALSE(col_2 < row_8);
    ASSERT_FALSE(col_2 < row_9);

    ASSERT_FALSE(col_3 < row_0);
    ASSERT_FALSE(col_3 < row_1);
    ASSERT_FALSE(col_3 < row_2);
    ASSERT_FALSE(col_3 < row_3);
    ASSERT_FALSE(col_3 < row_4);
    ASSERT_FALSE(col_3 < row_5);
    ASSERT_FALSE(col_3 < row_6);
    ASSERT_FALSE(col_3 < row_7);
    ASSERT_FALSE(col_3 < row_8);
    ASSERT_FALSE(col_3 < row_9);

    ASSERT_FALSE(col_4 < row_0);
    ASSERT_FALSE(col_4 < row_1);
    ASSERT_FALSE(col_4 < row_2);
    ASSERT_TRUE(col_4 < row_3);
    ASSERT_FALSE(col_4 < row_4);
    ASSERT_TRUE(col_4 < row_5);
    //ASSERT_FALSE(col_4 < row_6);  // C not comparable with D
    //ASSERT_FALSE(col_4 < row_7);  // C not comparable with D
    ASSERT_FALSE(col_4 < row_8);
    ASSERT_FALSE(col_4 < row_9);

    ASSERT_FALSE(col_5 < row_0);
    ASSERT_FALSE(col_5 < row_1);
    ASSERT_FALSE(col_5 < row_2);
    ASSERT_FALSE(col_5 < row_3);
    ASSERT_FALSE(col_5 < row_4);
    ASSERT_FALSE(col_5 < row_5);
    //ASSERT_FALSE(col_5 < row_6); // C not comparable with D
    //ASSERT_FALSE(col_5 < row_7); // C not comparable with D
    ASSERT_FALSE(col_5 < row_8);
    ASSERT_FALSE(col_5 < row_9);

    ASSERT_FALSE(col_6 < row_0);
    ASSERT_FALSE(col_6 < row_1);
    ASSERT_FALSE(col_6 < row_2);
    ASSERT_TRUE(col_6 < row_3);
    //ASSERT_FALSE(col_6 < row_4); // D not comparable with C
    //ASSERT_FALSE(col_6 < row_5); // D not comparable with C
    ASSERT_FALSE(col_6 < row_6);
    ASSERT_TRUE(col_6 < row_7);
    ASSERT_FALSE(col_6 < row_8);
    ASSERT_FALSE(col_6 < row_9);

    ASSERT_FALSE(col_7 < row_0);
    ASSERT_FALSE(col_7 < row_1);
    ASSERT_FALSE(col_7 < row_2);
    ASSERT_FALSE(col_7 < row_3);
    //ASSERT_FALSE(col_7 < row_4); // D not comparable with C
    //ASSERT_FALSE(col_7 < row_5); // D not comparable with C
    ASSERT_FALSE(col_7 < row_6);
    ASSERT_FALSE(col_7 < row_7);
    ASSERT_FALSE(col_7 < row_8);
    ASSERT_FALSE(col_7 < row_9);

    ASSERT_FALSE(col_8 < row_0);
    ASSERT_FALSE(col_8 < row_1);
    ASSERT_TRUE(col_8 < row_2);
    ASSERT_TRUE(col_8 < row_3);
    ASSERT_TRUE(col_8 < row_4);
    ASSERT_TRUE(col_8 < row_5);
    ASSERT_TRUE(col_8 < row_6);
    ASSERT_TRUE(col_8 < row_7);
    ASSERT_FALSE(col_8 < row_8);
    ASSERT_TRUE(col_8 < row_9);

    ASSERT_FALSE(col_9 < row_0);
    ASSERT_FALSE(col_9 < row_1);
    ASSERT_FALSE(col_9 < row_2);
    ASSERT_TRUE(col_9 < row_3);
    ASSERT_FALSE(col_9 < row_4);
    ASSERT_TRUE(col_9 < row_5);
    ASSERT_FALSE(col_9 < row_6);
    ASSERT_TRUE(col_9 < row_7);
    ASSERT_FALSE(col_9 < row_8);
    ASSERT_FALSE(col_9 < row_9);
}

TEST(Array, greater_operator)
{
    /**
        Testing by resolving the following truth table
        where C and D are not bitwise comparable
        And C is not comparable with D

    ---------------------------------------------------------------------------------------------------------------------------------
    |              | [] | [1] | [1,2] | [1,3] | [C(1), C(2)] | C(1), C(3) | [D(1), D(2)] | [D(1), D(3)] |  (u8) [1]  |  (u8) [1,2]  |
    ---------------------------------------------------------------------------------------------------------------------------------
    | []           |  0 |  1  |   1   |   1   |      1       |     1      |      1       |      1       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1]          |  0 |  0  |   1   |   1   |      1       |     1      |      1       |      1       |     0      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,2]        |  0 |  0  |   0   |   1   |      0       |     1      |      0       |      1       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,3]        |  0 |  0  |   0   |   0   |      0       |     0      |      0       |      0       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [C(1), C(2)] |  0 |  0  |   0   |   1   |      0       |     1      |     ---      |     ---      |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [C(1), C(3)] |  0 |  0  |   0   |   0   |      0       |     0      |     ---      |     ---      |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [D(1), D(2)] |  0 |  0  |   0   |   1   |     ---      |    ---     |      0       |      1       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [D(1), D(3)] |  0 |  0  |   0   |   0   |     ---      |    ---     |      0       |      0       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |   (u8) [1]   |  0 |  0  |   1   |   1   |      1       |     1      |      1       |      1       |     0      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |  (u8) [1,2]  |  0 |  0  |   0   |   1   |      0       |     1      |      0       |      1       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    */

    using C = hud::test::C;
    using D = hud::test::D;

    hud::array<i32> col_0;
    hud::array<i32> row_0;
    hud::array<i32> col_1({ 1 });
    hud::array<i32> row_1({ 1 });
    hud::array<i32> col_2({ 1,2 });
    hud::array<i32> row_2({ 1,2 });
    hud::array<i32> col_3({ 1,3 });
    hud::array<i32> row_3({ 1,3 });
    hud::array<c> col_4({ C{1}, C{2} });
    hud::array<c> row_4({ C{1}, C{2} });
    hud::array<c> col_5({ C{1}, C{3} });
    hud::array<c> row_5({ C{1}, C{3} });
    hud::array<D> col_6({ D{1}, D{2} });
    hud::array<D> row_6({ D{1}, D{2} });
    hud::array<D> col_7({ D{1}, D{3} });
    hud::array<D> row_7({ D{1}, D{3} });
    hud::array<u8> col_8({ u8(1) });
    hud::array<u8> row_8({ u8(1) });
    hud::array<u8> col_9({ u8(1), u8(2) });
    hud::array<u8> row_9({ u8(1), u8(2) });

    // Verify the truth table
    ASSERT_FALSE(col_0 > row_0);
    ASSERT_FALSE(col_0 > row_1);
    ASSERT_FALSE(col_0 > row_2);
    ASSERT_FALSE(col_0 > row_3);
    ASSERT_FALSE(col_0 > row_4);
    ASSERT_FALSE(col_0 > row_5);
    ASSERT_FALSE(col_0 > row_6);
    ASSERT_FALSE(col_0 > row_7);
    ASSERT_FALSE(col_0 > row_8);
    ASSERT_FALSE(col_0 > row_9);

    ASSERT_TRUE(col_1 > row_0);
    ASSERT_FALSE(col_1 > row_1);
    ASSERT_FALSE(col_1 > row_2);
    ASSERT_FALSE(col_1 > row_3);
    ASSERT_FALSE(col_1 > row_4);
    ASSERT_FALSE(col_1 > row_5);
    ASSERT_FALSE(col_1 > row_6);
    ASSERT_FALSE(col_1 > row_7);
    ASSERT_FALSE(col_1 > row_8);
    ASSERT_FALSE(col_1 > row_9);

    ASSERT_TRUE(col_2 > row_0);
    ASSERT_TRUE(col_2 > row_1);
    ASSERT_FALSE(col_2 > row_2);
    ASSERT_FALSE(col_2 > row_3);
    ASSERT_FALSE(col_2 > row_4);
    ASSERT_FALSE(col_2 > row_5);
    ASSERT_FALSE(col_2 > row_6);
    ASSERT_FALSE(col_2 > row_7);
    ASSERT_TRUE(col_2 > row_8);
    ASSERT_FALSE(col_2 > row_9);

    ASSERT_TRUE(col_3 > row_0);
    ASSERT_TRUE(col_3 > row_1);
    ASSERT_TRUE(col_3 > row_2);
    ASSERT_FALSE(col_3 > row_3);
    ASSERT_TRUE(col_3 > row_4);
    ASSERT_FALSE(col_3 > row_5);
    ASSERT_TRUE(col_3 > row_6);
    ASSERT_FALSE(col_3 > row_7);
    ASSERT_TRUE(col_3 > row_8);
    ASSERT_TRUE(col_3 > row_9);

    ASSERT_TRUE(col_4 > row_0);
    ASSERT_TRUE(col_4 > row_1);
    ASSERT_FALSE(col_4 > row_2);
    ASSERT_FALSE(col_4 > row_3);
    ASSERT_FALSE(col_4 > row_4);
    ASSERT_FALSE(col_4 > row_5);
    //ASSERT_FALSE(col_4 > row_6);  // C not comparable with D
    //ASSERT_FALSE(col_4 > row_7);  // C not comparable with D
    ASSERT_TRUE(col_4 > row_8);
    ASSERT_FALSE(col_4 > row_9);

    ASSERT_TRUE(col_5 > row_0);
    ASSERT_TRUE(col_5 > row_1);
    ASSERT_TRUE(col_5 > row_2);
    ASSERT_FALSE(col_5 > row_3);
    ASSERT_TRUE(col_5 > row_4);
    ASSERT_FALSE(col_5 > row_5);
    //ASSERT_FALSE(col_5 > row_6); // C not comparable with D
    //ASSERT_FALSE(col_5 > row_7); // C not comparable with D
    ASSERT_TRUE(col_5 > row_8);
    ASSERT_TRUE(col_5 > row_9);

    ASSERT_TRUE(col_6 > row_0);
    ASSERT_TRUE(col_6 > row_1);
    ASSERT_FALSE(col_6 > row_2);
    ASSERT_FALSE(col_6 > row_3);
    //ASSERT_FALSE(col_6 > row_4); // D not comparable with C
    //ASSERT_FALSE(col_6 > row_5); // D not comparable with C
    ASSERT_FALSE(col_6 > row_6);
    ASSERT_FALSE(col_6 > row_7);
    ASSERT_TRUE(col_6 > row_8);
    ASSERT_FALSE(col_6 > row_9);

    ASSERT_TRUE(col_7 > row_0);
    ASSERT_TRUE(col_7 > row_1);
    ASSERT_TRUE(col_7 > row_2);
    ASSERT_FALSE(col_7 > row_3);
    //ASSERT_FALSE(col_7 > row_4); // D not comparable with C
    //ASSERT_FALSE(col_7 > row_5); // D not comparable with C
    ASSERT_TRUE(col_7 > row_6);
    ASSERT_FALSE(col_7 > row_7);
    ASSERT_TRUE(col_7 > row_8);
    ASSERT_TRUE(col_7 > row_9);

    ASSERT_TRUE(col_8 > row_0);
    ASSERT_FALSE(col_8 > row_1);
    ASSERT_FALSE(col_8 > row_2);
    ASSERT_FALSE(col_8 > row_3);
    ASSERT_FALSE(col_8 > row_4);
    ASSERT_FALSE(col_8 > row_5);
    ASSERT_FALSE(col_8 > row_6);
    ASSERT_FALSE(col_8 > row_7);
    ASSERT_FALSE(col_8 > row_8);
    ASSERT_FALSE(col_8 > row_9);

    ASSERT_TRUE(col_9 > row_0);
    ASSERT_TRUE(col_9 > row_1);
    ASSERT_FALSE(col_9 > row_2);
    ASSERT_FALSE(col_9 > row_3);
    ASSERT_FALSE(col_9 > row_4);
    ASSERT_FALSE(col_9 > row_5);
    ASSERT_FALSE(col_9 > row_6);
    ASSERT_FALSE(col_9 > row_7);
    ASSERT_TRUE(col_9 > row_8);
    ASSERT_FALSE(col_9 > row_9);
}

TEST(Array, less_equal_operator)
{
    /**
        Testing by resolving the following truth table
        where C and D are not bitwise comparable
        And C is not comparable with D

    ---------------------------------------------------------------------------------------------------------------------------------
    |              | [] | [1] | [1,2] | [1,3] | [C(1), C(2)] | C(1), C(3) | [D(1), D(2)] | [D(1), D(3)] |  (u8) [1]  |  (u8) [1,2]  |
    ---------------------------------------------------------------------------------------------------------------------------------
    | []           |  1 |  0  |   0   |   0   |      0       |     0      |      0       |      0       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1]          |  1 |  1  |   0   |   0   |      0       |     0      |      0       |      0       |     1      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,2]        |  1 |  1  |   1   |   0   |      1       |     0      |      1       |      0       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,3]        |  1 |  1  |   1   |   1   |      1       |     1      |      1       |      1       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [C(1), C(2)] |  1 |  1  |   1   |   0   |      1       |     0      |     ---      |     ---      |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [C(1), C(3)] |  1 |  1  |   1   |   1   |      1       |     1      |     ---      |     ---      |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [D(1), D(2)] |  1 |  1  |   1   |   0   |     ---      |    ---     |      1       |      0       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [D(1), D(3)] |  1 |  1  |   1   |   1   |     ---      |    ---     |      1       |      1       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |   (u8) [1]   |  1 |  1  |   0   |   0   |      0       |     0      |      0       |      0       |     1      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |  (u8) [1,2]  |  1 |  1  |   1   |   0   |      1       |     0      |      1       |      0       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    */

    using C = hud::test::C;
    using D = hud::test::D;

    hud::array<i32> col_0;
    hud::array<i32> row_0;
    hud::array<i32> col_1({ 1 });
    hud::array<i32> row_1({ 1 });
    hud::array<i32> col_2({ 1,2 });
    hud::array<i32> row_2({ 1,2 });
    hud::array<i32> col_3({ 1,3 });
    hud::array<i32> row_3({ 1,3 });
    hud::array<c> col_4({ C{1}, C{2} });
    hud::array<c> row_4({ C{1}, C{2} });
    hud::array<c> col_5({ C{1}, C{3} });
    hud::array<c> row_5({ C{1}, C{3} });
    hud::array<D> col_6({ D{1}, D{2} });
    hud::array<D> row_6({ D{1}, D{2} });
    hud::array<D> col_7({ D{1}, D{3} });
    hud::array<D> row_7({ D{1}, D{3} });
    hud::array<u8> col_8({ u8(1) });
    hud::array<u8> row_8({ u8(1) });
    hud::array<u8> col_9({ u8(1), u8(2) });
    hud::array<u8> row_9({ u8(1), u8(2) });

    // Verify the truth table
    ASSERT_TRUE(col_0 <= row_0);
    ASSERT_TRUE(col_0 <= row_1);
    ASSERT_TRUE(col_0 <= row_2);
    ASSERT_TRUE(col_0 <= row_3);
    ASSERT_TRUE(col_0 <= row_4);
    ASSERT_TRUE(col_0 <= row_5);
    ASSERT_TRUE(col_0 <= row_6);
    ASSERT_TRUE(col_0 <= row_7);
    ASSERT_TRUE(col_0 <= row_8);
    ASSERT_TRUE(col_0 <= row_9);

    ASSERT_FALSE(col_1 <= row_0);
    ASSERT_TRUE(col_1 <= row_1);
    ASSERT_TRUE(col_1 <= row_2);
    ASSERT_TRUE(col_1 <= row_3);
    ASSERT_TRUE(col_1 <= row_4);
    ASSERT_TRUE(col_1 <= row_5);
    ASSERT_TRUE(col_1 <= row_6);
    ASSERT_TRUE(col_1 <= row_7);
    ASSERT_TRUE(col_1 <= row_8);
    ASSERT_TRUE(col_1 <= row_9);

    ASSERT_FALSE(col_2 <= row_0);
    ASSERT_FALSE(col_2 <= row_1);
    ASSERT_TRUE(col_2 <= row_2);
    ASSERT_TRUE(col_2 <= row_3);
    ASSERT_TRUE(col_2 <= row_4);
    ASSERT_TRUE(col_2 <= row_5);
    ASSERT_TRUE(col_2 <= row_6);
    ASSERT_TRUE(col_2 <= row_7);
    ASSERT_FALSE(col_2 <= row_8);
    ASSERT_TRUE(col_2 <= row_9);

    ASSERT_FALSE(col_3 <= row_0);
    ASSERT_FALSE(col_3 <= row_1);
    ASSERT_FALSE(col_3 <= row_2);
    ASSERT_TRUE(col_3 <= row_3);
    ASSERT_FALSE(col_3 <= row_4);
    ASSERT_TRUE(col_3 <= row_5);
    ASSERT_FALSE(col_3 <= row_6);
    ASSERT_TRUE(col_3 <= row_7);
    ASSERT_FALSE(col_3 <= row_8);
    ASSERT_FALSE(col_3 <= row_9);

    ASSERT_FALSE(col_4 <= row_0);
    ASSERT_FALSE(col_4 <= row_1);
    ASSERT_TRUE(col_4 <= row_2);
    ASSERT_TRUE(col_4 <= row_3);
    ASSERT_TRUE(col_4 <= row_4);
    ASSERT_TRUE(col_4 <= row_5);
    //ASSERT_FALSE(col_4 <= row_6);  // C not comparable with D
    //ASSERT_FALSE(col_4 <= row_7);  // C not comparable with D
    ASSERT_FALSE(col_4 <= row_8);
    ASSERT_TRUE(col_4 <= row_9);

    ASSERT_FALSE(col_5 <= row_0);
    ASSERT_FALSE(col_5 <= row_1);
    ASSERT_FALSE(col_5 <= row_2);
    ASSERT_TRUE(col_5 <= row_3);
    ASSERT_FALSE(col_5 <= row_4);
    ASSERT_TRUE(col_5 <= row_5);
    //ASSERT_FALSE(col_5 <= row_6); // C not comparable with D
    //ASSERT_FALSE(col_5 <= row_7); // C not comparable with D
    ASSERT_FALSE(col_5 <= row_8);
    ASSERT_FALSE(col_5 <= row_9);

    ASSERT_FALSE(col_6 <= row_0);
    ASSERT_FALSE(col_6 <= row_1);
    ASSERT_TRUE(col_6 <= row_2);
    ASSERT_TRUE(col_6 <= row_3);
    //ASSERT_FALSE(col_6 <= row_4); // D not comparable with C
    //ASSERT_FALSE(col_6 <= row_5); // D not comparable with C
    ASSERT_TRUE(col_6 <= row_6);
    ASSERT_TRUE(col_6 <= row_7);
    ASSERT_FALSE(col_6 <= row_8);
    ASSERT_TRUE(col_6 <= row_9);

    ASSERT_FALSE(col_7 <= row_0);
    ASSERT_FALSE(col_7 <= row_1);
    ASSERT_FALSE(col_7 <= row_2);
    ASSERT_TRUE(col_7 <= row_3);
    //ASSERT_FALSE(col_7 <= row_4); // D not comparable with C
    //ASSERT_FALSE(col_7 <= row_5); // D not comparable with C
    ASSERT_FALSE(col_7 <= row_6);
    ASSERT_TRUE(col_7 <= row_7);
    ASSERT_FALSE(col_7 <= row_8);
    ASSERT_FALSE(col_7 <= row_9);

    ASSERT_FALSE(col_8 <= row_0);
    ASSERT_TRUE(col_8 <= row_1);
    ASSERT_TRUE(col_8 <= row_2);
    ASSERT_TRUE(col_8 <= row_3);
    ASSERT_TRUE(col_8 <= row_4);
    ASSERT_TRUE(col_8 <= row_5);
    ASSERT_TRUE(col_8 <= row_6);
    ASSERT_TRUE(col_8 <= row_7);
    ASSERT_TRUE(col_8 <= row_8);
    ASSERT_TRUE(col_8 <= row_9);

    ASSERT_FALSE(col_9 <= row_0);
    ASSERT_FALSE(col_9 <= row_1);
    ASSERT_TRUE(col_9 <= row_2);
    ASSERT_TRUE(col_9 <= row_3);
    ASSERT_TRUE(col_9 <= row_4);
    ASSERT_TRUE(col_9 <= row_5);
    ASSERT_TRUE(col_9 <= row_6);
    ASSERT_TRUE(col_9 <= row_7);
    ASSERT_FALSE(col_9 <= row_8);
    ASSERT_TRUE(col_9 <= row_9);
}

TEST(Array, greater_equal_operator)
{
    /**
        Testing by resolving the following truth table
        where C and D are not bitwise comparable
        And C is not comparable with D

    ---------------------------------------------------------------------------------------------------------------------------------
    |              | [] | [1] | [1,2] | [1,3] | [C(1), C(2)] | C(1), C(3) | [D(1), D(2)] | [D(1), D(3)] |  (u8) [1]  |  (u8) [1,2]  |
    ---------------------------------------------------------------------------------------------------------------------------------
    | []           |  1 |  1  |   1   |   1   |      1       |     1      |      1       |      1       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1]          |  0 |  1  |   1   |   1   |      1       |     1      |      1       |      1       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,2]        |  0 |  0  |   1   |   1   |      1       |     1      |      1       |      1       |     0      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [1,3]        |  0 |  0  |   0   |   1   |      0       |     1      |      0       |      1       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [C(1), C(2)] |  0 |  0  |   1   |   1   |      1       |     1      |     ---      |     ---      |     0      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [C(1), C(3)] |  0 |  0  |   0   |   1   |      0       |     1      |     ---      |     ---      |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [D(1), D(2)] |  0 |  0  |   1   |   1   |     ---      |    ---     |      1       |      1       |     0      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    | [D(1), D(3)] |  0 |  0  |   0   |   1   |     ---      |    ---     |      0       |      1       |     0      |       0      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |   (u8) [1]   |  0 |  1  |   1   |   1   |      1       |     1      |      1       |      1       |     1      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    |  (u8) [1,2]  |  0 |  0  |   1   |   1   |      1       |     1      |      1       |      1       |     0      |       1      |
    ---------------------------------------------------------------------------------------------------------------------------------
    */


    using C = hud::test::C;
    using D = hud::test::D;

    hud::array<i32> col_0;
    hud::array<i32> row_0;
    hud::array<i32> col_1({ 1 });
    hud::array<i32> row_1({ 1 });
    hud::array<i32> col_2({ 1,2 });
    hud::array<i32> row_2({ 1,2 });
    hud::array<i32> col_3({ 1,3 });
    hud::array<i32> row_3({ 1,3 });
    hud::array<c> col_4({ C{1}, C{2} });
    hud::array<c> row_4({ C{1}, C{2} });
    hud::array<c> col_5({ C{1}, C{3} });
    hud::array<c> row_5({ C{1}, C{3} });
    hud::array<D> col_6({ D{1}, D{2} });
    hud::array<D> row_6({ D{1}, D{2} });
    hud::array<D> col_7({ D{1}, D{3} });
    hud::array<D> row_7({ D{1}, D{3} });
    hud::array<u8> col_8({ u8(1) });
    hud::array<u8> row_8({ u8(1) });
    hud::array<u8> col_9({ u8(1), u8(2) });
    hud::array<u8> row_9({ u8(1), u8(2) });

    // Verify the truth table
    ASSERT_TRUE(col_0 >= row_0);
    ASSERT_FALSE(col_0 >= row_1);
    ASSERT_FALSE(col_0 >= row_2);
    ASSERT_FALSE(col_0 >= row_3);
    ASSERT_FALSE(col_0 >= row_4);
    ASSERT_FALSE(col_0 >= row_5);
    ASSERT_FALSE(col_0 >= row_6);
    ASSERT_FALSE(col_0 >= row_7);
    ASSERT_FALSE(col_0 >= row_8);
    ASSERT_FALSE(col_0 >= row_9);

    ASSERT_TRUE(col_1 >= row_0);
    ASSERT_TRUE(col_1 >= row_1);
    ASSERT_FALSE(col_1 >= row_2);
    ASSERT_FALSE(col_1 >= row_3);
    ASSERT_FALSE(col_1 >= row_4);
    ASSERT_FALSE(col_1 >= row_5);
    ASSERT_FALSE(col_1 >= row_6);
    ASSERT_FALSE(col_1 >= row_7);
    ASSERT_TRUE(col_1 >= row_8);
    ASSERT_FALSE(col_1 >= row_9);

    ASSERT_TRUE(col_2 >= row_0);
    ASSERT_TRUE(col_2 >= row_1);
    ASSERT_TRUE(col_2 >= row_2);
    ASSERT_FALSE(col_2 >= row_3);
    ASSERT_TRUE(col_2 >= row_4);
    ASSERT_FALSE(col_2 >= row_5);
    ASSERT_TRUE(col_2 >= row_6);
    ASSERT_FALSE(col_2 >= row_7);
    ASSERT_TRUE(col_2 >= row_8);
    ASSERT_TRUE(col_2 >= row_9);

    ASSERT_TRUE(col_3 >= row_0);
    ASSERT_TRUE(col_3 >= row_1);
    ASSERT_TRUE(col_3 >= row_2);
    ASSERT_TRUE(col_3 >= row_3);
    ASSERT_TRUE(col_3 >= row_4);
    ASSERT_TRUE(col_3 >= row_5);
    ASSERT_TRUE(col_3 >= row_6);
    ASSERT_TRUE(col_3 >= row_7);
    ASSERT_TRUE(col_3 >= row_8);
    ASSERT_TRUE(col_3 >= row_9);

    ASSERT_TRUE(col_4 >= row_0);
    ASSERT_TRUE(col_4 >= row_1);
    ASSERT_TRUE(col_4 >= row_2);
    ASSERT_FALSE(col_4 >= row_3);
    ASSERT_TRUE(col_4 >= row_4);
    ASSERT_FALSE(col_4 >= row_5);
    //ASSERT_FALSE(col_4 >= row_6);  // C not comparable with D
    //ASSERT_FALSE(col_4 >= row_7);  // C not comparable with D
    ASSERT_TRUE(col_4 >= row_8);
    ASSERT_TRUE(col_4 >= row_9);

    ASSERT_TRUE(col_5 >= row_0);
    ASSERT_TRUE(col_5 >= row_1);
    ASSERT_TRUE(col_5 >= row_2);
    ASSERT_TRUE(col_5 >= row_3);
    ASSERT_TRUE(col_5 >= row_4);
    ASSERT_TRUE(col_5 >= row_5);
    //ASSERT_FALSE(col_5 >= row_6); // C not comparable with D
    //ASSERT_FALSE(col_5 >= row_7); // C not comparable with D
    ASSERT_TRUE(col_5 >= row_8);
    ASSERT_TRUE(col_5 >= row_9);

    ASSERT_TRUE(col_6 >= row_0);
    ASSERT_TRUE(col_6 >= row_1);
    ASSERT_TRUE(col_6 >= row_2);
    ASSERT_FALSE(col_6 >= row_3);
    //ASSERT_FALSE(col_6 >= row_4); // D not comparable with C
    //ASSERT_FALSE(col_6 >= row_5); // D not comparable with C
    ASSERT_TRUE(col_6 >= row_6);
    ASSERT_FALSE(col_6 >= row_7);
    ASSERT_TRUE(col_6 >= row_8);
    ASSERT_TRUE(col_6 >= row_9);

    ASSERT_TRUE(col_7 >= row_0);
    ASSERT_TRUE(col_7 >= row_1);
    ASSERT_TRUE(col_7 >= row_2);
    ASSERT_TRUE(col_7 >= row_3);
    //ASSERT_FALSE(col_7 >= row_4); // D not comparable with C
    //ASSERT_FALSE(col_7 >= row_5); // D not comparable with C
    ASSERT_TRUE(col_7 >= row_6);
    ASSERT_TRUE(col_7 >= row_7);
    ASSERT_TRUE(col_7 >= row_8);
    ASSERT_TRUE(col_7 >= row_9);

    ASSERT_TRUE(col_8 >= row_0);
    ASSERT_TRUE(col_8 >= row_1);
    ASSERT_FALSE(col_8 >= row_2);
    ASSERT_FALSE(col_8 >= row_3);
    ASSERT_FALSE(col_8 >= row_4);
    ASSERT_FALSE(col_8 >= row_5);
    ASSERT_FALSE(col_8 >= row_6);
    ASSERT_FALSE(col_8 >= row_7);
    ASSERT_TRUE(col_8 >= row_8);
    ASSERT_FALSE(col_8 >= row_9);

    ASSERT_TRUE(col_9 >= row_0);
    ASSERT_TRUE(col_9 >= row_1);
    ASSERT_TRUE(col_9 >= row_2);
    ASSERT_FALSE(col_9 >= row_3);
    ASSERT_TRUE(col_9 >= row_4);
    ASSERT_FALSE(col_9 >= row_5);
    ASSERT_TRUE(col_9 >= row_6);
    ASSERT_FALSE(col_9 >= row_7);
    ASSERT_TRUE(col_9 >= row_8);
    ASSERT_TRUE(col_9 >= row_9);
}