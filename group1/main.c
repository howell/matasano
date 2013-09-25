
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "convert.h"

// private functions
static void test_print_base64();
static void test_print_base16();

const uint8_t test_buf[] = {
    0x49, 0x27, 0x6d, 0x20, 0x6b, 0x69, 0x6c, 0x6c, 0x69, 0x6e, 0x67,
    0x20, 0x79, 0x6f, 0x75, 0x72, 0x20, 0x62, 0x72, 0x61, 0x69, 0x6e,
    0x20, 0x6c, 0x69, 0x6b, 0x65, 0x20, 0x61, 0x20, 0x70, 0x6f, 0x69,
    0x73, 0x6f, 0x6e, 0x6f, 0x75, 0x73, 0x20, 0x6d, 0x75, 0x73, 0x68,
    0x72, 0x6f, 0x6f, 0x6d
};

int main(void)
{
    test_print_base64();
    test_print_base16();
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

/*
 * Run a test on the raw-to-string base64 conversion. Currently relies on
 * programmer to verify by reading and comparing console output :-)
 */
static void test_print_base64()
{
    char *test_str = "pleasure.";
    print_base64((uint8_t *) test_str, strlen(test_str));
    // should print "cGxlYXN1cmUu"
    test_str = "leasure.";
    print_base64((uint8_t *) test_str, strlen(test_str));
    // should print "bGVhc3VyZS4="
    test_str = "easure.";
    print_base64((uint8_t *) test_str, strlen(test_str));
    // should print "ZWFzdXJlLg=="
    test_str = "asure.";
    print_base64((uint8_t *) test_str, strlen(test_str));
    // should print "YXN1cmUu"
    test_str = "sure.";
    print_base64((uint8_t *) test_str, strlen(test_str));
    // should print "c3VyZS4="
}

/*
 * Run a test on the raw-to-string base16 conversion
 */
static void test_print_base16()
{
    uint8_t hex_buf[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };
    print_base16(hex_buf, sizeof hex_buf);
    // should print out the hex alphabet
}

