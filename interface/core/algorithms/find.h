#ifndef HD_INC_CORE_ALGORITHMS_FIND_H
#define HD_INC_CORE_ALGORITHMS_FIND_H

namespace hud
{
    /**
     * Find the first element in the array of element matching the predicate
     * @tparam type_t The type of element
     * @tparam unary_t The type of the unary predicate
     * @param begin Pointer to the first element
     * @param end Pointer after the last element
     * @param predicate The unary predicate used to find the element
     * @return The index where the element is. Return -1 if not found
     */
    template<typename type_t, typename unary_t>
    static constexpr i32 find_index_if(const type_t *begin, const type_t *end, unary_t &&predicate)
    {
        for (const type_t *current = begin; current < end; ++current)
        {
            if (predicate(*current))
                return current - begin;
        }
        return -1;
    }

    /**
     * Find the first element in the array of element matching the predicate
     * @param begin Pointer to the first element
     * @param count Number of element after Begin to look
     * @param predicate The predicate used to find the element
     * @return The index where the element is. Return -1 if not found
     */
    template<typename type_t, typename unary_t>
    HD_FORCEINLINE static constexpr i32 find_index_if(const type_t *begin, const usize count, unary_t &&predicate)
    {
        return find_index_if(begin, begin + count, hud::forward<unary_t>(predicate));
    }

} // namespace hud

#endif // HD_INC_CORE_ALGORITHMS_FIND_H