#include <core/containers/array.h>

GTEST_TEST(array, iterators)
{


    // Non const Array
    {
        hud::array<i32> arr({ 10,20,30,40 });
        ASSERT_NE(arr.data(), nullptr);
        ASSERT_EQ(arr.count(), 4u);
        ASSERT_EQ(arr.max_count(), 4u);
        ASSERT_EQ(arr[0], 10);
        ASSERT_EQ(arr[1], 20);
        ASSERT_EQ(arr[2], 30);
        ASSERT_EQ(arr[3], 40);

        // constexpr Iterator begin() noexcept
        // constexpr Iterator end() noexcept
        ASSERT_EQ(arr.begin(), arr.data());
        ASSERT_EQ(arr.end(), arr.data() + arr.count());
        ASSERT_EQ(*arr.begin(), 10);
        ASSERT_EQ(*(arr.end() - 1), 40);
        ASSERT_EQ(static_cast<usize>(arr.end() - arr.begin()), arr.count());

    }

    // Const Array
    {
        const hud::array<i32> arr_const({ 10,20,30,40 });
        ASSERT_NE(arr_const.data(), nullptr);
        ASSERT_EQ(arr_const.count(), 4u);
        ASSERT_EQ(arr_const.max_count(), 4u);
        ASSERT_EQ(arr_const[0], 10);
        ASSERT_EQ(arr_const[1], 20);
        ASSERT_EQ(arr_const[2], 30);
        ASSERT_EQ(arr_const[3], 40);

        // constexpr ConstIterator begin() const noexcept
        // constexpr ConstIterator end() const noexcept
        ASSERT_EQ(arr_const.begin(), arr_const.data());
        ASSERT_EQ(arr_const.end(), arr_const.data() + arr_const.count());
        ASSERT_EQ(*arr_const.begin(), 10);
        ASSERT_EQ(*(arr_const.end() - 1), 40);
        ASSERT_EQ(static_cast<usize>(arr_const.end() - arr_const.begin()), arr_const.count());
    }
}

GTEST_TEST(array, range_for_loop)
{


    // Non const Array
    {
        hud::array<i32> arr({ 10,20,30,40 });
        ASSERT_NE(arr.data(), nullptr);
        ASSERT_EQ(arr.count(), 4u);
        ASSERT_EQ(arr.max_count(), 4u);
        ASSERT_EQ(arr[0], 10);
        ASSERT_EQ(arr[1], 20);
        ASSERT_EQ(arr[2], 30);
        ASSERT_EQ(arr[3], 40);

        i32 result[4];
        i32 index = 0;
        // constexpr Iterator begin() noexcept
        // constexpr Iterator end() noexcept
        for (auto& value : arr) {
            result[index++] = value;
        }
        ASSERT_EQ(result[0], arr[0]);
        ASSERT_EQ(result[1], arr[1]);
        ASSERT_EQ(result[2], arr[2]);
        ASSERT_EQ(result[3], arr[3]);

    }

    // Const Array
    {
        const hud::array<i32> arr({ 10,20,30,40 });
        ASSERT_NE(arr.data(), nullptr);
        ASSERT_EQ(arr.count(), 4u);
        ASSERT_EQ(arr.max_count(), 4u);
        ASSERT_EQ(arr[0], 10);
        ASSERT_EQ(arr[1], 20);
        ASSERT_EQ(arr[2], 30);
        ASSERT_EQ(arr[3], 40);

        i32 result[4];
        i32 index = 0;
        // constexpr Iterator begin() noexcept
        // constexpr Iterator end() noexcept
        for (auto& value : arr) {
            result[index++] = value;
        }
        ASSERT_EQ(result[0], arr[0]);
        ASSERT_EQ(result[1], arr[1]);
        ASSERT_EQ(result[2], arr[2]);
        ASSERT_EQ(result[3], arr[3]);
    }
}