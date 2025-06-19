
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <parse-tree.h>

ParseTreeNode *create_node(Tag tag, uint32_t length){
    
    ParseTreeNode *node = (ParseTreeNode *) malloc(sizeof(ParseTreeNode));

    if(node != NULL){
        node->tag = tag;
        node->length = length;
        node->content = NULL;
        node->childNum = 0;
        node->childCapacity = 0;
        node->children = NULL;
    }

    return node;
}


uint8_t append_children_node(ParseTreeNode *parent, ParseTreeNode *child){
    
    if(parent->childNum == parent->childCapacity){
        uint32_t newCapacity = (parent->childCapacity == 0 ? 1 : parent->childCapacity * 2);
        ParseTreeNode **new = realloc(parent->children, newCapacity * sizeof(ParseTreeNode));
        if(new == NULL){
            fprintf(stderr, "[ERROR] Failed to re-allocate children.\n");
            return 1;
        }

        parent->children = new;
        parent->childCapacity = newCapacity;
    }
    
    parent->children[parent->childNum] = child;
    parent->childNum++;

    return 0;
}

uint8_t init_parse_tree(ParseTree *parseTree){

    parseTree->root = create_node(TAG_UNDEFINED, 0);
    if(parseTree->root == NULL){
        fprintf(stderr, "[ERROR] Failed to allocate new tree node.\n");
        return 1;
    }

    return 0;
}

static void do_free_parse_tree(ParseTreeNode *curr){

    uint32_t n = curr->childNum;
    for(uint32_t i = 0; i < n; i++){
        do_free_parse_tree(curr->children[i]);
    }
    free(curr->content);
    if(curr->children){
        free(curr->children);
    }
    free(curr);
}

void free_parse_tree(ParseTree *parseTree){
    do_free_parse_tree(parseTree->root);
}


static void do_visualize_parse_tree(ParseTreeNode *curr, uint32_t depth){
    for(uint32_t i = 0; i < depth; i++){
        printf("--");
    }

    if(curr->length == INDETERMINATE_LENGTH){
        printf("Tag: 0x%.2x, Length: INF", curr->tag);
    }
    else{
        printf("Tag: 0x%.2x, Length: %d", curr->tag, curr->length);
    }

    if(curr->tag & (1 << 5)){   // Composite type
        printf("\n");
        uint32_t n = (curr->childNum);
        for(uint32_t i = 0; i < n; i++){
            do_visualize_parse_tree(curr->children[i], depth + 1);
        }
    }
    else{  // Primitive type
        printf(", content: ");
        uint32_t n = (curr->length);
        for(uint32_t i = 0; i < n; i++){
            printf("0x%.2x ", curr->content[i]);
        }
        printf("\n");
    }
}

void visualize_parse_tree(ParseTree *parseTree){
    do_visualize_parse_tree(parseTree->root->children[0], 0);
}