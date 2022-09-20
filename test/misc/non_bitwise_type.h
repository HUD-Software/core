#pragma once
#ifndef HD_INC_MISC_NON_BITWISE_TYPE_H
#define HD_INC_MISC_NON_BITWISE_TYPE_H
#include <core/minimal.h>
#include <type_traits>


namespace hud::test {


    /**
    * A non bitwise constructible, copyable or moveable type
    * The id is copied in order to know which NonBitwiseType is the original NonBitwiseType.
    */
    struct NonBitwiseType
    {
        /**
        * Default construct a NonBitwiseType
        */
        constexpr NonBitwiseType() noexcept = default;

        /**
        * Copy constructor
        * @param other The NonBitwiseType to copy
        */
        constexpr NonBitwiseType(const NonBitwiseType& other) noexcept
            : move_assigned_count(other.move_assigned_count)
            , copy_assigned_count(other.copy_assigned_count)
            , param_construct_count(other.param_construct_count)
            , move_construct_count(other.move_construct_count)
            , copy_construct_count(other.copy_construct_count + 1)
            , unique_id(other.unique_id)
            , is_destructor_counter(other.is_destructor_counter)
        {}

        /**
        * Move constructor
        * @param other The NonBitwiseType to move
        */
        constexpr NonBitwiseType(NonBitwiseType&& other) noexcept
            : move_assigned_count(other.move_assigned_count)
            , copy_assigned_count(other.copy_assigned_count)
            , param_construct_count(other.param_construct_count)
            , move_construct_count(other.move_construct_count + 1)
            , copy_construct_count(other.copy_construct_count)
            , unique_id(other.unique_id)
            , is_destructor_counter(other.is_destructor_counter)
        {}

        /**
        * Construct with a pointer on the given boolean.
        * The given bool is set to false if not nullptr.
        * @param ptr_to_bool Pointer to the boolean
        */
        constexpr NonBitwiseType(i32* ptr_to_destructor_counter) noexcept
            : param_construct_count(1)
            , is_destructor_counter(ptr_to_destructor_counter) {
            if (is_destructor_counter != nullptr) {
                *is_destructor_counter = 0;
            }
        }

        /**
        * Construct with a given id and keep a pointer on the given boolean.
        * The given bool is set to false if not nullptr.
        * @param id The id of the NonBitwiseType
        * @param ptr_to_bool Pointer to the boolean
        */
        constexpr NonBitwiseType(i32 id, i32* ptr_to_destructor_counter) noexcept
            : param_construct_count(1)
            , unique_id(id)
            , is_destructor_counter(ptr_to_destructor_counter) {
            if (is_destructor_counter != nullptr) {
                *is_destructor_counter = 0;
            }
        }

        /**
        * Copy assign another NonBitwiseType
        * @param other The NonBitwiseType to assign
        * @return *this
        */
        constexpr NonBitwiseType& operator=(const NonBitwiseType& other) noexcept {
            move_assigned_count = other.move_assigned_count;
            copy_assigned_count = other.copy_assigned_count + 1;
            param_construct_count = other.param_construct_count;
            move_construct_count = other.move_construct_count;
            copy_construct_count = other.copy_construct_count;
            unique_id = other.unique_id;
            is_destructor_counter = other.is_destructor_counter;
            return *this;
        }

        /**
        * Move assign another NonBitwiseType
        * @param other The NonBitwiseType to assign
        * @return *this
        */
        constexpr NonBitwiseType& operator=(NonBitwiseType&& other) noexcept {
            move_assigned_count = other.move_assigned_count + 1;
            copy_assigned_count = other.copy_assigned_count;
            param_construct_count = other.param_construct_count;
            move_construct_count = other.move_construct_count;
            copy_construct_count = other.copy_construct_count;
            unique_id = other.unique_id;
            is_destructor_counter = other.is_destructor_counter;
            return *this;
        }

        /**
        * Set the pointer to the boolean that is set to true when destructor is called
        * @param ptr_to_bool Pointer to the boolean
        */
        constexpr void set_dtor_counter_ptr(i32* ptr_to_destructor_counter) noexcept {
            is_destructor_counter = ptr_to_destructor_counter;
        }

        /** Destroy the NonBitwiseType instance and set the boolean pointer value to true. */
        constexpr virtual ~NonBitwiseType() noexcept {
            if (is_destructor_counter != nullptr) {
                ++(*is_destructor_counter);
            }
        }

        /** Retrieves count of move assignement done */
        [[nodiscard]]
        constexpr u32 move_assign_count() const noexcept {
            return move_assigned_count;
        }

        /** Retrieves count of copy assignement done */
        [[nodiscard]]
        constexpr u32 copy_assign_count() const noexcept {
            return copy_assigned_count;
        }

        /** Retrieves count of constructor done */
        [[nodiscard]]
        constexpr u32 constructor_count() const noexcept {
            return param_construct_count;
        }

        /** Retrieves count of move construction done */
        [[nodiscard]]
        constexpr u32 move_constructor_count() const noexcept {
            return move_construct_count;
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

        /** Retrieves pointer to the boolean set to true when the destructor is called. */
        [[nodiscard]]
        constexpr const i32* destructor_counter() const noexcept {
            return is_destructor_counter;
        }

    private:
        /** Informs if the move assign operator was called */
        u32 move_assigned_count = 0u;
        /** Informs if the copy assign operator was called */
        u32 copy_assigned_count = 0u;
        /** Informs if the constrcutor was called or not */
        u32 param_construct_count = 0u;
        /** Informs if the move constrcutor was called or not */
        u32 move_construct_count = 0u;
        /** Informs if the copy constrcutor was called or not */
        u32 copy_construct_count = 0u;
        /** The id */
        i32 unique_id = 0;
        /** Reference to the boolean to set to true when the destructor is called. */
        i32* is_destructor_counter = 0;
    };

    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator==(const NonBitwiseType& left, const NonBitwiseType& right) noexcept {
        return left.id() == right.id();
    }

    [[nodiscard]]
    HD_FORCEINLINE constexpr bool operator==(const NonBitwiseType& left, const i32 id) noexcept {
        return left.id() == id;
    }

    static_assert(std::is_move_constructible_v<NonBitwiseType>);
    static_assert(std::is_copy_constructible_v<NonBitwiseType>);
    static_assert(std::is_move_assignable_v<NonBitwiseType>);
    static_assert(std::is_copy_assignable_v<NonBitwiseType>);
    static_assert(std::is_destructible_v<NonBitwiseType>);
    static_assert(!hud::is_bitwise_copy_constructible_v<NonBitwiseType>);
    static_assert(!hud::is_bitwise_move_constructible_v<NonBitwiseType>);
    static_assert(!hud::is_bitwise_copy_assignable_v<NonBitwiseType>);
    static_assert(!hud::is_bitwise_move_assignable_v<NonBitwiseType>);


    struct NonBitwiseType2 : public NonBitwiseType {

        constexpr NonBitwiseType2(const NonBitwiseType& other) noexcept
            : NonBitwiseType(other) {
        }

        constexpr NonBitwiseType2(i32 id, i32* ptr_to_destructor_counter) noexcept
            : NonBitwiseType(id, ptr_to_destructor_counter) {
        }

        constexpr ~NonBitwiseType2() noexcept = default;
    };
} // namespace hud::test

#endif // HD_INC_MISC_NON_DEFAULT_CONSTRUCTIBLE_TYPE_H
