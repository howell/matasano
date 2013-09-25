
#ifndef ___convert_h___
#define ___convert_h___

#include <stdint.h>

/*
 * print the bytes in a buffer as hexadecimal characters
 * @param src pointer to data to print
 * @param len number of bytes in buffer
 */
void print_base16(const uint8_t *src, uint32_t len);

/*
 * Print a buffer in base 64. If the number of bytes in the input is not
 * a multiple of 3, then the input will be padded with 0 as necessary so
 * that it is. Trailing '=' characters indicate the number of bytes of
 * padding that were used, where '=' means one byte of padding and '==' two.
 * @param src pointer to data to print
 * @param len number of bytes in buffer
 */
void print_base64(const uint8_t *src, uint32_t len);

/*
 * Read a base 16 string & convert to raw bytes
 * @param destination buffer; already allocated
 * @param src source base 16 string
 * @param len number of characters in input string
 *        precondition: len >= sizeof dest buffer / 2
 */
void read_base16(uint8_t *dest, const char *src, uint32_t len);

/*
 * Read a base 64 string & convert to raw bytes
 * @param destination buffer; already allocated
 * @param src source base 64 string
 * @param len number of characters in input string
 *        precondition: must be multiple of 4, with any padding represented as =
 *        precondition: length of destination buffer >= 3/4 length of string
 * @return number of non-padding bytes decoded from the source string
 */
uint32_t read_base64(uint8_t *dest, const char *src, uint32_t len);

#endif  // ___convert_h___

