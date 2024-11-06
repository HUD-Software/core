#ifndef HD_INC_CORE_HASH_ALGORITHM_CRC32_HASH_H
#define HD_INC_CORE_HASH_ALGORITHM_CRC32_HASH_H

namespace hud::hash_algorithm
{

    struct crc32
    {

        /**
         * CRC-32 polynomial
         * Use by ( Source : Wikipedia https://en.wikipedia.org/wiki/Cyclic_redundancy_check#Commonly_used_and_standardized_CRCs ) :
         * ISO 3309 (HDLC), ANSI X3.66 (ADCCP), FIPS PUB 71, FED-STD-1003, ITU-type_t V.42, ISO/IEC/IEEE 802-3 (Ethernet), SATA, MPEG-2, PKZIP, Gzip, Bzip2, POSIX cksum,PNG, ZMODEM, many others
         */
        static constexpr u32 POLYNOMIAL = 0x04c11db7;

        /**
         * CRC-32 reversed polynomial
         * Use by ( Source : Wikipedia https://en.wikipedia.org/wiki/Cyclic_redundancy_check#Commonly_used_and_standardized_CRCs ) :
         * ISO 3309 (HDLC), ANSI X3.66 (ADCCP), FIPS PUB 71, FED-STD-1003, ITU-type_t V.42, ISO/IEC/IEEE 802-3 (Ethernet), SATA, MPEG-2, PKZIP, Gzip, Bzip2, POSIX cksum,PNG, ZMODEM, many others
         */
        static constexpr u32 REFLECTED_POLYNOMIAL = 0xedb88320;

        /** LookUpTable generated with polynomial 0xedb88320 */
        static const u32 CRC32_LOOKUP[8][256];

        /** Table used for combine */
        static const u32 X2N_TABLE[32];

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
         * Combine 2 crc32 hash
         * @param crc1 The first crc
         * @param crc2 The second crc
         * @param count_crc2 The count of bytes used to compute the crc2
         */
        [[nodiscard]] static HD_CORE_DLL u32 combine(u32 crc1, u32 crc2, usize count_crc2) noexcept;
    };

} // namespace hud::hash_algorithm

#endif // HD_INC_CORE_HASH_ALGORITHM_CRC32_HASH_H