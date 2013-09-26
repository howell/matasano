
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "convert.h"

// private functions
static void test_print_base64();
static void test_print_base16();
static void test_read_base16();
static void test_read_base64();
static void test_base_16();
static void test_base64();

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
    test_read_base16();
    test_read_base64();
    test_base_16();
    test_base64();
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

/*
 * Run a test on the string-to-raw base16 conversion
 */
static void test_read_base16()
{
    const char hex_str[] = "0123456789AbCDef";
    uint8_t out_buf[((sizeof hex_str) - 1) / 2]  = { 0 };
    read_base16(out_buf, hex_str, strlen(hex_str));
    print_base16(out_buf, sizeof out_buf);
    // should print hex alphabet
}

static void test_base_16()
{
    const char hex_str[] = "0123456789abcdef";
    uint8_t out_buf[((sizeof hex_str) - 1) / 2];
    read_base16(out_buf, hex_str, strlen(hex_str));
    char out_str[sizeof out_buf + 1] = { '\0' };
    sprint_base16(out_str, out_buf, sizeof out_buf);
    printf("%s\n", out_str);
    assert(strcmp(out_str, hex_str) == 0);
    printf("Base16 Test Passed!\n");
}

/*
 * Run a test on the string-to-raw base64 conversion
 */
static void test_read_base64()
{
    const char base64_str[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
//    const char base64_str[] = "SSdt";
    uint8_t out_buf[(((sizeof base64_str) - 1) * 3) / 4] = { 0 };
    read_base64(out_buf, base64_str, strlen(base64_str));
    print_base16(out_buf, sizeof out_buf);
    print_base64(out_buf, sizeof out_buf);
}

/*
 * Run a test using both read and print base64
 */
static void test_base64()
{
    // test by length ascending to easily print output as a (null term'd)
    // string
    const char *test_str = "c3VyZS4=";
    uint8_t test_out[50] = { 0 };
    uint32_t len = read_base64(test_out, test_str, strlen(test_str));
    printf("%s\n", test_out);   // should print "sure."
    char out_str[50] = { '\0'};
    sprint_base64(out_str, test_out, len);
    assert(strcmp(out_str, test_str) == 0);

    test_str = "YXN1cmUu";
    len = read_base64(test_out, test_str, strlen(test_str));
    printf("%s\n", test_out);   // should print "asure."
    print_base64(test_out, len);
    sprint_base64(out_str, test_out, len);
    assert(strcmp(out_str, test_str) == 0);

    test_str = "ZWFzdXJlLg==";
    len = read_base64(test_out, test_str, strlen(test_str));
    printf("%s\n", test_out);   // should print "easure."
    print_base64(test_out, len);
    sprint_base64(out_str, test_out, len);
    assert(strcmp(out_str, test_str) == 0);

    test_str = "cGxlYXN1cmUu";
    len = read_base64(test_out, test_str, strlen(test_str));
    printf("%s\n", test_out);   // should print "leasure."
    print_base64(test_out, len);
    sprint_base64(out_str, test_out, len);
    assert(strcmp(out_str, test_str) == 0);
    printf("Base64 Test Passed!\n");
}

