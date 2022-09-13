#pragma once
#ifndef HUD_INC_OSLAYER_STRING_H
#define HUD_INC_OSLAYER_STRING_H
#include "Array.h"

namespace hud {

    /**
    ASCII slice for the String class
    */
    struct AsciiSlice {
        
    public:
        Slice<u8> slice;
    };

    /**
    UTF-8 slice for the String class
    */
    struct Utf8Slice {

    public:
        Slice<u8> slice;
    };

    /**
    String representation
    Internally it's just an array of bytes.
    */
    class String {

    public:
        /**
        Default construct
        */
        explicit constexpr String() noexcept = default;

        
    private:
        Array<u8> bytes_array; // Array of bytes representing the string
    };
} // namespace hud

#endif // HUD_INC_OSLAYER_STRING_H
