
#ifndef ___convert_h___
#define ___convert_h___

#include <stdint.h>
#include <stddef.h>

/*
 * print the bytes in a buffer as hexadecimal characters
 * @param src pointer to data to print
 * @param len number of bytes in buffer
 *        precondition: length of src buffer >= len
 */
void print_base16(const uint8_t *src, size_t len);

/*
 * Perform the same operation as print_base16 but write the output to a
 * string (including a nul character) instead of printing it.
 * @param dest pointer to string to write output to
 *        precondition: length of dest buffer >= 2 * length of src buffer + 1
 * @param src pointer to data to print
 * @param len number of bytes in buffer
 *        precondition: length of src buffer >= len
 */
void sprint_base16(char *dest, const uint8_t *src, size_t len);

/*
 * Print a buffer in base 64. If the number of bytes in the input is not
 * a multiple of 3, then the input will be padded with 0 as necessary so
 * that it is. Trailing '=' characters indicate the number of bytes of
 * padding that were used, where '=' means one byte of padding and '==' two.
 * @param src pointer to data to print
 * @param len number of bytes in buffer
 *        precondition: length of src buffer >= len
 */
void print_base64(const uint8_t *src, size_t len);

/*
 * Perform the same operation as print_base64 but write the output to a
 * (null-terminated) string instead of printing it.
 * @param dest pointer to string to write encoded output to
 * @param src pointer to buffer to encode
 * @param len number of bytes to read from src buffer
 *        precondition: length of src buffer >= len
 *        precondition: length of dest buffer >= ((4 * len) / 3) + 1
 */
void sprint_base64(char *dest, const uint8_t *src, size_t len);

/*
 * Read a base 16 string & convert to raw bytes
 * @param dest destination buffer for decoded bytes; already allocated
 *        precondition: length of dest buffer >= length of src / 2
 * @param src source base 16 string
 * @param len number of characters in input string
 *        precondition: length of src buffer >= len
 */
void read_base16(uint8_t *dest, const char *src, size_t len);

/*
 * Read a base 64 string & convert to raw bytes
 * @param destination buffer; already allocated
 *        precondition: length of destination buffer >= 3/4 length of string
 * @param src source base 64 string
 * @param len number of characters in input string
 *        precondition: must be multiple of 4, with any padding represented as =
 *        precondition: length of src string >= len
 * @return number of non-padding bytes decoded from the source string
 */
size_t read_base64(uint8_t *dest, const char *src, size_t len);

#endif  // ___convert_h___

