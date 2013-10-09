
#include <string.h>
#include <float.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "text_score.h"

// private functions
uint32_t diff_frequencies(const struct letter_frequencies *src1,
        const struct letter_frequencies *src2);
static uint32_t absolute_difference(uint32_t x, uint32_t y);
static uint32_t differing_bits(uint8_t x, uint8_t y);
static int qsort_comp(const void *p1, const void *p2);

const static struct letter_frequencies english_language = {
    .freqs = {
        633,    // a
        138,    // b
        208,    // c
        339,    // d
        1056,   // e
        183,    // f
        154,    // g
        513,    // h
        577,    // i
        14,     // j
        49,     // k
        327,    // l
        224,    // m
        574,    // n
        613,    // o
        128,    // p
        9,      // q
        496,    // r
        502,    // s
        714,    // t
        230,    // u
        86,     // v
        186,    // w
        12,     // x
        193,    // y
        13,     // z
        1814    //  ' '
    }
};

static const size_t FREQS_LEN = sizeof english_language.freqs /
    sizeof english_language.freqs[0];

/*
 * Calculate the frequency of each letter in a string
 * @param src the string to evaluate
 * @param out pointer to letter_frequencies struct to write the output to
 * @return the number of letters found in the string
 */
uint32_t calculate_letter_frequencies(const char *src,
        struct letter_frequencies *out)
{
    if (!src || !out)
        return 0;
    memset(out, 0, sizeof *out);
    size_t i = 0;
    uint32_t letters = 0;
    while (src[i] != '\0') {
        char c = src[i];
        if (isalpha(c)) {
            c = tolower(c);
            uint32_t index = c - 'a';
            assert(index < FREQS_LEN);
            out->freqs[index] += 1;
            ++letters;
        } else if (c == ' ') {
            out->freqs[LF_SPACE_INDEX] += 1;
            ++letters;
        }
        ++i;
    }
    // Normalize
    if (letters != 0)
        for (i = 0; i < FREQS_LEN; ++i)
            out->freqs[i] = (out->freqs[i]  * LF_FP_BASE * 100) / letters;
    return letters;
}

/*
 * Calculate the difference in letter frequencies between a given distribution
 * and the english language
 * @param src pointer to letter frequencies to compare
 * @return absolute difference in frequencies between source and the english
 *         language, the sum of the absolute differences in the frequency of
 *         each letter
 */
uint32_t compare_to_english(const struct letter_frequencies *src)
{
    if (!src)
        return UINT32_MAX;
    return diff_frequencies(src, &english_language);
}

/*
 * Print a letter frequency struct
 * @param src pointer to frequencies to print
 */
void print_frequencies(const struct letter_frequencies *src)
{
    if (!src)
        return;
    size_t i;
    for (i = 0; i < FREQS_LEN; ++i)
        printf("%c : %0d.%02d%%\n", 'a' + (char) i, src->freqs[i] / LF_FP_BASE,
                src->freqs[i] % LF_FP_BASE);
}

/*
 * Calculate the absolute difference in letter frequencies between two
 * distributions
 * @param src1 one of the letter frequencies to compare
 * @param src2 second letter frequencies to compare
 * @return absolute difference in frequencies between src1 and src2, e.g.
 *         the sum of the absolute differences in the frequency of each letter
 */
uint32_t diff_frequencies(const struct letter_frequencies *src1,
        const struct letter_frequencies *src2)
{
    if (!src1 || !src2)
        return UINT32_MAX;
    uint32_t difference = 0;
    size_t i;
    for(i = 0; i < FREQS_LEN; ++i)
        difference += absolute_difference(src1->freqs[i], src2->freqs[i]);
    return difference;
}

/*
 *
 */
uint32_t sorted_diff(const struct letter_frequencies *src,
        const struct letter_frequencies *to)
{
    struct letter_frequencies src_sorted, to_sorted;
    memcpy(&src_sorted, src, sizeof src_sorted);
    memcpy(&to_sorted, to, sizeof to_sorted);
    qsort(src_sorted.freqs, FREQS_LEN, sizeof src_sorted.freqs[0], qsort_comp);
    qsort(to_sorted.freqs, FREQS_LEN, sizeof to_sorted.freqs[0], qsort_comp);
    uint32_t score = 0;
    for (size_t i = 0; i < FREQS_LEN; ++i) {

    }
    return score;
}

/*
 * Comparator function for stdlib qsort function for letter frequencies.
 * Currently letter frequencies are uint32_t's
 * Sort in descending order
 * @return < 0 if *p1 goes before *p2
 *          0  if *p1 is equivalent to *p2
 *         > 0 if *p1 goes after *p2
 */
static int qsort_comp(const void *p1, const void *p2)
{
    const uint32_t *a = p1;
    const uint32_t *b = p2;
    if (*a > *b)
        return -1;
    if (*a < *b)
        return 1;
    return 0;
}

/*
 * Return the absolute difference between two uint32_t
 * @param x
 * @param y
 * @return |x - y|
 */
static uint32_t absolute_difference(uint32_t x, uint32_t y)
{
    if (x > y)
        return x - y;
    return y - x;
}

/*
 * Compute the Hamming distance (the number of differing bits) between two
 * equal-length buffers
 * @param src1 first buffer
 * @param src2 second buffer
 * @param len length of the two buffers
 *        precondition: length of source buffers are equal
 *        precondition: length of both source buffers >= len
 * @return number of differing bits between inputs
 */
uint32_t hamming_distance(const uint8_t *src1, const uint8_t *src2, size_t len)
{
    if (!src1 || !src2)
        return UINT32_MAX;
    uint32_t distance = 0;
    size_t i;
    for (i = 0; i < len; ++i)
        distance += differing_bits(src1[i], src2[i]);
    return distance;
}

/*
 * Count the number of differing bits in two characters
 * @param x
 * @param y
 * @return the number of differing bits between x and y, e.g. the Hamming
 * distance for a string of length one
 */
static uint32_t differing_bits(uint8_t x, uint8_t y)
{
    uint32_t diff = 0;
    uint32_t z = x ^ y;
    while (z) {
        if (z & 1)
            ++diff;
        z = z >> 1;
    }
    return diff;
}

