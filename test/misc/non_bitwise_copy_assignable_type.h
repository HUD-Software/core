#pragma once
#ifndef HD_INC_MISC_NON_BITWISE_COPY_ASSIGNABLE_TYPE_H
#define HD_INC_MISC_NON_BITWISE_COPY_ASSIGNABLE_TYPE_H
#include <core/minimal.h>
#include <type_traits>
#include <core/traits/is_bitwise_copy_assignable.h>
#include <core/traits/is_bitwise_copy_constructible.h>

namespace hud::test {

    /**
    * A copy assignable type that is not a bitwise assignable type
    * The id is copied in order to know which NonBitwiseCopyAssignableType is the original NonBitwiseCopyAssignableType.
    */
    struct NonBitwiseCopyAssignableType
    {
        /**
        * Default construct a NonBitwiseCopyAssignableType
        */
        constexpr NonBitwiseCopyAssignableType() noexcept = default;

        /**
        * Copy constructor
        * @param other The NonBitwiseCopyAssignableType to copy
        */
        constexpr NonBitwiseCopyAssignableType(const NonBitwiseCopyAssignableType& other) noexcept 
            : copy_assigned_count(other.copy_assigned_count)
            , copy_construct_count(other.copy_construct_count+1)
            , unique_id(other.unique_id)
        {}

        /**
        * Construct with a given id
        * @tparam Integral The integral type to set
        * @param id The id of the NonBitwiseCopyAssignableType
        */
        template<typename Integral>
        constexpr NonBitwiseCopyAssignableType(Integral id) noexcept requires(hud::is_integral_v<Integral>)
            : unique_id(static_cast<i32>(id)) {
        }

        /**
        * Assign another NonBitwiseCopyAssignableType to this
        * @param other The NonBitwiseCopyAssignableType to assign
        * @return *this
        */
        constexpr NonBitwiseCopyAssignableType& operator=(const NonBitwiseCopyAssignableType& other) noexcept {
            if (&other != this) {
                copy_assigned_count = other.copy_assigned_count + 1;
                copy_construct_count = other.copy_construct_count;
                unique_id = other.unique_id;
            }
            return *this;
        }

        /**
        * Assign an id to this
        * @param id The id to assign
        * @return *this
        */
        constexpr NonBitwiseCopyAssignableType& operator=(i32 id) noexcept {
            unique_id = id;
            return *this;
        }

        /** Retrieves count of copy assignement done */
        [[nodiscard]]
        constexpr u32 copy_assign_count() const noexcept {
            return copy_assigned_count;
        }

        /** Retrieves count of copy construction done */
        [[nodiscard]]
        constexpr u32 copy_constructor_count() const noexcept {
            return copy_construct_count;
        }

        /** Retrives the id */
        [[nodiscard]]
        constexpr i32 id() const noexcept {
            return unique_id;
        }

    private:
        NonBitwiseCopyAssignableType(NonBitwiseCopyAssignableType&&) = delete;
        NonBitwiseCopyAssignableType& operator=(NonBitwiseCopyAssignableType&&) = delete;

    private:
        /** Informs if the copy assign operator was called */
        u32 copy_assigned_count = 0u;
        /** Informs if the copy constrcutor was called or not */
        u32 copy_construct_count = 0u;
        /** The id */
        i32 unique_id = 0;
    };

static_assert(std::is_copy_assignable_v<NonBitwiseCopyAssignableType>);
static_assert(std::is_copy_constructible_v<NonBitwiseCopyAssignableType>);
static_assert(!hud::is_bitwise_copy_assignable_v<NonBitwiseCopyAssignableType>);
static_assert(!hud::is_bitwise_copy_constructible_v<NonBitwiseCopyAssignableType>);


    /**
    * A copy assignable type that is not a bitwise assignable type
    * The id is copied in order to know which NonBitwiseCopyAssignableType2 is the original NonBitwiseCopyAssignableType2.
    * This type can be assigned with a NonBitwiseCopyAssignableType
    */
    struct NonBitwiseCopyAssignableType2 : public NonBitwiseCopyAssignableType
    {
        /** Default construct */
        constexpr NonBitwiseCopyAssignableType2() noexcept = default;
        constexpr NonBitwiseCopyAssignableType2(const NonBitwiseCopyAssignableType2&) noexcept = default;
        /**
        * Construct with a given id
        * @param id The id of the NonBitwiseCopyAssignableType
        */
        constexpr NonBitwiseCopyAssignableType2(i32 id) noexcept
            : NonBitwiseCopyAssignableType(id) {
        }

        /**
        * Copy constructor
        * @param other The NonBitwiseCopyAssignableType to copy
        */
        constexpr NonBitwiseCopyAssignableType2(const NonBitwiseCopyAssignableType& other) noexcept
            : NonBitwiseCopyAssignableType(other){
        }

        /**
        * Assign a NonBitwiseCopyAssignableType to this
        * @param other The NonBitwiseCopyAssignableType to assign
        * @return *this
        */
        constexpr NonBitwiseCopyAssignableType2& operator=(const NonBitwiseCopyAssignableType& other) noexcept{
            if (&other != this) {
                NonBitwiseCopyAssignableType::operator=(other);
            }
            return *this;
        }

        constexpr NonBitwiseCopyAssignableType2& operator=(const NonBitwiseCopyAssignableType2& other) noexcept = default;
    };

    static_assert(std::is_copy_assignable_v<NonBitwiseCopyAssignableType2>);
    static_assert(std::is_copy_constructible_v<NonBitwiseCopyAssignableType2>);
    static_assert(std::is_constructible_v<NonBitwiseCopyAssignableType2, const NonBitwiseCopyAssignableType&>);
    static_assert(!hud::is_bitwise_copy_assignable_v<NonBitwiseCopyAssignableType2>);
    static_assert(!hud::is_bitwise_copy_constructible_v<NonBitwiseCopyAssignableType2>);


    /**
    * A copy assignable type that is not a bitwise assignable type
    * The id is copied in order to know which NonBitwiseCopyAssignableType2 is the original NonBitwiseCopyAssignableType2.
    * This type can be assigned with a NonBitwiseCopyAssignableType
    */
    struct NonBitwiseCopyAssignableType3 : public NonBitwiseCopyAssignableType2 {

    public:

        constexpr NonBitwiseCopyAssignableType3() noexcept = default;

        constexpr NonBitwiseCopyAssignableType3(i32* increment_ptr) noexcept
            : NonBitwiseCopyAssignableType2()
            , increment(increment_ptr) {
        }

        /**
        * Copy constructor
        * @param other The NonBitwiseCopyAssignableType to copy
        */
        constexpr NonBitwiseCopyAssignableType3(const NonBitwiseCopyAssignableType3& other) noexcept
            : NonBitwiseCopyAssignableType2(other)
            , increment(other.increment) {
        }

        /**
       * Assign a NonBitwiseCopyAssignableType to this
       * @param other The NonBitwiseCopyAssignableType to assign
       * @return *this
       */
        constexpr NonBitwiseCopyAssignableType3& operator=(const NonBitwiseCopyAssignableType3& other) noexcept {
            if (&other != this) {
                NonBitwiseCopyAssignableType2::operator=(other);
                increment = other.increment;
                if (increment) {
                    ++(*increment);
                }
            }
            return *this;
        }

        /** Retrives the pointer to the incremented integer */
        [[nodiscard]]
        constexpr i32* incrementation_ptr() const noexcept {
            return increment;
        }

    private:
        /** Pointer to an integer to increment when the constructor is called */
        i32* increment = nullptr;
    };

    static_assert(std::is_copy_assignable_v<NonBitwiseCopyAssignableType3>);
    static_assert(std::is_copy_constructible_v<NonBitwiseCopyAssignableType3>);
    static_assert(std::is_constructible_v<NonBitwiseCopyAssignableType3, const NonBitwiseCopyAssignableType3&>);
    static_assert(!hud::is_bitwise_copy_assignable_v<NonBitwiseCopyAssignableType3>);
    static_assert(!hud::is_bitwise_copy_constructible_v<NonBitwiseCopyAssignableType3>);


    /**
    * A copy assignable type that is not a bitwise assignable type
    * The id is copied in order to know which NonBitwiseCopyAssignableType2 is the original NonBitwiseCopyAssignableType2.
    * This type can be assigned with a NonBitwiseCopyAssignableType
    */
    struct NonBitwiseCopyAssignableType4 : public NonBitwiseCopyAssignableType3
    {
        /** Default construct */
        constexpr NonBitwiseCopyAssignableType4() noexcept = default;

        /** */
        constexpr NonBitwiseCopyAssignableType4(i32* increment_ptr) noexcept
            : NonBitwiseCopyAssignableType3(increment_ptr){
        }

        /**
        * Copy constructor
        * @param other The NonBitwiseCopyAssignableType to copy
        */
        constexpr NonBitwiseCopyAssignableType4(const NonBitwiseCopyAssignableType3& other) noexcept
            : NonBitwiseCopyAssignableType3(other) {
            
        }

        /**
        * Assign a NonBitwiseCopyAssignableType to this
        * @param other The NonBitwiseCopyAssignableType to assign
        * @return *this
        */
        constexpr NonBitwiseCopyAssignableType4& operator=(const NonBitwiseCopyAssignableType3& other) noexcept {
            if (&other != this) {
                NonBitwiseCopyAssignableType3::operator=(other);
                if (incrementation_ptr()) {
                    increment_value = *incrementation_ptr();
                }
            }
            return *this;
        }

        constexpr NonBitwiseCopyAssignableType4& operator=(const NonBitwiseCopyAssignableType4& other) noexcept {
            if (&other != this) {
                NonBitwiseCopyAssignableType3::operator=(other);
                if (incrementation_ptr()) {
                    increment_value = *incrementation_ptr();
                }
            }
            return *this;
        }

        /** Retrieves the incremented value when the copy or move consutrctor was called */
        [[nodiscard]]
        constexpr i32 copy_order() const noexcept {
            return increment_value;
        }
    private:
        /** The value of the incrementation pointer whe the copy or move constructor is called */
        i32 increment_value = -1;
    };

    static_assert(std::is_copy_assignable_v<NonBitwiseCopyAssignableType4>);
    static_assert(std::is_copy_constructible_v<NonBitwiseCopyAssignableType4>);
    static_assert(std::is_constructible_v<NonBitwiseCopyAssignableType4, const NonBitwiseCopyAssignableType3&>);
    static_assert(!hud::is_bitwise_copy_assignable_v<NonBitwiseCopyAssignableType4>);
    static_assert(!hud::is_bitwise_copy_constructible_v<NonBitwiseCopyAssignableType4>);
} // namespace hud::test

#endif // HD_INC_MISC_NON_BITWISE_COPY_CONSTRUCTIBLE_TYPE_H

