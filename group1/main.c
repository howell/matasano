
#include <stdint.h>
#include <stdio.h>

#include "convert.h"


const uint8_t test_buf[] = {
    0x49, 0x27, 0x6d, 0x20, 0x6b, 0x69, 0x6c, 0x6c, 0x69, 0x6e, 0x67,
    0x20, 0x79, 0x6f, 0x75, 0x72, 0x20, 0x62, 0x72, 0x61, 0x69, 0x6e,
    0x20, 0x6c, 0x69, 0x6b, 0x65, 0x20, 0x61, 0x20, 0x70, 0x6f, 0x69,
    0x73, 0x6f, 0x6e, 0x6f, 0x75, 0x73, 0x20, 0x6d, 0x75, 0x73, 0x68,
    0x72, 0x6f, 0x6f, 0x6d
};

int main(void)
{
    char *test_str = "pleasure.";
    print_base64((uint8_t*) test_str, strlen(test_str));
    test_str = "leasure.";
    print_base64((uint8_t*) test_str, strlen(test_str));
    test_str = "easure.";
    print_base64((uint8_t*) test_str, strlen(test_str));
    test_str = "asure.";
    print_base64((uint8_t*) test_str, strlen(test_str));
    test_str = "sure.";
    print_base64((uint8_t*) test_str, strlen(test_str));
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
