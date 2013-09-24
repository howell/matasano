
#ifndef ___convert_h___
#define ___convert_h___

#include <stdint.h>

/*
 * print a buffer in base 16
 * @param buf pointer to data to print
 * @param len number of bytes in buffer
 */
void print_base16(uint8_t *buf, uint32_t len);

/*
 * print a buffer in base 64
 * @param buf pointer to data to print
 * @param len number of bytes in buffer
 */
void print_base64(uint8_t *buf, uint32_t len);

/*
 * Read a base 16 string & convert to raw bytes
 * @param destination buffer; already allocated
 * @param src source base 16 string
 * @param len number of bytes to read
 */
void read_base16(uint8_t *dest, uint8_t *src, uint32_t len);

/*
 * Read a base 64 string & convert to raw bytes
 * @param destination buffer; already allocated
 * @param src source base 64 string
 * @param len number of bytes to read
 */
void read_base64(uint8_t *dest, uint8_t *src, uint32_t len);

#endif  // ___convert_h___

