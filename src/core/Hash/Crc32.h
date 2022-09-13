#pragma once
#ifndef HD_INC_OSLAYER_HASH_CRC32_HASH_H
#define HD_INC_OSLAYER_HASH_CRC32_HASH_H


namespace hud::Hash{

    struct Crc32 {

        /**
        * CRC32 hash based on Intel Slice-by-8 algorithm
        * Polynomial used is 0x04C11DB7
        * @param buffer Pointer to the first byte to hash
        * @param count Number of bit to hash
        * @param seed The seed to use. Default is 0.
        * @return The CRC32 of the buffer
        */
        [[nodiscard]]
        static u32 hash(const u8* buffer, usize count, const u32 seed = 0) noexcept;

        /**
        * Check that the CRC32 lookup table have correct values
        * Intent to be used only for test validation
        */
        [[nodiscard]]
        static bool is_lookup_table_values_correct() noexcept;
    };

} // namespace hud 

#endif // HD_INC_OSLAYER_HASH_CRC32_HASH_H