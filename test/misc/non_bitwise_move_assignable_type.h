#ifndef HD_INC_MISC_NON_BITWISE_MOVE_ASSIGNABLE_TYPE_H
#define HD_INC_MISC_NON_BITWISE_MOVE_ASSIGNABLE_TYPE_H
#include <core/minimal.h>
#include <type_traits>
#include <core/traits/is_bitwise_move_assignable.h>
#include <core/traits/is_bitwise_move_constructible.h>

namespace hud_test
{

    /**
     * A move assignable type that is not a bitwise assignable type
     * The id is copied in order to know which NonBitwiseMoveAssignableType is the original NonBitwiseMoveAssignableType.
     */
    struct NonBitwiseMoveAssignableType
    {
        /**
         * Default constrcut a NonBitwiseMoveAssignableType
         */
        constexpr NonBitwiseMoveAssignableType() noexcept = default;

        /**
         * Copy constructor
         * @param other The hud_test::non_bitwise_copy_assignable_type to copy
         */
        constexpr NonBitwiseMoveAssignableType(const NonBitwiseMoveAssignableType &other) noexcept
            : move_assigned_count(other.move_assigned_count)
            , copy_assigned_count(other.copy_assigned_count)
            , move_construct_count(other.move_construct_count)
            , copy_construct_count(other.copy_construct_count + 1)
            , unique_id(other.unique_id)
        {
        }

        /**
         * Move constructor
         * @param other The NonBitwiseMoveAssignableType to move
         */
        constexpr NonBitwiseMoveAssignableType(NonBitwiseMoveAssignableType &&other) noexcept
            : move_assigned_count(other.move_assigned_count)
            , copy_assigned_count(other.copy_assigned_count)
            , move_construct_count(other.move_construct_count + 1)
            , copy_construct_count(other.copy_construct_count)
            , unique_id(other.unique_id)
        {
        }

        /**
         * Construct with a given id
         * @param id The id of the NonBitwiseMoveAssignableType
         */
        constexpr NonBitwiseMoveAssignableType(i32 id) noexcept
            : unique_id(id)
        {
        }

        /**
         * Assign another NonBitwiseMoveAssignableType to this
         * @param other The NonBitwiseMoveAssignableType to assign
         * @return *this
         */
        constexpr NonBitwiseMoveAssignableType &operator=(NonBitwiseMoveAssignableType &&other) noexcept
        {
            move_assigned_count = other.move_assigned_count + 1;
            move_construct_count = other.move_construct_count;
            copy_construct_count = other.copy_construct_count;
            copy_assigned_count = other.copy_assigned_count;
            unique_id = other.unique_id;
            return *this;
        }

        /**
         * Assign another NonBitwiseMoveAssignableType to this
         * @param other The NonBitwiseMoveAssignableType to assign
         * @return *this
         */
        constexpr NonBitwiseMoveAssignableType &operator=(const NonBitwiseMoveAssignableType &other) noexcept
        {
            move_assigned_count = other.move_assigned_count;
            move_construct_count = other.move_construct_count;
            copy_assigned_count = other.copy_assigned_count + 1;
            unique_id = other.unique_id;
            return *this;
        }

        /** Retrieves count of move assignement done */
        [[nodiscard]] constexpr u32 move_assign_count() const noexcept
        {
            return move_assigned_count;
        }

        /** Retrieves count of copy assignement done */
        [[nodiscard]] constexpr u32 copy_assign_count() const noexcept
        {
            return copy_assigned_count;
        }

        /** Retrieves count of move construction done */
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

    private:
        /** Informs if the move assign operator was called */
        u32 move_assigned_count = 0u;
        /** Informs if the copy assign operator was called */
        u32 copy_assigned_count = 0u;
        /** Informs if the move constrcutor was called or not */
        u32 move_construct_count = 0u;
        /** Informs if the copy constrcutor was called or not */
        u32 copy_construct_count = 0u;
        /** The id */
        i32 unique_id = 0;
    };

    static_assert(std::is_move_assignable_v<NonBitwiseMoveAssignableType>);
    static_assert(std::is_move_constructible_v<NonBitwiseMoveAssignableType>);
    static_assert(!hud::is_bitwise_move_assignable_v<NonBitwiseMoveAssignableType>);
    static_assert(!hud::is_bitwise_move_constructible_v<NonBitwiseMoveAssignableType>);

    /**
     * A move assignable type that is not a bitwise assignable type
     * The id is copied in order to know which NonBitwiseMoveAssignableType2 is the original NonBitwiseMoveAssignableType2.
     * This type can be assigned with a NonBitwiseMoveAssignableType2
     */
    struct NonBitwiseMoveAssignableType2 : public NonBitwiseMoveAssignableType
    {

        /** Default construct */
        constexpr NonBitwiseMoveAssignableType2() noexcept = default;
        constexpr NonBitwiseMoveAssignableType2(const NonBitwiseMoveAssignableType2 &) noexcept = default;
        constexpr NonBitwiseMoveAssignableType2(NonBitwiseMoveAssignableType2 &&) noexcept = default;

        /**
         * Construct with a given id
         * @param id The id of the NonBitwiseMoveAssignableType2
         */
        constexpr NonBitwiseMoveAssignableType2(i32 id) noexcept
            : NonBitwiseMoveAssignableType(id)
        {
        }

        /**
         * Copy constructor
         * @param other The hud_test::non_bitwise_copy_assignable_type to copy
         */
        constexpr NonBitwiseMoveAssignableType2(const NonBitwiseMoveAssignableType &other) noexcept
            : NonBitwiseMoveAssignableType(other)
        {
        }

        /**
         * Move constructor
         * @param other The hud_test::non_bitwise_copy_assignable_type to copy
         */
        constexpr NonBitwiseMoveAssignableType2(NonBitwiseMoveAssignableType &&other) noexcept
            : NonBitwiseMoveAssignableType(std::forward<NonBitwiseMoveAssignableType>(other))
        {
        }

        /**
         * Assign another NonBitwiseMoveAssignableType2 to this
         * @param other The NonBitwiseMoveAssignableType2 to assign
         * @return *this
         */
        constexpr NonBitwiseMoveAssignableType2 &operator=(NonBitwiseMoveAssignableType &&other) noexcept
        {
            NonBitwiseMoveAssignableType::operator=(std::forward<NonBitwiseMoveAssignableType>(other));
            return *this;
        }

        constexpr NonBitwiseMoveAssignableType2 &operator=(const NonBitwiseMoveAssignableType2 &other) noexcept = default;
        constexpr NonBitwiseMoveAssignableType2 &operator=(NonBitwiseMoveAssignableType2 &&other) noexcept = default;
    };

    static_assert(std::is_move_assignable_v<NonBitwiseMoveAssignableType>);
    static_assert(std::is_move_constructible_v<NonBitwiseMoveAssignableType2>);
    static_assert(std::is_constructible_v<NonBitwiseMoveAssignableType2, NonBitwiseMoveAssignableType &&>);
    static_assert(!hud::is_bitwise_move_assignable_v<NonBitwiseMoveAssignableType2>);
    static_assert(!hud::is_bitwise_move_constructible_v<NonBitwiseMoveAssignableType2>);

    /**
     * A move assignable type that is not a bitwise assignable type
     * The id is copied in order to know which NonBitwiseMoveAssignableType2 is the original NonBitwiseMoveAssignableType2.
     * This type can be assigned with a NonBitwiseMoveAssignableType
     */
    struct NonBitwiseMoveAssignableType3 : public NonBitwiseMoveAssignableType2
    {

    public:
        constexpr NonBitwiseMoveAssignableType3() noexcept = default;

        constexpr NonBitwiseMoveAssignableType3(i32 *increment_ptr) noexcept
            : NonBitwiseMoveAssignableType2()
            , increment(increment_ptr)
        {
        }

        /**
         * Move constructor
         * @param other The hud_test::non_bitwise_copy_assignable_type to copy
         */
        constexpr NonBitwiseMoveAssignableType3(NonBitwiseMoveAssignableType3 &&other) noexcept
            : NonBitwiseMoveAssignableType2(std::forward<NonBitwiseMoveAssignableType3>(other))
            , increment(other.increment)
        {
        }

        /**
         * Assign a NonBitwiseMoveAssignableType to this
         * @param other The NonBitwiseMoveAssignableType to assign
         * @return *this
         */
        constexpr NonBitwiseMoveAssignableType3 &operator=(const NonBitwiseMoveAssignableType3 &other) noexcept
        {
            if (&other != this)
            {
                NonBitwiseMoveAssignableType2::operator=(other);
                increment = other.increment;
                if (increment)
                {
                    ++(*increment);
                }
            }
            return *this;
        }

        /**
         * Assign a NonBitwiseMoveAssignableType to this
         * @param other The NonBitwiseMoveAssignableType to assign
         * @return *this
         */
        constexpr NonBitwiseMoveAssignableType3 &operator=(NonBitwiseMoveAssignableType3 &&other) noexcept
        {
            if (&other != this)
            {
                NonBitwiseMoveAssignableType2::operator=(std::forward<NonBitwiseMoveAssignableType3>(other));
                increment = other.increment;
                if (increment)
                {
                    ++(*increment);
                }
            }
            return *this;
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

    static_assert(std::is_move_assignable_v<NonBitwiseMoveAssignableType3>);
    static_assert(std::is_move_constructible_v<NonBitwiseMoveAssignableType3>);
    static_assert(std::is_constructible_v<NonBitwiseMoveAssignableType3, NonBitwiseMoveAssignableType3 &&>);
    static_assert(!hud::is_bitwise_move_assignable_v<NonBitwiseMoveAssignableType3>);
    static_assert(!hud::is_bitwise_move_constructible_v<NonBitwiseMoveAssignableType3>);

    /**
     * A move assignable type that is not a bitwise assignable type
     * The id is copied in order to know which NonBitwiseMoveAssignableType2 is the original NonBitwiseMoveAssignableType2.
     * This type can be assigned with a NonBitwiseMoveAssignableType
     */
    struct NonBitwiseMoveAssignableType4 : public NonBitwiseMoveAssignableType3
    {
        /** Default construct */
        constexpr NonBitwiseMoveAssignableType4() noexcept = default;
        constexpr NonBitwiseMoveAssignableType4(NonBitwiseMoveAssignableType4 &&) noexcept = default;

        constexpr NonBitwiseMoveAssignableType4(i32 *increment_ptr) noexcept
            : NonBitwiseMoveAssignableType3(increment_ptr)
        {
        }

        /**
         * Copy constructor
         * @param other The hud_test::non_bitwise_copy_assignable_type to copy
         */
        constexpr NonBitwiseMoveAssignableType4(NonBitwiseMoveAssignableType3 &&other) noexcept
            : NonBitwiseMoveAssignableType3(std::forward<NonBitwiseMoveAssignableType3>(other))
        {
        }

        constexpr NonBitwiseMoveAssignableType4 &operator=(const NonBitwiseMoveAssignableType4 &other) noexcept
        {
            if (&other != this)
            {
                NonBitwiseMoveAssignableType4::operator=(other);
                if (incrementation_ptr())
                {
                    increment_value = *incrementation_ptr();
                }
            }
            return *this;
        }

        constexpr NonBitwiseMoveAssignableType4 &operator=(NonBitwiseMoveAssignableType4 &&other) noexcept
        {
            if (&other != this)
            {
                NonBitwiseMoveAssignableType3::operator=(std::forward<NonBitwiseMoveAssignableType3>(other));
                if (incrementation_ptr())
                {
                    increment_value = *incrementation_ptr();
                }
            }
            return *this;
        }

        /**
         * Assign a hud_test::non_bitwise_copy_assignable_type to this
         * @param other The hud_test::non_bitwise_copy_assignable_type to assign
         * @return *this
         */
        constexpr NonBitwiseMoveAssignableType4 &operator=(const NonBitwiseMoveAssignableType3 &other) noexcept
        {
            if (&other != this)
            {
                NonBitwiseMoveAssignableType3::operator=(other);
                if (incrementation_ptr())
                {
                    increment_value = *incrementation_ptr();
                }
            }
            return *this;
        }

        constexpr NonBitwiseMoveAssignableType4 &operator=(NonBitwiseMoveAssignableType3 &&other) noexcept
        {
            if (&other != this)
            {
                NonBitwiseMoveAssignableType3::operator=(std::forward<NonBitwiseMoveAssignableType3>(other));
                if (incrementation_ptr())
                {
                    increment_value = *incrementation_ptr();
                }
            }
            return *this;
        }

        /** Retrieves the incremented value when the copy or move consutrctor was called */
        [[nodiscard]] constexpr i32 move_order() const noexcept
        {
            return increment_value;
        }

    private:
        /** The value of the incrementation pointer whe the copy or move constructor is called */
        i32 increment_value = -1;
    };

    static_assert(std::is_move_assignable_v<NonBitwiseMoveAssignableType4>);
    static_assert(std::is_move_constructible_v<NonBitwiseMoveAssignableType4>);
    static_assert(std::is_constructible_v<NonBitwiseMoveAssignableType4, NonBitwiseMoveAssignableType3 &&>);
    static_assert(!hud::is_bitwise_move_assignable_v<NonBitwiseMoveAssignableType4>);
    static_assert(!hud::is_bitwise_move_constructible_v<NonBitwiseMoveAssignableType4>);
} // namespace hud_test

#endif // HD_INC_MISC_NON_BITWISE_COPY_CONSTRUCTIBLE_TYPE_H
