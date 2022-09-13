#pragma once
#ifndef HD_INC_OSLAYER_TEMPLATES_LEXICOGRAPHICAL_COMPARE_H
#define HD_INC_OSLAYER_TEMPLATES_LEXICOGRAPHICAL_COMPARE_H
#include "../Memory.h"
#include "../Traits/IsPointer.h"

namespace hud {

    /**
    * Checks if the first range [first_1, last_1[ is lexicographically less than the second range [first_2, last_2[.
    * A lexicographical comparison is the kind of comparison generally used to sort words alphabetically in dictionaries; 
    * It involves comparing sequentially the elements that have the same position in both ranges against each other until 
    * one element is not equivalent to the other. The result of comparing these first non-matching elements is the result 
    * of the lexicographical comparison.
    * If both sequences compare equal until one of them ends, the shorter sequence is lexicographically less than the longer one.
    * @tparam It_1 First iterator type
    * @tparam It_2 Second iterator type
    * @param first_1 The initial positions of the first sequence
    * @param last_1 The final positions of the first sequence. last element is not include
    * @param first_2 The initial positions of the second sequence
    * @param last_2 The final positions of the second sequence. last element is not include
    * @return true if the first range compares lexicographically less than than the second, false otherwise (including when all the elements of both ranges are equivalent).
    */
    template<typename It_1, typename It_2>
    [[nodiscard]]
    inline constexpr bool lexicographical_compare(It_1 first_1, It_1 last_1, It_2 first_2, It_2 last_2) noexcept {
        for (; (first_1 != last_1) && (first_2 != last_2); ++first_1, ++first_2) {
            if (*first_1 < *first_2) {
                return true;
            }
            if (*first_2 < *first_1) {
                return false;
            }
        }
        return (first_1 == last_1) && (first_2 != last_2);
    }

    /**
    * Checks if the first array is lexicographically less than the second array.
    * A lexicographical comparison is the kind of comparison generally used to sort words alphabetically in dictionaries;
    * It involves comparing sequentially the elements that have the same position in both ranges against each other until
    * one element is not equivalent to the other. The result of comparing these first non-matching elements is the result
    * of the lexicographical comparison.
    * If both sequences compare equal until one of them ends, the shorter sequence is lexicographically less than the longer one.
    * @tparam Arr_1 First array type
    * @tparam size_1 First array size
    * @tparam Arr_2 Second array type
    * @tparam size_2 Second array size
    * @param arr_1 The first array
    * @param arr_2 The second array
    * @return true if the first array compares lexicographically less than than the second, false otherwise (including when all the elements of both ranges are equivalent).
    */
    template<typename Arr_1, usize size_1, typename Arr_2, usize size_2>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool lexicographical_compare(const Arr_1(&arr_1)[size_1], const Arr_2(&arr_2)[size_2]) noexcept {
        return lexicographical_compare(arr_1, arr_1 + size_1, arr_2, arr_2 + size_2);
    }

    /**
    * Checks if the first range [first_1, last_1[ is lexicographically less than the second range [first_2, last_2[.
    * A lexicographical comparison is the kind of comparison generally used to sort words alphabetically in dictionaries;
    * It involves comparing sequentially the elements that have the same position in both ranges against each other until
    * one element is not equivalent to the other. The result of comparing these first non-matching elements is the result
    * of the lexicographical comparison.
    * If both sequences compare equal until one of them ends, the shorter sequence is lexicographically less than the longer one.
    * This overload only participates in overload resolution if sizeof(It_1) == 1 and sizeof(It_2) == 1.
    * @tparam It_1 First pointer type
    * @tparam It_2 Second pointer type
    * @param first_1 The initial positions of the first sequence
    * @param last_1 The final positions of the first sequence. last element is not include
    * @param first_2 The initial positions of the second sequence
    * @param last_2 The final positions of the second sequence. last element is not include
    * @return true if the first range compares lexicographically less than than the second, false otherwise (including when all the elements of both ranges are equivalent).
    */
    template<typename It_1, typename It_2>
    [[nodiscard]]
    inline constexpr bool lexicographical_compare(It_1* first_1, It_1* last_1, It_2* first_2, It_2* last_2) noexcept requires(sizeof(It_1) == 1 && sizeof(It_2) == 1) {
        const usize size_1 = static_cast<usize>(last_1 - first_1);
        const usize size_2 = static_cast<usize>(last_2 - first_2);
        const bool size_1_less_than_size_2 = size_1 < size_2;
        const i32 diff = Memory::compare(first_1, first_2, size_1_less_than_size_2 ? size_1 : size_2);
        return diff < 0 || (diff == 0 && size_1_less_than_size_2);
    }

    /**
    * Checks if the first range [first_1, first_1+count[ is lexicographically less than the second range [first_2, first_2+count[.
    * A lexicographical comparison is the kind of comparison generally used to sort words alphabetically in dictionaries;
    * It involves comparing sequentially the elements that have the same position in both ranges against each other until
    * one element is not equivalent to the other. The result of comparing these first non-matching elements is the result
    * of the lexicographical comparison.
    * If both sequences compare equal until one of them ends, the shorter sequence is lexicographically less than the longer one.
    * This overload only participates in overload resolution if sizeof(It_1) == 1 and sizeof(It_2) == 1.
    * @tparam It_1 First pointer type
    * @tparam It_2 Second pointer type
    * @param buffer_1 The initial positions of the first sequence
    * @param buffer_2 The initial positions of the second sequence
    * @param count Size of both sequence in bytes
    * @return true if the first range compares lexicographically less than than the second, false otherwise (including when all the elements of both ranges are equivalent).
    */
    template<typename It_1, typename It_2>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool lexicographical_compare(It_1* buffer_1, It_2* buffer_2, usize size) noexcept requires(sizeof(It_1) == 1 && sizeof(It_2) == 1) {
        return Memory::compare_less(buffer_1, buffer_2, size);
    }
    
    /**
    * Checks if the first array is lexicographically less than the second array.
    * A lexicographical comparison is the kind of comparison generally used to sort words alphabetically in dictionaries;
    * It involves comparing sequentially the elements that have the same position in both ranges against each other until
    * one element is not equivalent to the other. The result of comparing these first non-matching elements is the result
    * of the lexicographical comparison.
    * If both sequences compare equal until one of them ends, the shorter sequence is lexicographically less than the longer one.
    * This overload only participates in overload resolution if sizeof(It_1) == 1 and sizeof(It_2) == 1 and both array have the same size.
    * @tparam Arr_1 First array type
    * @tparam Arr_2 Second array type
    * @tparam size First and second array size
    * @param arr_1 The first array
    * @param arr_2 The second array
    * @return true if the first array compares lexicographically less than than the second, false otherwise (including when all the elements of both ranges are equivalent).
    */
    template<typename Arr_1, typename Arr_2, usize size>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool lexicographical_compare(const Arr_1(&arr_1)[size], const Arr_2(&arr_2)[size]) noexcept requires(sizeof(Arr_1) == 1 && sizeof(Arr_2) == 1) {
         return Memory::compare_less(arr_1, arr_2, size);
    }

} // namespace hud 

#endif // HD_INC_OSLAYER_TEMPLATES_LEXICOGRAPHICAL_COMPARE_H
