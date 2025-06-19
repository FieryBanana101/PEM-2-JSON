
#include <build-json.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>

static void do_build_json(ParseTreeNode *curr, FILE *file, uint32_t depth){

    char buffer[MAX_JSON_SIZE];

    if(curr->tag & (1 << 5)){   // Composite type

        for(uint32_t i = 0; i < depth; i++){
            strcat(buffer, "  ");
        }

        if(depth > 0){
            strcat(buffer, "\"key\" : ");
        }
        strcat(buffer, "{\n");
        fputs(buffer, file);
        buffer[0] = '\0';

        uint32_t n = curr->childNum;
        for(uint32_t i = 0; i < n; i++){
            do_build_json(curr->children[i], file, depth + 1);
            if(i < n - 1){
                fputs(",\n", file);
            }
            else{
                fputs("\n", file);
            }
        }

        for(uint32_t i = 0; i < depth; i++){
            strcat(buffer, "  ");
        }
        strcat(buffer, "}");
        fputs(buffer, file);
        buffer[0] = '\0';
    }
    else{   // Primitive type
        for(uint32_t i = 0; i < depth; i++){
            strcat(buffer, "  ");
        }

        uint32_t n = curr->length;

        strcat(buffer, "\"key\" : \"0x");
        for(uint32_t i = 0; i < n; i++){
            char hex[3];
            sprintf(hex, "%.2x", curr->content[i]);
            strcat(buffer, hex);
        }
        
        strcat(buffer, "\"");
        fputs(buffer, file);
        buffer[0] = '\0';
    }

}


void build_json(ParseTree *parseTree, FILE *file){
    do_build_json(parseTree->root->children[0], file, 0);
}