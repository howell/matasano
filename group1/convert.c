
#include "convert.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

// Private functions
static void read_3bytes_base64(const uint8_t *src, char *out);
static char to_base64(uint8_t num);
static uint8_t char16_to_raw(char char16);
static uint8_t char64_to_raw(char char64);

/*
 * print the bytes in a buffer as hexadecimal characters
 * @param src pointer to data to print
 * @param len number of bytes in buffer
 */
void print_base16(const uint8_t *src, uint32_t len)
{
    uint32_t i;
    for (i = 0; i < len; i++)
        printf("%0x", src[i]);
    printf("\n");
}

/*
 * Print a buffer in base 64. If the number of bytes in the input is not
 * a multiple of 3, then the input will be padded with 0 as necessary so
 * that it is. Trailing '=' characters indicate the number of bytes of
 * padding that were used, where '=' means one byte of padding and '==' two.
 * @param src pointer to data to print
 * @param len number of bytes in buffer
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
    uint32_t off = groups_of_4 * 3;
    uint32_t remaining = len % 3;
    if (remaining) {
        uint8_t last_group[3] = { 0 };
        for (i = 0; i < remaining; ++i)
            last_group[i] = src[off + i];
        read_3bytes_base64(last_group, printable_group);
        char padding = "="[0];
        printable_group[3] = padding;
        if (remaining == 1)
            printable_group[2] = padding;
        printf("%s", printable_group);
    }
    printf("\n");
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
 * @param src source base 16 string
 * @param len number of bytes to read
 */
void read_base16(uint8_t *dest, const uint8_t *src, uint32_t len)
{
    uint32_t i;
    for (i = 0; i < len; ++i) {
        uint8_t char16 = src[i];
        uint32_t shft = 4 * (1 - (i % 2));
        uint8_t raw = char16_to_raw(char16);
        if (raw > 15)
            return;     // error detected
        dest[i/2] = (dest[i/2] & (0xf0 >> shft)) | (raw << shft);
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
 * @param src source base 64 string
 * @param len number of bytes to read
 */
void read_base64(uint8_t *dest, const uint8_t * src, uint32_t len)
{
    uint32_t groups_of_4 = len / 4;  // groups of 4 base 64 numbers
    uint32_t i;
    uint8_t base64_nums[4];
    for (i = 0; i < groups_of_4; ++i) {
        uint32_t j, idx;
        idx = 4 * i;
        for (j = 0; j < 4; ++j) {
            base64_nums[j] = char64_to_raw(src[idx + j]);
            if (base64_nums[j] > 63)
                return;
        }
        dest[idx] = (base64_nums[0] << 2) | (base64_nums[1] >> 6);
        dest[idx + 1] = (base64_nums[1] << 4) | (base64_nums[2] & 0xf);
        dest[idx + 2] = (base64_nums[2] << 6) | base64_nums[3];
    }
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
    printf("bad base64 char: %x", char64);
    return 255;
}


