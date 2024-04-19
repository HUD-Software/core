#ifndef HD_INC_MISC_DEFAULT_CONSTRUCTIBLE_TYPE_H
#define HD_INC_MISC_DEFAULT_CONSTRUCTIBLE_TYPE_H
#include <core/minimal.h>

namespace hud_test
{

    /**
     * A default constructible type
     */
    struct default_constructible_type
    {

        static constexpr const i32 DEFAULT_ID_VALUE = hud::i32_max;

        /** Default constructor */
        constexpr default_constructible_type() noexcept
            : default_construct_count(1)
        {
        }

        /**
         * Constructor
         * @param id The ID to of the default_constructible_type
         */
        constexpr default_constructible_type(i32 id) noexcept
            : ctor_count(1u)
            , unique_id(id)
        {
        }

        /**
         * Copy construct a default_constructible_type
         * @param other The default_constructible_type to copy
         */
        constexpr default_constructible_type(const default_constructible_type &other) noexcept
            : default_construct_count(other.default_construct_count)
            , move_construct_count(other.move_construct_count)
            , copy_construct_count(other.copy_construct_count + 1)
            , ctor_count(other.ctor_count)
            , unique_id(other.unique_id)
        {
        }

        /**
         * Move construct a default_constructible_type
         * @param other The default_constructible_type to move
         */
        constexpr default_constructible_type(default_constructible_type &&other) noexcept
            : default_construct_count(other.default_construct_count)
            , move_construct_count(other.move_construct_count + 1)
            , copy_construct_count(other.copy_construct_count)
            , ctor_count(other.ctor_count)
            , unique_id(other.unique_id)
        {
        }

        /** Retrieves the count of default construction done */
        [[nodiscard]] constexpr u32 default_constructor_count() const noexcept
        {
            return default_construct_count;
        }

        /** Retrieves the count of move constructor done */
        [[nodiscard]] constexpr u32 move_constructor_count() const noexcept
        {
            return move_construct_count;
        }

        /** Retrieves the count of copy construction done */
        [[nodiscard]] constexpr u32 copy_constructor_count() const noexcept
        {
            return copy_construct_count;
        }

        /** Retrieves the count of constructor done */
        [[nodiscard]] constexpr u32 constructor_count() const noexcept
        {
            return ctor_count;
        }

        /** Retrieves the id, id is i32_max when default constructor is called */
        [[nodiscard]] constexpr i32 id() const noexcept
        {
            return unique_id;
        }

    private:
        /** Informs if the default constructor was called or not */
        u32 default_construct_count = 0u;
        /** Informs if the move constructor was called or not during construction */
        u32 move_construct_count = 0u;
        /** Informs if the copy constrcutor was called or not */
        u32 copy_construct_count = 0u;
        /** Informs if the constructor was called or not */
        u32 ctor_count = 0u;
        /** The id */
        i32 unique_id = DEFAULT_ID_VALUE;
    };

} // namespace hud_test

#endif // HD_INC_MISC_DEFAULT_CONSTRUCTIBLE_TYPE_H
