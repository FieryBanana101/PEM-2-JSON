
#ifndef ASN1_DATA_H
#define ASN1_DATA_H

#include <stdint.h>

typedef enum {
    TAG_UNDEFINED = 0x00,
    TAG_BOOLEAN = 0x01,
    TAG_INTEGER = 0x02,
    TAG_BIT_STRING = 0x03,
    TAG_OCTET_STRING = 0x04,
    TAG_NULL = 0x05,
    TAG_OBJECT_ID = 0x06,
    TAG_UTF8_STRING = 0x0c,
    TAG_NUMERIC_STRING = 0x12,
    TAG_PRINTABLE_STRING = 0x13,
    TAG_T61_STRING = 0x14,
    TAG_IA5_STRING = 0x16,
    TAG_UTC_TIME = 0x17,
    TAG_SEQUENCE = 0x30,
    TAG_SET = 0x31
} Tag;

#define INDETERMINATE_LENGTH -1

#define MAX_PARSED_TAG_SIZE 30
#define MAX_PARSED_VALUE_SIZE 10000
#define MAX_INTEGER_DIGIT 10000

uint8_t parse_tag_type(Tag tag, char *parsedTag);

uint8_t parse_boolean(uint8_t *value, uint32_t length, char *parsedValue);

uint8_t parse_integer(uint8_t *value, uint32_t length, char *parsedValue);

uint8_t parse_null(uint8_t *value, uint32_t length, char *parsedValue);

uint8_t parse_object_id(uint8_t *value, uint32_t length, char *parsedValue);

uint8_t parse_bit_string(uint8_t *value, uint32_t length, char *parsedValue);

uint8_t parse_octet_string(uint8_t *value, uint32_t length, char *parsedValue);

uint8_t parse_general_string(uint8_t *value, uint32_t length, char *parsedValue);

uint8_t parse_utc_time(uint8_t *value, uint32_t length, char *parsedValue);



#endif