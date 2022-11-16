#include <core/slice.h>
#include <core/templates/swap.h>

GTEST_TEST(slice, default_constructor) {


    // constexpr Slice() noexcept
    hud::slice<i32> slice;
    ASSERT_EQ(slice.data(), nullptr);
    ASSERT_TRUE(slice.is_empty());
    ASSERT_EQ(slice.count(), 0u);
}

GTEST_TEST(slice, constructor) {


    i32 arr[4] = { 10,20,30,40 };

    // constexpr Slice(type_t* first, usize count) noexcept
    hud::slice<i32> slice(arr, 4);
    ASSERT_EQ(slice.data(), arr);
    ASSERT_FALSE(slice.is_empty());
    ASSERT_EQ(slice.count(), 4u);
}

GTEST_TEST(slice, copy_constructor) {


    i32 arr[4] = { 10,20,30,40 };

    hud::slice<i32> slice(arr, 4);
    ASSERT_EQ(slice.data(), arr);
    ASSERT_FALSE(slice.is_empty());
    ASSERT_EQ(slice.count(), 4u);

    // Slice(const Slice& other) noexcept
    hud::slice<i32> copy(slice);
    ASSERT_EQ(copy.data(), arr);
    ASSERT_FALSE(copy.is_empty());
    ASSERT_EQ(copy.count(), 4u);
    ASSERT_EQ(slice.data(), arr);
    ASSERT_FALSE(slice.is_empty());
    ASSERT_EQ(slice.count(), 4u);
}

GTEST_TEST(slice, move_constructor) {


    i32 arr[4] = { 10,20,30,40 };

    hud::slice<i32> slice(arr, 4);
    ASSERT_EQ(slice.data(), arr);
    ASSERT_FALSE(slice.is_empty());
    ASSERT_EQ(slice.count(), 4u);

    // constexpr Slice(Slice&& other) noexcept
    hud::slice<i32> moved(hud::move(slice));
    ASSERT_EQ(moved.data(), arr);
    ASSERT_FALSE(moved.is_empty());
    ASSERT_EQ(moved.count(), 4u);
    ASSERT_EQ(slice.data(), nullptr);
    ASSERT_TRUE(slice.is_empty());
    ASSERT_EQ(slice.count(), 0u);
}

GTEST_TEST(slice, copy_assignment) {


    i32 arr[4] = { 10,20,30,40 };

    hud::slice<i32> slice(arr, 4);
    ASSERT_EQ(slice.data(), arr);
    ASSERT_FALSE(slice.is_empty());
    ASSERT_EQ(slice.count(), 4u);

    hud::slice<i32> copy;
    ASSERT_EQ(copy.data(), nullptr);
    ASSERT_TRUE(copy.is_empty());
    ASSERT_EQ(copy.count(), 0u);
    {
        // Slice& operator=(const Slice& other) noexcept
        copy = slice;
        ASSERT_EQ(copy.data(), arr);
        ASSERT_FALSE(copy.is_empty());
        ASSERT_EQ(copy.count(), 4u);
        ASSERT_EQ(slice.data(), arr);
        ASSERT_FALSE(slice.is_empty());
        ASSERT_EQ(slice.count(), 4u);
    }
}

GTEST_TEST(slice, move_assignment) {
 
    
    i32 arr[4] = { 10,20,30,40 };

    hud::slice<i32> slice(arr, 4);
    ASSERT_EQ(slice.data(), arr);
    ASSERT_FALSE(slice.is_empty());
    ASSERT_EQ(slice.count(), 4u);

    hud::slice<i32> copy;
    ASSERT_EQ(copy.data(), nullptr);
    ASSERT_TRUE(copy.is_empty());
    ASSERT_EQ(copy.count(), 0u);
    {
        // constexpr Slice& operator=(Slice&& other) noexcept
        copy = hud::move(slice);
        ASSERT_EQ(copy.data(), arr);
        ASSERT_FALSE(copy.is_empty());
        ASSERT_EQ(copy.count(), 4u);
        ASSERT_EQ(slice.data(), nullptr);
        ASSERT_TRUE(slice.is_empty());
        ASSERT_EQ(slice.count(), 0u);
    }
}

GTEST_TEST(slice, reset) {
 
    
    i32 arr[4] = { 10,20,30,40 };
    hud::slice<i32> slice(arr, 4);
    ASSERT_EQ(slice.data(), arr);
    ASSERT_FALSE(slice.is_empty());
    ASSERT_EQ(slice.count(), 4u);

    // void reset() noexcept
    slice.reset();

    ASSERT_EQ(slice.data(), nullptr);
    ASSERT_TRUE(slice.is_empty());
    ASSERT_EQ(slice.count(), 0u);
}

GTEST_TEST(slice, operator_indexed_accessor) {
 
    
    i32 arr[4] = { 10,20,30,40 };

    // constexpr hud::add_lvalue_reference_t<type_t> operator[](const index at) noexcept
    hud::slice<i32> slice(arr, 4);
    ASSERT_EQ(slice.data(), arr);
    ASSERT_FALSE(slice.is_empty());
    ASSERT_EQ(slice.count(), 4u);
    ASSERT_EQ(slice[0], 10);
    ASSERT_EQ(slice[1], 20);
    ASSERT_EQ(slice[2], 30);
    ASSERT_EQ(slice[3], 40);

    // constexpr hud::add_const_t<hud::add_lvalue_reference_t<type_t>> operator[](const index at) const noexcept
    const hud::slice<i32> slice2(arr, 4);
    ASSERT_EQ(slice2.data(), arr);
    ASSERT_FALSE(slice2.is_empty());
    ASSERT_EQ(slice2.count(), 4u);
    ASSERT_EQ(slice2[0], 10);
    ASSERT_EQ(slice2[1], 20);
    ASSERT_EQ(slice2[2], 30);
    ASSERT_EQ(slice2[3], 40);
}

GTEST_TEST(slice, is_empty) {
 
    
    i32 arr[4] = { 10,20,30,40 };
    hud::slice<i32> slice(arr, 4);
    hud::slice<i32> empty;
    ASSERT_FALSE(slice.is_empty());
    ASSERT_TRUE(empty.is_empty());
}

GTEST_TEST(slice, data) {
 
    
    i32 arr[4] = { 10,20,30,40 };
    hud::slice<i32> slice(arr, 4);
    hud::slice<i32> slice2;
    ASSERT_EQ(slice.data(), &arr[0]);
    ASSERT_EQ(slice2.data(), nullptr);
}

GTEST_TEST(slice, count) {
 
    
    i32 arr[4] = { 10,20,30,40 };
    hud::slice<i32> slice(arr, 4);
    hud::slice<i32> slice2;
    ASSERT_EQ(slice.count(), 4u);
    ASSERT_EQ(slice2.count(), 0u);
}

GTEST_TEST(slice, byte_count) {
    i32 arr[4] = { 10,20,30,40 };
    hud::slice<i32> slice(arr, 4);
    hud::slice<i32> slice2;
    ASSERT_EQ(slice.byte_count(), 4u * sizeof(i32));
    ASSERT_EQ(slice2.byte_count(), 0u * sizeof(i32));
}

GTEST_TEST(slice, is_valid_index) {
    i32 arr[4] = { 10,20,30,40 };
    hud::slice<i32> slice(arr, 4);
    ASSERT_TRUE(slice.is_valid_index(0));
    ASSERT_TRUE(slice.is_valid_index(1));
    ASSERT_TRUE(slice.is_valid_index(2));
    ASSERT_TRUE(slice.is_valid_index(3));
    ASSERT_FALSE(slice.is_valid_index(4));
    hud::slice<i32> empty_slice;
    ASSERT_FALSE(empty_slice.is_valid_index(0));
}

GTEST_TEST(slice, is_valid_range) {
    i32 arr[4] = { 10,20,30,40 };
    hud::slice<i32> slice(arr, 4);

    // Ranges with 0 elements are valid
    ASSERT_TRUE(slice.is_valid_range(0, 0));
    ASSERT_TRUE(slice.is_valid_range(1, 0));
    ASSERT_TRUE(slice.is_valid_range(2, 0));
    ASSERT_TRUE(slice.is_valid_range(3, 0));
    
    // Ranges with more at least count() elements are valid
    ASSERT_TRUE(slice.is_valid_range(0, 1));
    ASSERT_TRUE(slice.is_valid_range(1, 1));
    ASSERT_TRUE(slice.is_valid_range(2, 1));
    ASSERT_TRUE(slice.is_valid_range(3, 1));
    ASSERT_TRUE(slice.is_valid_range(0, 4));
    ASSERT_TRUE(slice.is_valid_range(1, 3));
    ASSERT_TRUE(slice.is_valid_range(2, 2));
    ASSERT_TRUE(slice.is_valid_range(3, 1));

    // Ranges with invalid index are not valid
    ASSERT_FALSE(slice.is_valid_range(4, 0));
    ASSERT_FALSE(slice.is_valid_range(4, 1));

    // Ranges with more than count() elements are invalid
    ASSERT_FALSE(slice.is_valid_range(0, 5));
    ASSERT_FALSE(slice.is_valid_range(1, 4));
    ASSERT_FALSE(slice.is_valid_range(2, 3));
    ASSERT_FALSE(slice.is_valid_range(3, 2));

    // Ranges [0;0] is invalid on empty slice
    hud::slice<i32> empty_slice;
    ASSERT_FALSE(empty_slice.is_valid_range(0, 0));
    
}

GTEST_TEST(slice, sub_slice) {


    i32 arr[4] = { 10,20,30,40 };
    hud::slice<i32> slice(arr, 4);
    ASSERT_EQ(slice.data(), &arr[0]);
    ASSERT_EQ(slice.count(), 4u);

    // sub_slice of whole slice
    {
        hud::slice<i32> sub_slice(slice.sub_slice(0, slice.count()));
        ASSERT_EQ(slice.data(), &arr[0]);
        ASSERT_EQ(slice.count(), 4u);
        ASSERT_EQ(sub_slice.data(), &arr[0]);
        ASSERT_EQ(sub_slice.count(), 4u);

        // Additional test when iterating on the sub_slice
        usize idx = 0;
        for (; idx < sub_slice.count(); idx++) {
            ASSERT_EQ(&sub_slice[idx], &slice[idx]);
        }
        ASSERT_EQ(idx, 4u);
    }

    // sub_slice of nothing
    {
        hud::slice<i32> sub_slice(slice.sub_slice(0, 0));
        ASSERT_EQ(slice.data(), &arr[0]);
        ASSERT_EQ(slice.count(), 4u);
        ASSERT_EQ(sub_slice.data(), &arr[0]);
        ASSERT_EQ(sub_slice.count(), 0u);
    }

    // sub_slice of first part of sequence
    {
        hud::slice<i32> sub_slice(slice.sub_slice(0, slice.count() / 2));
        ASSERT_EQ(slice.data(), &arr[0]);
        ASSERT_EQ(slice.count(), 4u);
        ASSERT_EQ(sub_slice.data(), &arr[0]);
        ASSERT_EQ(sub_slice.count(), 2u);

        // Additional test when iterating on the sub_slice
        usize idx = 0;
        for (; idx < sub_slice.count(); idx++) {
            ASSERT_EQ(&sub_slice[idx], &slice[idx]);
        }
        ASSERT_EQ(idx, 2u);
    }

    // sub_slice of last part of sequence
    {
        hud::slice<i32> sub_slice(slice.sub_slice(slice.count() / 2, slice.count() / 2)); // sub slice from half to half end
        ASSERT_EQ(slice.data(), &arr[0]);
        ASSERT_EQ(slice.count(), 4u);
        ASSERT_EQ(sub_slice.data(), &arr[2]);
        ASSERT_EQ(sub_slice.count(), 2u);

        // Additional test when iterating on the sub_slice
        usize idx = 0;
        for (; idx < sub_slice.count(); idx++) {
            ASSERT_EQ(&sub_slice[idx], &slice[idx + slice.count() / 2]);
        }
        ASSERT_EQ(idx, 2u);
    }
}

GTEST_TEST(slice, swap) {


    i32 arr[4] = { 10,20,30,40 };
    hud::slice<i32> slice(arr, 4);
    ASSERT_EQ(slice.data(), arr);
    ASSERT_FALSE(slice.is_empty());
    ASSERT_EQ(slice.count(), 4u);
    hud::slice<i32> slice2;
    ASSERT_EQ(slice2.data(), nullptr);
    ASSERT_TRUE(slice2.is_empty());
    ASSERT_EQ(slice2.count(), 0u);

    // constexpr void swap(Slice& other) noexcept 
    hud::swap(slice, slice2);
    ASSERT_EQ(slice2.data(), arr);
    ASSERT_FALSE(slice2.is_empty());
    ASSERT_EQ(slice2.count(), 4u);
    ASSERT_EQ(slice.data(), nullptr);
    ASSERT_TRUE(slice.is_empty());
    ASSERT_EQ(slice.count(), 0u);
}

GTEST_TEST(slice, iterators) {


    // constexpr Iterator begin() noexcept
    // constexpr Iterator end() noexcept
    i32 arr[4] = { 10,20,30,40 };
    hud::slice<i32> slice(arr, 4);
    ASSERT_EQ(slice.begin(), &arr[0]);
    ASSERT_EQ(slice.end(), &arr[4]);
    ASSERT_EQ(*slice.begin(), 10);
    ASSERT_EQ(*(slice.end() - 1), 40);
    ASSERT_EQ(slice.end() - slice.begin(), static_cast<isize>(slice.count()));

    // constexpr ConstIterator begin() const noexcept
    // constexpr ConstIterator end() const noexcept
    const hud::slice<i32> slice_const(arr, 4);
    ASSERT_EQ(slice_const.begin(), &arr[0]);
    ASSERT_EQ(slice_const.end(), &arr[4]);
    ASSERT_EQ(*slice_const.begin(), 10);
    ASSERT_EQ(*(slice_const.end() - 1), 40);
    ASSERT_EQ(slice_const.end() - slice_const.begin(), static_cast<isize>(slice.count()));
}

GTEST_TEST(slice, range_for_loop) {


    i32 arr[4] = { 10,20,30,40 };
    hud::slice<i32> slice(arr, 4);

    i32 result[4];
    i32 index = 0;
    // constexpr Iterator begin() noexcept
    // constexpr Iterator end() noexcept
    for (auto& value : slice) {
        result[index++] = value;
    }
    ASSERT_EQ(result[0], arr[0]);
    ASSERT_EQ(result[1], arr[1]);
    ASSERT_EQ(result[2], arr[2]);
    ASSERT_EQ(result[3], arr[3]);

    const hud::slice<i32>& slice_const = slice;
    index = 0;
    hud::memory::set_zero(result);

    // constexpr ConstIterator begin() const noexcept
    // constexpr ConstIterator end() const noexcept
    for (auto& value : slice_const) {
        result[index++] = value;
    }
    ASSERT_EQ(result[0], arr[0]);
    ASSERT_EQ(result[1], arr[1]);
    ASSERT_EQ(result[2], arr[2]);
    ASSERT_EQ(result[3], arr[3]);
}