#include <core/containers/vector.h>

GTEST_TEST(vector, iterators)
{
    // Non const Array
    {
        hud::vector<i32> arr({10, 20, 30, 40});
        hud_assert_ne(arr.data(), nullptr);
        hud_assert_eq(arr.count(), 4u);
        hud_assert_eq(arr.max_count(), 4u);
        hud_assert_eq(arr[0], 10);
        hud_assert_eq(arr[1], 20);
        hud_assert_eq(arr[2], 30);
        hud_assert_eq(arr[3], 40);

        // constexpr Iterator begin() noexcept
        // constexpr Iterator end() noexcept
        hud_assert_eq(arr.begin(), arr.data());
        hud_assert_eq(arr.end(), arr.data() + arr.count());
        hud_assert_eq(*arr.begin(), 10);
        hud_assert_eq(*(arr.end() - 1), 40);
        hud_assert_eq(static_cast<usize>(arr.end() - arr.begin()), arr.count());
    }

    // Const Array
    {
        const hud::vector<i32> arr_const({10, 20, 30, 40});
        hud_assert_ne(arr_const.data(), nullptr);
        hud_assert_eq(arr_const.count(), 4u);
        hud_assert_eq(arr_const.max_count(), 4u);
        hud_assert_eq(arr_const[0], 10);
        hud_assert_eq(arr_const[1], 20);
        hud_assert_eq(arr_const[2], 30);
        hud_assert_eq(arr_const[3], 40);

        // constexpr ConstIterator begin() const noexcept
        // constexpr ConstIterator end() const noexcept
        hud_assert_eq(arr_const.begin(), arr_const.data());
        hud_assert_eq(arr_const.end(), arr_const.data() + arr_const.count());
        hud_assert_eq(*arr_const.begin(), 10);
        hud_assert_eq(*(arr_const.end() - 1), 40);
        hud_assert_eq(static_cast<usize>(arr_const.end() - arr_const.begin()), arr_const.count());
    }
}

GTEST_TEST(vector, range_for_loop)
{
    // Non const Array
    {
        hud::vector<i32> arr({10, 20, 30, 40});
        hud_assert_ne(arr.data(), nullptr);
        hud_assert_eq(arr.count(), 4u);
        hud_assert_eq(arr.max_count(), 4u);
        hud_assert_eq(arr[0], 10);
        hud_assert_eq(arr[1], 20);
        hud_assert_eq(arr[2], 30);
        hud_assert_eq(arr[3], 40);

        i32 result[4];
        usize index = 0;
        // constexpr Iterator begin() noexcept
        // constexpr Iterator end() noexcept
        for (auto &value : arr) {
            result[index++] = value;
        }
        hud_assert_eq(result[0], arr[0]);
        hud_assert_eq(result[1], arr[1]);
        hud_assert_eq(result[2], arr[2]);
        hud_assert_eq(result[3], arr[3]);
    }

    // Const Array
    {
        const hud::vector<i32> arr({10, 20, 30, 40});
        hud_assert_ne(arr.data(), nullptr);
        hud_assert_eq(arr.count(), 4u);
        hud_assert_eq(arr.max_count(), 4u);
        hud_assert_eq(arr[0], 10);
        hud_assert_eq(arr[1], 20);
        hud_assert_eq(arr[2], 30);
        hud_assert_eq(arr[3], 40);

        i32 result[4];
        usize index = 0;
        // constexpr Iterator begin() noexcept
        // constexpr Iterator end() noexcept
        for (auto &value : arr) {
            result[index++] = value;
        }
        hud_assert_eq(result[0], arr[0]);
        hud_assert_eq(result[1], arr[1]);
        hud_assert_eq(result[2], arr[2]);
        hud_assert_eq(result[3], arr[3]);
    }
}