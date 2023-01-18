#ifndef HD_INC_MISC_NON_BITWISE_COPY_CONSTRUCTIBLE_TYPE_H
#define HD_INC_MISC_NON_BITWISE_COPY_CONSTRUCTIBLE_TYPE_H
#include <core/minimal.h>
#include <type_traits>
#include <core/traits/is_bitwise_copy_constructible.h>

namespace hud_test
{

    /**
     * A copy constructible type that is not a bitwise copyable type
     * The id is copied in order to know which NonBitwiseCopyConstructibleType is the original NonBitwiseCopyConstructibleType.
     */
    struct NonBitwiseCopyConstructibleType
    {
    public:
        /**
         * Construct with a given id
         * @tparam Integral The integral type to set
         * @param id The id of the NonBitwiseCopyConstructibleType
         */
        constexpr NonBitwiseCopyConstructibleType() noexcept = default;

        /**
         * Construct with a given id
         * @tparam Integral The integral type to set
         * @param id The id of the NonBitwiseCopyConstructibleType
         */
        template<typename Integral>
        requires(hud::is_integral_v<Integral>)
        constexpr NonBitwiseCopyConstructibleType(Integral id) noexcept
            : unique_id(static_cast<i32>(id))
        {
        }

        /**
         * Copy construct a NonBitwiseCopyConstructibleType
         * @param other The NonBitwiseCopyConstructibleType to copy
         */
        constexpr NonBitwiseCopyConstructibleType(const NonBitwiseCopyConstructibleType &other) noexcept
            : copy_construct_count(other.copy_construct_count + 1)
            , unique_id(other.unique_id)
        {
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
        NonBitwiseCopyConstructibleType(NonBitwiseCopyConstructibleType &&other) = delete;
        NonBitwiseCopyConstructibleType &operator=(const NonBitwiseCopyConstructibleType &) = delete;
        NonBitwiseCopyConstructibleType &operator=(NonBitwiseCopyConstructibleType &&) = delete;

    private:
        /** Count of copy construction done */
        u32 copy_construct_count = 0u;

        /** The id */
        i32 unique_id = 0;
    };

    static_assert(std::is_copy_constructible_v<NonBitwiseCopyConstructibleType>);
    static_assert(!hud::is_bitwise_copy_constructible_v<NonBitwiseCopyConstructibleType>);

    /**
     * A copy constructible type that is not a bitwise copyable type
     * The id is copied in order to know which NonBitwiseCopyConstructibleType is the original NonBitwiseCopyConstructibleType.
     * This type can be construct with a hud_test::non_bitwise_copy_assignable_type
     */
    struct NonBitwiseCopyConstructibleType2 : public NonBitwiseCopyConstructibleType
    {
        /**
         * Construct with an id set to -1.
         * You can set the id with set_id().
         */
        constexpr NonBitwiseCopyConstructibleType2() noexcept = default;

        /**
         * Construct with a given id
         * @param id The id of the NonBitwiseCopyConstructibleType2
         */
        constexpr NonBitwiseCopyConstructibleType2(i32 id) noexcept
            : NonBitwiseCopyConstructibleType(id)
        {
        }

        /**
         * Copy construct a NonBitwiseCopyConstructibleType2
         * @param other The NonBitwiseCopyConstructibleType2 to copy
         */
        constexpr NonBitwiseCopyConstructibleType2(const NonBitwiseCopyConstructibleType &other) noexcept
            : NonBitwiseCopyConstructibleType(other)
        {
        }
    };

    static_assert(std::is_constructible_v<NonBitwiseCopyConstructibleType2, const NonBitwiseCopyConstructibleType &>);
    static_assert(std::is_copy_constructible_v<NonBitwiseCopyConstructibleType2>);
    static_assert(!hud::is_bitwise_copy_constructible_v<NonBitwiseCopyConstructibleType2>);

    /**
     * A copy constructible type that is not a bitwise moveable type
     * The id is copied in order to know which NonBitwiseMoveConstructibleType is the original NonBitwiseMoveConstructibleType.
     * This type add a incremanation pointer information to the type used by NonBitwiseMoveConstructibleType4 when this type is constructed.
     */
    struct NonBitwiseCopyConstructibleType3 : public NonBitwiseCopyConstructibleType2
    {

    public:
        /**
         * Default construct NonBitwiseMoveConstructibleType3
         * Set the incrementation pointer to nullptr
         */
        constexpr NonBitwiseCopyConstructibleType3() noexcept
            : NonBitwiseCopyConstructibleType2()
        {
        }

        constexpr NonBitwiseCopyConstructibleType3(i32 *increment_ptr) noexcept
            : NonBitwiseCopyConstructibleType2()
            , increment(increment_ptr)
        {
        }

        constexpr NonBitwiseCopyConstructibleType3(const NonBitwiseCopyConstructibleType3 &other) noexcept
            : NonBitwiseCopyConstructibleType2(other)
            , increment(other.increment)
        {
            if (increment)
            {
                ++(*increment);
            }
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

    static_assert(std::is_constructible_v<NonBitwiseCopyConstructibleType3, const NonBitwiseCopyConstructibleType3 &>);
    static_assert(std::is_copy_constructible_v<NonBitwiseCopyConstructibleType3>);
    static_assert(!hud::is_bitwise_copy_constructible_v<NonBitwiseCopyConstructibleType3>);

    struct NonBitwiseCopyConstructibleType4 : public NonBitwiseCopyConstructibleType3
    {
        /**
         * Default construct NonBitwiseMoveConstructibleType4
         * Set the incrementation pointer to nullptr
         */
        constexpr NonBitwiseCopyConstructibleType4() noexcept = default;

        constexpr NonBitwiseCopyConstructibleType4(i32 *increment_ptr) noexcept
            : NonBitwiseCopyConstructibleType3(increment_ptr)
        {
        }

        constexpr NonBitwiseCopyConstructibleType4(const NonBitwiseCopyConstructibleType4 &other) noexcept
            : NonBitwiseCopyConstructibleType3(other)
        {
            if (incrementation_ptr())
            {
                increment_value = *incrementation_ptr();
            }
        }

        /**
         * Copy construct a NonBitwiseMoveConstructibleType4
         * @param other The NonBitwiseMoveConstructibleType3 to move
         */
        constexpr NonBitwiseCopyConstructibleType4(const NonBitwiseCopyConstructibleType3 &other) noexcept
            : NonBitwiseCopyConstructibleType3(other)
        {
            if (incrementation_ptr())
            {
                increment_value = *incrementation_ptr();
            }
        }

        /** Retrieves the incremented value when the copy or move consutrctor was called */
        [[nodiscard]] constexpr i32 ctor_order() const noexcept
        {
            return increment_value;
        }

        constexpr NonBitwiseCopyConstructibleType4(NonBitwiseCopyConstructibleType4 &&other) = delete;
        constexpr NonBitwiseCopyConstructibleType4(NonBitwiseCopyConstructibleType3 &&other) = delete;

    private:
        /** The value of the incrementation pointer whe the copy or move constructor is called */
        i32 increment_value = -1;
    };

    static_assert(std::is_constructible_v<NonBitwiseCopyConstructibleType4, const NonBitwiseCopyConstructibleType3 &>);
    static_assert(std::is_copy_constructible_v<NonBitwiseCopyConstructibleType4>);
    static_assert(!hud::is_bitwise_copy_constructible_v<NonBitwiseCopyConstructibleType4>);
    static_assert(!hud::is_move_constructible_v<NonBitwiseCopyConstructibleType4, NonBitwiseCopyConstructibleType3>);

} // namespace hud_test

#endif // HD_INC_MISC_NON_BITWISE_COPY_CONSTRUCTIBLE_TYPE_H
