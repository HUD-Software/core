#include <core/slice.h>
#include <core/templates/swap.h>

GTEST_TEST(slice, default_constructor)
{

    // constexpr Slice() noexcept
    hud::slice<i32> slice;
    hud_assert_eq(slice.data(), nullptr);
    hud_assert_true(slice.is_empty());
    hud_assert_eq(slice.count(), 0u);
}

GTEST_TEST(slice, constructor)
{

    i32 arr[4] = {10, 20, 30, 40};

    // constexpr Slice(type_t* first, usize count) noexcept
    hud::slice<i32> slice(arr, 4);
    hud_assert_eq(slice.data(), arr);
    hud_assert_false(slice.is_empty());
    hud_assert_eq(slice.count(), 4u);
}

GTEST_TEST(slice, copy_constructor)
{

    i32 arr[4] = {10, 20, 30, 40};

    hud::slice<i32> slice(arr, 4);
    hud_assert_eq(slice.data(), arr);
    hud_assert_false(slice.is_empty());
    hud_assert_eq(slice.count(), 4u);

    // Slice(const Slice& other) noexcept
    hud::slice<i32> copy(slice);
    hud_assert_eq(copy.data(), arr);
    hud_assert_false(copy.is_empty());
    hud_assert_eq(copy.count(), 4u);
    hud_assert_eq(slice.data(), arr);
    hud_assert_false(slice.is_empty());
    hud_assert_eq(slice.count(), 4u);
}

GTEST_TEST(slice, move_constructor)
{

    i32 arr[4] = {10, 20, 30, 40};

    hud::slice<i32> slice(arr, 4);
    hud_assert_eq(slice.data(), arr);
    hud_assert_false(slice.is_empty());
    hud_assert_eq(slice.count(), 4u);

    // constexpr Slice(Slice&& other) noexcept
    hud::slice<i32> moved(hud::move(slice));
    hud_assert_eq(moved.data(), arr);
    hud_assert_false(moved.is_empty());
    hud_assert_eq(moved.count(), 4u);
    hud_assert_eq(slice.data(), nullptr);
    hud_assert_true(slice.is_empty());
    hud_assert_eq(slice.count(), 0u);
}

GTEST_TEST(slice, copy_assignment)
{

    i32 arr[4] = {10, 20, 30, 40};

    hud::slice<i32> slice(arr, 4);
    hud_assert_eq(slice.data(), arr);
    hud_assert_false(slice.is_empty());
    hud_assert_eq(slice.count(), 4u);

    hud::slice<i32> copy;
    hud_assert_eq(copy.data(), nullptr);
    hud_assert_true(copy.is_empty());
    hud_assert_eq(copy.count(), 0u);
    {
        // Slice& operator=(const Slice& other) noexcept
        copy = slice;
        hud_assert_eq(copy.data(), arr);
        hud_assert_false(copy.is_empty());
        hud_assert_eq(copy.count(), 4u);
        hud_assert_eq(slice.data(), arr);
        hud_assert_false(slice.is_empty());
        hud_assert_eq(slice.count(), 4u);
    }
}

GTEST_TEST(slice, move_assignment)
{

    i32 arr[4] = {10, 20, 30, 40};

    hud::slice<i32> slice(arr, 4);
    hud_assert_eq(slice.data(), arr);
    hud_assert_false(slice.is_empty());
    hud_assert_eq(slice.count(), 4u);

    hud::slice<i32> copy;
    hud_assert_eq(copy.data(), nullptr);
    hud_assert_true(copy.is_empty());
    hud_assert_eq(copy.count(), 0u);
    {
        // constexpr Slice& operator=(Slice&& other) noexcept
        copy = hud::move(slice);
        hud_assert_eq(copy.data(), arr);
        hud_assert_false(copy.is_empty());
        hud_assert_eq(copy.count(), 4u);
        hud_assert_eq(slice.data(), nullptr);
        hud_assert_true(slice.is_empty());
        hud_assert_eq(slice.count(), 0u);
    }
}

GTEST_TEST(slice, reset)
{

    i32 arr[4] = {10, 20, 30, 40};
    hud::slice<i32> slice(arr, 4);
    hud_assert_eq(slice.data(), arr);
    hud_assert_false(slice.is_empty());
    hud_assert_eq(slice.count(), 4u);

    // void reset() noexcept
    slice.reset();

    hud_assert_eq(slice.data(), nullptr);
    hud_assert_true(slice.is_empty());
    hud_assert_eq(slice.count(), 0u);
}

GTEST_TEST(slice, operator_indexed_accessor)
{

    i32 arr[4] = {10, 20, 30, 40};

    // constexpr hud::add_lvalue_reference_t<type_t> operator[](const index at) noexcept
    hud::slice<i32> slice(arr, 4);
    hud_assert_eq(slice.data(), arr);
    hud_assert_false(slice.is_empty());
    hud_assert_eq(slice.count(), 4u);
    hud_assert_eq(slice[0], 10);
    hud_assert_eq(slice[1], 20);
    hud_assert_eq(slice[2], 30);
    hud_assert_eq(slice[3], 40);

    // constexpr hud::add_const_t<hud::add_lvalue_reference_t<type_t>> operator[](const index at) const noexcept
    const hud::slice<i32> slice2(arr, 4);
    hud_assert_eq(slice2.data(), arr);
    hud_assert_false(slice2.is_empty());
    hud_assert_eq(slice2.count(), 4u);
    hud_assert_eq(slice2[0], 10);
    hud_assert_eq(slice2[1], 20);
    hud_assert_eq(slice2[2], 30);
    hud_assert_eq(slice2[3], 40);
}

GTEST_TEST(slice, is_empty)
{

    i32 arr[4] = {10, 20, 30, 40};
    hud::slice<i32> slice(arr, 4);
    hud::slice<i32> empty;
    hud_assert_false(slice.is_empty());
    hud_assert_true(empty.is_empty());
}

GTEST_TEST(slice, data)
{

    i32 arr[4] = {10, 20, 30, 40};
    hud::slice<i32> slice(arr, 4);
    hud::slice<i32> slice2;
    hud_assert_eq(slice.data(), &arr[0]);
    hud_assert_eq(slice2.data(), nullptr);
}

GTEST_TEST(slice, count)
{

    i32 arr[4] = {10, 20, 30, 40};
    hud::slice<i32> slice(arr, 4);
    hud::slice<i32> slice2;
    hud_assert_eq(slice.count(), 4u);
    hud_assert_eq(slice2.count(), 0u);
}

GTEST_TEST(slice, byte_count)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::slice<i32> slice(arr, 4);
    hud::slice<i32> slice2;
    hud_assert_eq(slice.byte_count(), 4u * sizeof(i32));
    hud_assert_eq(slice2.byte_count(), 0u * sizeof(i32));
}

GTEST_TEST(slice, is_valid_index)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::slice<i32> slice(arr, 4);
    hud_assert_true(slice.is_valid_index(0));
    hud_assert_true(slice.is_valid_index(1));
    hud_assert_true(slice.is_valid_index(2));
    hud_assert_true(slice.is_valid_index(3));
    hud_assert_false(slice.is_valid_index(4));
    hud::slice<i32> empty_slice;
    hud_assert_false(empty_slice.is_valid_index(0));
}

GTEST_TEST(slice, is_valid_range)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::slice<i32> slice(arr, 4);

    // Ranges with 0 elements are valid
    hud_assert_true(slice.is_valid_range(0, 0));
    hud_assert_true(slice.is_valid_range(1, 0));
    hud_assert_true(slice.is_valid_range(2, 0));
    hud_assert_true(slice.is_valid_range(3, 0));

    // Ranges with more at least count() elements are valid
    hud_assert_true(slice.is_valid_range(0, 1));
    hud_assert_true(slice.is_valid_range(1, 1));
    hud_assert_true(slice.is_valid_range(2, 1));
    hud_assert_true(slice.is_valid_range(3, 1));
    hud_assert_true(slice.is_valid_range(0, 4));
    hud_assert_true(slice.is_valid_range(1, 3));
    hud_assert_true(slice.is_valid_range(2, 2));
    hud_assert_true(slice.is_valid_range(3, 1));

    // Ranges with invalid index are not valid
    hud_assert_false(slice.is_valid_range(4, 0));
    hud_assert_false(slice.is_valid_range(4, 1));

    // Ranges with more than count() elements are invalid
    hud_assert_false(slice.is_valid_range(0, 5));
    hud_assert_false(slice.is_valid_range(1, 4));
    hud_assert_false(slice.is_valid_range(2, 3));
    hud_assert_false(slice.is_valid_range(3, 2));

    // Ranges [0;0] is invalid on empty slice
    hud::slice<i32> empty_slice;
    hud_assert_false(empty_slice.is_valid_range(0, 0));
}

GTEST_TEST(slice, sub_slice)
{

    i32 arr[4] = {10, 20, 30, 40};
    hud::slice<i32> slice(arr, 4);
    hud_assert_eq(slice.data(), &arr[0]);
    hud_assert_eq(slice.count(), 4u);

    // sub_slice of whole slice
    {
        hud::slice<i32> sub_slice(slice.sub_slice(0, slice.count()));
        hud_assert_eq(slice.data(), &arr[0]);
        hud_assert_eq(slice.count(), 4u);
        hud_assert_eq(sub_slice.data(), &arr[0]);
        hud_assert_eq(sub_slice.count(), 4u);

        // Additional test when iterating on the sub_slice
        usize idx = 0;
        for (; idx < sub_slice.count(); idx++)
        {
            hud_assert_eq(&sub_slice[idx], &slice[idx]);
        }
        hud_assert_eq(idx, 4u);
    }

    // sub_slice of nothing
    {
        hud::slice<i32> sub_slice(slice.sub_slice(0, 0));
        hud_assert_eq(slice.data(), &arr[0]);
        hud_assert_eq(slice.count(), 4u);
        hud_assert_eq(sub_slice.data(), &arr[0]);
        hud_assert_eq(sub_slice.count(), 0u);
    }

    // sub_slice of first part of sequence
    {
        hud::slice<i32> sub_slice(slice.sub_slice(0, slice.count() / 2));
        hud_assert_eq(slice.data(), &arr[0]);
        hud_assert_eq(slice.count(), 4u);
        hud_assert_eq(sub_slice.data(), &arr[0]);
        hud_assert_eq(sub_slice.count(), 2u);

        // Additional test when iterating on the sub_slice
        usize idx = 0;
        for (; idx < sub_slice.count(); idx++)
        {
            hud_assert_eq(&sub_slice[idx], &slice[idx]);
        }
        hud_assert_eq(idx, 2u);
    }

    // sub_slice of last part of sequence
    {
        hud::slice<i32> sub_slice(slice.sub_slice(slice.count() / 2, slice.count() / 2)); // sub slice from half to half end
        hud_assert_eq(slice.data(), &arr[0]);
        hud_assert_eq(slice.count(), 4u);
        hud_assert_eq(sub_slice.data(), &arr[2]);
        hud_assert_eq(sub_slice.count(), 2u);

        // Additional test when iterating on the sub_slice
        usize idx = 0;
        for (; idx < sub_slice.count(); idx++)
        {
            hud_assert_eq(&sub_slice[idx], &slice[idx + slice.count() / 2]);
        }
        hud_assert_eq(idx, 2u);
    }
}

GTEST_TEST(slice, swap)
{

    i32 arr[4] = {10, 20, 30, 40};
    hud::slice<i32> slice(arr, 4);
    hud_assert_eq(slice.data(), arr);
    hud_assert_false(slice.is_empty());
    hud_assert_eq(slice.count(), 4u);
    hud::slice<i32> slice2;
    hud_assert_eq(slice2.data(), nullptr);
    hud_assert_true(slice2.is_empty());
    hud_assert_eq(slice2.count(), 0u);

    // constexpr void swap(Slice& other) noexcept
    hud::swap(slice, slice2);
    hud_assert_eq(slice2.data(), arr);
    hud_assert_false(slice2.is_empty());
    hud_assert_eq(slice2.count(), 4u);
    hud_assert_eq(slice.data(), nullptr);
    hud_assert_true(slice.is_empty());
    hud_assert_eq(slice.count(), 0u);
}

GTEST_TEST(slice, iterators)
{

    // constexpr Iterator begin() noexcept
    // constexpr Iterator end() noexcept
    i32 arr[4] = {10, 20, 30, 40};
    hud::slice<i32> slice(arr, 4);
    hud_assert_eq(slice.begin(), &arr[0]);
    hud_assert_eq(slice.end(), &arr[4]);
    hud_assert_eq(*slice.begin(), 10);
    hud_assert_eq(*(slice.end() - 1), 40);
    hud_assert_eq(slice.end() - slice.begin(), static_cast<isize>(slice.count()));

    // constexpr ConstIterator begin() const noexcept
    // constexpr ConstIterator end() const noexcept
    const hud::slice<i32> slice_const(arr, 4);
    hud_assert_eq(slice_const.begin(), &arr[0]);
    hud_assert_eq(slice_const.end(), &arr[4]);
    hud_assert_eq(*slice_const.begin(), 10);
    hud_assert_eq(*(slice_const.end() - 1), 40);
    hud_assert_eq(slice_const.end() - slice_const.begin(), static_cast<isize>(slice.count()));
}

GTEST_TEST(slice, range_for_loop)
{

    i32 arr[4] = {10, 20, 30, 40};
    hud::slice<i32> slice(arr, 4);

    i32 result[4];
    i32 index = 0;
    // constexpr Iterator begin() noexcept
    // constexpr Iterator end() noexcept
    for (auto &value : slice)
    {
        result[index++] = value;
    }
    hud_assert_eq(result[0], arr[0]);
    hud_assert_eq(result[1], arr[1]);
    hud_assert_eq(result[2], arr[2]);
    hud_assert_eq(result[3], arr[3]);

    const hud::slice<i32> &slice_const = slice;
    index = 0;
    hud::memory::set_zero(result);

    // constexpr ConstIterator begin() const noexcept
    // constexpr ConstIterator end() const noexcept
    for (auto &value : slice_const)
    {
        result[index++] = value;
    }
    hud_assert_eq(result[0], arr[0]);
    hud_assert_eq(result[1], arr[1]);
    hud_assert_eq(result[2], arr[2]);
    hud_assert_eq(result[3], arr[3]);
}