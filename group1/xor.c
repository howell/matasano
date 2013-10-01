/*
 * xor.c
 * Functions related to xor-ing for the Matasano crypto challenges,
 * particularly:
 *  1) Fixed xor
 *  2) Single-character xor Cipher
 *  3) Detecting single-character xor
 *  4) Repeating-key xor cipher
 *  5) ...
 */

#include <stdlib.h>
#include <float.h>

#include "xor.h"
#include "text_score.h"

/*
 * Compute the xor of two equal-length buffers
 * @param dest pointer to buffer to write the output to
 * @param src1 pointer to first input buffer
 * @param src2 pointer to second input buffer, order of params doesn't matter
 * @param len number of bytes to xor
 *        precondition: length of destination and src buffers >= len
 */
void fixed_xor(uint8_t *dest, const uint8_t *src1, const uint8_t *src2,
        size_t len)
{
    if (!src1 || !src2)
        return;
    // TODO: possible use of the restrict keyword?
    size_t i;
    for (i = 0; i < len; ++i)
        dest[i] = src1[i] ^ src2[i];
}

/*
 * XOR each byte in a buffer with a given key
 * @param key
 * @param src
 * @param dest
 * @param len number of bytes to xor
 *        precondition: length of src and dest buffers >= len
 */
void repeated_byte_xor(uint8_t key, const uint8_t *src, uint8_t *dest,
        size_t len)
{
    if (!src || !dest)
        return;
    size_t i;
    for (i = 0; i < len; ++i)
        dest[i] = src[i] ^ key;
}

/*
 * Break a repeated key xor by decypting with each possible key and analyzing
 * the letter frequencies of each text. Look for the key that produces text
 * that most closely resembles the letter frequencies of the english language.
 * @param src pointer to encrpyted (english language) string
 * @param len length of src buffer
 *        precondition: length of src buffer >= len
 * @return best guess for the key that src has been repeat-key-encrypted with
 */
uint8_t detect_repeated_byte_xor(const uint8_t *src, size_t len)
{
    if (!src)
        return 0;
    char *decrypted = malloc(len);
    if (!decrypted)
        return 0;
    size_t i;
    uint8_t best_guess = 0;
    double closest_diff = DBL_MAX;
    for (i = 0; i <= UINT8_MAX; ++i) {
        uint8_t key = i;
        repeated_byte_xor(key, src, (uint8_t *) decrypted, len);
        struct letter_frequencies freqs = { {0} };
        calculate_letter_frequencies(decrypted, &freqs);
        double diff = compare_to_english(&freqs);
        if (diff < closest_diff) {
            closest_diff = diff;
            best_guess = key;
        }
    }
    free(decrypted);
    return best_guess;
}

