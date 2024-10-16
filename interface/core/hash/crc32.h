#ifndef HD_INC_CORE_HASH_ALGORITHM_CRC32_HASH_H
#define HD_INC_CORE_HASH_ALGORITHM_CRC32_HASH_H

namespace hud::hash_algorithm
{

    struct crc32
    {

        /**
         * CRC32 hash based on Intel slice-by-8 algorithm
         * Polynomial used is 0x04C11DB7
         * @param buffer Pointer to the first byte to hash
         * @param count Number of bit to hash
         * @param seed The seed to use. Default is 0.
         * @return The CRC32 of the buffer
         */
        [[nodiscard]] static HD_CORE_DLL u32 hash(const u8 *buffer, usize count, const u32 seed = 0) noexcept;

        /**
         * Check that the CRC32 lookup table have correct values
         * Intent to be used only for test validation
         */
        [[nodiscard]] static HD_CORE_DLL bool is_lookup_table_values_correct() noexcept;

        [[nodiscard]] static HD_CORE_DLL u32 combine(u32 crc1, u32 crc2, usize count_crc2) noexcept;
    };

} // namespace hud::hash_algorithm

#endif // HD_INC_CORE_HASH_ALGORITHM_CRC32_HASH_H