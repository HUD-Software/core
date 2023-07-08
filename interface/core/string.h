#ifndef HUD_INC_OSLAYER_STRING_H
#define HUD_INC_OSLAYER_STRING_H
#include "array.h"

namespace hud
{

    /**
    ASCII slice for the string class
    */
    struct ascii_slice
    {

    public:
        slice<u8> slice;
    };

    /**
    UTF-8 slice for the string class
    */
    struct utf8_slice
    {

    public:
        slice<u8> slice;
    };

    /**
    string representation
    Internally it's just an array of bytes.
    */
    class string
    {

    public:
        /**
        Default construct
        */
        explicit constexpr string() noexcept = default;

    private:
        array<u8> bytes_array; // array of bytes representing the string
    };
} // namespace hud

#endif // HUD_INC_OSLAYER_STRING_H
