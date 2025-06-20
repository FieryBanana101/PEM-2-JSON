
#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <stdio.h>
#include <stdint.h>

#include <asn1-data.h>

typedef struct Node {
    Tag tag;
    uint32_t length;
    uint8_t *content;
    uint32_t childNum;
    uint32_t childCapacity;
    struct Node **children;
} ParseTreeNode;

typedef struct {
    ParseTreeNode *root;
} ParseTree;


uint8_t init_parse_tree(ParseTree *parseTree);

void free_parse_tree(ParseTree *parseTree);

void visualize_parse_tree(ParseTree *parseTree, FILE *file);

ParseTreeNode *create_node(Tag tag, uint32_t length);

uint8_t append_children_node(ParseTreeNode *parent, ParseTreeNode *child);

#endif