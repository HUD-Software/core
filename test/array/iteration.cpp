#include <core/containers/array.h>

GTEST_TEST(array, iterators)
{


    // Non const Array
    {
        hud::array<i32> arr({ 10,20,30,40 });
        GTEST_ASSERT_NE(arr.data(), nullptr);
        GTEST_ASSERT_EQ(arr.count(), 4u);
        GTEST_ASSERT_EQ(arr.max_count(), 4u);
        GTEST_ASSERT_EQ(arr[0], 10);
        GTEST_ASSERT_EQ(arr[1], 20);
        GTEST_ASSERT_EQ(arr[2], 30);
        GTEST_ASSERT_EQ(arr[3], 40);

        // constexpr Iterator begin() noexcept
        // constexpr Iterator end() noexcept
        GTEST_ASSERT_EQ(arr.begin(), arr.data());
        GTEST_ASSERT_EQ(arr.end(), arr.data() + arr.count());
        GTEST_ASSERT_EQ(*arr.begin(), 10);
        GTEST_ASSERT_EQ(*(arr.end() - 1), 40);
        GTEST_ASSERT_EQ(static_cast<usize>(arr.end() - arr.begin()), arr.count());

    }

    // Const Array
    {
        const hud::array<i32> arr_const({ 10,20,30,40 });
        GTEST_ASSERT_NE(arr_const.data(), nullptr);
        GTEST_ASSERT_EQ(arr_const.count(), 4u);
        GTEST_ASSERT_EQ(arr_const.max_count(), 4u);
        GTEST_ASSERT_EQ(arr_const[0], 10);
        GTEST_ASSERT_EQ(arr_const[1], 20);
        GTEST_ASSERT_EQ(arr_const[2], 30);
        GTEST_ASSERT_EQ(arr_const[3], 40);

        // constexpr ConstIterator begin() const noexcept
        // constexpr ConstIterator end() const noexcept
        GTEST_ASSERT_EQ(arr_const.begin(), arr_const.data());
        GTEST_ASSERT_EQ(arr_const.end(), arr_const.data() + arr_const.count());
        GTEST_ASSERT_EQ(*arr_const.begin(), 10);
        GTEST_ASSERT_EQ(*(arr_const.end() - 1), 40);
        GTEST_ASSERT_EQ(static_cast<usize>(arr_const.end() - arr_const.begin()), arr_const.count());
    }
}

GTEST_TEST(array, range_for_loop)
{


    // Non const Array
    {
        hud::array<i32> arr({ 10,20,30,40 });
        GTEST_ASSERT_NE(arr.data(), nullptr);
        GTEST_ASSERT_EQ(arr.count(), 4u);
        GTEST_ASSERT_EQ(arr.max_count(), 4u);
        GTEST_ASSERT_EQ(arr[0], 10);
        GTEST_ASSERT_EQ(arr[1], 20);
        GTEST_ASSERT_EQ(arr[2], 30);
        GTEST_ASSERT_EQ(arr[3], 40);

        i32 result[4];
        i32 index = 0;
        // constexpr Iterator begin() noexcept
        // constexpr Iterator end() noexcept
        for (auto& value : arr) {
            result[index++] = value;
        }
        GTEST_ASSERT_EQ(result[0], arr[0]);
        GTEST_ASSERT_EQ(result[1], arr[1]);
        GTEST_ASSERT_EQ(result[2], arr[2]);
        GTEST_ASSERT_EQ(result[3], arr[3]);

    }

    // Const Array
    {
        const hud::array<i32> arr({ 10,20,30,40 });
        GTEST_ASSERT_NE(arr.data(), nullptr);
        GTEST_ASSERT_EQ(arr.count(), 4u);
        GTEST_ASSERT_EQ(arr.max_count(), 4u);
        GTEST_ASSERT_EQ(arr[0], 10);
        GTEST_ASSERT_EQ(arr[1], 20);
        GTEST_ASSERT_EQ(arr[2], 30);
        GTEST_ASSERT_EQ(arr[3], 40);

        i32 result[4];
        i32 index = 0;
        // constexpr Iterator begin() noexcept
        // constexpr Iterator end() noexcept
        for (auto& value : arr) {
            result[index++] = value;
        }
        GTEST_ASSERT_EQ(result[0], arr[0]);
        GTEST_ASSERT_EQ(result[1], arr[1]);
        GTEST_ASSERT_EQ(result[2], arr[2]);
        GTEST_ASSERT_EQ(result[3], arr[3]);
    }
}