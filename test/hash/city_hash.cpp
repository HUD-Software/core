#include <core/hash/city_hash.h>
#include <cityhash/city.h> // Google CityHash

#include <core/cstring.h>

namespace hud_test
{
    static u32 city_hash_32_result[256] = {0xdc56d17a, 0xc0a92754, 0x2a1678b6, 0xd7c992e2, 0x616e1132, 0xfe6e37d4, 0x5145897e, 0xcfea845d, 0xeb0fd2d6, 0x7cd3d6e0, 0x14e52250, 0x931da128, 0xde42ef1c, 0x8add7404, 0x69976bd0, 0xcacd0542, 0x17aebf87, 0x1e9bcbda, 0xba680c4b, 0xf7cfbfda, 0xc41a2a96, 0x4a3f2b87, 0x4b7dd7b7, 0x1de0e4f5, 0x60cf6aa4, 0x2e6ddf78, 0x17a5df60, 0x100139a4, 0xef678131, 0xc158707d, 0xd3d91d57, 0xe4345328, 0x68943315, 0xe14e6d9e, 0x33f96086, 0x7549c70a, 0x43f74d13, 0x26911fcc, 0xc4c58416, 0xa104234, 0x5042df8c, 0x7044f7ca, 0x55dbbbf, 0xce39467e, 0xfdb76981, 0x716e5ed0, 0x5d0c428, 0x1950b972, 0x99fa0a24, 0xf531d568, 0x789e78e, 0x56cd4f9b, 0x2b0a28b4, 0x22498538, 0x2691989e, 0x82ee00db, 0xdc77f8be, 0xfeb09805, 0x5ad5eea, 0xc449c697, 0x8ebc55ce, 0xc119319b, 0x42561cd0, 0xefb0d898, 0x53d2a4c3, 0x7a49977d, 0x1a7a3b62, 0xc9cbb478, 0xd04a9ff8, 0x1607f8fa, 0x1f71fd8b, 0xd44629b3, 0x5d709512, 0x9cd509c2, 0x8db9c50a, 0x9497192a, 0xa0090ca2, 0xbe3f0434, 0xfe2c61ea, 0x4ea7f018, 0xdd027e4a, 0x7e8714f9, 0x812718c7, 0x27319a37, 0x1bec91e2, 0xd7fa11f6, 0x2a4d4a55, 0xb456be6f, 0xe417cd30, 0x5728c0fb, 0x5b566ca8, 0x7b146f39, 0xc197980, 0x28add1ff, 0x3e7095b3, 0xeb0ab40f, 0x611a1bec, 0xc60b1d1f, 0xfb75ed9f, 0xe5c2ed21, 0x56e258e5, 0xb9b1f97a, 0xcbbab8b3, 0x1c8bcd5a, 0x360ff812, 0x593d2794, 0xe3b5e900, 0x79b282bc, 0x91559ebf, 0x64af1417, 0x42ad606f, 0xe9101891, 0xaa82f3b0, 0xfb4bd972, 0xa8c7ae49, 0xce7a4050, 0x94a56c68, 0xf2919a58, 0x36cb6074, 0x127e2749, 0xe00d134e, 0x4221cee7, 0x573feb8c, 0x14df4883, 0xdc8c23fb, 0xbdbf5cba, 0x81dc28d9, 0x558331a9, 0x5321e430, 0xd737c0c8, 0x6c4e7352, 0x985deedc, 0xbee2fac2, 0x61f57165, 0xe0ea33b6, 0x80b29f25, 0x37775178, 0x418437a1, 0x6c8b569e, 0xf770d828, 0x6fabe476, 0x6ec19d97, 0xcb410b3c, 0xd1c84844, 0x889018d1, 0x415435fe, 0xe4a4ec1d, 0xfa49fa16, 0x97eadb32, 0xa0bbfb93, 0xd9fe5c31, 0x6201ccad, 0xdc04203c, 0x35a284fe, 0x4e8d35b, 0x136ab950, 0xc4b4c7f6, 0x479df6ff, 0x2a4964e1, 0xbebd4da7, 0xa048bb31, 0xb6a95161, 0xb88fbb76, 0xeb3804d8, 0x81878252, 0x93453547, 0xd9cf26f, 0x8a8848cd, 0xb2e83544, 0xbb2079bf, 0xbff1e5bb, 0x9313a579, 0x2f35813f, 0xba7d9b0e, 0x8c116f5c, 0x2690b5df, 0x19d487af, 0x7245211e, 0x9b41b711, 0x246fa530, 0xc42c933a, 0x2dc64f76, 0x51910402, 0xb59e8a94, 0xcf299383, 0xf63a92a4, 0xa6e69cdb, 0xdef3a2ca, 0xd488d2c6, 0x3f54a353, 0x42ecb6fa, 0xf0246bfd, 0xbfe4087c, 0xca89cd7e, 0xfae767d, 0x730608c9, 0xa8ccc638, 0x31eb911f, 0x4da366e, 0x35b52ca6, 0x62d5b95a, 0xe0cd6d86, 0x710912ee, 0xa9a0e174, 0x7c1b479e, 0x664bf777, 0x1cd8f425, 0x195ed250, 0x91b5afee, 0x419f49ce, 0xf4bbf69a, 0x890715ec, 0xf781eb91, 0x245c4d39, 0xee4e8908, 0xf086cba5, 0x196c8a16, 0xd3d419b7, 0xd293cba7, 0x6aaa38e8, 0xe54dca45, 0x5ba0b60f, 0xafabbd81, 0xea4531cd, 0x67a4a588, 0x803d4c59, 0x994a71bb, 0x369a676c, 0xe99cc2c4, 0xff00a6d8, 0xe96df54e, 0xfa5f33a0, 0x49194151, 0x35ead1a, 0xcc8b2d4b, 0x2fa7895d, 0x9deddbc0, 0x1adaffbd, 0x9ad66495, 0x6b8c1d02, 0x3a9f56d0, 0xb9ccb4fd, 0x1fb0ee25, 0x982b4896, 0x8dd93d5, 0x3282814b, 0x696151d2, 0x428f9244, 0x3ef8a0ba, 0xbafc4de5, 0x4ca78146, 0xa87374f5, 0x4fa36aaf, 0x4967e1ca, 0x6491ed46, 0x966e909f};

    static u64 city_hash_64_result[256] = {0x9ae16a3b2f90404f, 0xbe6056edf5e94b54, 0xc2a04665ed038d75, 0x94a13d22e9eba49a, 0x82bffd898958e540, 0xb4bfa9e87732c149, 0x92fdbcd8e94a2333, 0xa2e0bff20db0a6a1, 0xad5a13e1e8e93b98, 0x81371e150e4ad84f, 0x9b704db2b2d6ffea, 0xf3212b3c1d803add, 0x9fd5df33aefc3d7d, 0xc1c5cf5c853d3c92, 0x4bd9a87bb2bb4671, 0x862a51555943bd9d, 0xefd25a0a34156d4, 0xbbb6a6f8f20d1f1c, 0x79f8c18b091f57e6, 0x9397ab7e5511df31, 0x8dd7e7f1bf16a0e9, 0xebed9d4fcec9fa24, 0x28e6d435e458f020, 0x662e334e298fddc, 0x3f3b313dcbd16ec7, 0xb49fc64083cc4c3e, 0x1313dbdeed4e9ded, 0x427e4dbedaea19f6, 0xa56007058a23efa6, 0x96081fedc82adf9f, 0x48abb0e9ebd50ea7, 0xfbd950af27ef6941, 0x1a9d8199972cdf49, 0x46e1378cbc22daba, 0xb5bac3abe70d1522, 0x28e59da3f3008300, 0x11dfc4eea43101, 0x3645772b95bd3743, 0x629946fa201ac819, 0xa5f6faba3c6fb303, 0x36f7dd7a65e93d1c, 0xc07403048a1eeda7, 0xca4d3c89744f43a3, 0x4cf58d31ed02513e, 0x9e07c00bd59d7a01, 0x5e776542af3c0519, 0xfa62d2a4bc5d87c6, 0xb7f64877a70ee14a, 0xc7099eac443f4625, 0x60b11c741d047a11, 0x73286092ee9394c9, 0x7877a8d9b5a4b929, 0xd589cb2ce84723a8, 0xa85c240f3e025587, 0xc3d98ecdb6bf52cd, 0xf890c6323948a021, 0x96a5683630a4333f, 0xa84168edcd8646cc, 0x67f1e6f0ccfdf70e, 0x84cb84c6edffeab6, 0xe954fc8f7968db69, 0xb29212c2d2ef6b03, 0x2f20733b20979be3, 0xaf30927a77ada6ef, 0xe99ab80f5ec7dca5, 0xac589c990483dd2e, 0xaf6f3ea7de248f72, 0x15452a0335f8d3ff, 0x87faaf9ffc00b792, 0xd735fd9242f41d1d, 0xbebcb91fd6057b44, 0x177474a5d1ebbe6, 0x9273be0076008b09, 0x4ff1e9fb068a6a2b, 0xa9bd698beab91622, 0x112ef9fd43f6ab0c, 0xbd41cfd5fb432baf, 0x20959f28fef778b5, 0x7dc5c0f6bffa9f3a, 0x5d8d82ca24381650, 0x4feedc7dada54816, 0x993dad25f4602235, 0x48ac17a39fa67d56, 0xa69f17df1e2b88c4, 0x62e9f87e7484d48e, 0x86b1f8e0d03141a2, 0x4ccf1f0ffc2468c4, 0xc0828aa177219532, 0xffdce2c7bf331b8f, 0x6992c7d1be0fd9ca, 0xd1715e9954348633, 0xcd69d2919e30aa60, 0xa5f2c86dc3e7480b, 0x536f357a66399901, 0xb9da9a2379c19379, 0x364ae6782f889be4, 0xe3f6cd656b9c26be, 0xb1541a33562869ea, 0x72bf686a93a755af, 0xaf958d2eb9ed0dff, 0xa47fe83e60b34cc6, 0xd61f11279fb8a7e5, 0x7ebe4b3d74386431, 0x41cc5ad24988a733, 0xb87a4fb288a06c2, 0xe3c3810bf671386c, 0x7c1f3a08a8e24551, 0x8edc6a63e8b10115, 0xa0ce313ca06c2d01, 0x531f781e2c8264bc, 0xd2a65241d3709bbe, 0x96042182140074fd, 0xc2bb46ba9af19ed3, 0xe031e70d24a3eb33, 0xf9d935c7400b2407, 0x663845cc33b07f0a, 0xca3fc22c3bcd0200, 0x2c8dc0bcac69bd81, 0x3e49372b20c3da04, 0xbb9384101d3bbe76, 0x612b5e5a1c296054, 0x1f5f351cc71b79a3, 0x5775c3be1a6af9fa, 0xde6a78ea8ba8dd0e, 0x90c6012b151e7de3, 0x8db4fd55a93183f1, 0xb4e37477c19e1ef3, 0xefd614390a7b1d95, 0x10b153630af1f395, 0x46be8f236f918770, 0x471b693b3226bc9f, 0xa46e7f270ac7812c, 0xa0ac294f8aa62d68, 0xf31dd191643f41ac, 0xf2961255518716c0, 0x4059d62c19ad9e14, 0xe0df742546f097ac, 0xdff7feab5ca4d8b7, 0xa8957af3f869e737, 0xd44357cd3043768c, 0xd6a0214e8245fea9, 0x5f50d9a469932ba7, 0x9f0c60bf760a9194, 0x370bcbd5b6ececed, 0x917922f50dea675e, 0xad2d366db2564e3c, 0xb121fb5707cdc3ff, 0x1183b019cd0033c5, 0x89f48ac41a6728ad, 0x6131b64dd658e8dc, 0x6e3aa5f0f424ec59, 0x2d701e8579d7970a, 0x3d70b9b6255a92a1, 0xb2dbd54203588d4b, 0x71a7c5fa31ddf60e, 0xd9707c56d133b51e, 0xa9968c6d71b9e415, 0x6a305f39728ed0c2, 0x7f6cc5b333cc4ee2, 0x23231e2aef8c2c48, 0x45af15babe5d7c65, 0xada4d4a86da2e1ab, 0xe6258fc2db0fac5d, 0x14aa57bff770e293, 0x654528c6ddc74de9, 0x8b36ea8945b7ed49, 0xc6bcf2a03a78abe9, 0xcc8f0587fa2af10c, 0xda636827d57fa655, 0x9051b5c3b87fef61, 0xef16c25c523903d5, 0x7327f0b055f6ec29, 0x71531d073f8b8314, 0x4d1543172054f04e, 0x3d4dc4e9ab8ba392, 0xa42bc184eb199366, 0x918b995f565e97e8, 0xd4c768803157baa4, 0xe638d049da45b516, 0xf1dce6e38d5f7674, 0x48448058a3c22a6d, 0x498e30a542aac033, 0xded4e1b2f9853ba5, 0xf9c7ee3e76b38bf0, 0xa99afa5245d0bef8, 0x21276f9f37743e1e, 0xe34e51d2c449d0d0, 0xfca6453578ceddc1, 0x838269263cf16f34, 0xa048360ccaffcffc, 0xc017e5cfd9ebec42, 0x214efe903049aac9, 0xd5ba92662e2d5074, 0xe56476a1931c992a, 0x71c47a32f8190d15, 0x348d5eaf446f1e0d, 0x4031a8122018a4a1, 0xe1e736a1c15db548, 0x63d193dce2ebda88, 0x2270c0be9bc5af94, 0xf4d24e8c7493c3d8, 0x4eb48586354313fc, 0xc5520e2d63e6fed5, 0x95204130763e6755, 0xec2fa707c89c58df, 0x73f8c1f9961bcf8c, 0xfb1755f531e55bc7, 0x9cd07d7167b6154, 0x80c47e3f57bd03c1, 0x72b348f7ffcb1ca8, 0xda6e47db129e8a22, 0x660b38cc131965a1, 0x748a65bed0389a40, 0xa58c597d5f008d19, 0x6071bf4d2700ed9d, 0x16e19ccd578efd5e, 0x5b053799983874da, 0x1918a56191596766, 0x27649c3904f890e1, 0x1227f1a51144baa6, 0x7a779add715ab5e4, 0x386f3389e479c08c, 0x47808cd6db853d47, 0xcfd9c5c21c04f4e6, 0x29ec12f72e3d6b8, 0x638cb224e487c08e, 0xb5c95c00242919ce, 0x85771ffe82ceafdf, 0x79d2c1883cf91538, 0x83ddb31eb6008a1, 0x1e6cfc9472b49f3a, 0xce8854acea5e24b8, 0x27afb26b41020580, 0x9d25c68f8bcf1c3a, 0x805402644867c522, 0x41e8c5ffc4ea5d00, 0xcd81dd45d46e0c8d, 0xa8cdf8d8619a150e, 0xd8caa106697706fb, 0x6a1406de52333f04, 0x619f36b7b1eb4599, 0xff8eecd0580401d2, 0xce75d7dd5a10eb0a, 0xe0a9177ae74d947, 0xacf8b56059b2d479, 0x21575fd3c592dbdc, 0x4aa4877e2e125fff, 0xbaea8c242d7282a1, 0xd5e260b93a603406, 0xa26b3c14f4a7523f, 0xf1bb64925dda14de, 0xee68f22335ac8309, 0xb9ddc15698504d79, 0xf99b26823feb8a52, 0xa794ae84547aaf93, 0x57466e1c585015b8};
} // namespace hud_test

static constexpr const ansichar *lipsum = "Ut venenatis velit sit amet bibendum sodales.Ut laoreet cursus quam, in pellentesque urna ullamcorper at.Vestibulum pulvinar odio purus, sit amet vestibulum quam pharetra vitae.Morbi posuere est eu arcu interdum, \
et porttitor lacus tempor.Nulla non libero porta, faucibus ex eleifend, sodales magna.Sed eget scelerisque sem.Pellentesque vel nulla et ipsum ultrices ultricies. \
Mauris molestie, dolor non porttitor lobortis, erat nunc fringilla nulla, a consequat justo odio ut lorem.Cras maximus tristique erat, sit amet facilisis urna euismod in.\
Maecenas sit amet congue magna.Donec nisl mauris, tempus eu mauris eget, iaculis sodales libero.Nulla hendrerit bibendum magna, at egestas leo semper ac.Donec porttitor ut orci id ornare.Praesent id nunc risus.Fusce consequat tortor at enim cursus, sed mattis lorem gravida.Orci varius. ";

GTEST_TEST(cityhash, hash32)
{
    // hash32 of nullptr
    u32 hash_nullptr = hud::hash_algorithm::city_hash::hash_32(nullptr, 0);
    hud_assert_eq(hash_nullptr, 0XDC56D17A);
    hud_assert_eq(CityHash32(nullptr, 0), hash_nullptr);

    ansichar key[256];
    hud::memory::set_zero(key);

    for (u8 i = 0; i < hud::u8_max; i++)
    {
        key[i] = static_cast<ansichar>(i);
        u32 out = hud::hash_algorithm::city_hash::hash_32(key, i);

        // Ensure we have same result as Google implementation
        hud_assert_eq(out, CityHash32(reinterpret_cast<const ansichar *>(key), i));

        hud_assert_eq(hud_test::city_hash_32_result[i], out);
    }

    // Test with the lipsum
    const u32 hash = hud::hash_algorithm::city_hash::hash_32(lipsum, hud::cstring::length(lipsum));
    hud_assert_eq(hash, CityHash32(lipsum, hud::cstring::length(lipsum)));

    // Test with "key"
    constexpr const usize len = hud::cstring::length("key");
    hud_assert_eq(hud::hash_algorithm::city_hash::hash_32("key", len), 0x1096A99Du);
}

GTEST_TEST(cityhash, hash32_is_usable_in_constexpr)
{
    constexpr u32 hash_nullptr = hud::hash_algorithm::city_hash::hash_32(nullptr, 0);
    hud_assert_eq(hash_nullptr, 0XDC56D17A);
    hud_assert_eq(CityHash32(nullptr, 0), hash_nullptr);

    constexpr u32 hash_lipsum = hud::hash_algorithm::city_hash::hash_32(lipsum, hud::cstring::length(lipsum));
    // Test with the lipsum
    hud_assert_eq(hash_lipsum, CityHash32(lipsum, hud::cstring::length(lipsum)));
}

GTEST_TEST(cityhash, hash64)
{
    // hash64 of nullptr
    hud_assert_eq(hud::hash_algorithm::city_hash::hash_64(nullptr, 0), 0X9AE16A3B2F90404Full);

    ansichar key[256];
    hud::memory::set_zero(key);

    for (usize i = 0; i < 256; i++)
    {
        key[i] = static_cast<i8>(i);
        u64 out = hud::hash_algorithm::city_hash::hash_64(key, i);
        hud_assert_eq(hud_test::city_hash_64_result[i], out);
    }

    // Test with the lipsum
    const u64 hash = hud::hash_algorithm::city_hash::hash_64(lipsum, hud::cstring::length(lipsum));
    hud_assert_eq(hash, CityHash64(lipsum, hud::cstring::length(lipsum)));

    // Test with "key"
    constexpr const usize len = hud::cstring::length("key");
    hud_assert_eq(hud::hash_algorithm::city_hash::hash_64("key", len), 0x0BB7560E11262CDB);
}

GTEST_TEST(cityhash, hash64_is_usable_in_constexpr)
{
    constexpr u64 hash_nullptr = hud::hash_algorithm::city_hash::hash_64(nullptr, 0);
    hud_assert_eq(hash_nullptr, 0x9AE16A3B2F90404Full);
    hud_assert_eq(CityHash64(nullptr, 0), hash_nullptr);

    constexpr u64 hash_lipsum = hud::hash_algorithm::city_hash::hash_64(lipsum, hud::cstring::length(lipsum));

    // Test with the lipsum
    hud_assert_eq(hash_lipsum, CityHash64(lipsum, hud::cstring::length(lipsum)));
}

GTEST_TEST(cityhash, hash128)
{
    // hash128 of nullptr
    uint128 hash = CityHash128(nullptr, 0);
    hud_assert_eq(Uint128Low64(hash), 0x3df09dfc64c09a2bllu);
    hud_assert_eq(Uint128High64(hash), 0x3cb540c392e51e29llu);

    ansichar key[256];

    hud::memory::set_zero(key);

    for (usize i = 0; i < 256; i++)
    {
        key[i] = static_cast<ansichar>(i);
        u64 out = hud::hash_algorithm::city_hash::hash_64(key, i);
        hud_assert_eq(hud_test::city_hash_64_result[i], out);
    }
}

GTEST_TEST(cityhash, combine_32)
{
    // This result come from abseil with modification of the hash seed 0 to use the Hash128to64 to combine 2 32 bits hash
    const usize len = hud::cstring::length("key");

    u32 hash = hud::hash_algorithm::city_hash::hash_32("key", len);
    u32 combined = hud::hash_algorithm::city_hash::combine_32(0, hash);
    hud_assert_eq(combined, 0x105695BEu);

    u32 hash_2 = hud::hash_algorithm::city_hash::hash_32((const ansichar *)&len, sizeof(len));
    hud_assert_eq(hash_2, 0x8D14653Fu);
#if defined(HD_TARGET_32_BITS)
    hud_assert_eq(hud::hash_algorithm::city_hash::combine_32(combined, hash_2), 0xE66FB46Bu);
#else // HD_TARGET_64_BITS
    hud_assert_eq(hud::hash_algorithm::city_hash::combine_32(combined, hash_2), 0xE66FB46Bu);
#endif
}

GTEST_TEST(cityhash, combine_64)
{
    // This result come from abseil with modification of the hash seed 0 to use the Hash128to64 to combine 2 64 bits hash
    const usize len = hud::cstring::length("key");
    u64 hash = hud::hash_algorithm::city_hash::hash_64("key", len);
    u64 combined = hud::hash_algorithm::city_hash::combine_64(0, hash);
    hud_assert_eq(combined, 0xCEAAB8E77B74C2E7u);

    u64 hash_2 = hud::hash_algorithm::city_hash::hash_64((const ansichar *)&len, sizeof(len));
    hud_assert_eq(hash_2, 0x253692A8EE00085Cu);
#if defined(HD_TARGET_32_BITS)
    hud_assert_eq(hud::hash_algorithm::city_hash::combine_64(combined, hash_2), 0x60A95A74371605F2);
#else // HD_TARGET_64_BITS
    hud_assert_eq(hud::hash_algorithm::city_hash::combine_64(combined, hash_2), 0x746D68F6EB969EB7u);
#endif
}
