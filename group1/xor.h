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

/*
 * XOR each byte in a buffer with a given key
 * @param key
 * @param src
 * @param dest
 * @param len number of bytes to xor
 *        precondition: length of src and dest buffers >= len
 */
void repeated_byte_xor(uint8_t key, const uint8_t *src, uint8_t *dest,
        size_t len);

/*
 * Perform a repeated key xor encryption
 * @param key buffer that serves as the key
 * @param key_size length of the key
 *        precondition: length of key buffer >= key_size
 * @param src buffer to be encrypted
 * @param dest buffer to hold the result
 * @param len number of bytes to encrpyt
 *        precondition: length of source and destination buffers >= len
 */
void repeated_key_xor(const uint8_t *key, size_t key_size, const uint8_t *src,
        uint8_t *dest, size_t len);

/*
 * Break a repeated key xor by decypting with each possible key and analyzing
 * the letter frequencies of each text. Look for the key that produces text
 * that most closely resembles the letter frequencies of the english language.
 * @param src pointer to encrpyted (english language) string
 * @param len length of src buffer
 *        precondition: length of src buffer >= len
 * @return best guess for the key that src has been repeat-key-encrypted with
 */
uint8_t detect_repeated_byte_xor(const uint8_t *src, size_t len);

#endif  // ___xor_h___
