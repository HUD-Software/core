#pragma once
#ifndef HD_INC_MISC_NON_BITWISE_MOVE_CONSTRUCTIBLE_TYPE_H
#define HD_INC_MISC_NON_BITWISE_MOVE_CONSTRUCTIBLE_TYPE_H
#include <core/minimal.h>
#include <type_traits>
#include <core/traits/is_bitwise_move_constructible.h>

namespace hud_test
{

    /**
     * A move constructible type that is not a bitwise moveable type
     * The id is copied in order to know which NonBitwiseMoveConstructibleType is the original NonBitwiseMoveConstructibleType.
     */
    struct NonBitwiseMoveConstructibleType
    {

        constexpr NonBitwiseMoveConstructibleType() noexcept = default;

        /**
         * Construct with a given id
         * @tparam Integral The integral type to set
         * @param id The id of the NonBitwiseMoveConstructibleType
         */
        template <typename Integral>
        constexpr NonBitwiseMoveConstructibleType(Integral id) noexcept
            requires(hud::is_integral_v<Integral>)
            : unique_id(static_cast<i32>(id))
        {
        }

        /**
         * Copy construct a NonBitwiseMoveConstructibleType
         * @param other The NonBitwiseMoveConstructibleType to copy
         */
        constexpr NonBitwiseMoveConstructibleType(const NonBitwiseMoveConstructibleType &other) noexcept
            : move_construct_count(other.move_construct_count), copy_construct_count(other.copy_construct_count + 1), unique_id(other.unique_id)
        {
        }

        /**
         * Move construct a NonBitwiseMoveConstructibleType
         * @param other The NonBitwiseMoveConstructibleType to move
         */
        constexpr NonBitwiseMoveConstructibleType(NonBitwiseMoveConstructibleType &&other) noexcept
            : move_construct_count(other.move_construct_count + 1), copy_construct_count(other.copy_construct_count), unique_id(other.unique_id)
        {
        }

        /** Retrieves count of move constructor done */
        [[nodiscard]] constexpr u32 move_constructor_count() const noexcept
        {
            return move_construct_count;
        }

        /** Retrieves count of copy construction done */
        [[nodiscard]] constexpr u32 copy_constructor_count() const noexcept
        {
            return copy_construct_count;
        }

        /** Retrives the id */
        [[nodiscard]] constexpr i32 id() const noexcept
        {
            return unique_id;
        }

        /** Set the id */
        constexpr void set_id(const i32 id) noexcept
        {
            unique_id = id;
        }

    private:
        NonBitwiseMoveConstructibleType &operator=(const NonBitwiseMoveConstructibleType &) = delete;
        NonBitwiseMoveConstructibleType &operator=(NonBitwiseMoveConstructibleType &&) = delete;

    private:
        /** Informs if the move constructor was called or not during construction */
        u32 move_construct_count = 0u;
        /** Informs if the copy constrcutor was called or not */
        u32 copy_construct_count = 0u;
        /** The id */
        i32 unique_id = -1;
    };

    static_assert(std::is_move_constructible_v<NonBitwiseMoveConstructibleType>);
    static_assert(!hud::is_bitwise_move_constructible_v<NonBitwiseMoveConstructibleType>);
    static_assert(std::is_copy_constructible_v<NonBitwiseCopyConstructibleType>);
    static_assert(!hud::is_bitwise_copy_constructible_v<NonBitwiseCopyConstructibleType>);

    /**
     * A move constructible type that is not a bitwise moveable type
     * The id is copied in order to know which NonBitwiseMoveConstructibleType is the original NonBitwiseMoveConstructibleType.
     * This is similar to NonBitwiseMoveConstructibleType, it exist only have a type that is not the same as NonBitwiseMoveConstructibleType for tests
     */
    struct NonBitwiseMoveConstructibleType2 : public NonBitwiseMoveConstructibleType
    {
        /**
         * Construct with an id set to -1.
         * You can set the id with set_id().
         */
        constexpr NonBitwiseMoveConstructibleType2() noexcept
            : NonBitwiseMoveConstructibleType(-1)
        {
        }

        /**
         * Construct with a given id
         * @param id The id of the NonBitwiseMoveConstructibleType
         */
        constexpr NonBitwiseMoveConstructibleType2(i32 id) noexcept
            : NonBitwiseMoveConstructibleType(id)
        {
        }

        /**
         * Copy construct a NonBitwiseMoveConstructibleType
         * @param other The NonBitwiseMoveConstructibleType to move
         */
        constexpr NonBitwiseMoveConstructibleType2(const NonBitwiseMoveConstructibleType &other) noexcept
            : NonBitwiseMoveConstructibleType(other)
        {
        }

        /**
         * Move construct a NonBitwiseMoveConstructibleType
         * @param other The NonBitwiseMoveConstructibleType to move
         */
        constexpr NonBitwiseMoveConstructibleType2(NonBitwiseMoveConstructibleType &&other) noexcept
            : NonBitwiseMoveConstructibleType(std::forward<NonBitwiseMoveConstructibleType>(other))
        {
        }
    };
    static_assert(std::is_move_constructible_v<NonBitwiseMoveConstructibleType2>);
    static_assert(!hud::is_bitwise_move_constructible_v<NonBitwiseMoveConstructibleType2>);
    static_assert(std::is_constructible_v<NonBitwiseMoveConstructibleType2, NonBitwiseMoveConstructibleType &&>);
    // static_assert(std::is_constructible_v<NonBitwiseMoveConstructibleType2, const NonBitwiseMoveConstructibleType&>);
    // static_assert(std::is_copy_constructible_v<NonBitwiseMoveConstructibleType2>);
    static_assert(!hud::is_bitwise_copy_constructible_v<NonBitwiseMoveConstructibleType2>);

    /**
     * A move constructible type that is not a bitwise moveable type
     * The id is copied in order to know which NonBitwiseMoveConstructibleType is the original NonBitwiseMoveConstructibleType.
     * This type add a incremanation pointer information to the type used by NonBitwiseMoveConstructibleType4 when this type is constructed.
     */
    struct NonBitwiseMoveConstructibleType3 : public NonBitwiseMoveConstructibleType2
    {

    public:
        /**
         * Default construct NonBitwiseMoveConstructibleType3
         * Set the incrementation pointer to nullptr
         */
        constexpr NonBitwiseMoveConstructibleType3() noexcept = default;

        constexpr NonBitwiseMoveConstructibleType3(i32 *increment_ptr) noexcept
            : NonBitwiseMoveConstructibleType2(), increment(increment_ptr)
        {
        }

        constexpr NonBitwiseMoveConstructibleType3(const NonBitwiseMoveConstructibleType3 &other) noexcept
            : NonBitwiseMoveConstructibleType2(other), increment(other.increment)
        {
        }

        constexpr NonBitwiseMoveConstructibleType3(NonBitwiseMoveConstructibleType3 &&other) noexcept
            : NonBitwiseMoveConstructibleType2(std::forward<NonBitwiseMoveConstructibleType2>(other)), increment(other.increment)
        {
        }

        /** Retrives the pointer to the incremented integer */
        [[nodiscard]] constexpr i32 *incrementation_ptr() const noexcept
        {
            return increment;
        }

    private:
        /** Pointer to an integer to increment when the constructor is called */
        i32 *increment = nullptr;
    };

    struct NonBitwiseMoveConstructibleType4 : public NonBitwiseMoveConstructibleType3
    {
        /**
         * Default construct NonBitwiseMoveConstructibleType4
         * Set the incrementation pointer to nullptr
         */
        constexpr NonBitwiseMoveConstructibleType4() noexcept = default;

        constexpr NonBitwiseMoveConstructibleType4(i32 *increment_ptr) noexcept
            : NonBitwiseMoveConstructibleType3(increment_ptr)
        {
        }

        /**
         * Move construct a NonBitwiseMoveConstructibleType4
         * @param other The NonBitwiseMoveConstructibleType3 to move
         */
        constexpr NonBitwiseMoveConstructibleType4(const NonBitwiseMoveConstructibleType4 &other) noexcept
            : NonBitwiseMoveConstructibleType3(other)
        {
            if (incrementation_ptr())
            {
                increment_value = ++(*incrementation_ptr());
            }
        }
        /**
         * Move construct a NonBitwiseMoveConstructibleType4
         * @param other The NonBitwiseMoveConstructibleType3 to move
         */
        constexpr NonBitwiseMoveConstructibleType4(NonBitwiseMoveConstructibleType4 &&other) noexcept
            : NonBitwiseMoveConstructibleType3(std::forward<NonBitwiseMoveConstructibleType3>(other))
        {
            if (incrementation_ptr())
            {
                increment_value = ++(*incrementation_ptr());
            }
        }

        /**
         * Copy construct a NonBitwiseMoveConstructibleType4
         * @param other The NonBitwiseMoveConstructibleType3 to move
         */
        constexpr NonBitwiseMoveConstructibleType4(const NonBitwiseMoveConstructibleType3 &other) noexcept
            : NonBitwiseMoveConstructibleType3(other)
        {
            if (incrementation_ptr())
            {
                increment_value = ++(*incrementation_ptr());
            }
        }

        /**
         * Move construct a NonBitwiseMoveConstructibleType4
         * @param other The NonBitwiseMoveConstructibleType3 to move
         */
        constexpr NonBitwiseMoveConstructibleType4(NonBitwiseMoveConstructibleType3 &&other) noexcept
            : NonBitwiseMoveConstructibleType3(std::forward<NonBitwiseMoveConstructibleType3>(other))
        {
            if (incrementation_ptr())
            {
                increment_value = ++(*incrementation_ptr());
            }
        }

        /** Retrieves the incremented value when the copy or move consutrctor was called */
        [[nodiscard]] constexpr i32 ctor_order() const noexcept
        {
            return increment_value;
        }

    private:
        /** The value of the incrementation pointer whe the copy or move constructor is called */
        i32 increment_value = -1;
    };

} // namespace hud_test

#endif // HD_INC_MISC_NON_BITWISE_MOVE_CONSTRUCTIBLE_TYPE_H
