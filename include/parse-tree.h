
#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <asn1-data.h>

typedef struct Node {
    Tag tag;
    ssize_t length;
    uint8_t *content;
    ssize_t childNum;
    ssize_t childCapacity;
    struct Node **children;
} ParseTreeNode;

typedef struct {
    ParseTreeNode *root;
} ParseTree;


uint8_t init_parse_tree(ParseTree *parseTree);

void free_parse_tree(ParseTree *parseTree);

void visualize_parse_tree(ParseTree *parseTree);

ParseTreeNode *create_node(Tag tag, ssize_t length);

uint8_t append_children_node(ParseTreeNode *parent, ParseTreeNode *child);

#endif