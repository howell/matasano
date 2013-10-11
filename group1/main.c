
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
static void test_break_repeat_byte();
static void test_hamming_distance();
static void test_repeat_key_xor();
static void test_break_repeat_key();
static void test_transpose();

int main(void)
{
    test_fixed_xor();
    test_print_base64();
    test_print_base16();
    test_read_base16();
    test_read_base64();
    test_base_16();
    test_base64();
    test_break_repeat_byte();
    test_hamming_distance();
    test_repeat_key_xor();
    test_transpose();
    test_break_repeat_key();
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
static void test_break_repeat_byte()
{
    char cipher_text[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    uint8_t raw_cipher[(sizeof cipher_text - 1) / 2];
    read_base16(raw_cipher, cipher_text, strlen(cipher_text));
    uint8_t key = detect_repeated_byte_xor(raw_cipher, sizeof raw_cipher);
    repeated_byte_xor(key, raw_cipher, raw_cipher, sizeof raw_cipher);
    assert(memcmp(raw_cipher, "Cooking MC's like a pound of bacon",
                sizeof raw_cipher) == 0);
    printf("Repeat byte xor test passed!\n");
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

    a = "roses";
    b = "toned";
    hamming = hamming_distance((uint8_t *) a, (uint8_t *) b, strlen(a));
    expected = 3;
    assert(hamming = expected);

    printf("Hamming distance test passed!\n");
}

/*
 * Test the repeated key xor function
 */
static void test_repeat_key_xor()
{
    uint8_t key[] = { 'I', 'C', 'E' };
    const char src[] = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
    uint8_t encrpyted[sizeof src - 1];
    repeated_key_xor(key, sizeof key, (uint8_t *) src, encrpyted,
            sizeof encrpyted);
    const char expected[] = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";
    char test_out_str[sizeof expected];
    sprint_base16(test_out_str, encrpyted, sizeof encrpyted);
    assert(strcmp(test_out_str, expected) == 0);
    printf("Repeat key xor test passed!\n");
}

/*
 * Test the transpose function
 */
static void test_transpose()
{
    size_t rows = 3;
    const uint8_t test1[] = { 0x01, 0x02, 0x03,
                              0x04, 0x05, 0x06,
                              0x07, 0x08, 0x09 };
    uint8_t dest1[sizeof test1];
    transpose(dest1, test1, sizeof test1, rows);
    const uint8_t expected1[] = { 0x01, 0x04, 0x07,
                                  0x02, 0x05, 0x08,
                                  0x03, 0x06, 0x09 };
    assert(memcmp(dest1, expected1, sizeof dest1) == 0);

    rows = 3;
    const uint8_t test2[] = { 0x01, 0x02, 0x03, 0xDE,
                              0x04, 0x05, 0x06, 0xAD,
                              0x07, 0x08, 0x09, 0x0B };
    uint8_t dest2[sizeof test2];
    transpose(dest2, test2, sizeof test2, rows);
    const uint8_t expected2[] = { 0x01, 0x04, 0x07,
                                  0x02, 0x05, 0x08,
                                  0x03, 0x06, 0x09,
                                  0xDE, 0xAD, 0x0B };
    assert(memcmp(dest2, expected2, sizeof dest2) == 0);

    rows = 3;
    const uint8_t test3[] = { 1,  2,  3, 4,
                              5,  6,  7, 8,
                              9, 10, 11, 12 };
    uint8_t dest3[sizeof test3];
    transpose(dest3, test3, sizeof test3, rows);
    const uint8_t expected3[] = { 1, 5,  9,
                                  2, 6, 10,
                                  3, 7, 11,
                                  4, 8, 12 };
    assert(memcmp(dest3, expected3, sizeof dest3) == 0);
    printf("Transpose test passed!\n");
}

/*
 * Test the function that breaks repeat key xor encryption
 */
const char cipher_text64[] = "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";
static void test_break_repeat_key()
{
    uint8_t raw_ct[(3 * (sizeof cipher_text64 - 1)) / 4];
    size_t unpadded = read_base64(raw_ct, cipher_text64, strlen(cipher_text64));
    uint8_t key[40];
    size_t key_size = break_repeated_key_xor(raw_ct, unpadded, key, 40);
    const char expected[] = "I'm back and I'm ringin' the bell";
    assert(key_size == 29);
    repeated_key_xor(key, key_size, raw_ct, raw_ct, unpadded);
    assert(strncmp((char *) raw_ct, expected, sizeof expected - 1) == 0);
    printf("Break repeat key xor test passed!\n");
}

