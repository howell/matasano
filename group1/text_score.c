
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "text_score.h"

// private functions
static uint32_t is_letter(char c);
static char downcase(char c);

static struct letter_frequencies english_languange = {
    .freqs = {
        7.74,   // a
        1.70,   // b
        2.55,   // c
        4.14,   // d
        12.90,  // e
        2.24,   // f
        1.89,   // g
        6.27,   // h
        7.06,   // i
        0.18,   // j
        0.61,   // k
        4.00,   // l
        2.74,   // m
        7.02,   // n
        7.50,   // o
        1.57,   // p
        0.12,   // q
        6.07,   // r
        6.14,   // s
        8.73,   // t
        2.81,   // u
        1.06,   // v
        2.28,   // w
        0.16,   // x
        2.36,   // y
        0.17    // z
    }
};

/*
 * Calculate the frequency of each letter in a string
 * @param src the string to evaluate
 * @param out pointer to letter_frequencies struct to write the output to
 */
void calculate_letter_frequencies(const char *src,
        struct letter_frequencies *out)
{
    if (!src || !out)
        return;
    memset(out, 0, sizeof *out);
    size_t i = 0;
    size_t letters = 0;
    while (src[i] != '\0') {
        char c = src[i];
        if (is_letter(c)) {
            c = downcase(c);
            uint32_t index = c - 'a';
            assert(index < sizeof out->freqs);
            out->freqs[index] += 1;
            ++letters;
        }
        ++i;
    }
    // Normalize
    if (letters == 0)
        letters = 1;
    for (i = 0; i < sizeof out->freqs; ++i)
        out->freqs[i] = (out->freqs[i]  * 100.0) / (double) letters;
}

/*
 * Print a letter frequency struct
 * @param src pointer to frequencies to print
 */
void print_frequencies(const struct letter_frequencies *src)
{
    size_t i;
    for (i = 0; i < 26; ++i)
        printf("%c : %.2f%%\n", 'a' + (char) i, src->freqs[i]);
}

/*
 * Check if a character is a letter, e.g. in the range a-zA-Z
 * @return 1 if true, 0 otherwise
 */
static uint32_t is_letter(char c)
{
    if ((c >= 'a') && (c <= 'z'))
        return 1;
    if ((c >= 'A') && (c <= 'Z'))
        return 1;
    return 0;
}

/*
 * Returns the lowercase version of a letter. If it is uppercase, conver it to
 * lower case; otherwise, return unchanged
 * @return lowercase version of the letter
 */
static char downcase(char c)
{
    if ((c >= 'A') && (c <= 'Z'))
        return c - 'A' + 'a';
    return c;
}

