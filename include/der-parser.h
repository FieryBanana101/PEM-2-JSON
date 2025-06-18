

#ifndef DER_PARSER_H
#define DER_PARSER_H

#include <stdint.h>
#include <stddef.h>

#include <parse-tree.h>

#define MAX_DER_FILE_SIZE 10000

uint8_t parse_der(uint8_t *der, ssize_t der_size, ParseTree *parseTree);

#endif