/*
 * xor.c
 * Functions related to xor-ing for the Matasano crypto challenges,
 * particularly:
 *  1) Fixed xor
 *  2) Single-character xor cipher
 *  3) Detecting single-character xor
 *  4) Repeating-key xor cipher
 *  5) ...
 */

#include <stdlib.h>
#include <float.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "xor.h"
#include "text_score.h"
#include "convert.h"

// Private functions
static void transpose(uint8_t *dest, const uint8_t *src, size_t len,
        size_t block_size);

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
        uint8_t *dest, size_t len)
{
    size_t i, j, block;
    for (i = 0; i < len / key_size; ++i) {
        block = i * key_size;
        for (j = 0; j < key_size; ++j)
            dest[block + j] = key[j] ^ src[block + j];
    }
    size_t remaining = len % key_size;
    block = i * key_size;
    for (j = 0; j < remaining; ++j)
        dest[block + j] = key[j] ^ src[block + j];
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

/*
 * Break cipher text that has been encrpyted with repeated-key xoring
 * @param cipher_text
 * @param len length of cipher text buffer
 *        precondition: length of cipher text buffer >= len
 * @param key buffer to output the key that is found for the ciphertext
 * @param max_key_size maximum key length to search for
 *        precondition: length of key buffer >= max_key_size
 *        precondition: length of cipher_text >= 2 * max_key_size
 * @return size of the found key
 */
size_t break_repeated_key_xor(const uint8_t *cipher_text, size_t len,
        uint8_t *key, size_t max_key_size)
{
    if (!cipher_text || !key)
        return 0;
    assert(len >= 2 * max_key_size);    // TODO - handle this
    size_t key_size;
    size_t likely_key_size = 0;
    uint32_t min_hamming = UINT32_MAX;
    for (key_size = 2; key_size <= max_key_size; ++key_size) {
        size_t i;
        //uint32_t hamming = 0;
        for (i = 0; i < 1; i++) {
            //hamming += hamming_distance(cipher_text + (
        }
        // TODO: possible need for more precision?
        uint32_t hamming = 100 * hamming_distance(cipher_text, cipher_text + key_size,
                key_size) / key_size;
        //printf("hamming for key size %lu = %d\n", key_size, hamming);
        if (hamming < min_hamming) {
            min_hamming = hamming;
            likely_key_size = key_size;
            printf("%d, %lu\n", min_hamming, likely_key_size);
        }
    }
    printf("Likely key size = %lu\n", likely_key_size);
    uint8_t *transposed = calloc(len, sizeof(uint8_t));
    if (!transposed)
        return 0;
    transpose(transposed, cipher_text, len, likely_key_size);
    size_t block_size = len / likely_key_size;
    size_t i;
    for (i = 0; i < len / block_size; i++) {
        size_t j;
        for (j = 0; j < block_size; j++) {
            printf("%lu, %lu\n", i, j);
            assert(transposed[block_size * i + j] == cipher_text[block_size * j + i]);
        }
    }
    for (i = 0; i < likely_key_size; i++) {
        key[i] = detect_repeated_byte_xor(transposed + block_size * i,
                block_size);
        printf("key[%lu] = 0x%02x | %c\n", i, key[i], key[i]);
    }
    free(transposed);
    return likely_key_size;
}

/*
 * Transpose a buffer of equal-length blocks, making block N of the destination
 * the N'th byte of each block of the source. The block size of the destination
 * is then equal to the length of the source divided by blocks size, which must
 * divide evenly
 * @param dest buffer to write the destination to
 * @param src buffer to transpose
 * @param len length of the source and destination buffers
 *        precondition: length of src and dest >= len
 * @param block_size the size of the blocks in the source buffer to transpose;
 *        the transposed blocks in the destiantion will have size
 *        len / block_size
 *        precondition: len % block_size == 0
 */
static void transpose(uint8_t *dest, const uint8_t *src, size_t len,
        size_t block_size)
{
    if (!dest || !src)
        return;
//    assert(len % block_size == 0);
    size_t transp_block_size = len / block_size;
    size_t transp_blocks = len / transp_block_size;
    size_t i;
    for (i = 0; i < transp_blocks; i++) {
        size_t transp_block_index = i * transp_block_size;
        size_t j;
        for (j = 0; j < transp_block_size; ++j) {
            if (transp_block_index + j < len) {
                size_t src_block_index = j * block_size;
                if (src_block_index + i < len)
                    dest[transp_block_index + j] = src[src_block_index + i];
            }
        }
    }
}

/*
 * Test the transpose function
 */
void test_transpose()
{
    size_t block_size = 3;
    const uint8_t test1[] = { 0x01, 0x02, 0x03,
                              0x04, 0x05, 0x06,
                              0x07, 0x08, 0x09 };
    uint8_t dest1[sizeof test1];
    transpose(dest1, test1, sizeof test1, block_size);
    const uint8_t expected1[] = { 0x01, 0x04, 0x07,
                                  0x02, 0x05, 0x08,
                                  0x03, 0x06, 0x09 };
    assert(memcmp(dest1, expected1, sizeof dest1) == 0);

    block_size = 4;
    const uint8_t test2[] = { 0x01, 0x02, 0x03, 0xDE,
                              0x04, 0x05, 0x06, 0xAD,
                              0x07, 0x08, 0x09, 0x0B };
    uint8_t dest2[sizeof test2];
    transpose(dest2, test2, sizeof test2, block_size);
    const uint8_t expected2[] = { 0x01, 0x04, 0x07,
                                  0x02, 0x05, 0x08,
                                  0x03, 0x06, 0x09,
                                  0xDE, 0xAD, 0x0B };
    assert(memcmp(dest2, expected2, sizeof dest2) == 0);
    printf("Transpose test passed!\n");
}

