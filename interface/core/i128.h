#ifndef HD_INC_CORE_I128_H
#define HD_INC_CORE_I128_H
#include "traits/conditional.h"

#if !defined(HD_INC_CORE_TYPES_H)
    #error i128.h must be included after or in types.h
#endif
#include "assert.h"
#include <math.h> //ldexpf

namespace hud
{
    class i128;
    class u128;

    namespace details::i128
    {

        static inline void u128_from_f32(f32 value, u64 &high, u64 &low) noexcept
        {
            // TODO, put this in cpp and use hud functions
            // check(std::isfinite(hud::f64_max));
            //  If value is greater than value*(2^64)
            if (value >= ldexpf(static_cast<f32>(1), 64))
            {
                high = static_cast<u64>(ldexpf(value, -64));
                low = static_cast<u64>(value - ldexpf(static_cast<f32>(high), 64));
            }
            else
            {
                high = 0;
                low = static_cast<u64>(value);
            }
        }

        static inline void u128_from_f64(f64 value, u64 &high, u64 &low) noexcept
        {

            // TODO, put this in cpp and use hud functions

            // Check not NaN
            // check(std::isfinite(value));

            // Check if it can fit in 128 bit
            // check((std::numeric_limits<f64>::max_exponent <= 127) || (value >= -std::ldexp(static_cast<f64>(1), 127)));
            // check(value < std::ldexp(static_cast<f64>(1), 127));

            // If value is greater than value*(2^64)
            if (value >= ldexp(static_cast<f64>(1), 64))
            {
                high = static_cast<u64>(ldexp(value, -64));
                low = static_cast<u64>(value - ldexp(static_cast<f64>(high), 64));
            }
            else
            {
                high = 0;
                low = static_cast<u64>(value);
            }
        }

        /** i128 no intrinsic implementation for little endian platform. */
        struct i128_little
        {
            /** Default constructor. */
            constexpr i128_little() = default;

            /** Construct a i128 from low and high part. */
            constexpr i128_little(i64 high, u64 low) noexcept
                : low_ {low}
                , high_ {high}
            {
            }

            /** Construct a i128 from i32. */
            constexpr i128_little(i32 value) noexcept
                : low_ {static_cast<u64>(value)}
                , high_ {value < 0 ? ~i64 {0} : 0}
            {
            }

            /** Construct a i128 from u32. */
            constexpr i128_little(u32 value) noexcept
                : low_ {value}
                , high_ {0}
            {
            }

            /** Construct a i128 from i64. */
            constexpr i128_little(i64 value) noexcept
                : low_ {static_cast<u64>(value)}
                , high_ {value < 0 ? ~i64 {0} : 0}
            {
            }

            /** Construct a i128 from u64. */
            constexpr i128_little(u64 value) noexcept
                : low_ {value}
                , high_ {0}
            {
            }

            /** Construct a i128 from f32. */
            i128_little(f32 value) noexcept
            {
            }

            /** Construct a i128 from f64. */
            i128_little(f64 value) noexcept
            {
            }

            /** Construct a i128 from u128. */
            explicit constexpr i128_little(hud::u128 value) noexcept;

            /** The low part of the i128. */
            u64 low_;
            /** The high part of the i128. */
            i64 high_;
        };

        /** i128 no intrinsic implementation for big endian platform. */
        struct i128_big
        {
            /** Default constructor. */
            constexpr i128_big() = default;

            /** Construct a i128 from low and high part. */
            constexpr i128_big(i64 high, u64 low) noexcept
                : high_ {high}
                , low_ {low}
            {
            }

            /** Construct a i128 from i32. */
            constexpr i128_big(i32 value) noexcept
                : high_ {value < 0 ? ~i64 {0} : 0}
                , low_ {static_cast<u64>(value)}
            {
            }

            /** Construct a i128 from u32. */
            constexpr i128_big(u32 value) noexcept
                : high_ {0}
                , low_ {value}
            {
            }

            /** Construct a i128 from i64. */
            constexpr i128_big(i64 value) noexcept
                : high_ {value < 0 ? ~i64 {0} : 0}
                , low_ {static_cast<u64>(value)}
            {
            }

            /** Construct a i128 from u64. */
            constexpr i128_big(u64 value) noexcept
                : high_ {0}
                , low_ {value}
            {
            }

            /** Construct a i128 from u128. */
            explicit constexpr i128_big(hud::u128 value) noexcept;

            /** Construct a i128 from f32. */
            i128_big(f32 value) noexcept
            {
            }

            /** Construct a i128 from f64. */
            i128_big(f64 value) noexcept
            {
            }

            /** The high part of the i128. */
            i64 high_;
            /** The low part of the i128. */
            u64 low_;
        };

        using i128_impl = hud::conditional_t<hud::compilation::is_endianness(endianness_e::little), details::i128::i128_little, details::i128::i128_big>;

        /** u128 no intrinsic implementation for little endian platform. */
        struct u128_little
        {
            /** Default constructor. */
            constexpr u128_little() = default;

            /** Construct a u128 from low and high part. */
            constexpr u128_little(u64 high, u64 low) noexcept
                : low_ {low}
                , high_ {high}
            {
            }

            /** Construct a u128 from i32. */
            constexpr u128_little(i32 value) noexcept
                : low_ {static_cast<u64>(value)}
                , high_ {value < 0 ? hud::u64_max : 0}
            {
            }

            /** Construct a u128 from u32. */
            constexpr u128_little(u32 value) noexcept
                : low_ {value}
                , high_ {0}
            {
            }

            /** Construct a u128 from i64. */
            constexpr u128_little(i64 value) noexcept
                : low_ {static_cast<u64>(value)}
                , high_ {value < 0 ? hud::u64_max : 0}
            {
            }

            /** Construct a u128 from u64. */
            constexpr u128_little(u64 value) noexcept
                : low_ {value}
                , high_ {0}
            {
            }

            /** Construct a u128 from i128. */
            explicit constexpr u128_little(hud::i128 value) noexcept;

            /** Construct a i128 from f32. */
            u128_little(f32 value) noexcept
            {
                u128_from_f32(value, high_, low_);
            }

            /** Construct a i128 from f64. */
            u128_little(f64 value) noexcept
            {
                u128_from_f64(value, high_, low_);
            }

            /** The low part of the u128. */
            u64 low_;
            /** The high part of the u128. */
            u64 high_;
        };

        /** u128 no intrinsic implementation for big endian platform. */
        struct u128_big
        {
            /** Default constructor. */
            constexpr u128_big() = default;

            /** Construct a u128 from low and high part. */
            constexpr u128_big(u64 high, u64 low) noexcept
                : high_ {high}
                , low_ {low}
            {
            }

            /** Construct a u128 from i32. */
            constexpr u128_big(i32 value) noexcept
                : high_ {value < 0 ? hud::u64_max : 0}
                , low_ {static_cast<u64>(value)}
            {
            }

            /** Construct a u128 from u32. */
            constexpr u128_big(u32 value) noexcept
                : high_ {0}
                , low_ {value}
            {
            }

            /** Construct a u128 from i64. */
            constexpr u128_big(i64 value) noexcept
                : high_ {value < 0 ? hud::u64_max : 0}
                , low_ {static_cast<u64>(value)}
            {
            }

            /** Construct a u128 from u64. */
            constexpr u128_big(u64 value) noexcept
                : high_ {0}
                , low_ {value}
            {
            }

            /** Construct a u128 from i128. */
            explicit constexpr u128_big(hud::i128 value) noexcept;

            /** Construct a i128 from f32. */
            u128_big(f32 value) noexcept
            {
                u128_from_f32(value, high_, low_);
            }

            /** Construct a i128 from f64. */
            u128_big(f64 value) noexcept
            {
                u128_from_f64(value, high_, low_);
            }

            /** The high part of the u128. */
            u64 high_;
            /** The low part of the u128. */
            u64 low_;
        };

        using u128_impl = hud::conditional_t<hud::compilation::is_endianness(endianness_e::little), details::i128::u128_little, details::i128::u128_big>;
    } // namespace details::i128

    class i128
        : protected details::i128::i128_impl
    {
    private:
        using super = details::i128::i128_impl;

    public:
        /** Default constructor. */
        constexpr i128() = default;

        /** Construct a i128 from low and high part. */
        constexpr i128(i64 high, u64 low) noexcept
            : super(low, high)
        {
        }

        /** Constructor i128 from i32. */
        constexpr i128(i32 value) noexcept
            : super(value)
        {
        }

        /** Constructor i128 from u32. */
        constexpr i128(u32 value) noexcept
            : super(value)
        {
        }

        /** Constructor i128 from i64. */
        constexpr i128(i64 value) noexcept
            : super(value)
        {
        }

        /** Constructor i128 from u64. */
        constexpr i128(u64 value) noexcept
            : super(value)
        {
        }

        /** Constructor i128 from u128. */
        explicit constexpr i128(u128 value) noexcept;

        /** Construct a i128 from f32. */
        i128(f32 value) noexcept
            : super(value)
        {
        }

        /** Construct a i128 from f64. */
        i128(f64 value) noexcept
            : super(value)
        {
        }

        /** Retrieves the low part of the i128. */
        [[nodiscard]] constexpr u64 low() const noexcept
        {
            return low_;
        }

        /** Retrieves the high part of the i128. */
        [[nodiscard]] constexpr i64 high() const noexcept
        {
            return high_;
        }
    };

    /**
     * Checks whether right and left i128 are equal.
     * @param left The left i128 to compare
     * @param right The right i128 to compare
     * @param true if right and left i128 are equal, false otherwise
     */
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(const i128 &left, const i128 &right) noexcept
    {
        return left.low() == right.low() && left.high() == right.high();
    }

    /**
     * Checks whether right and left i128 are not equal.
     * @param left The left i128 to compare
     * @param right The right i128 to compare
     * @param true if right and left i128 are equal, false otherwise
     */
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator!=(const i128 &left, const i128 &right) noexcept
    {
        return !(left == right);
    }

    static inline constexpr i128 i128_max = i128 {i64_max, u64_max};
    static inline constexpr i128 i128_min = i128 {i64_min, 0u};

    class u128
        : protected details::i128::u128_impl
    {
    private:
        using super = details::i128::u128_impl;

    public:
        /** Default constructor. */
        constexpr u128() = default;

        /** Construct a u128 from low and high part. */
        constexpr u128(u64 high, u64 low) noexcept
            : super(low, high)
        {
        }

        /** Constructor u128 from i32. */
        constexpr u128(i32 value) noexcept
            : super(value)
        {
        }

        /** Constructor u128 from u32. */
        constexpr u128(u32 value) noexcept
            : super(value)
        {
        }

        /** Constructor u128 from i64. */
        constexpr u128(i64 value) noexcept
            : super(value)
        {
        }

        /** Constructor u128 from u64. */
        constexpr u128(u64 value) noexcept
            : super(value)
        {
        }

        /** Constructor u128 from u128. */
        explicit constexpr u128(i128 value) noexcept
            : super(value)
        {
        }

        /** Construct a u128 from f32. */
        u128(f32 value) noexcept
            : super(value)
        {
        }

        /** Construct a u128 from f64. */
        u128(f64 value) noexcept
            : super(value)
        {
        }

        /** Retrieves the low part of the u128. */
        [[nodiscard]] constexpr u64 low() const noexcept
        {
            return low_;
        }

        /** Retrieves the high part of the u128. */
        [[nodiscard]] constexpr u64 high() const noexcept
        {
            return high_;
        }

        /** Cast to bool. */
        [[nodiscard]] constexpr operator bool() const noexcept
        {
            return low_ || high_;
        }

        /** Cast to i8. */
        [[nodiscard]] constexpr operator i8() const noexcept
        {
            return static_cast<i8>(low_);
        }

        /** Cast to u8. */
        [[nodiscard]] constexpr operator u8() const noexcept
        {
            return static_cast<u8>(low_);
        }

        /** Cast to i16. */
        [[nodiscard]] constexpr operator i16() const noexcept
        {
            return static_cast<i16>(low_);
        }

        /** Cast to u16. */
        [[nodiscard]] constexpr operator u16() const noexcept
        {
            return static_cast<u16>(low_);
        }

        /** Cast to i32. */
        [[nodiscard]] constexpr operator i32() const noexcept
        {
            return static_cast<i32>(low_);
        }

        /** Cast to u32. */
        [[nodiscard]] constexpr operator u32() const noexcept
        {
            return static_cast<u32>(low_);
        }

        /** Cast to i64. */
        [[nodiscard]] constexpr operator i64() const noexcept
        {
            return static_cast<i64>(low_);
        }

        /** Cast to u64. */
        [[nodiscard]] constexpr operator u64() const noexcept
        {
            return low_;
        }

        /** Cast to ansichar. */
        [[nodiscard]] constexpr operator ansichar() const noexcept
        {
            return static_cast<ansichar>(low_);
        }

        /** Cast to wchar. */
        [[nodiscard]] constexpr operator wchar() const noexcept
        {
            return static_cast<wchar>(low_);
        }

        /** Cast to char16. */
        [[nodiscard]] constexpr operator char16() const noexcept
        {
            return static_cast<char16>(low_);
        }

        /** Cast to char32. */
        [[nodiscard]] constexpr operator char32() const noexcept
        {
            return static_cast<char32>(low_);
        }

        /** Cast to f32. */
        [[nodiscard]] operator f32() const noexcept
        {
            return static_cast<f32>(low_) + ldexpf(static_cast<f32>(high_), 64);
        }

        /** Cast to f64. */
        [[nodiscard]] operator f64() const noexcept
        {
            return static_cast<f64>(low_) + ldexp(static_cast<f64>(high_), 64);
        }
    };

    /**
     * Checks whether right and left u128 are equal.
     * @param left The left u128 to compare
     * @param right The right u128 to compare
     * @param true if right and left u128 are equal, false otherwise
     */
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(const u128 &left, const u128 &right) noexcept
    {
        return left.low() == right.low() && left.high() == right.high();
    }

    /**
     * Checks whether right and left u128 are not equal.
     * @param left The left u128 to compare
     * @param right The right u128 to compare
     * @param true if right and left u128 are equal, false otherwise
     */
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator!=(const u128 &left, const u128 &right) noexcept
    {
        return !(left == right);
    }

    static inline constexpr u128 u128_max = u128 {u64_max, u64_max};
    static inline constexpr u128 u128_min = u128 {u64_min, 0u};

    namespace details::i128
    {
        /** Construct a i128 from u128. */
        constexpr i128_little::i128_little(hud::u128 value) noexcept
            : low_(value.low())
            , high_(static_cast<i64>(value.high()))
        {
        }

        /** Construct a i128 from u128. */
        constexpr i128_big::i128_big(hud::u128 value) noexcept
            : high_(static_cast<i64>(value.high()))
            , low_(value.low())
        {
        }

        constexpr u128_little::u128_little(hud::i128 value) noexcept
            : low_(value.low())
            , high_(static_cast<u64>(value.high()))
        {
        }

        constexpr u128_big::u128_big(hud::i128 value) noexcept
            : high_(static_cast<u64>(value.high()))
            , low_(value.low())
        {
        }
    } // namespace details::i128

    constexpr i128::i128(u128 value) noexcept
        : super(value)
    {
    }

} // namespace hud

#endif // HD_INC_CORE_I128_H