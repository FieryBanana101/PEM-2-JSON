
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <pem-parser.h>
#include <der-parser.h>

uint32_t base64_value[128];

void base64_value_init(void){

    for(ssize_t i = 0; i < 128; i++){
        base64_value[i] = 64;
    }

    uint32_t counter = 0;
    for(ssize_t i = 'A'; i <= 'Z'; i++){
        base64_value[i] = counter; counter++;
    }
    for(ssize_t i = 'a'; i <= 'z'; i++){
        base64_value[i] = counter; counter++;
    }
    for(ssize_t i = '0'; i <= '9'; i++){
        base64_value[i] = counter; counter++;
    }

    base64_value['+'] = counter; counter++;
    base64_value['/'] = counter; counter++;
    base64_value['='] = 0;
}


ssize_t base64_decode(char *encoded, uint8_t *decoded){
    ssize_t n = strlen(encoded);

    if(n % 4){
        fprintf(stderr, "[ERROR] base64 encoded text length must be a multiple of four, length: %lu.\n",  n);
        return 0;
    }

    for(ssize_t i = 0; i < n; i++){
        if(encoded[i] < 0 || encoded[i] >= 128 || base64_value[(size_t) encoded[i]] == 64){
            fprintf(stderr, "[ERROR] Invalid base64 character (%c).\n", encoded[i]);
            return 0;
        }
    }

    ssize_t decoded_size = 0;
    for(ssize_t i = 0; i < n; i += 4){

        uint32_t value = 0;
        ssize_t padding_count = 0;
        for(ssize_t j  = 0; j < 4; j++){
            value <<= 6;
            value += base64_value[(size_t) encoded[i + j]];
            if(encoded[i + j] == '=') padding_count++;
        }
 

        decoded_size += 3 - padding_count;
        for(ssize_t j = 0; j < 3; j++){
            if(j >= padding_count){
                decoded_size--;
                decoded[decoded_size] = value % 256;
            }
            value >>= 8;
        }
        decoded_size += 3 - padding_count;
    }   

    return decoded_size;
}


uint8_t parse_pem_header(char *header){
    return 0;
}


uint8_t parse_pem_footer(char *header){
    return 0;
}


uint8_t parse_pem(FILE *file, ParseTree *parseTree){

    uint8_t der[MAX_DER_FILE_SIZE];
    ssize_t der_size = 0;

    char buffer[MAX_FILE_BUFFER_SIZE];
    uint8_t decoded[MAX_FILE_BUFFER_SIZE];

    fgets(buffer, MAX_FILE_BUFFER_SIZE, file);
    if(parse_pem_header(buffer)){
        fprintf(stderr, "[ERROR] Failed to parse PEM header.\n");
        return 1;
    }
    
    while(fgets(buffer, MAX_FILE_BUFFER_SIZE, file)){
        if(strncmp(buffer, "-----", 5) == 0){
            break;
        }

        buffer[strlen(buffer) - 1] = '\0';
        ssize_t decoded_size = base64_decode(buffer, decoded);
        if(decoded_size == 0){
            printf("[ERROR] Invalid PEM content.\n");
            return 1;
        }

        for(ssize_t i = 0; i < decoded_size; i++){
            der[der_size] = decoded[i];
            der_size++;
        }
    }

    if(parse_pem_footer(buffer)){
        fprintf(stderr, "[ERROR] Failed to parse PEM footer.\n");
        return 1;
    }

    return parse_der(der, der_size, parseTree);
}