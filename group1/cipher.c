/*
 * cipher.c
 * Functions related to block ciphers, particularly aes-ecb, for the Matasano
 * crypto challenges.
 *  1) Detect if something has been ecb encrypted
 */

#include <string.h>

#include "cipher.h"

/*
 * Guess whether a given cipher text has been encrypted using aes in 128-bit
 * ecb mode. Since ecb under a given key will always map the same 16-byte
 * plaintext block to the same 16-byte ciphertext block, check the ciphertext
 * for a repeated block.
 * @return 1 if a repeated block is found, otherwise 0
 */
uint32_t is_ecb_encrypted(const uint8_t *ciphertext, size_t len)
{
    if (!ciphertext)
        return 0;
    size_t blocks = len / 16;
    for (size_t i = 0; i < blocks; ++i)
        for (size_t j = i + 1; j < blocks; ++j)
            if (memcmp(ciphertext + 16*i, ciphertext + 16*j, 16) == 0)
                return 1;
    return 0;
}

