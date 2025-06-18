#ifndef PEM_PARSER_H
#define PEM_PARSER_H

#include <stdint.h>
#include <der-parser.h>

#define MAX_FILE_BUFFER_SIZE 10000

extern uint32_t base64_value[];

void base64_value_init(void);

ssize_t base64_decode(char *encoded, uint8_t *decoded);

uint8_t parse_pem(FILE *file, ParseTree *parseTree);

#endif