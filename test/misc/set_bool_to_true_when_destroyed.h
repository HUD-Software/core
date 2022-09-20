#pragma once
#ifndef HD_INC_MISC_SETBOOLTOTRUEWHENDESTROYED_H
#define HD_INC_MISC_SETBOOLTOTRUEWHENDESTROYED_H
#include <core/traits/is_bitwise_copy_constructible.h>
#include <core/traits/is_bitwise_move_constructible.h>
#include <core/traits/is_bitwise_copy_assignable.h>
#include <core/traits/is_bitwise_move_assignable.h>

namespace hud::test {

    
    /**
    * SetBoolToTrueWhenDestroyed set the given boolean to true when it's instance is destroyed.
    * The given boolean is set to false when the SetBoolToTrueWhenDestroyed instance is created.
    * SetBoolToTrueWhenDestroyed is not bitwise copy or move assignable or constructible to avoid copy/move optimisation when used in containers
    * This class is usefull to detect if the destructor is called in some algorithm.
    */
    struct SetBoolToTrueWhenDestroyed {
       
        /** Construct a SetBoolToTrueWhenDestroyed */
        constexpr SetBoolToTrueWhenDestroyed() noexcept = default;

        /**
        * Construct a SetBoolToTrueWhenDestroyed that keep a pointer on the given boolean.
        * The given bool is set to false if not nullptr.
        * @param ptr_to_bool Pointer to the boolean
        */
        constexpr SetBoolToTrueWhenDestroyed(i32* ptr_to_i32) noexcept
            : is_destructor_called(ptr_to_i32) {
        }

        /**
        * Copy constructor
        * @param other The SetBoolToTrueWhenDestroyed to copy
        */
        constexpr SetBoolToTrueWhenDestroyed(const SetBoolToTrueWhenDestroyed& other) noexcept
            : is_destructor_called(other.is_destructor_called){
        }

        /**
        * Move constructor
        * @param other The SetBoolToTrueWhenDestroyed to move
        */
        constexpr SetBoolToTrueWhenDestroyed(SetBoolToTrueWhenDestroyed&& other) noexcept
            : is_destructor_called(other.is_destructor_called) {
        }

        /**
        * Assign a pointer to a bool to this
        * Replace the pointer to the bool
        * @param ptr_to_bool Pointer to the boolean
        * @return *this
        */
        constexpr SetBoolToTrueWhenDestroyed& operator=(i32* ptr_to_i32) noexcept {
            is_destructor_called = ptr_to_i32;
            return *this;
        }

        /**
        * Copy assign another SetBoolToTrueWhenDestroyed
        * @param other The SetBoolToTrueWhenDestroyed to assign
        * @return *this
        */
        constexpr SetBoolToTrueWhenDestroyed& operator=(const SetBoolToTrueWhenDestroyed& other) noexcept {
            is_destructor_called = other.is_destructor_called;
            return *this;
        }

        /**
        * Move assign another SetBoolToTrueWhenDestroyed
        * @param other The SetBoolToTrueWhenDestroyed to assign
        * @return *this
        */
        constexpr SetBoolToTrueWhenDestroyed& operator=(SetBoolToTrueWhenDestroyed&& other) noexcept {
            is_destructor_called = other.is_destructor_called;
            return *this;
        }

        /** Destroy the SetBoolToTrueWhenDestroyed instance and set the boolean pointer value to true. */
        constexpr ~SetBoolToTrueWhenDestroyed() noexcept {
            if (is_destructor_called != nullptr) {
                ++(*is_destructor_called);
            }
        }
        
        /** Retrieves pointer to the boolean set to true when the destructor is called. */
        [[nodiscard]]
        constexpr const i32* ptr() const noexcept {
            return is_destructor_called;
        }

        /** Set the pointer to the boolean that is set to true when the destructor is called */
        constexpr void set_ptr(i32* ptr_to_i32) noexcept {
            is_destructor_called = ptr_to_i32;
        }

    private:
        /** Reference to the boolean to set to true when the destructor is called. */
        i32* is_destructor_called = nullptr;
    };

static_assert(!hud::is_bitwise_copy_constructible_v<SetBoolToTrueWhenDestroyed>);
static_assert(!hud::is_bitwise_move_constructible_v<SetBoolToTrueWhenDestroyed>);
static_assert(!hud::is_bitwise_copy_assignable_v<SetBoolToTrueWhenDestroyed>);
static_assert(!hud::is_bitwise_move_assignable_v<SetBoolToTrueWhenDestroyed>);
} // namespace hud::test

#endif // HD_INC_MISC_SETBOOLTOTRUEWHENDESTROYED_H
