
#include "convert.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

// Private functions
static void read_3bytes_base64(uint8_t *buf, char *out);

// data types
#pragma pack(1)
    // warning: the order of these bitfields is implementation defined
    struct base64_group {
        uint8_t fourth : 6;
        uint8_t third  : 6;
        uint8_t second : 6;
        uint8_t first  : 6;
    };
#pragma pack()

const uint8_t test_buf[] = {
    0x49, 0x27, 0x6d, 0x20, 0x6b, 0x69, 0x6c, 0x6c, 0x69, 0x6e, 0x67,
    0x20, 0x79, 0x6f, 0x75, 0x72, 0x20, 0x62, 0x72, 0x61, 0x69, 0x6e,
    0x20, 0x6c, 0x69, 0x6b, 0x65, 0x20, 0x61, 0x20, 0x70, 0x6f, 0x69,
    0x73, 0x6f, 0x6e, 0x6f, 0x75, 0x73, 0x20, 0x6d, 0x75, 0x73, 0x68,
    0x72, 0x6f, 0x6f, 0x6d
};

/*
 * print a buffer in base 16
 * @param buf pointer to data to print
 * @param len number of bytes in buffer
 */
void print_base16(uint8_t *buf, uint32_t len)
{
    uint32_t i;
    for (i = 0; i < len; i++) {
        printf("%0x", buf[i]);
    }
}

/*
 * Print a buffer in base 64. If the number of bytes in the input is not
 * a multiple of 3, then the input will be padded with 0 as necessary so
 * that it is. Trailing '=' characters indicate the number of bytes of
 * padding that were used.
 * @param buf pointer to data to print
 * @param len number of bytes in buffer
 */
void print_base64(uint8_t *buf, uint32_t len)
{
    // process the input as groups of 4 6-bit numbers
    uint32_t groups_of_4 = len / 3;  // groups of 4 base64 numbers
    char printable_group[5] = { '\0' }; // include a null
    for (uint32_t i = 0; i < groups_of_4; i++) {
        read_3bytes_base64(buf + i * 3, printable_group);
        printf("%s", printable_group);
    }
    // process any remaining bytes with padding as necessary
    uint32_t off = groups_of_4 * 3;
    uint32_t remaining = len % 3;
    if (remaining) {
        uint8_t last_group[3] = { 0 };
        for (uint32_t i = 0; i < remaining; ++i)
            last_group[i] = buf[off + i];
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
static void read_3bytes_base64(uint8_t *buf, char *out)
{
    uint8_t first = buf[0] >> 2;
    uint8_t second = ((buf[0] &  0x3) << 4) | (buf[1] >> 4);
    uint8_t third = ((buf[1] & 0xf) << 2) | (buf[2] >> 6);
    uint8_t fourth = buf[2] & 0x3f;
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
char to_base64(uint8_t num)
{
    char base64_alphabet[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    if (num < 64)
        return base64_alphabet[num];
    printf("bad base64 number!\n");
    return '\0';
}

/*
 * Read a base 16 string & convert to raw bytes
 * @param destination buffer; already allocated
 * @param src source base 16 string
 * @param len number of bytes to read
 */
void read_base16(uint8_t *dest, uint8_t *src, uint32_t len)
{
    uint32_t i;
    for (i = 0; i < len; ++i) {
        uint8_t char16 = src[i];
        uint32_t shft = 4 * (1 - (i % 2));
        uint8_t raw = char16_to_raw(char16);
        if (raw > 15)
            continue;
        dest[i/2] = (dest[i/2] & (0xf0 >> shft)) | (raw << shft);
    }
}

/*
 * Decode a single base 16 character to raw
 * @param char16 base 16 character to decode
 * @return raw integer
 */
uint8_t char16_to_raw(uint8_t char16)
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
void read_base64(uint8_t *dest, uint8_t *src, uint32_t len)
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
uint8_t char64_to_raw(uint8_t char64)
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

int main(void)
{
    uint8_t *test_str = "pleasure.";
    print_base64(test_str, strlen(test_str));
    test_str = "leasure.";
    print_base64(test_str, strlen(test_str));
    test_str = "easure.";
    print_base64(test_str, strlen(test_str));
    test_str = "asure.";
    print_base64(test_str, strlen(test_str));
    test_str = "sure.";
    print_base64(test_str, strlen(test_str));
//    char hex_str[] = "0123456789aBcDEF";
//    uint32_t hex_len = strlen(hex_str);
//    uint8_t dest[hex_len / 2];
//    print_base16(test_buf, sizeof test_buf);
//    printf("\n");
//    print_base64(test_buf, sizeof test_buf);
//    printf("\n");
//    read_base16(dest, hex_str, hex_len);
//    print_base16(dest, sizeof dest);
//    printf("\n");
    return 0;
}

