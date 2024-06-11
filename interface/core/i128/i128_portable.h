#ifndef HD_INC_CORE_I128_I128_PORTABLE_H
#define HD_INC_CORE_I128_I128_PORTABLE_H

#if !defined(HD_INTRINSIC_INT128_SUPPORTED)
    #error i128_intrinsics.h is included but HD_INTRINSIC_INT128_SUPPORTED is not set
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
        struct alignas(16) i128_little
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
        struct alignas(16) i128_big
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
        struct alignas(16) u128_little
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
        struct alignas(16) u128_big
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
} // namespace hud

#endif // HD_INC_CORE_I128_I128_PORTABLE_H