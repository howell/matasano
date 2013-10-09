
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
static double dot_product(const struct letter_frequencies *a,
        const struct letter_frequencies *b);

const static struct letter_frequencies english_language = {
    .freqs = {
        6.33,    // a
        1.38,    // b
        2.08,    // c
        3.39,    // d
        10.56,   // e
        1.83,    // f
        1.54,    // g
        5.13,    // h
        5.77,    // i
        0.14,    // j
        0.49,    // k
        3.27,    // l
        2.24,    // m
        5.74,    // n
        6.13,    // o
        1.28,    // p
        0.9,     // q
        4.96,    // r
        5.02,    // s
        7.14,    // t
        2.30,    // u
        0.86,    // v
        1.86,    // w
        0.12,    // x
        1.93,    // y
        0.13,    // z
        18.14    //  ' '
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
            size_t index = c - 'a';
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
    if (i != 0)
        for (size_t j = 0; j < FREQS_LEN; ++j)
            out->freqs[j] = (out->freqs[j]  * 100.0) / (double) i;
    return letters;
}

/*
 * Calculate the difference in letter frequencies between a given distribution
 * and the english language
 * @param src pointer to letter frequencies to compare
 * @return a score representing a frequency distribution's similarity to the
 *         english language, where a higher score represents a closer match.
 */
double compare_to_english(const struct letter_frequencies *src)
{
    if (!src)
        return 0;
    return dot_product(src, &english_language);
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
    for (i = 0; i < FREQS_LEN; ++i) {
        char current;
        if (i == LF_SPACE_INDEX)
            current = ' ';
        else
            current = 'a' + (char) i;
        printf("%c : %.2f%%\n", current, src->freqs[i]);
    }
}

/*
 * Take the dot product of two frequency distributions
 */
static double dot_product(const struct letter_frequencies *a,
        const struct letter_frequencies *b)
{
    double dot_product = 0;
    for (size_t i = 0; i < FREQS_LEN; ++i)
        dot_product += (a->freqs[i] * b->freqs[i]);
    return dot_product;
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

