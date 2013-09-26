
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "convert.h"

// Private functions
static void read_3bytes_base64(const uint8_t *src, char *out);
static char to_base64(uint8_t num);
static uint8_t char16_to_raw(char char16);
static void decode_4bytes_base64(const char *src, uint8_t *dest);
static uint8_t char64_to_raw(char char64);
static void read_base64_with_padding(char *dest, const uint8_t *src,
        uint32_t len);

/*
 * print the bytes in a buffer as hexadecimal characters
 * @param src pointer to data to print
 * @param len number of bytes in buffer
 *        precondition: length of src buffer >= len
 */
void print_base16(const uint8_t *src, uint32_t len)
{
    uint32_t i;
    for (i = 0; i < len; i++)
        printf("%02x", src[i]);
    printf("\n");
}

/*
 * Perform the same operation as print_base16 but write the output to a
 * string (including a nul character) instead of printing it.
 * @param dest pointer to string to write output to
 *        precondition: length of dest buffer >= 2 * length of src buffer + 1
 * @param src pointer to data to print
 * @param len number of bytes in buffer
 *        precondition: length of src buffer >= len
 */
void sprint_base16(char *dest, const uint8_t *src, uint32_t len)
{
    uint32_t i;
    for (i = 0; i < len; i++)
        sprintf(dest + i * 2, "%02x", src[i]);
    // sprintf automatically includes nul terminator
}

/*
 * Print a buffer in base 64. If the number of bytes in the input is not
 * a multiple of 3, then the input will be padded with 0 as necessary so
 * that it is. Trailing '=' characters indicate the number of bytes of
 * padding that were used, where '=' means one byte of padding and '==' two.
 * @param src pointer to data to print
 * @param len number of bytes in buffer
 *        precondition: length of src buffer >= len
 */
void print_base64(const uint8_t *src, uint32_t len)
{
    // process the input as groups of 4 6-bit numbers
    uint32_t groups_of_4 = len / 3;  // groups of 4 base64 numbers
    char printable_group[5] = { '\0' }; // print as a string
    uint32_t i;
    for (i = 0; i < groups_of_4; i++) {
        read_3bytes_base64(src + i * 3, printable_group);
        printf("%s", printable_group);
    }
    // process any remaining bytes with padding as necessary
    uint32_t remaining = len % 3;
    if (remaining) {
        uint32_t off = groups_of_4 * 3;
        read_base64_with_padding(printable_group, src + off, remaining);
        printf("%s", printable_group);
    }
    printf("\n");
}

/*
 * Perform the same operation as print_base64 but write the output to a
 * (null-terminated) string instead of printing it.
 * @param dest pointer to string to write encoded output to
 * @param src pointer to buffer to encode
 * @param len number of bytes to read from src buffer
 *        precondition: length of src buffer >= len
 *        precondition: length of dest buffer >= ((4 * len) / 3) + 1
 */
void sprint_base64(char *dest, const uint8_t *src, uint32_t len)
{
    uint32_t groups_of_4 = len / 3;
    uint32_t i, out_index = 0;
    for (i = 0; i < groups_of_4; ++i) {
        read_3bytes_base64(src + i * 3, dest + out_index);
        out_index += 4;
    }
    uint32_t remaining = len % 3;
    if (remaining) {
        uint32_t off = groups_of_4 * 3;
        read_base64_with_padding(dest + out_index, src + off, remaining);
        out_index += 4;
    }
    dest[out_index] = '\0';
}

/*
 * Encode a group of at most 3 raw base-64 numbers to 4 printable characters,
 * with the padding character '=' appended to indicate the number of bytes
 * of padding used.
 * @param dest pointer to buffer to write the output to; does not write a null
 *        character.
 *        precondition: length of destination buffer >= 4
 * @param src pointer to raw base64 numbers to decode
 *        precondition: length of src buffer >= 3
 * @param len number of bytes in the src buffer
 *        precondition: len < 3 - only use this function to encode a group of
 *        less than 4 (3 bytes) base64 numbers
 */
static void read_base64_with_padding(char *dest, const uint8_t *src,
        uint32_t len)
{
    assert(len < 3);
    // copy into intermediate buffer to ensure 0-padding without reading
    // out-of-bounds
    uint8_t last_group[3] = { 0 };
    uint32_t i;
    for (i = 0; i < len; ++i)
        last_group[i] = src[i];
    read_3bytes_base64(last_group, dest);
    char padding = "="[0];
    dest[3] = padding;
    if (len == 1)
        dest[2] = padding;
}

/*
 * Read three bytes as four base-64 numbers
 * @param buf pointer to bytes to print
 *        precondition: length >= 3
 * @param out string to write the formatted output to
 *        precondition: length >= 4
 */
static void read_3bytes_base64(const uint8_t *src, char *out)
{
    uint8_t first = src[0] >> 2;
    uint8_t second = ((src[0] &  0x3) << 4) | (src[1] >> 4);
    uint8_t third = ((src[1] & 0xf) << 2) | (src[2] >> 6);
    uint8_t fourth = src[2] & 0x3f;
    out[0] = to_base64(first);
    out[1] = to_base64(second);
    out[2] = to_base64(third);
    out[3] = to_base64(fourth);
}

/*
 * Return the printable character for a base 64 number
 * @param num base 64 number
 * @return printable character for num if it exists, else \0
 */
static char to_base64(uint8_t num)
{
    char base64_alphabet[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    if (num < 64)
        return base64_alphabet[num];
    printf("bad base64 number!\n");
    return '\0';
}

/*
 * Read a base 16 string & convert to raw bytes
 * @param dest destination buffer for decoded bytes; already allocated
 *        precondition: length of dest buffer >= length of src / 2
 * @param src source base 16 string
 * @param len number of characters in input string
 *        precondition: length of src buffer >= len
 */
void read_base16(uint8_t *dest, const char *src, uint32_t len)
{
    uint32_t i;
    for (i = 0; i < len; ++i) {
        char char16 = src[i];
        uint8_t raw = char16_to_raw(char16);
        if (raw > 15)
            return;     // error detected
        uint32_t shift = 4 * (1 - (i % 2));
        dest[i/2] = (dest[i/2] & (0xf0 >> shift)) | (raw << shift);
    }
}

/*
 * Decode a single base 16 character to raw
 * @param char16 base 16 character to decode
 * @return raw integer
 */
static uint8_t char16_to_raw(char char16)
{
    if (char16 >= '0' && char16 <= '9')
        return char16 - '0';
    if (char16 >= 'a' && char16 <= 'f')
        return char16 - 'a' + 10;
    if (char16 >= 'A' && char16 <= 'F')
        return char16 - 'A' + 10;
    printf("bad hex char: %x\n", char16);
    return 255;
}

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
uint32_t read_base64(uint8_t *dest, const char * src, uint32_t len)
{
    uint32_t groups_of_4 = len / 4;  // groups of 4 base 64 numbers
    uint32_t i;
    uint32_t out_index = 0;
    // decode in groups of 4
    for (i = 0; i < groups_of_4; ++i) {
        const char *current_group = src + i * 4;
        decode_4bytes_base64(current_group, dest + out_index);
        out_index += 3;
    }
    char padding = "="[0];
    if (src[len - 1] == padding) {
        --out_index;
        if (src[len - 2] == padding)
            --out_index;
    }
    return out_index;
}

/*
 * Decode 4 base64 characters
 * @param src pointer to 4 characters to decode
 *        precondition: length of src >= 4
 * @param dest pointer to buffer to write decoded base64 to
 *        precondition: length of dest >= 3
 */
static void decode_4bytes_base64(const char *src, uint8_t *dest)
{
    uint8_t group_of_4[4];
    uint32_t i;
    for (i = 0; i < 4; ++i)
        group_of_4[i] = char64_to_raw(src[i]);
    dest[0] = (group_of_4[0] << 2) | (group_of_4[1] >> 4);
    dest[1] = (group_of_4[1] << 4) | (group_of_4[2] >> 2);
    dest[2] = (group_of_4[2] << 6) | group_of_4[3];
}

/*
 * Decode a single base 64 character to raw
 * @param char64 base 16 character to decode
 * @return raw integer
 */
static uint8_t char64_to_raw(char char64)
{
    if (char64 >= 'A' && char64 <= 'Z')
        return char64 - 'A';
    if (char64 >= 'a' && char64 <= 'z')
        return char64 - 'a' + 26;
    if (char64 >= '0' && char64 <= '9')
        return char64 - '0' + 52;
    if (char64 == '+')
        return 62;
    if (char64 == '/')
        return 63;
    if (char64 == "="[0])   // padding
        return 0;
    printf("bad base64 char: 0x%02x", char64);
    return 255;
}


