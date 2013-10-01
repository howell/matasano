
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "convert.h"
#include "xor.h"
#include "text_score.h"

// private functions
static void test_print_base64();
static void test_print_base16();
static void test_read_base16();
static void test_read_base64();
static void test_base_16();
static void test_base64();
static void test_fixed_xor();
static void test_break_repeat_key();
static void test_hamming_distance();


int main(void)
{
    test_fixed_xor();
    test_print_base64();
    test_print_base16();
    test_read_base16();
    test_read_base64();
    test_base_16();
    test_base64();
    test_break_repeat_key();
    test_hamming_distance();
    return 0;
}

/*
 * Run a test on the raw-to-string base64 conversion. Currently relies on
 * programmer to verify by reading and comparing console output :-)
 */
static void test_print_base64()
{
    char *test_str = "sure.";
    char test_output[32] = { '\0' };
    sprint_base64(test_output, (uint8_t *) test_str, strlen(test_str));
    assert(strcmp(test_output, "c3VyZS4=") == 0);
    test_str = "asure.";
    sprint_base64(test_output, (uint8_t *) test_str, strlen(test_str));
    assert(strcmp(test_output, "YXN1cmUu") == 0);
    test_str = "easure.";
    sprint_base64(test_output, (uint8_t *) test_str, strlen(test_str));
    assert(strcmp(test_output, "ZWFzdXJlLg==") == 0);
    test_str = "leasure.";
    sprint_base64(test_output, (uint8_t *) test_str, strlen(test_str));
    assert(strcmp(test_output, "bGVhc3VyZS4=") == 0);
    test_str = "pleasure.";
    sprint_base64(test_output, (uint8_t *) test_str, strlen(test_str));
    assert(strcmp(test_output, "cGxlYXN1cmUu") == 0);
    printf("Print base64 test passed!\n");
}

/*
 * Run a test on the raw-to-string base16 conversion
 */
static void test_print_base16()
{
    uint8_t hex_buf[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };
    char test_output[32] = { '\0' };
    sprint_base16(test_output, hex_buf, sizeof hex_buf);
    assert(strcmp(test_output, "0123456789abcdef") == 0);
    printf("Print base16 test passed!\n");
}

/*
 * Run a test on the string-to-raw base16 conversion
 */
static void test_read_base16()
{
    const char hex_str[] = "0123456789AbCDef";
    uint8_t out_buf[((sizeof hex_str) - 1) / 2]  = { 0 };
    read_base16(out_buf, hex_str, strlen(hex_str));
    const uint8_t expected[sizeof out_buf] = {0x01, 0x23, 0x45, 0x67, 0x89,
        0xAB, 0xCD, 0xEF };
    assert(memcmp(out_buf, expected, sizeof out_buf) == 0);
    printf("Read base16 test passed!\n");
}

static void test_base_16()
{
    const char *hex_str = "0123456789abcdef";
    uint8_t out_buf[8];
    read_base16(out_buf, hex_str, strlen(hex_str));
    char out_str[sizeof out_buf + 1] = { '\0' };
    sprint_base16(out_str, out_buf, sizeof out_buf);
    assert(strcmp(out_str, hex_str) == 0);
    printf("Base16 round-trip test passed!\n");
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
    const uint8_t expected[sizeof out_buf] = {
        0x49, 0x27, 0x6d, 0x20, 0x6b, 0x69, 0x6c, 0x6c, 0x69, 0x6e, 0x67,
        0x20, 0x79, 0x6f, 0x75, 0x72, 0x20, 0x62, 0x72, 0x61, 0x69, 0x6e,
        0x20, 0x6c, 0x69, 0x6b, 0x65, 0x20, 0x61, 0x20, 0x70, 0x6f, 0x69,
        0x73, 0x6f, 0x6e, 0x6f, 0x75, 0x73, 0x20, 0x6d, 0x75, 0x73, 0x68,
        0x72, 0x6f, 0x6f, 0x6d };
    assert(memcmp(out_buf, expected, sizeof out_buf) == 0);
    printf("Read base64 test passed!\n");
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
    size_t len = read_base64(test_out, test_str, strlen(test_str));
    assert(memcmp(test_out, "sure.", 5) == 0);
    char out_str[50] = { '\0'};
    sprint_base64(out_str, test_out, len);
    assert(strcmp(out_str, test_str) == 0);

    test_str = "YXN1cmUu";
    len = read_base64(test_out, test_str, strlen(test_str));
    assert(memcmp(test_out, "asure.", 6) == 0);
    sprint_base64(out_str, test_out, len);
    assert(strcmp(out_str, test_str) == 0);

    test_str = "ZWFzdXJlLg==";
    len = read_base64(test_out, test_str, strlen(test_str));
    assert(memcmp(test_out, "easure.", 7) == 0);
    sprint_base64(out_str, test_out, len);
    assert(strcmp(out_str, test_str) == 0);

    test_str = "cGxlYXN1cmUu";
    len = read_base64(test_out, test_str, strlen(test_str));
    assert(memcmp(test_out, "pleasure.", 9) == 0);
    sprint_base64(out_str, test_out, len);
    assert(strcmp(out_str, test_str) == 0);

    printf("Base64 round-trip test passed!\n");
}

/*
 * Test the fixed_xor function
 */
static void test_fixed_xor()
{
    const char src1[] = "1c0111001f010100061a024b53535009181c";
    const char src2[] = "686974207468652062756c6c277320657965";
    assert(sizeof src1 == sizeof src2);
    uint8_t src1_raw[((sizeof src1) - 1) / 2];
    uint8_t src2_raw[((sizeof src2) - 1) / 2];
    read_base16(src1_raw, src1, strlen(src1));
    read_base16(src2_raw, src2, strlen(src2));
    uint8_t out_raw[sizeof src1_raw];
    fixed_xor(out_raw, src1_raw, src2_raw, sizeof out_raw);
    char out_str[sizeof src1];
    sprint_base16(out_str, out_raw, sizeof out_raw);
    const char *expected = "746865206b696420646f6e277420706c6179";
    assert(strcmp(out_str, expected) == 0);
    printf("Fixed XOR Test Passed!\n");

}

/*
 * Test breaking repeated key xor
 */
static void test_break_repeat_key()
{
    char cipher_text[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    uint8_t raw_cipher[(sizeof cipher_text - 1) / 2];
    read_base16(raw_cipher, cipher_text, strlen(cipher_text));
    uint8_t key = detect_repeated_key_xor(raw_cipher, sizeof raw_cipher);
    repeated_key_xor(key, raw_cipher, raw_cipher, sizeof raw_cipher);
    assert(memcmp(raw_cipher, "Cooking MC's like a pound of bacon",
                sizeof raw_cipher) == 0);
    printf("Passed repeat key xor test!\n");
}

/*
 * Test the hamming distance function
 */
static void test_hamming_distance()
{
    const char *a = "this is a test";
    const char *b = "wokka wokka!!!";
    uint32_t hamming = hamming_distance((uint8_t *) a, (uint8_t *) b,
            strlen(a));
    uint32_t expected = 37;
    assert(hamming == expected);
}

