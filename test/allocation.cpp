#include <core/allocators/allocation.h>
#include <core/templates/swap.h>

GTEST_TEST(allocation, default_constructor)
{
    // constexpr Allocation() noexcept
    hud::allocation<i32> slice;
    GTEST_ASSERT_EQ(slice.data(), nullptr);
    GTEST_ASSERT_TRUE(slice.is_empty());
    GTEST_ASSERT_EQ(slice.count(), 0u);
}

GTEST_TEST(allocation, constructor)
{
    i32 arr[4] = {10, 20, 30, 40};

    // constexpr Allocation(type_t* first, usize count) noexcept
    hud::allocation<i32> slice(arr, 4);
    GTEST_ASSERT_EQ(slice.data(), arr);
    GTEST_ASSERT_FALSE(slice.is_empty());
    GTEST_ASSERT_EQ(slice.count(), 4u);
}

GTEST_TEST(allocation, copy_constructor)
{
    GTEST_ASSERT_FALSE((hud::is_copy_constructible_v<hud::allocation<i32>>));
}

GTEST_TEST(allocation, move_constructor)
{
    i32 arr[4] = {10, 20, 30, 40};

    hud::allocation<i32> slice(arr, 4);
    GTEST_ASSERT_EQ(slice.data(), arr);
    GTEST_ASSERT_FALSE(slice.is_empty());
    GTEST_ASSERT_EQ(slice.count(), 4u);

    // constexpr Allocation(Allocation&& other) noexcept
    hud::allocation<i32> moved(hud::move(slice));
    GTEST_ASSERT_EQ(moved.data(), arr);
    GTEST_ASSERT_FALSE(moved.is_empty());
    GTEST_ASSERT_EQ(moved.count(), 4u);
    GTEST_ASSERT_EQ(slice.data(), nullptr);
    GTEST_ASSERT_TRUE(slice.is_empty());
    GTEST_ASSERT_EQ(slice.count(), 0u);
}

GTEST_TEST(allocation, copy_assignment)
{
    GTEST_ASSERT_FALSE((hud::is_copy_assignable_v<hud::allocation<i32>>));
}

GTEST_TEST(allocation, move_assignment)
{
    i32 arr[4] = {10, 20, 30, 40};

    hud::allocation<i32> slice(arr, 4);
    GTEST_ASSERT_EQ(slice.data(), arr);
    GTEST_ASSERT_FALSE(slice.is_empty());
    GTEST_ASSERT_EQ(slice.count(), 4u);

    hud::allocation<i32> copy;
    GTEST_ASSERT_EQ(copy.data(), nullptr);
    GTEST_ASSERT_TRUE(copy.is_empty());
    GTEST_ASSERT_EQ(copy.count(), 0u);
    {
        // constexpr Allocation& operator=(Allocation&& other) noexcept
        copy = hud::move(slice);
        GTEST_ASSERT_EQ(copy.data(), arr);
        GTEST_ASSERT_FALSE(copy.is_empty());
        GTEST_ASSERT_EQ(copy.count(), 4u);
        GTEST_ASSERT_EQ(slice.data(), nullptr);
        GTEST_ASSERT_TRUE(slice.is_empty());
        GTEST_ASSERT_EQ(slice.count(), 0u);
    }
}

GTEST_TEST(allocation, reset)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::allocation<i32> slice(arr, 4);
    GTEST_ASSERT_EQ(slice.data(), arr);
    GTEST_ASSERT_FALSE(slice.is_empty());
    GTEST_ASSERT_EQ(slice.count(), 4u);

    // void reset() noexcept
    slice.leak();

    GTEST_ASSERT_EQ(slice.data(), nullptr);
    GTEST_ASSERT_TRUE(slice.is_empty());
    GTEST_ASSERT_EQ(slice.count(), 0u);
}

GTEST_TEST(allocation, operator_indexed_accessor)
{
    i32 arr[4] = {10, 20, 30, 40};

    // constexpr hud::add_lvalue_reference_t<type_t> operator[](const index at) noexcept
    hud::allocation<i32> slice(arr, 4);
    GTEST_ASSERT_EQ(slice.data(), arr);
    GTEST_ASSERT_FALSE(slice.is_empty());
    GTEST_ASSERT_EQ(slice.count(), 4u);
    GTEST_ASSERT_EQ(slice[0], 10);
    GTEST_ASSERT_EQ(slice[1], 20);
    GTEST_ASSERT_EQ(slice[2], 30);
    GTEST_ASSERT_EQ(slice[3], 40);

    // constexpr hud::add_const_t<hud::add_lvalue_reference_t<type_t>> operator[](const index at) const noexcept
    const hud::allocation<i32> slice2(arr, 4);
    GTEST_ASSERT_EQ(slice2.data(), arr);
    GTEST_ASSERT_FALSE(slice2.is_empty());
    GTEST_ASSERT_EQ(slice2.count(), 4u);
    GTEST_ASSERT_EQ(slice2[0], 10);
    GTEST_ASSERT_EQ(slice2[1], 20);
    GTEST_ASSERT_EQ(slice2[2], 30);
    GTEST_ASSERT_EQ(slice2[3], 40);
}

GTEST_TEST(allocation, is_empty)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::allocation<i32> slice(arr, 4);
    hud::allocation<i32> empty;
    GTEST_ASSERT_FALSE(slice.is_empty());
    GTEST_ASSERT_TRUE(empty.is_empty());
}

GTEST_TEST(allocation, data)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::allocation<i32> slice(arr, 4);
    hud::allocation<i32> slice2;
    GTEST_ASSERT_EQ(slice.data(), &arr[0]);
    GTEST_ASSERT_EQ(slice2.data(), nullptr);
}

GTEST_TEST(allocation, data_at)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::allocation<i32> slice(arr, 4);
    GTEST_ASSERT_EQ(slice.data_at(0u), &arr[0]);
    GTEST_ASSERT_EQ(slice.data_at(1u), &arr[1]);
    GTEST_ASSERT_EQ(slice.data_at(2u), &arr[2]);
    GTEST_ASSERT_EQ(slice.data_at(3u), &arr[3]);
}

GTEST_TEST(allocation, count)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::allocation<i32> slice(arr, 4);
    hud::allocation<i32> slice2;
    GTEST_ASSERT_EQ(slice.count(), 4u);
    GTEST_ASSERT_EQ(slice2.count(), 0u);
}

GTEST_TEST(allocation, byte_count)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::allocation<i32> slice(arr, 4);
    hud::allocation<i32> slice2;
    GTEST_ASSERT_EQ(slice.byte_count(), 4u * sizeof(i32));
    GTEST_ASSERT_EQ(slice2.byte_count(), 0u * sizeof(i32));
}

GTEST_TEST(allocation, sub_slice)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::allocation<i32> allocation(arr, 4);
    GTEST_ASSERT_EQ(allocation.data(), &arr[0]);
    GTEST_ASSERT_EQ(allocation.count(), 4u);

    // sub_slice of whole slice
    {
        hud::slice<i32> sub_slice(allocation.sub_slice(0, allocation.count()));
        GTEST_ASSERT_EQ(allocation.data(), &arr[0]);
        GTEST_ASSERT_EQ(allocation.count(), 4u);
        GTEST_ASSERT_EQ(sub_slice.data(), &arr[0]);
        GTEST_ASSERT_EQ(sub_slice.count(), 4u);

        // Additional test when iterating on the sub_slice
        usize idx = 0;
        for (; idx < sub_slice.count(); idx++)
        {
            GTEST_ASSERT_EQ(&sub_slice[idx], &allocation[idx]);
        }
        GTEST_ASSERT_EQ(idx, 4u);
    }

    // sub_slice of nothing
    {
        hud::slice<i32> sub_slice(allocation.sub_slice(0, 0));
        GTEST_ASSERT_EQ(allocation.data(), &arr[0]);
        GTEST_ASSERT_EQ(allocation.count(), 4u);
        GTEST_ASSERT_EQ(sub_slice.data(), &arr[0]);
        GTEST_ASSERT_EQ(sub_slice.count(), 0u);
    }

    // sub_slice of first part of sequence
    {
        hud::slice<i32> sub_slice(allocation.sub_slice(0, allocation.count() / 2));
        GTEST_ASSERT_EQ(allocation.data(), &arr[0]);
        GTEST_ASSERT_EQ(allocation.count(), 4u);
        GTEST_ASSERT_EQ(sub_slice.data(), &arr[0]);
        GTEST_ASSERT_EQ(sub_slice.count(), 2u);

        // Additional test when iterating on the sub_slice
        usize idx = 0;
        for (; idx < sub_slice.count(); idx++)
        {
            GTEST_ASSERT_EQ(&sub_slice[idx], &allocation[idx]);
        }
        GTEST_ASSERT_EQ(idx, 2u);
    }

    // sub_slice of last part of sequence
    {
        hud::slice<i32> sub_slice(allocation.sub_slice(allocation.count() / 2, allocation.count() / 2)); // sub slice from half to half end
        GTEST_ASSERT_EQ(allocation.data(), &arr[0]);
        GTEST_ASSERT_EQ(allocation.count(), 4u);
        GTEST_ASSERT_EQ(sub_slice.data(), &arr[2]);
        GTEST_ASSERT_EQ(sub_slice.count(), 2u);

        // Additional test when iterating on the sub_slice
        usize idx = 0;
        for (; idx < sub_slice.count(); idx++)
        {
            GTEST_ASSERT_EQ(&sub_slice[idx], &allocation[idx + allocation.count() / 2]);
        }
        GTEST_ASSERT_EQ(idx, 2u);
    }
}

GTEST_TEST(allocation, to_slice)
{
    // to_slice of empty allocation is empty slice
    {
        hud::allocation<i32> allocation(nullptr, 0u);
        GTEST_ASSERT_EQ(allocation.data(), nullptr);
        GTEST_ASSERT_EQ(allocation.count(), 0u);

        hud::slice<i32> slice(allocation.to_slice());
        GTEST_ASSERT_EQ(slice.data(), allocation.data());
        GTEST_ASSERT_EQ(slice.count(), allocation.count());
    }

    // to_slice of non empty allocation is Slice over the whole allocation.
    {
        i32 arr[4] = {10, 20, 30, 40};
        hud::allocation<i32> allocation(arr, 4u);
        GTEST_ASSERT_EQ(allocation.data(), &arr[0]);
        GTEST_ASSERT_EQ(allocation.count(), 4u);

        hud::slice<i32> slice(allocation.to_slice());
        GTEST_ASSERT_EQ(slice.data(), allocation.data());
        GTEST_ASSERT_EQ(slice.count(), allocation.count());
    }
}

GTEST_TEST(allocation, swap)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::allocation<i32> slice(arr, 4);
    GTEST_ASSERT_EQ(slice.data(), arr);
    GTEST_ASSERT_FALSE(slice.is_empty());
    GTEST_ASSERT_EQ(slice.count(), 4u);
    hud::allocation<i32> slice2;
    GTEST_ASSERT_EQ(slice2.data(), nullptr);
    GTEST_ASSERT_TRUE(slice2.is_empty());
    GTEST_ASSERT_EQ(slice2.count(), 0u);

    // constexpr void swap(Allocation& other) noexcept
    hud::swap(slice, slice2);
    GTEST_ASSERT_EQ(slice2.data(), arr);
    GTEST_ASSERT_FALSE(slice2.is_empty());
    GTEST_ASSERT_EQ(slice2.count(), 4u);
    GTEST_ASSERT_EQ(slice.data(), nullptr);
    GTEST_ASSERT_TRUE(slice.is_empty());
    GTEST_ASSERT_EQ(slice.count(), 0u);
}

GTEST_TEST(allocation, iterators)
{
    // constexpr Iterator begin() noexcept
    // constexpr Iterator end() noexcept
    i32 arr[4] = {10, 20, 30, 40};
    hud::allocation<i32> slice(arr, 4);
    GTEST_ASSERT_EQ(slice.begin(), &arr[0]);
    GTEST_ASSERT_EQ(slice.end(), &arr[4]);
    GTEST_ASSERT_EQ(*slice.begin(), 10);
    GTEST_ASSERT_EQ(*(slice.end() - 1), 40);
    GTEST_ASSERT_EQ(slice.end() - slice.begin(), static_cast<isize>(slice.count()));

    // constexpr ConstIterator begin() const noexcept
    // constexpr ConstIterator end() const noexcept
    const hud::allocation<i32> slice_const(arr, 4);
    GTEST_ASSERT_EQ(slice_const.begin(), &arr[0]);
    GTEST_ASSERT_EQ(slice_const.end(), &arr[4]);
    GTEST_ASSERT_EQ(*slice_const.begin(), 10);
    GTEST_ASSERT_EQ(*(slice_const.end() - 1), 40);
    GTEST_ASSERT_EQ(slice_const.end() - slice_const.begin(), static_cast<isize>(slice.count()));
}

GTEST_TEST(allocation, range_for_loop)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::allocation<i32> slice(arr, 4);

    i32 result[4];
    i32 index = 0;
    // constexpr Iterator begin() noexcept
    // constexpr Iterator end() noexcept
    for (auto &value : slice)
    {
        result[index++] = value;
    }
    GTEST_ASSERT_EQ(result[0], arr[0]);
    GTEST_ASSERT_EQ(result[1], arr[1]);
    GTEST_ASSERT_EQ(result[2], arr[2]);
    GTEST_ASSERT_EQ(result[3], arr[3]);

    const hud::allocation<i32> &slice_const = slice;
    index = 0;
    hud::memory::set_zero(result);

    // constexpr ConstIterator begin() const noexcept
    // constexpr ConstIterator end() const noexcept
    for (auto &value : slice_const)
    {
        result[index++] = value;
    }
    GTEST_ASSERT_EQ(result[0], arr[0]);
    GTEST_ASSERT_EQ(result[1], arr[1]);
    GTEST_ASSERT_EQ(result[2], arr[2]);
    GTEST_ASSERT_EQ(result[3], arr[3]);
}