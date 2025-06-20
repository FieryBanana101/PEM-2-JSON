
#include <asn1-data.h>

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <gmp.h>

uint8_t parse_tag_type(Tag tag, char *parsedTag){
    switch(tag){
        case TAG_SEQUENCE:
            strcpy(parsedTag, "Sequence");
            break;

        case TAG_SET:
            strcpy(parsedTag, "Set");
            break;

        case TAG_BOOLEAN:
            strcpy(parsedTag, "Boolean");
            break;
            
        case TAG_INTEGER:
            strcpy(parsedTag, "Integer");
            break;

        case TAG_NULL:
            strcpy(parsedTag, "NULL");
            break;

        case TAG_OBJECT_ID:
            strcpy(parsedTag, "OID");
            break;

        case TAG_BIT_STRING:
            strcpy(parsedTag, "Bit string");
            break;

        case TAG_OCTET_STRING:
            strcpy(parsedTag, "Octet string");
            break;

        case TAG_UTF8_STRING:
            strcpy(parsedTag, "UTF-8 string");
            break;

        case TAG_NUMERIC_STRING:
            strcpy(parsedTag, "Numeric string");
            break;

        case TAG_PRINTABLE_STRING:
            strcpy(parsedTag, "Printable string");
            break;

        case TAG_T61_STRING:
            strcpy(parsedTag, "T61 string");
            break;

        case TAG_IA5_STRING:
            strcpy(parsedTag, "IA5 string");
            break;

        case TAG_UTC_TIME:
            strcpy(parsedTag, "UTC time");
            break;

        default:
            if(tag & (1 << 5)){  // Another possible composite value which we don't know the actual tag value
                strcpy(parsedTag, "Context specific");
                break;
            }
            else{
                fprintf(stderr, "[ERROR] Invalid tag found during JSON generation (Tag: 0x%.2x)\n", tag);
                return 1;
            }
    }
    return 0;
}

uint8_t parse_boolean(uint8_t *value, uint32_t length, char *parsedValue){
    if(length != 1) return 1;

    if(~value[0]){ // All bits are not set to one, false
        strcpy(parsedValue, "false");
    }
    else{  // All bits are set to one, true
        strcpy(parsedValue, "true");
    }
    return 0;
}

uint8_t parse_integer(uint8_t *value, uint32_t length, char *parsedValue){
    mpz_t num;
    mpz_init(num);
    mpz_import(num, length, 1, 1, 1, 0, value);
    mpz_get_str(parsedValue, 10, num);
    return 0;
}

uint8_t parse_null(uint8_t *value, uint32_t length, char *parsedValue){
    strcpy(parsedValue, "null");
    return 0;
}

uint8_t parse_object_id(uint8_t *value, uint32_t length, char *parsedValue){
    sprintf(parsedValue, "\"%d.%d", (uint8_t) value[0] / 40, (uint8_t) value[0] % 40);
    uint32_t sub = 0;
    for(uint32_t i = 1; i < length; i++){
        sub <<= 7;
        if(value[i] & (1 << 7)){
            sub += (value[i] ^ (1 << 7));
        }
        else{
            sub += value[i];
            char temp[10];
            sprintf(temp, ".%d", sub);
            strcat(parsedValue, temp);
            sub = 0;
        }
    }
    strcat(parsedValue, "\"");
    return 0;
}

uint8_t parse_bit_string(uint8_t *value, uint32_t length, char *parsedValue){
    
    char temp[length * 100];
    for(uint32_t i = 0; i < length; i++){
        char binary[9];
        for(uint32_t j = 0; j < 8; j++){
            if(value[i] & (1 << j)){
                binary[7-j] = '1';
            }
            else{
                binary[7-j] = '0';
            }
        }
        binary[8] = '\0';
        strcat(temp, binary);
    }

    // Remove leading '0'
    uint32_t i = 0;
    while(temp[i] == '0') i++;

    strcat(parsedValue, "\"0b");
    strcat(parsedValue, temp + i);
    strcat(parsedValue, "\"");
    return 0;
}

uint8_t parse_octet_string(uint8_t *value, uint32_t length, char *parsedValue){
    strcat(parsedValue, "\"0x");
    for(uint32_t i = 0; i < length; i++){
        char hex[3];
        sprintf(hex, "%.2x", value[i]);
        strcat(parsedValue, hex);
    }
    strcat(parsedValue, "\"");
    return 0;
}

uint8_t parse_general_string(uint8_t *value, uint32_t length, char *parsedValue){
    strcat(parsedValue, "\"");
    for(uint32_t i = 0; i < length; i++){
        char ch[8];
        sprintf(ch, "%c", value[i]);

        // Add escape if character is a double quote
        if(value[i] == '"'){  
            strcat(parsedValue, "\\");
        }
        strcat(parsedValue, ch);
    }
    strcat(parsedValue, "\"");
    return 0;
}

uint8_t parse_utc_time(uint8_t *value, uint32_t length, char *parsedValue){
    if(length != 13 || value[length - 1] != 'Z'){
        fprintf(stderr, "[ERROR] Invalid UTC time format, failed to parse UTC time.\n");
        return 1;
    }

    strcat(parsedValue, "\"");

    char time[100];
    if(value[0] >= '5'){ // Year is in 19XX format
        sprintf(time, "19%c%c-%c%c-%c%cT%c%c:%c%c:%c%cZ", value[0], value[1], value[2], value[3], value[4], value[5], value[6], value[7], value[8], value[9], value[10], value[11]);
    }
    else{ // Year is in 20XX format
        sprintf(time, "20%c%c-%c%c-%c%cT%c%c:%c%c:%c%cZ", value[0], value[1], value[2], value[3], value[4], value[5], value[6], value[7], value[8], value[9], value[10], value[11]);
    }

    strcat(parsedValue, time);
    strcat(parsedValue, "\"");
    return 0;
}
