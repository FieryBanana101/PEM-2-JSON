
#include <stdio.h>
#include <stdlib.h>

#include <der-parser.h>

static uint32_t parse_length(uint8_t *der, uint32_t *idx){

    uint32_t length;

    if(der[*idx] & (1 << 7)){   // INDETERMINATE_LENGTH form or long form
        uint32_t bytesNum = der[*idx] ^ (1 << 7);
        (*idx)++;

        if(bytesNum == 0){  // INDETERMINATE_LENGTH form
            length = INDETERMINATE_LENGTH;
        }
        else{  // Long form
            length = 0;
            uint32_t last = (*idx) + bytesNum;
            while((*idx) < last){
                length <<= 8;
                length += der[*idx];
                (*idx)++;
            }
        }
    }
    else{  // Short form
        length = der[*idx];
        (*idx)++;
    }

    return length;
}


static uint8_t do_parse_der(uint8_t *der, uint32_t der_size,  uint32_t *idx, ParseTreeNode *parent){
    
    uint32_t left = *idx;
    Tag tag = der[left];
    left++;


    uint8_t ret;
    if(tag & (1 << 5)){ // Constructed type, recurse again
        uint32_t length = parse_length(der, &left);

        ParseTreeNode *child = create_node(tag, length);
        if(child == NULL){
            fprintf(stderr, "[ERROR] Failed to allocate child node.\n");
            return 1;
        }

        ret = append_children_node(parent, child);
        if(!ret){
            if(length == INDETERMINATE_LENGTH){
                while(left + 1 < der_size && (der[left] != 0x00 || der[left+1] != 0x00)){
                    ret |= do_parse_der(der, der_size, &left, child);
                    if(ret) return 1;
                }
                left += 2;
            }
            else{
                uint32_t right = left + length - 1;
                while(left <= right){
                    ret |= do_parse_der(der, der_size, &left, child);
                    if(ret) return 1;
                }
            }
            *idx = left;
        }
    }
    else{  // Primitive type, base case of recursion
        uint32_t length = parse_length(der, &left);
        uint8_t isIndeterminate = 0;

        if(length == INDETERMINATE_LENGTH){
            length = 0;
            uint32_t temp = left;
            while(temp + 1 < der_size && (der[temp] != 0x00 || der[temp + 1] != 0x00)){
                temp++;
                length++;
            }

            isIndeterminate = 1;
        }

        uint8_t *content = (uint8_t *) malloc(sizeof(uint8_t) * length);
        ParseTreeNode *child = create_node(tag, length);

        if(child == NULL || content == NULL){
            fprintf(stderr, "[ERROR] Failed to allocate child leaf node.\n");
            return 1;
        }

        uint32_t j = 0;
        for(uint32_t i = left; i <= left + length - 1; i++){
            content[j] = der[i];
            j++;
        }
        left += length;
        if(isIndeterminate){
            left += 2;
        }

        *idx = left;
        child->content = content;
        ret = append_children_node(parent, child);
    }

    return ret;
}


uint8_t parse_der(uint8_t *der, uint32_t der_size, ParseTree *parseTree){

    uint32_t idx = 0;
    uint8_t ret = do_parse_der(der, der_size, &idx, parseTree->root);

    if(idx != der_size){
        fprintf(stderr, "[ERROR] Something went wrong, not all DER bytes were parsed (Parser stopped at byte %d out of %d byte).\n", idx, der_size);
        return 1;
    }

    if(parseTree->root->childNum > 1){
        fprintf(stderr, "[ERROR] DER form multiple rooted tree, invalid/malformed PEM file.\n");
        return 1;
    }

    return ret;
}