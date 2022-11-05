#pragma once
#ifndef HD_INC_CORE_TEMPLATES_LEXICOGRAPHICAL_COMPARE_H
#define HD_INC_CORE_TEMPLATES_LEXICOGRAPHICAL_COMPARE_H
#include "../memory.h"
#include "../traits/is_pointer.h"

namespace hud {

    /**
    * Checks if the first range [first_1, last_1[ is lexicographically less than the second range [first_2, last_2[.
    * A lexicographical comparison is the kind of comparison generally used to sort words alphabetically in dictionaries; 
    * It involves comparing sequentially the elements that have the same position in both ranges against each other until 
    * one element is not equivalent to the other. The result of comparing these first non-matching elements is the result 
    * of the lexicographical comparison.
    * If both sequences compare equal until one of them ends, the shorter sequence is lexicographically less than the longer one.
    * @tparam it1_t First iterator type
    * @tparam it2_t Second iterator type
    * @param first_1 The initial positions of the first sequence
    * @param last_1 The final positions of the first sequence. last element is not include
    * @param first_2 The initial positions of the second sequence
    * @param last_2 The final positions of the second sequence. last element is not include
    * @return true if the first range compares lexicographically less than than the second, false otherwise (including when all the elements of both ranges are equivalent).
    */
    template<typename it1_t, typename it2_t>
    [[nodiscard]]
    inline constexpr bool lexicographical_compare(it1_t first_1, it1_t last_1, it2_t first_2, it2_t last_2) noexcept {
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
    * @tparam array1_t First array type
    * @tparam size_1 First array size
    * @tparam array2_t Second array type
    * @tparam size_2 Second array size
    * @param arr_1 The first array
    * @param arr_2 The second array
    * @return true if the first array compares lexicographically less than than the second, false otherwise (including when all the elements of both ranges are equivalent).
    */
    template<typename array1_t, usize size_1, typename array2_t, usize size_2>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool lexicographical_compare(const array1_t(&arr_1)[size_1], const array2_t(&arr_2)[size_2]) noexcept {
        return lexicographical_compare(arr_1, arr_1 + size_1, arr_2, arr_2 + size_2);
    }

    /**
    * Checks if the first range [first_1, last_1[ is lexicographically less than the second range [first_2, last_2[.
    * A lexicographical comparison is the kind of comparison generally used to sort words alphabetically in dictionaries;
    * It involves comparing sequentially the elements that have the same position in both ranges against each other until
    * one element is not equivalent to the other. The result of comparing these first non-matching elements is the result
    * of the lexicographical comparison.
    * If both sequences compare equal until one of them ends, the shorter sequence is lexicographically less than the longer one.
    * This overload only participates in overload resolution if sizeof(it1_t) == 1 and sizeof(it2_t) == 1.
    * @tparam it1_t First pointer type
    * @tparam it2_t Second pointer type
    * @param first_1 The initial positions of the first sequence
    * @param last_1 The final positions of the first sequence. last element is not include
    * @param first_2 The initial positions of the second sequence
    * @param last_2 The final positions of the second sequence. last element is not include
    * @return true if the first range compares lexicographically less than than the second, false otherwise (including when all the elements of both ranges are equivalent).
    */
    template<typename it1_t, typename it2_t>
    [[nodiscard]]
    inline constexpr bool lexicographical_compare(it1_t* first_1, it1_t* last_1, it2_t* first_2, it2_t* last_2) noexcept requires(sizeof(it1_t) == 1 && sizeof(it2_t) == 1) {
        const usize size_1 = static_cast<usize>(last_1 - first_1);
        const usize size_2 = static_cast<usize>(last_2 - first_2);
        const bool size_1_less_than_size_2 = size_1 < size_2;
        const i32 diff = hud::memory::compare(first_1, first_2, size_1_less_than_size_2 ? size_1 : size_2);
        return diff < 0 || (diff == 0 && size_1_less_than_size_2);
    }

    /**
    * Checks if the first range [first_1, first_1+count[ is lexicographically less than the second range [first_2, first_2+count[.
    * A lexicographical comparison is the kind of comparison generally used to sort words alphabetically in dictionaries;
    * It involves comparing sequentially the elements that have the same position in both ranges against each other until
    * one element is not equivalent to the other. The result of comparing these first non-matching elements is the result
    * of the lexicographical comparison.
    * If both sequences compare equal until one of them ends, the shorter sequence is lexicographically less than the longer one.
    * This overload only participates in overload resolution if sizeof(it1_t) == 1 and sizeof(it2_t) == 1.
    * @tparam it1_t First pointer type
    * @tparam it2_t Second pointer type
    * @param buffer_1 The initial positions of the first sequence
    * @param buffer_2 The initial positions of the second sequence
    * @param count Size of both sequence in bytes
    * @return true if the first range compares lexicographically less than than the second, false otherwise (including when all the elements of both ranges are equivalent).
    */
    template<typename it1_t, typename it2_t>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool lexicographical_compare(it1_t* buffer_1, it2_t* buffer_2, usize size) noexcept requires(sizeof(it1_t) == 1 && sizeof(it2_t) == 1) {
        return hud::memory::compare_less(buffer_1, buffer_2, size);
    }
    
    /**
    * Checks if the first array is lexicographically less than the second array.
    * A lexicographical comparison is the kind of comparison generally used to sort words alphabetically in dictionaries;
    * It involves comparing sequentially the elements that have the same position in both ranges against each other until
    * one element is not equivalent to the other. The result of comparing these first non-matching elements is the result
    * of the lexicographical comparison.
    * If both sequences compare equal until one of them ends, the shorter sequence is lexicographically less than the longer one.
    * This overload only participates in overload resolution if sizeof(it1_t) == 1 and sizeof(it2_t) == 1 and both array have the same size.
    * @tparam array1_t First array type
    * @tparam array2_t Second array type
    * @tparam size First and second array size
    * @param arr_1 The first array
    * @param arr_2 The second array
    * @return true if the first array compares lexicographically less than than the second, false otherwise (including when all the elements of both ranges are equivalent).
    */
    template<typename array1_t, typename array2_t, usize size>
    [[nodiscard]]
    HD_FORCEINLINE constexpr bool lexicographical_compare(const array1_t(&arr_1)[size], const array2_t(&arr_2)[size]) noexcept requires(sizeof(array1_t) == 1 && sizeof(array2_t) == 1) {
         return hud::memory::compare_less(arr_1, arr_2, size);
    }

} // namespace hud 

#endif // HD_INC_CORE_TEMPLATES_LEXICOGRAPHICAL_COMPARE_H
