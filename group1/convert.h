
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
 * @param len number of bytes to read
 */
void read_base16(uint8_t *dest, const uint8_t *src, uint32_t len);

/*
 * Read a base 64 string & convert to raw bytes
 * @param destination buffer; already allocated
 * @param src source base 64 string
 * @param len number of bytes to read
 */
void read_base64(uint8_t *dest, const uint8_t *src, uint32_t len);

#endif  // ___convert_h___

