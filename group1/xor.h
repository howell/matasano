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

#ifndef ___xor_h___
#define ___xor_h___

#include <stdint.h>
#include <stddef.h>

/*
 * Compute the xor of two equal-length buffers
 * @param dest pointer to buffer to write the output to
 * @param src1 pointer to first input buffer
 * @param src2 pointer to second input buffer, order of params doesn't matter
 * @param len number of bytes to xor
 *        precondition: length of destination and src buffers >= len
 */
void fixed_xor(uint8_t *dest, const uint8_t *src1, const uint8_t *src2,
        size_t len);

#endif  // ___xor_h___
