#ifndef HD_INC_MISC_NON_BITWISE_MOVE_CONSTRUCTIBLE_TYPE_H
#define HD_INC_MISC_NON_BITWISE_MOVE_CONSTRUCTIBLE_TYPE_H
#include <core/minimal.h>
#include <core/traits/is_bitwise_move_constructible.h>

namespace hud_test
{

    /**
     * A move constructible type that is not a bitwise movable type
     * The id is copied in order to know which non_bitwise_move_constructible_type is the original non_bitwise_move_constructible_type.
     */
    struct non_bitwise_move_constructible_type
    {

        constexpr non_bitwise_move_constructible_type() noexcept = default;

        /**
         * Construct with a given id
         * @tparam Integral The integral type to set
         * @param id The id of the non_bitwise_move_constructible_type
         */
        template<typename Integral>
        requires(hud::is_integral_v<Integral>)
        constexpr non_bitwise_move_constructible_type(Integral id) noexcept
            : unique_id(static_cast<i32>(id))
        {
        }

        /**
         * Copy construct a non_bitwise_move_constructible_type
         * @param other The non_bitwise_move_constructible_type to copy
         */
        constexpr non_bitwise_move_constructible_type(const non_bitwise_move_constructible_type &other) noexcept
            : move_construct_count(other.move_construct_count)
            , copy_construct_count(other.copy_construct_count + 1)
            , unique_id(other.unique_id)
        {
        }

        /**
         * Move construct a non_bitwise_move_constructible_type
         * @param other The non_bitwise_move_constructible_type to move
         */
        constexpr non_bitwise_move_constructible_type(non_bitwise_move_constructible_type &&other) noexcept
            : move_construct_count(other.move_construct_count + 1)
            , copy_construct_count(other.copy_construct_count)
            , unique_id(other.unique_id)
        {
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
        non_bitwise_move_constructible_type &operator=(const non_bitwise_move_constructible_type &) = delete;
        non_bitwise_move_constructible_type &operator=(non_bitwise_move_constructible_type &&) = delete;

    private:
        /** Informs if the move constructor was called or not during construction */
        u32 move_construct_count = 0u;
        /** Informs if the copy constrcutor was called or not */
        u32 copy_construct_count = 0u;
        /** The id */
        i32 unique_id = -1;
    };

    static_assert(hud::is_move_constructible_v<non_bitwise_move_constructible_type>);
    static_assert(!hud::is_bitwise_move_constructible_v<non_bitwise_move_constructible_type>);
    static_assert(hud::is_copy_constructible_v<non_bitwise_copy_constructible_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<non_bitwise_copy_constructible_type>);

    /**
     * A move constructible type that is not a bitwise movable type
     * The id is copied in order to know which non_bitwise_move_constructible_type is the original non_bitwise_move_constructible_type.
     * This is similar to non_bitwise_move_constructible_type, it exist only have a type that is not the same as non_bitwise_move_constructible_type for tests
     */
    struct non_bitwise_move_constructible_type2 : public non_bitwise_move_constructible_type
    {
        /**
         * Construct with an id set to -1.
         * You can set the id with set_id().
         */
        constexpr non_bitwise_move_constructible_type2() noexcept
            : non_bitwise_move_constructible_type(-1)
        {
        }

        /**
         * Construct with a given id
         * @param id The id of the non_bitwise_move_constructible_type
         */
        constexpr non_bitwise_move_constructible_type2(i32 id) noexcept
            : non_bitwise_move_constructible_type(id)
        {
        }

        /**
         * Copy construct a non_bitwise_move_constructible_type
         * @param other The non_bitwise_move_constructible_type to move
         */
        constexpr non_bitwise_move_constructible_type2(const non_bitwise_move_constructible_type &other) noexcept
            : non_bitwise_move_constructible_type(other)
        {
        }

        /**
         * Move construct a non_bitwise_move_constructible_type
         * @param other The non_bitwise_move_constructible_type to move
         */
        constexpr non_bitwise_move_constructible_type2(non_bitwise_move_constructible_type &&other) noexcept
            : non_bitwise_move_constructible_type(std::forward<non_bitwise_move_constructible_type>(other))
        {
        }
    };

    static_assert(hud::is_move_constructible_v<non_bitwise_move_constructible_type2>);
    static_assert(!hud::is_bitwise_move_constructible_v<non_bitwise_move_constructible_type2>);
    static_assert(!hud::is_bitwise_move_constructible_v<non_bitwise_move_constructible_type2, non_bitwise_move_constructible_type>);
    static_assert(!hud::is_bitwise_copy_constructible_v<non_bitwise_move_constructible_type2>);

    /**
     * A move constructible type that is not a bitwise movable type
     * The id is copied in order to know which non_bitwise_move_constructible_type is the original non_bitwise_move_constructible_type.
     * This type add a incremanation pointer information to the type used by non_bitwise_move_constructible_type4 when this type is constructed.
     */
    struct non_bitwise_move_constructible_type3 : public non_bitwise_move_constructible_type2
    {

    public:
        /**
         * Default construct non_bitwise_move_constructible_type3
         * Set the incrementation pointer to nullptr
         */
        constexpr non_bitwise_move_constructible_type3() noexcept = default;

        constexpr non_bitwise_move_constructible_type3(i32 *increment_ptr) noexcept
            : non_bitwise_move_constructible_type2()
            , increment(increment_ptr)
        {
        }

        constexpr non_bitwise_move_constructible_type3(const non_bitwise_move_constructible_type3 &other) noexcept
            : non_bitwise_move_constructible_type2(other)
            , increment(other.increment)
        {
        }

        constexpr non_bitwise_move_constructible_type3(non_bitwise_move_constructible_type3 &&other) noexcept
            : non_bitwise_move_constructible_type2(std::forward<non_bitwise_move_constructible_type2>(other))
            , increment(other.increment)
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

    struct non_bitwise_move_constructible_type4 : public non_bitwise_move_constructible_type3
    {
        /**
         * Default construct non_bitwise_move_constructible_type4
         * Set the incrementation pointer to nullptr
         */
        constexpr non_bitwise_move_constructible_type4() noexcept = default;

        constexpr non_bitwise_move_constructible_type4(i32 *increment_ptr) noexcept
            : non_bitwise_move_constructible_type3(increment_ptr)
        {
        }

        /**
         * Move construct a non_bitwise_move_constructible_type4
         * @param other The non_bitwise_move_constructible_type3 to move
         */
        constexpr non_bitwise_move_constructible_type4(const non_bitwise_move_constructible_type4 &other) noexcept
            : non_bitwise_move_constructible_type3(other)
        {
            if (incrementation_ptr())
            {
                increment_value = ++(*incrementation_ptr());
            }
        }

        /**
         * Move construct a non_bitwise_move_constructible_type4
         * @param other The non_bitwise_move_constructible_type3 to move
         */
        constexpr non_bitwise_move_constructible_type4(non_bitwise_move_constructible_type4 &&other) noexcept
            : non_bitwise_move_constructible_type3(std::forward<non_bitwise_move_constructible_type3>(other))
        {
            if (incrementation_ptr())
            {
                increment_value = ++(*incrementation_ptr());
            }
        }

        /**
         * Copy construct a non_bitwise_move_constructible_type4
         * @param other The non_bitwise_move_constructible_type3 to move
         */
        constexpr non_bitwise_move_constructible_type4(const non_bitwise_move_constructible_type3 &other) noexcept
            : non_bitwise_move_constructible_type3(other)
        {
            if (incrementation_ptr())
            {
                increment_value = ++(*incrementation_ptr());
            }
        }

        /**
         * Move construct a non_bitwise_move_constructible_type4
         * @param other The non_bitwise_move_constructible_type3 to move
         */
        constexpr non_bitwise_move_constructible_type4(non_bitwise_move_constructible_type3 &&other) noexcept
            : non_bitwise_move_constructible_type3(std::forward<non_bitwise_move_constructible_type3>(other))
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
