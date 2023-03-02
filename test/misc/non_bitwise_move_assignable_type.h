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
     * The id is copied in order to know which non_bitwise_move_assignable_type is the original non_bitwise_move_assignable_type.
     */
    struct non_bitwise_move_assignable_type
    {
        /**
         * Default constrcut a non_bitwise_move_assignable_type
         */
        constexpr non_bitwise_move_assignable_type() noexcept = default;

        /**
         * Copy constructor
         * @param other The hud_test::non_bitwise_copy_assignable_type to copy
         */
        constexpr non_bitwise_move_assignable_type(const non_bitwise_move_assignable_type &other) noexcept
            : move_assigned_count(other.move_assigned_count)
            , copy_assigned_count(other.copy_assigned_count)
            , move_construct_count(other.move_construct_count)
            , copy_construct_count(other.copy_construct_count + 1)
            , unique_id(other.unique_id)
        {
        }

        /**
         * Move constructor
         * @param other The non_bitwise_move_assignable_type to move
         */
        constexpr non_bitwise_move_assignable_type(non_bitwise_move_assignable_type &&other) noexcept
            : move_assigned_count(other.move_assigned_count)
            , copy_assigned_count(other.copy_assigned_count)
            , move_construct_count(other.move_construct_count + 1)
            , copy_construct_count(other.copy_construct_count)
            , unique_id(other.unique_id)
        {
        }

        /**
         * Construct with a given id
         * @param id The id of the non_bitwise_move_assignable_type
         */
        constexpr non_bitwise_move_assignable_type(i32 id) noexcept
            : unique_id(id)
        {
        }

        /**
         * Assign another non_bitwise_move_assignable_type to this
         * @param other The non_bitwise_move_assignable_type to assign
         * @return *this
         */
        constexpr non_bitwise_move_assignable_type &operator=(non_bitwise_move_assignable_type &&other) noexcept
        {
            move_assigned_count = other.move_assigned_count + 1;
            move_construct_count = other.move_construct_count;
            copy_construct_count = other.copy_construct_count;
            copy_assigned_count = other.copy_assigned_count;
            unique_id = other.unique_id;
            return *this;
        }

        /**
         * Assign another non_bitwise_move_assignable_type to this
         * @param other The non_bitwise_move_assignable_type to assign
         * @return *this
         */
        constexpr non_bitwise_move_assignable_type &operator=(const non_bitwise_move_assignable_type &other) noexcept
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

    static_assert(std::is_move_assignable_v<non_bitwise_move_assignable_type>);
    static_assert(std::is_move_constructible_v<non_bitwise_move_assignable_type>);
    static_assert(!hud::is_bitwise_move_assignable_v<non_bitwise_move_assignable_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<non_bitwise_move_assignable_type>);

    /**
     * A move assignable type that is not a bitwise assignable type
     * The id is copied in order to know which non_bitwise_move_assignable_type2 is the original non_bitwise_move_assignable_type2.
     * This type can be assigned with a non_bitwise_move_assignable_type2
     */
    struct non_bitwise_move_assignable_type2 : public non_bitwise_move_assignable_type
    {

        /** Default construct */
        constexpr non_bitwise_move_assignable_type2() noexcept = default;
        constexpr non_bitwise_move_assignable_type2(const non_bitwise_move_assignable_type2 &) noexcept = default;
        constexpr non_bitwise_move_assignable_type2(non_bitwise_move_assignable_type2 &&) noexcept = default;

        /**
         * Construct with a given id
         * @param id The id of the non_bitwise_move_assignable_type2
         */
        constexpr non_bitwise_move_assignable_type2(i32 id) noexcept
            : non_bitwise_move_assignable_type(id)
        {
        }

        /**
         * Copy constructor
         * @param other The hud_test::non_bitwise_copy_assignable_type to copy
         */
        constexpr non_bitwise_move_assignable_type2(const non_bitwise_move_assignable_type &other) noexcept
            : non_bitwise_move_assignable_type(other)
        {
        }

        /**
         * Move constructor
         * @param other The hud_test::non_bitwise_copy_assignable_type to copy
         */
        constexpr non_bitwise_move_assignable_type2(non_bitwise_move_assignable_type &&other) noexcept
            : non_bitwise_move_assignable_type(std::forward<non_bitwise_move_assignable_type>(other))
        {
        }

        /**
         * Assign another non_bitwise_move_assignable_type2 to this
         * @param other The non_bitwise_move_assignable_type2 to assign
         * @return *this
         */
        constexpr non_bitwise_move_assignable_type2 &operator=(non_bitwise_move_assignable_type &&other) noexcept
        {
            non_bitwise_move_assignable_type::operator=(std::forward<non_bitwise_move_assignable_type>(other));
            return *this;
        }

        constexpr non_bitwise_move_assignable_type2 &operator=(const non_bitwise_move_assignable_type2 &other) noexcept = default;
        constexpr non_bitwise_move_assignable_type2 &operator=(non_bitwise_move_assignable_type2 &&other) noexcept = default;
    };

    static_assert(std::is_move_assignable_v<non_bitwise_move_assignable_type>);
    static_assert(std::is_move_constructible_v<non_bitwise_move_assignable_type2>);
    static_assert(std::is_constructible_v<non_bitwise_move_assignable_type2, non_bitwise_move_assignable_type &&>);
    static_assert(!hud::is_bitwise_move_assignable_v<non_bitwise_move_assignable_type2>);
    static_assert(!hud::is_bitwise_move_constructible_v<non_bitwise_move_assignable_type2>);

    /**
     * A move assignable type that is not a bitwise assignable type
     * The id is copied in order to know which non_bitwise_move_assignable_type2 is the original non_bitwise_move_assignable_type2.
     * This type can be assigned with a non_bitwise_move_assignable_type
     */
    struct non_bitwise_move_assignable_type3 : public non_bitwise_move_assignable_type2
    {

    public:
        constexpr non_bitwise_move_assignable_type3() noexcept = default;

        constexpr non_bitwise_move_assignable_type3(i32 *increment_ptr) noexcept
            : non_bitwise_move_assignable_type2()
            , increment(increment_ptr)
        {
        }

        /**
         * Move constructor
         * @param other The hud_test::non_bitwise_copy_assignable_type to copy
         */
        constexpr non_bitwise_move_assignable_type3(non_bitwise_move_assignable_type3 &&other) noexcept
            : non_bitwise_move_assignable_type2(std::forward<non_bitwise_move_assignable_type3>(other))
            , increment(other.increment)
        {
        }

        /**
         * Assign a non_bitwise_move_assignable_type to this
         * @param other The non_bitwise_move_assignable_type to assign
         * @return *this
         */
        constexpr non_bitwise_move_assignable_type3 &operator=(const non_bitwise_move_assignable_type3 &other) noexcept
        {
            if (&other != this)
            {
                non_bitwise_move_assignable_type2::operator=(other);
                increment = other.increment;
                if (increment)
                {
                    ++(*increment);
                }
            }
            return *this;
        }

        /**
         * Assign a non_bitwise_move_assignable_type to this
         * @param other The non_bitwise_move_assignable_type to assign
         * @return *this
         */
        constexpr non_bitwise_move_assignable_type3 &operator=(non_bitwise_move_assignable_type3 &&other) noexcept
        {
            if (&other != this)
            {
                non_bitwise_move_assignable_type2::operator=(std::forward<non_bitwise_move_assignable_type3>(other));
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

    static_assert(std::is_move_assignable_v<non_bitwise_move_assignable_type3>);
    static_assert(std::is_move_constructible_v<non_bitwise_move_assignable_type3>);
    static_assert(std::is_constructible_v<non_bitwise_move_assignable_type3, non_bitwise_move_assignable_type3 &&>);
    static_assert(!hud::is_bitwise_move_assignable_v<non_bitwise_move_assignable_type3>);
    static_assert(!hud::is_bitwise_move_constructible_v<non_bitwise_move_assignable_type3>);

    /**
     * A move assignable type that is not a bitwise assignable type
     * The id is copied in order to know which non_bitwise_move_assignable_type2 is the original non_bitwise_move_assignable_type2.
     * This type can be assigned with a non_bitwise_move_assignable_type
     */
    struct non_bitwise_move_assignable_type4 : public non_bitwise_move_assignable_type3
    {
        /** Default construct */
        constexpr non_bitwise_move_assignable_type4() noexcept = default;
        constexpr non_bitwise_move_assignable_type4(non_bitwise_move_assignable_type4 &&) noexcept = default;

        constexpr non_bitwise_move_assignable_type4(i32 *increment_ptr) noexcept
            : non_bitwise_move_assignable_type3(increment_ptr)
        {
        }

        /**
         * Copy constructor
         * @param other The hud_test::non_bitwise_copy_assignable_type to copy
         */
        constexpr non_bitwise_move_assignable_type4(non_bitwise_move_assignable_type3 &&other) noexcept
            : non_bitwise_move_assignable_type3(std::forward<non_bitwise_move_assignable_type3>(other))
        {
        }

        constexpr non_bitwise_move_assignable_type4 &operator=(const non_bitwise_move_assignable_type4 &other) noexcept
        {
            if (&other != this)
            {
                non_bitwise_move_assignable_type4::operator=(other);
                if (incrementation_ptr())
                {
                    increment_value = *incrementation_ptr();
                }
            }
            return *this;
        }

        constexpr non_bitwise_move_assignable_type4 &operator=(non_bitwise_move_assignable_type4 &&other) noexcept
        {
            if (&other != this)
            {
                non_bitwise_move_assignable_type3::operator=(std::forward<non_bitwise_move_assignable_type3>(other));
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
        constexpr non_bitwise_move_assignable_type4 &operator=(const non_bitwise_move_assignable_type3 &other) noexcept
        {
            if (&other != this)
            {
                non_bitwise_move_assignable_type3::operator=(other);
                if (incrementation_ptr())
                {
                    increment_value = *incrementation_ptr();
                }
            }
            return *this;
        }

        constexpr non_bitwise_move_assignable_type4 &operator=(non_bitwise_move_assignable_type3 &&other) noexcept
        {
            if (&other != this)
            {
                non_bitwise_move_assignable_type3::operator=(std::forward<non_bitwise_move_assignable_type3>(other));
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

    static_assert(std::is_move_assignable_v<non_bitwise_move_assignable_type4>);
    static_assert(std::is_move_constructible_v<non_bitwise_move_assignable_type4>);
    static_assert(std::is_constructible_v<non_bitwise_move_assignable_type4, non_bitwise_move_assignable_type3 &&>);
    static_assert(!hud::is_bitwise_move_assignable_v<non_bitwise_move_assignable_type4>);
    static_assert(!hud::is_bitwise_move_constructible_v<non_bitwise_move_assignable_type4>);
} // namespace hud_test

#endif // HD_INC_MISC_NON_BITWISE_COPY_CONSTRUCTIBLE_TYPE_H
