#include <core/allocators/allocation.h>
#include <core/templates/swap.h>

GTEST_TEST(memory_allocation, default_constructor)
{
    // constexpr Allocation() noexcept
    hud::memory_allocation<i32> slice;
    hud_assert_eq(slice.data(), nullptr);
    hud_assert_true(slice.is_empty());
    hud_assert_eq(slice.count(), 0u);
}

GTEST_TEST(memory_allocation, constructor)
{
    i32 arr[4] = {10, 20, 30, 40};

    // constexpr Allocation(type_t* first, usize count) noexcept
    hud::memory_allocation<i32> slice(arr, 4);
    hud_assert_eq(slice.data(), arr);
    hud_assert_false(slice.is_empty());
    hud_assert_eq(slice.count(), 4u);
}

GTEST_TEST(memory_allocation, copy_constructor)
{
    hud_assert_false((hud::is_copy_constructible_v<hud::memory_allocation<i32>>));
}

GTEST_TEST(memory_allocation, move_constructor)
{
    i32 arr[4] = {10, 20, 30, 40};

    hud::memory_allocation<i32> slice(arr, 4);
    hud_assert_eq(slice.data(), arr);
    hud_assert_false(slice.is_empty());
    hud_assert_eq(slice.count(), 4u);

    // constexpr Allocation(Allocation&& other) noexcept
    hud::memory_allocation<i32> moved(hud::move(slice));
    hud_assert_eq(moved.data(), arr);
    hud_assert_false(moved.is_empty());
    hud_assert_eq(moved.count(), 4u);
    hud_assert_eq(slice.data(), nullptr);
    hud_assert_true(slice.is_empty());
    hud_assert_eq(slice.count(), 0u);
}

GTEST_TEST(memory_allocation, copy_assignment)
{
    hud_assert_false((hud::is_copy_assignable_v<hud::memory_allocation<i32>>));
}

GTEST_TEST(memory_allocation, move_assignment)
{
    i32 arr[4] = {10, 20, 30, 40};

    hud::memory_allocation<i32> slice(arr, 4);
    hud_assert_eq(slice.data(), arr);
    hud_assert_false(slice.is_empty());
    hud_assert_eq(slice.count(), 4u);

    hud::memory_allocation<i32> copy;
    hud_assert_eq(copy.data(), nullptr);
    hud_assert_true(copy.is_empty());
    hud_assert_eq(copy.count(), 0u);

    // Assign with other
    {
        // constexpr Allocation& operator=(Allocation&& other) noexcept
        copy = hud::move(slice);
        hud_assert_eq(copy.data(), arr);
        hud_assert_false(copy.is_empty());
        hud_assert_eq(copy.count(), 4u);
        hud_assert_eq(slice.data(), nullptr);
        hud_assert_true(slice.is_empty());
        hud_assert_eq(slice.count(), 0u);
    }

    // Assign with self
    {
        copy = hud::move(copy);
        hud_assert_eq(copy.data(), arr);
        hud_assert_false(copy.is_empty());
        hud_assert_eq(copy.count(), 4u);
    }
}

GTEST_TEST(memory_allocation, reset)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::memory_allocation<i32> slice(arr, 4);
    hud_assert_eq(slice.data(), arr);
    hud_assert_false(slice.is_empty());
    hud_assert_eq(slice.count(), 4u);

    // void reset() noexcept
    slice.leak();

    hud_assert_eq(slice.data(), nullptr);
    hud_assert_true(slice.is_empty());
    hud_assert_eq(slice.count(), 0u);
}

GTEST_TEST(memory_allocation, operator_indexed_accessor)
{
    i32 arr[4] = {10, 20, 30, 40};

    // constexpr hud::add_lvalue_reference_t<type_t> operator[](const index at) noexcept
    hud::memory_allocation<i32> slice(arr, 4);
    hud_assert_eq(slice.data(), arr);
    hud_assert_false(slice.is_empty());
    hud_assert_eq(slice.count(), 4u);
    hud_assert_eq(slice[0], 10);
    hud_assert_eq(slice[1], 20);
    hud_assert_eq(slice[2], 30);
    hud_assert_eq(slice[3], 40);

    // constexpr hud::add_const_t<hud::add_lvalue_reference_t<type_t>> operator[](const index at) const noexcept
    const hud::memory_allocation<i32> slice2(arr, 4);
    hud_assert_eq(slice2.data(), arr);
    hud_assert_false(slice2.is_empty());
    hud_assert_eq(slice2.count(), 4u);
    hud_assert_eq(slice2[0], 10);
    hud_assert_eq(slice2[1], 20);
    hud_assert_eq(slice2[2], 30);
    hud_assert_eq(slice2[3], 40);
}

GTEST_TEST(memory_allocation, is_empty)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::memory_allocation<i32> slice(arr, 4);
    hud::memory_allocation<i32> empty;
    hud_assert_false(slice.is_empty());
    hud_assert_true(empty.is_empty());
}

GTEST_TEST(memory_allocation, data)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::memory_allocation<i32> slice(arr, 4);
    hud::memory_allocation<i32> slice2;
    hud_assert_eq(slice.data(), &arr[0]);
    hud_assert_eq(slice2.data(), nullptr);
}

GTEST_TEST(memory_allocation, data_at)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::memory_allocation<i32> slice(arr, 4);
    hud_assert_eq(slice.data_at(0u), &arr[0]);
    hud_assert_eq(slice.data_at(1u), &arr[1]);
    hud_assert_eq(slice.data_at(2u), &arr[2]);
    hud_assert_eq(slice.data_at(3u), &arr[3]);
}

GTEST_TEST(memory_allocation, count)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::memory_allocation<i32> slice(arr, 4);
    hud::memory_allocation<i32> slice2;
    hud_assert_eq(slice.count(), 4u);
    hud_assert_eq(slice2.count(), 0u);
}

GTEST_TEST(memory_allocation, byte_count)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::memory_allocation<i32> slice(arr, 4);
    hud::memory_allocation<i32> slice2;
    hud_assert_eq(slice.byte_count(), 4u * sizeof(i32));
    hud_assert_eq(slice2.byte_count(), 0u * sizeof(i32));
}

GTEST_TEST(memory_allocation, sub_slice)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::memory_allocation<i32> allocation(arr, 4);
    hud_assert_eq(allocation.data(), &arr[0]);
    hud_assert_eq(allocation.count(), 4u);

    // sub_slice of whole slice
    {
        hud::slice<i32> sub_slice(allocation.sub_slice(0, allocation.count()));
        hud_assert_eq(allocation.data(), &arr[0]);
        hud_assert_eq(allocation.count(), 4u);
        hud_assert_eq(sub_slice.data(), &arr[0]);
        hud_assert_eq(sub_slice.count(), 4u);

        // Additional test when iterating on the sub_slice
        usize idx = 0;
        for (; idx < sub_slice.count(); idx++)
        {
            hud_assert_eq(&sub_slice[idx], &allocation[idx]);
        }
        hud_assert_eq(idx, 4u);
    }

    // sub_slice of nothing
    {
        hud::slice<i32> sub_slice(allocation.sub_slice(0, 0));
        hud_assert_eq(allocation.data(), &arr[0]);
        hud_assert_eq(allocation.count(), 4u);
        hud_assert_eq(sub_slice.data(), &arr[0]);
        hud_assert_eq(sub_slice.count(), 0u);
    }

    // sub_slice of first part of sequence
    {
        hud::slice<i32> sub_slice(allocation.sub_slice(0, allocation.count() / 2));
        hud_assert_eq(allocation.data(), &arr[0]);
        hud_assert_eq(allocation.count(), 4u);
        hud_assert_eq(sub_slice.data(), &arr[0]);
        hud_assert_eq(sub_slice.count(), 2u);

        // Additional test when iterating on the sub_slice
        usize idx = 0;
        for (; idx < sub_slice.count(); idx++)
        {
            hud_assert_eq(&sub_slice[idx], &allocation[idx]);
        }
        hud_assert_eq(idx, 2u);
    }

    // sub_slice of last part of sequence
    {
        hud::slice<i32> sub_slice(allocation.sub_slice(allocation.count() / 2, allocation.count() / 2)); // sub slice from half to half end
        hud_assert_eq(allocation.data(), &arr[0]);
        hud_assert_eq(allocation.count(), 4u);
        hud_assert_eq(sub_slice.data(), &arr[2]);
        hud_assert_eq(sub_slice.count(), 2u);

        // Additional test when iterating on the sub_slice
        usize idx = 0;
        for (; idx < sub_slice.count(); idx++)
        {
            hud_assert_eq(&sub_slice[idx], &allocation[idx + allocation.count() / 2]);
        }
        hud_assert_eq(idx, 2u);
    }
}

GTEST_TEST(memory_allocation, to_slice)
{
    // to_slice of empty allocation is empty slice
    {
        hud::memory_allocation<i32> allocation(nullptr, nullptr);
        hud_assert_eq(allocation.data(), nullptr);
        hud_assert_eq(allocation.count(), 0u);

        hud::slice<i32> slice(allocation.to_slice());
        hud_assert_eq(slice.data(), allocation.data());
        hud_assert_eq(slice.count(), allocation.count());
    }

    // to_slice of non empty allocation is Slice over the whole allocation.
    {
        i32 arr[4] = {10, 20, 30, 40};
        hud::memory_allocation<i32> allocation(arr, 4u);
        hud_assert_eq(allocation.data(), &arr[0]);
        hud_assert_eq(allocation.count(), 4u);

        hud::slice<i32> slice(allocation.to_slice());
        hud_assert_eq(slice.data(), allocation.data());
        hud_assert_eq(slice.count(), allocation.count());
    }
}

GTEST_TEST(memory_allocation, swap)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::memory_allocation<i32> slice(arr, 4);
    hud_assert_eq(slice.data(), arr);
    hud_assert_false(slice.is_empty());
    hud_assert_eq(slice.count(), 4u);
    hud::memory_allocation<i32> slice2;
    hud_assert_eq(slice2.data(), nullptr);
    hud_assert_true(slice2.is_empty());
    hud_assert_eq(slice2.count(), 0u);

    // constexpr void swap(Allocation& other) noexcept
    hud::swap(slice, slice2);
    hud_assert_eq(slice2.data(), arr);
    hud_assert_false(slice2.is_empty());
    hud_assert_eq(slice2.count(), 4u);
    hud_assert_eq(slice.data(), nullptr);
    hud_assert_true(slice.is_empty());
    hud_assert_eq(slice.count(), 0u);
}

GTEST_TEST(memory_allocation, iterators)
{
    // constexpr Iterator begin() noexcept
    // constexpr Iterator end() noexcept
    i32 arr[4] = {10, 20, 30, 40};
    hud::memory_allocation<i32> slice(arr, 4);
    hud_assert_eq(slice.begin(), &arr[0]);
    hud_assert_eq(slice.end(), &arr[4]);
    hud_assert_eq(*slice.begin(), 10);
    hud_assert_eq(*(slice.end() - 1), 40);
    hud_assert_eq(slice.end() - slice.begin(), static_cast<isize>(slice.count()));

    // constexpr ConstIterator begin() const noexcept
    // constexpr ConstIterator end() const noexcept
    const hud::memory_allocation<i32> slice_const(arr, 4);
    hud_assert_eq(slice_const.begin(), &arr[0]);
    hud_assert_eq(slice_const.end(), &arr[4]);
    hud_assert_eq(*slice_const.begin(), 10);
    hud_assert_eq(*(slice_const.end() - 1), 40);
    hud_assert_eq(slice_const.end() - slice_const.begin(), static_cast<isize>(slice.count()));
}

GTEST_TEST(memory_allocation, range_for_loop)
{
    i32 arr[4] = {10, 20, 30, 40};
    hud::memory_allocation<i32> slice(arr, 4);

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

    const hud::memory_allocation<i32> &slice_const = slice;
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