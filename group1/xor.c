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

#include "xor.h"

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
void repeated_key_xor(uint8_t key, const uint8_t *src, uint8_t *dest,
        size_t len)
{
    if (!src || !dest)
        return;
    size_t i;
    for (i = 0; i < len; ++i)
        dest[i] = src[i] ^ key;
}

