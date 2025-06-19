
#ifndef BUILD_JSON
#define BUILD_JSON

#include <stdio.h>
#include <stdint.h>

#include <parse-tree.h>
#include <asn1-data.h>

#define MAX_JSON_SIZE 100000

void build_json(ParseTree *parseTree, FILE *buffer);

#endif