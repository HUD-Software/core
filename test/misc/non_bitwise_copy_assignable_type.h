#ifndef HD_INC_MISC_NON_BITWISE_COPY_ASSIGNABLE_TYPE_H
#define HD_INC_MISC_NON_BITWISE_COPY_ASSIGNABLE_TYPE_H
#include <core/minimal.h>
#include <core/traits/is_bitwise_copy_assignable.h>
#include <core/traits/is_bitwise_copy_constructible.h>

namespace hud_test
{

    /**
     * A copy assignable type that is not a bitwise assignable type
     * The id is copied in order to know which non_bitwise_copy_assignable_type is the original non_bitwise_copy_assignable_type.
     */
    struct non_bitwise_copy_assignable_type
    {
        /**
         * Default construct a non_bitwise_copy_assignable_type
         */
        constexpr non_bitwise_copy_assignable_type() noexcept = default;

        /**
         * Copy constructor
         * @param other The non_bitwise_copy_assignable_type to copy
         */
        constexpr non_bitwise_copy_assignable_type(const non_bitwise_copy_assignable_type &other) noexcept
            : copy_assigned_count(other.copy_assigned_count)
            , copy_construct_count(other.copy_construct_count + 1)
            , unique_id(other.unique_id)
        {
        }

        /**
         * Construct with a given id
         * @tparam Integral The integral type to set
         * @param id The id of the non_bitwise_copy_assignable_type
         */
        template<typename Integral>
        requires(hud::is_integral_v<Integral>)
        constexpr non_bitwise_copy_assignable_type(Integral id) noexcept
            : unique_id(static_cast<i32>(id))
        {
        }

        /**
         * Assign another non_bitwise_copy_assignable_type to this
         * @param other The non_bitwise_copy_assignable_type to assign
         * @return *this
         */
        constexpr non_bitwise_copy_assignable_type &operator=(const non_bitwise_copy_assignable_type &other) noexcept
        {
            if (&other != this)
            {
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
        constexpr non_bitwise_copy_assignable_type &operator=(i32 id) noexcept
        {
            unique_id = id;
            return *this;
        }

        /** Retrieves the count of copy assignement done */
        [[nodiscard]] constexpr u32 copy_assign_count() const noexcept
        {
            return copy_assigned_count;
        }

        /** Retrieves the count of copy construction done */
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
        non_bitwise_copy_assignable_type(non_bitwise_copy_assignable_type &&) = delete;
        non_bitwise_copy_assignable_type &operator=(non_bitwise_copy_assignable_type &&) = delete;

    private:
        /** Informs if the copy assign operator was called */
        u32 copy_assigned_count = 0u;
        /** Informs if the copy constrcutor was called or not */
        u32 copy_construct_count = 0u;
        /** The id */
        i32 unique_id = 0;
    };

    static_assert(hud::is_copy_assignable_v<non_bitwise_copy_assignable_type>);
    static_assert(hud::is_copy_constructible_v<non_bitwise_copy_assignable_type>);
    static_assert(!hud::is_bitwise_copy_assignable_v<non_bitwise_copy_assignable_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<non_bitwise_copy_assignable_type>);

    /**
     * A copy assignable type that is not a bitwise assignable type
     * The id is copied in order to know which non_bitwise_copy_assignable_type_2 is the original non_bitwise_copy_assignable_type_2.
     * This type can be assigned with a non_bitwise_copy_assignable_type
     */
    struct non_bitwise_copy_assignable_type_2 : public non_bitwise_copy_assignable_type
    {
        /** Default construct */
        constexpr non_bitwise_copy_assignable_type_2() noexcept = default;
        constexpr non_bitwise_copy_assignable_type_2(const non_bitwise_copy_assignable_type_2 &) noexcept = default;

        /**
         * Construct with a given id
         * @param id The id of the non_bitwise_copy_assignable_type
         */
        constexpr non_bitwise_copy_assignable_type_2(i32 id) noexcept
            : non_bitwise_copy_assignable_type(id)
        {
        }

        /**
         * Copy constructor
         * @param other The non_bitwise_copy_assignable_type to copy
         */
        constexpr non_bitwise_copy_assignable_type_2(const non_bitwise_copy_assignable_type &other) noexcept
            : non_bitwise_copy_assignable_type(other)
        {
        }

        /**
         * Assign a non_bitwise_copy_assignable_type to this
         * @param other The non_bitwise_copy_assignable_type to assign
         * @return *this
         */
        constexpr non_bitwise_copy_assignable_type_2 &operator=(const non_bitwise_copy_assignable_type &other) noexcept
        {
            if (&other != this)
            {
                non_bitwise_copy_assignable_type::operator=(other);
            }
            return *this;
        }

        constexpr non_bitwise_copy_assignable_type_2 &operator=(const non_bitwise_copy_assignable_type_2 &other) noexcept = default;
    };

    static_assert(hud::is_copy_assignable_v<non_bitwise_copy_assignable_type_2>);
    static_assert(hud::is_copy_constructible_v<non_bitwise_copy_assignable_type_2>);
    static_assert(hud::is_copy_constructible_v<non_bitwise_copy_assignable_type_2, non_bitwise_copy_assignable_type>);
    static_assert(!hud::is_bitwise_copy_assignable_v<non_bitwise_copy_assignable_type_2>);
    static_assert(!hud::is_bitwise_copy_constructible_v<non_bitwise_copy_assignable_type_2>);

    /**
     * A copy assignable type that is not a bitwise assignable type
     * The id is copied in order to know which non_bitwise_copy_assignable_type_2 is the original non_bitwise_copy_assignable_type_2.
     * This type can be assigned with a non_bitwise_copy_assignable_type
     */
    struct non_bitwise_copy_assignable_type_3 : public non_bitwise_copy_assignable_type_2
    {

    public:
        constexpr non_bitwise_copy_assignable_type_3() noexcept = default;

        constexpr non_bitwise_copy_assignable_type_3(i32 *increment_ptr) noexcept
            : non_bitwise_copy_assignable_type_2()
            , increment(increment_ptr)
        {
        }

        /**
         * Copy constructor
         * @param other The non_bitwise_copy_assignable_type to copy
         */
        constexpr non_bitwise_copy_assignable_type_3(const non_bitwise_copy_assignable_type_3 &other) noexcept
            : non_bitwise_copy_assignable_type_2(other)
            , increment(other.increment)
        {
        }

        /**
         * Assign a non_bitwise_copy_assignable_type to this
         * @param other The non_bitwise_copy_assignable_type to assign
         * @return *this
         */
        constexpr non_bitwise_copy_assignable_type_3 &operator=(const non_bitwise_copy_assignable_type_3 &other) noexcept
        {
            if (&other != this)
            {
                non_bitwise_copy_assignable_type_2::operator=(other);
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

    static_assert(hud::is_copy_assignable_v<non_bitwise_copy_assignable_type_3>);
    static_assert(hud::is_copy_constructible_v<non_bitwise_copy_assignable_type_3>);
    static_assert(!hud::is_bitwise_copy_assignable_v<non_bitwise_copy_assignable_type_3>);
    static_assert(!hud::is_bitwise_copy_constructible_v<non_bitwise_copy_assignable_type_3>);

    /**
     * A copy assignable type that is not a bitwise assignable type
     * The id is copied in order to know which non_bitwise_copy_assignable_type_2 is the original non_bitwise_copy_assignable_type_2.
     * This type can be assigned with a non_bitwise_copy_assignable_type
     */
    struct non_bitwise_copy_assignable_type_4 : public non_bitwise_copy_assignable_type_3
    {
        /** Default construct */
        constexpr non_bitwise_copy_assignable_type_4() noexcept = default;

        /** */
        constexpr non_bitwise_copy_assignable_type_4(i32 *increment_ptr) noexcept
            : non_bitwise_copy_assignable_type_3(increment_ptr)
        {
        }

        /**
         * Copy constructor
         * @param other The non_bitwise_copy_assignable_type to copy
         */
        constexpr non_bitwise_copy_assignable_type_4(const non_bitwise_copy_assignable_type_3 &other) noexcept
            : non_bitwise_copy_assignable_type_3(other)
        {
        }

        /**
         * Assign a non_bitwise_copy_assignable_type to this
         * @param other The non_bitwise_copy_assignable_type to assign
         * @return *this
         */
        constexpr non_bitwise_copy_assignable_type_4 &operator=(const non_bitwise_copy_assignable_type_3 &other) noexcept
        {
            if (&other != this)
            {
                non_bitwise_copy_assignable_type_3::operator=(other);
                if (incrementation_ptr())
                {
                    increment_value = *incrementation_ptr();
                }
            }
            return *this;
        }

        constexpr non_bitwise_copy_assignable_type_4 &operator=(const non_bitwise_copy_assignable_type_4 &other) noexcept
        {
            if (&other != this)
            {
                non_bitwise_copy_assignable_type_3::operator=(other);
                if (incrementation_ptr())
                {
                    increment_value = *incrementation_ptr();
                }
            }
            return *this;
        }

        /** Retrieves the incremented value when the copy or move consutrctor was called */
        [[nodiscard]] constexpr i32 copy_order() const noexcept
        {
            return increment_value;
        }

    private:
        /** The value of the incrementation pointer whe the copy or move constructor is called */
        i32 increment_value = -1;
    };

    static_assert(hud::is_copy_assignable_v<non_bitwise_copy_assignable_type_4>);
    static_assert(hud::is_copy_constructible_v<non_bitwise_copy_assignable_type_4>);
    static_assert(hud::is_copy_constructible_v<non_bitwise_copy_assignable_type_4, non_bitwise_copy_assignable_type_3>);
    static_assert(!hud::is_bitwise_copy_assignable_v<non_bitwise_copy_assignable_type_4>);
    static_assert(!hud::is_bitwise_copy_constructible_v<non_bitwise_copy_assignable_type_4>);
} // namespace hud_test

#endif // HD_INC_MISC_NON_BITWISE_COPY_CONSTRUCTIBLE_TYPE_H
