/*
 * cipher.h
 * Functions related to block ciphers, particularly aes-ecb, for the Matasano
 * crypto challenges.
 *  1) Detect if something has been ecb encrypted
 */

#ifndef ___cipher_h___
#define ___cipher_h___

#include <stdint.h>
#include <stddef.h>

/*
 * Guess whether a given cipher text has been encrypted using aes in 128-bit
 * ecb mode. Since ecb under a given key will always map the same 16-byte
 * plaintext block to the same 16-byte ciphertext block, check the ciphertext
 * for a repeated block.
 * @return 1 if a repeated block is found, otherwise 0
 */
uint32_t is_ecb_encrypted(const uint8_t *ciphertext, size_t len);

#endif  // ___cipher_h___

