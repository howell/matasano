
#ifndef ___text_score_h___
#define ___text_score_h___

#include <stdint.h>

// Type to represent a set of letter frequencies.
struct letter_frequencies {
    double freqs[26]; // store the frequency of each letter, with 'a' at index
                        // 0, 'b' at index 1, ..., and 'z' at index 25
};


/*
 * Calculate the frequency of each letter in a string
 * @param src the string to evaluate
 * @param out pointer to letter_frequencies struct to write the output to
 */
void calculate_letter_frequencies(const char *src,
        struct letter_frequencies *out);
/*
 * Calculate the difference in letter frequencies between a given distribution
 * and the english language
 * @param src pointer to letter frequencies to compare
 * @return absolute difference in frequencies between source and the english
 *         language, the sum of the absolute differences in the frequency of
 *         each letter
 */
double compare_to_english(const struct letter_frequencies *src);

/*
 * Print a letter frequency struct
 * @param src pointer to frequencies to print
 */
void print_frequencies(const struct letter_frequencies *src);

#endif  // ___text_score_h___
