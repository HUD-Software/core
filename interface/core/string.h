#ifndef HUD_INC_OSLAYER_STRING_H
#define HUD_INC_OSLAYER_STRING_H
#include "vector.h"

namespace hud
{

    /**
     * ASCII slice for the string class
     */
    struct cstring_slice
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
    Internally it's just an vector of bytes.
    */
    class string
    {

    public:
        /**
        Default construct
        */
        explicit constexpr string() noexcept = default;

    private:
        vector<u8> bytes_array; // vector of bytes representing the string
    };
} // namespace hud

#endif // HUD_INC_OSLAYER_STRING_H
