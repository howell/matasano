
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
 * Return the printable character for a base 64 number
 * @param num base 64 number
 * @return printable character for num if it exists, else \0
 */
char to_base64(uint8_t num);

/*
 * Read a base 16 string & convert to raw bytes
 * @param destination buffer; already allocated
 * @param src source base 16 string
 * @param len number of bytes to read
 */
void read_base16(uint8_t *dest, uint8_t *src, uint32_t len);

/*
 * Decode a single base 16 character to raw
 * @param char16 base 16 character to decode
 * @return raw integer
 */
uint8_t char16_to_raw(uint8_t char16);

/*
 * Read a base 64 string & convert to raw bytes
 * @param destination buffer; already allocated
 * @param src source base 64 string
 * @param len number of bytes to read
 */
void read_base64(uint8_t *dest, uint8_t *src, uint32_t len);

/*
 * Decode a single base 64 character to raw
 * @param char64 base 16 character to decode
 * @return raw integer
 */
uint8_t char64_to_raw(uint8_t char64);

#endif  // ___convert_h___

