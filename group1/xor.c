/*
 * xor.c
 * Functions related to xor-ing for the Matasano crypto challenges,
 * particularly:
 *  1) Fixed xor
 *  2) Single-character xor cipher
 *  3) Detecting single-character xor
 *  4) Finding which string out of many has been single-character xor'd
 *  5) Repeating-key xor cipher
 *  6) Braking a repeating-key xor cipher
 */

#include <stdlib.h>
#include <float.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "xor.h"
#include "text_score.h"
#include "convert.h"

// Private functions
static uint8_t find_likely_key_size(const uint8_t *cipher_text, size_t len,
        size_t max_key_size);

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
    for (size_t i = 0; i < len; ++i)
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
    char decrypted[len];
    uint8_t best_guess = 0;
    double highest_score = DBL_MIN;
    struct letter_frequencies lfs;
    for (size_t i = 0; i <= UINT8_MAX; ++i) {
        uint8_t key = i;
        repeated_byte_xor(key, src, (uint8_t *) decrypted, len);
        struct letter_frequencies freqs = { {0} };
        calculate_letter_frequencies(decrypted, len, &freqs);
        double score = compare_to_english(&freqs);
        if (score > highest_score) {
            lfs = freqs;
            highest_score = score;
            best_guess = key;
        }
    }
    return best_guess;
}

/*
 * Take an array of strings and return the one that is most likely to have
 * been encrpyted with repeated-byte xor
 * @param candidadtes array of pointers to strings
 * @param num number of candidates
 *        precondition: num >= length of candidates array
 * @return pointer to candidate most likely to have been repeated-byte xor'd
 */
const char *find_repeated_byte_xor(const char **candidates, size_t num)
{
    size_t index_of_most_likely = 0;
    double best_score = DBL_MIN;
    uint8_t best_key = 0;
    for (size_t i = 0; i < num; i++) {
        size_t raw_size = strlen(candidates[i]) / 2;
        uint8_t raw[raw_size];
        read_base16(raw, candidates[i], raw_size * 2);
        uint8_t key = detect_repeated_byte_xor(raw, raw_size);
        repeated_byte_xor(key, raw, raw, raw_size);
        struct letter_frequencies lfs;
        calculate_letter_frequencies((char *) raw, raw_size, &lfs);
        double score = compare_to_english(&lfs);
        if (score > best_score) {
            best_score = score;
            index_of_most_likely = i;
            best_key = key;
        }
    }
    return candidates[index_of_most_likely];
}

/*
 * Break cipher text that has been encrpyted with repeated-key xoring
 * @param cipher_text
 * @param len length of cipher text buffer
 *        precondition: length of cipher text buffer >= len
 * @param key buffer to output the key that is found for the ciphertext
 * @param max_key_size maximum key length to search for
 *        precondition: length of key buffer >= max_key_size
 *        precondition: length of cipher_text >= 6 * max_key_size
 * @return size of the found key
 */
size_t break_repeated_key_xor(const uint8_t *cipher_text, size_t len,
        uint8_t *key, size_t max_key_size)
{
    if (!cipher_text || !key)
        return 0;
    size_t likely_key_size = find_likely_key_size(cipher_text, len,
            max_key_size);
    uint8_t transposed[len];
    size_t block_size = len / likely_key_size;
    transpose(transposed, cipher_text, len, block_size);
    for (size_t j = 0; j < likely_key_size; j++) {
        key[j] = detect_repeated_byte_xor(transposed + block_size * j,
                block_size);
    }
    return likely_key_size;
}

/*
 * Find the most likely key size for some repeated key xor'd cipher text up
 * to a certain maximum key size. Based on the hamming distance between blocks.
 * See http://crypto.stackexchange.com/a/8118 for why that is.
 * @param cipher_text pointer to cipher text
 * @param len length of cipher text
 *        precondition: length of cipher text >= len
 *        currently, also require that len >= 6 * max_key_size because that's
 *        how many blocks it's taking to get accurate results
 * @param max_key_size largest key size to check
 * @return most likely key size for the given cipher text
 */
static uint8_t find_likely_key_size(const uint8_t *cipher_text, size_t len,
        size_t max_key_size)
{
    if (!cipher_text)
        return 0;
    assert(len >= 6 * max_key_size);    // TODO - handle this
    size_t likely_key_size = 0;
    uint32_t min_hamming = UINT32_MAX;
    for (size_t key_size = 1; key_size <= max_key_size; ++key_size) {
        uint32_t hamming = 0;
        // Take 6 block's worth - that was the smallest size that worked...
        for (size_t i = 0; i < 6; i++) {
            hamming += hamming_distance(cipher_text + 2 * i * key_size,
                    cipher_text + (2 * i + 1) * key_size, key_size);
        }
        hamming = hamming * 100 / key_size; // 100 is just a rounding factor
        if (hamming < min_hamming) {
            min_hamming = hamming;
            likely_key_size = key_size;
        }
    }
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
 * @param rows the number of "rows" in the source buffer. Then the number of
 *        "columns" will be len / rows. The transposed buffer will then have
 *        "columns" rows of size "rows"
 *        precondition: len % block_size == 0
 */
void transpose(uint8_t *dest, const uint8_t *src, size_t len,
        size_t rows)
{
    if (!dest || !src)
        return;
    size_t src_cols = len / rows;
    size_t transp_rows = src_cols;
    size_t transp_cols = rows;
    for (size_t i = 0; i < transp_rows; i++)
        for (size_t j = 0; j < transp_cols; ++j)
            dest[transp_cols * i + j] = src[src_cols * j + i];
}

