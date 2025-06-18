
#include <stdio.h>
#include <stdlib.h>

#include <der-parser.h>

static ssize_t parse_length(uint8_t *der, ssize_t *idx){

    ssize_t length;

    if(der[*idx] & (1 << 7)){   // INDETERMINATE_LENGTH form or long form
        ssize_t bytesNum = der[*idx] ^ (1 << 7);
        (*idx)++;

        if(bytesNum == 0){  // INDETERMINATE_LENGTH form
            length = INDETERMINATE_LENGTH;
        }
        else{  // Long form
            length = 0;
            ssize_t last = (*idx) + bytesNum;
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


static uint8_t do_parse_der(uint8_t *der, ssize_t der_size,  ssize_t *idx, ParseTreeNode *parent){
    
    ssize_t left = *idx;
    Tag tag = der[left];
    left++;


    uint8_t ret;
    if(tag & (1 << 5)){ // Constructed type, recurse again
        ssize_t length = parse_length(der, &left);

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
                ssize_t right = left + length - 1;
                while(left <= right){
                    ret |= do_parse_der(der, der_size, &left, child);
                    if(ret) return 1;
                }
            }
            *idx = left;
        }
    }
    else{  // Primitive type, base case of recursion
        ssize_t length = parse_length(der, &left);
        uint8_t isIndeterminate = 0;

        if(length == INDETERMINATE_LENGTH){
            length = 0;
            ssize_t temp = left;
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

        ssize_t j = 0;
        for(ssize_t i = left; i <= left + length - 1; i++){
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


uint8_t parse_der(uint8_t *der, ssize_t der_size, ParseTree *parseTree){

    ssize_t idx = 0;
    uint8_t ret = do_parse_der(der, der_size, &idx, parseTree->root);

    if(idx != der_size){
        fprintf(stderr, "[ERROR] Something went wrong, not all DER bytes were parsed (Parser stopped at byte %ld out of %ld byte).\n", idx, der_size);
        return 1;
    }
    return ret;
}