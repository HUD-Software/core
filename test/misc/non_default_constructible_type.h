#ifndef HD_INC_MISC_NON_DEFAULT_CONSTRUCTIBLE_TYPE_H
#define HD_INC_MISC_NON_DEFAULT_CONSTRUCTIBLE_TYPE_H
#include <core/minimal.h>

namespace hud_test
{

    /**
     * A type that is not default constructible
     */
    struct NonDefaultConstructibleType
    {

        /** Disable default construtor */
        constexpr NonDefaultConstructibleType() noexcept = delete;

        /**
         * Constructor
         * @param id The ID to of the NonDefaultConstructibleType
         */
        constexpr NonDefaultConstructibleType(const i32 id) noexcept
            : ctor_count(1u)
            , unique_id(id)
        {
        }

        /**
         * Copy construct a NonDefaultConstructibleType
         * @param other The NonDefaultConstructibleType to copy
         */
        constexpr NonDefaultConstructibleType(const NonDefaultConstructibleType &other) noexcept
            : move_construct_count(other.move_construct_count)
            , copy_construct_count(other.copy_construct_count + 1)
            , ctor_count(other.ctor_count)
            , unique_id(other.unique_id)
        {
        }

        /**
         * Move construct a NonDefaultConstructibleType
         * @param other The NonDefaultConstructibleType to move
         */
        constexpr NonDefaultConstructibleType(NonDefaultConstructibleType &&other) noexcept
            : move_construct_count(other.move_construct_count + 1)
            , copy_construct_count(other.copy_construct_count)
            , ctor_count(other.ctor_count)
            , unique_id(other.unique_id)
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

        /** Retrieves count of constructor done */
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
        /** Informs if the move constructor was called or not during construction */
        u32 move_construct_count = 0u;
        /** Informs if the copy constrcutor was called or not */
        u32 copy_construct_count = 0u;
        /** Informs if the constructor was called or not */
        u32 ctor_count = 0u;
        /** The id */
        i32 unique_id;
    };

} // namespace hud_test

#endif // HD_INC_MISC_NON_DEFAULT_CONSTRUCTIBLE_TYPE_H
