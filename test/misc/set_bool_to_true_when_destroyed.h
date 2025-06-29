#ifndef HD_INC_MISC_SETBOOLTOTRUEWHENDESTROYED_H
#define HD_INC_MISC_SETBOOLTOTRUEWHENDESTROYED_H
#include <core/minimal.h>
#include <core/traits/is_bitwise_copy_constructible.h>
#include <core/traits/is_bitwise_move_constructible.h>
#include <core/traits/is_bitwise_copy_assignable.h>
#include <core/traits/is_bitwise_move_assignable.h>
#include <core/templates/equal.h>

namespace hud_test
{

    /**
     * SetBoolToTrueWhenDestroyed set the given boolean to true when it's instance is destroyed.
     * The given boolean is set to false when the SetBoolToTrueWhenDestroyed instance is created.
     * SetBoolToTrueWhenDestroyed is not bitwise copy or move assignable or constructible to avoid copy/move optimisation when used in containers
     * This class is usefull to detect if the destructor is called in some algorithm.
     */
    struct SetBoolToTrueWhenDestroyed
    {

        /** Construct a SetBoolToTrueWhenDestroyed */
        constexpr SetBoolToTrueWhenDestroyed() noexcept = default;

        /**
         * Construct a SetBoolToTrueWhenDestroyed that keep a pointer on the given boolean.
         * The given bool is set to false if not nullptr.
         * @param ptr_to_bool Pointer to the boolean
         */
        constexpr SetBoolToTrueWhenDestroyed(i32 *ptr_to_i32) noexcept
            : is_destructor_called(ptr_to_i32)
        {
        }

        /**
         * Construct a SetBoolToTrueWhenDestroyed that keep a pointer on the given boolean.
         * The given bool is set to false if not nullptr.
         * @param ptr_to_bool Pointer to the boolean
         */
        constexpr SetBoolToTrueWhenDestroyed(i32 id, i32 *ptr_to_i32) noexcept
            : id_(id)
            , is_destructor_called(ptr_to_i32)
        {
        }

        /**
         * Copy constructor
         * @param other The SetBoolToTrueWhenDestroyed to copy
         */
        constexpr SetBoolToTrueWhenDestroyed(const SetBoolToTrueWhenDestroyed &other) noexcept
            : is_destructor_called(other.is_destructor_called)
        {
        }

        /**
         * Move constructor
         * @param other The SetBoolToTrueWhenDestroyed to move
         */
        constexpr SetBoolToTrueWhenDestroyed(SetBoolToTrueWhenDestroyed &&other) noexcept
            : is_destructor_called(other.is_destructor_called)
        {
        }

        /**
         * Assign a pointer to a bool to this
         * Replace the pointer to the bool
         * @param ptr_to_bool Pointer to the boolean
         * @return *this
         */
        constexpr SetBoolToTrueWhenDestroyed &operator=(i32 *ptr_to_i32) noexcept
        {
            is_destructor_called = ptr_to_i32;
            return *this;
        }

        /**
         * Copy assign another SetBoolToTrueWhenDestroyed
         * @param other The SetBoolToTrueWhenDestroyed to assign
         * @return *this
         */
        constexpr SetBoolToTrueWhenDestroyed &operator=(const SetBoolToTrueWhenDestroyed &other) noexcept
        {
            is_destructor_called = other.is_destructor_called;
            return *this;
        }

        /**
         * Move assign another SetBoolToTrueWhenDestroyed
         * @param other The SetBoolToTrueWhenDestroyed to assign
         * @return *this
         */
        constexpr SetBoolToTrueWhenDestroyed &operator=(SetBoolToTrueWhenDestroyed &&other) noexcept
        {
            is_destructor_called = other.is_destructor_called;
            other.is_destructor_called = nullptr;
            return *this;
        }

        /** Destroy the SetBoolToTrueWhenDestroyed instance and set the boolean pointer value to true. */
        constexpr ~SetBoolToTrueWhenDestroyed() noexcept
        {
            if (is_destructor_called != nullptr)
            {
                ++(*is_destructor_called);
            }
        }

        /** Retrieves pointer to the boolean set to true when the destructor is called. */
        [[nodiscard]] constexpr const i32 *ptr() const noexcept
        {
            return is_destructor_called;
        }

        /** Retrieves pointer to the boolean set to true when the destructor is called. */
        [[nodiscard]] constexpr i32 id() const noexcept
        {
            return id_;
        }

        /** Set the pointer to the boolean that is set to true when the destructor is called */
        constexpr void set_ptr(i32 *ptr_to_i32) noexcept
        {
            is_destructor_called = ptr_to_i32;
        }

    private:
        /** ID */
        i32 id_ {0};
        /** Reference to the boolean to set to true when the destructor is called. */
        i32 *is_destructor_called = nullptr;
    };

    static_assert(!hud::is_bitwise_copy_constructible_v<SetBoolToTrueWhenDestroyed>);
    static_assert(!hud::is_bitwise_move_constructible_v<SetBoolToTrueWhenDestroyed>);
    static_assert(!hud::is_bitwise_copy_assignable_v<SetBoolToTrueWhenDestroyed>);
    static_assert(!hud::is_bitwise_move_assignable_v<SetBoolToTrueWhenDestroyed>);

    [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(const SetBoolToTrueWhenDestroyed &left, const SetBoolToTrueWhenDestroyed &right) noexcept
    {
        return left.id() == right.id();
    }

    [[nodiscard]] HD_FORCEINLINE constexpr bool operator!=(const SetBoolToTrueWhenDestroyed &left, const SetBoolToTrueWhenDestroyed &right) noexcept
    {
        return !(left == right);
    }

    [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(const SetBoolToTrueWhenDestroyed &left, const i32 id) noexcept
    {
        return left.id() == id;
    }

} // namespace hud_test

namespace hud
{
    /** Checks whether its two arguments of type type_t compare equal (as returned by operator ==). */
    template<>
    struct equal<hud_test::SetBoolToTrueWhenDestroyed>
    {
        using is_transparent = void;

        /** Member function returning whether the arguments compare equal (lhs==rhs). */
        [[nodiscard]] constexpr bool operator()(const hud_test::SetBoolToTrueWhenDestroyed &lhs, const i32 &rhs) const noexcept
        {
            return lhs == rhs;
        }

        [[nodiscard]] constexpr bool operator()(const hud_test::SetBoolToTrueWhenDestroyed &lhs, const hud_test::SetBoolToTrueWhenDestroyed &rhs) const noexcept
        {
            return lhs == rhs;
        }
    };

    template<>
    struct hash_32<hud_test::SetBoolToTrueWhenDestroyed>
    {
        [[nodiscard]] constexpr u32 operator()(const hud_test::SetBoolToTrueWhenDestroyed &custom) const
        {
            return hud::hash_32<i32> {}(custom.id());
        }

        [[nodiscard]] constexpr u32 operator()(const i32 custom) const
        {
            return hud::hash_32<i32> {}(custom);
        }
    };

    template<>
    struct hash_64<hud_test::SetBoolToTrueWhenDestroyed>
    {
        [[nodiscard]] constexpr u64 operator()(const hud_test::SetBoolToTrueWhenDestroyed &custom) const
        {
            return hud::hash_64<i32> {}(custom.id());
        }

        [[nodiscard]] constexpr u32 operator()(const i32 custom) const
        {
            return hud::hash_32<i32> {}(custom);
        }
    };

} // namespace hud
#endif // HD_INC_MISC_SETBOOLTOTRUEWHENDESTROYED_H
