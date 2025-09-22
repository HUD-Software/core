#ifndef HD_INC_CORE_STRING_STRING_VIEW_H
#define HD_INC_CORE_STRING_STRING_VIEW_H

namespace hud
{
    /**
     * Valid UTF-8 string view
     */
    template<typename char_t>
    struct cstring_view
    {

    private:
        /** An UTF-8 valid string view. */
        char8 *ptr_;
        usize len;
    };

} // namespace hud

#endif // HD_INC_CORE_STRING_STRING_VIEW_H