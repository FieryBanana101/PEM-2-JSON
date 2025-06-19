
#ifndef BUILD_JSON
#define BUILD_JSON

#include <stdio.h>
#include <stdint.h>

#include <parse-tree.h>

#define MAX_JSON_SIZE 100000

uint8_t build_json(ParseTree *parseTree, FILE *buffer);

#endif