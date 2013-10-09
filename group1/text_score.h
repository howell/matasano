
#ifndef ___text_score_h___
#define ___text_score_h___

#include <stdint.h>
#include <stddef.h>

// Type to represent a set of letter frequencies.
struct letter_frequencies {
    uint32_t freqs[26]; // store the frequency of each letter, with 'a' at index
                        // 0, 'b' at index 1, ..., and 'z' at index 25
                        // as a fixed point number with base LF_FP_BASE
};
#define LF_FP_BASE 100


/*
 * Calculate the frequency of each letter in a string
 * @param src the string to evaluate
 * @param out pointer to letter_frequencies struct to write the output to
 * @return the number of letters found in the string
 */
uint32_t calculate_letter_frequencies(const char *src,
        struct letter_frequencies *out);
/*
 * Calculate the difference in letter frequencies between a given distribution
 * and the english language
 * @param src pointer to letter frequencies to compare
 * @return absolute difference in frequencies between source and the english
 *         language, the sum of the absolute differences in the frequency of
 *         each letter
 */
uint32_t compare_to_english(const struct letter_frequencies *src);

/*
 * Print a letter frequency struct
 * @param src pointer to frequencies to print
 */
void print_frequencies(const struct letter_frequencies *src);

/*
 * Compute the Hamming distance (the number of differing bits) between two
 * equal-length buffers
 * @param src1 first buffer
 * @param src2 second buffer
 * @param len length of the two buffers
 *        precondition: length of source buffers are equal
 *        precondition: length of both source buffers >= len
 */
uint32_t hamming_distance(const uint8_t *src1, const uint8_t *src2, size_t len);

#endif  // ___text_score_h___
