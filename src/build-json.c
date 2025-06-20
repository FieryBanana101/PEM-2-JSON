
#include <build-json.h>
#include <asn1-data.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint8_t parse_value(Tag tag, uint32_t length, uint8_t *value, char *parsedTag, char *parsedValue){

    if(parse_tag_type(tag, parsedTag)) return 1;

    switch(tag){
        case TAG_BOOLEAN:
            return parse_boolean(value, length, parsedValue);
            
        case TAG_INTEGER:
            return parse_integer(value, length, parsedValue);

        case TAG_NULL:
            return parse_null(value, length, parsedValue);

        case TAG_OBJECT_ID:
            return parse_object_id(value, length, parsedValue);

        case TAG_BIT_STRING:
            return parse_bit_string(value, length, parsedValue);

        case TAG_OCTET_STRING:
            return parse_octet_string(value, length, parsedValue);

        case TAG_UTF8_STRING:
        case TAG_NUMERIC_STRING:
        case TAG_PRINTABLE_STRING:
        case TAG_T61_STRING:
        case TAG_IA5_STRING:
            return parse_general_string(value, length, parsedValue);

        case TAG_UTC_TIME:
            return parse_utc_time(value, length, parsedValue);

        default:
            fprintf(stderr, "[ERROR] Invalid tag found during JSON generation (Tag: 0x%.2x)\n", tag);
            return 1;
    }
}

static uint8_t do_build_json(ParseTreeNode *curr, FILE *file, uint32_t depth){

    char buffer[MAX_JSON_SIZE], temp[MAX_JSON_SIZE], parsedTag[MAX_PARSED_TAG_SIZE], parsedValue[MAX_PARSED_VALUE_SIZE];
    buffer[0] = '\0';
    temp[0] = '\0';
    parsedTag[0] = '\0';
    parsedValue[0] = '\0';

    if(curr->tag & (1 << 5)){   // Composite type

        for(uint32_t i = 0; i < depth; i++){
            strcat(buffer, "  ");
        }

        if(depth > 0){
            if(parse_tag_type(curr->tag, parsedTag)) return 1;
            sprintf(temp, "\"%s\" : ", parsedTag);
            strcat(buffer, temp);
        }
        strcat(buffer, "{\n");
        fputs(buffer, file);
        buffer[0] = '\0';
        temp[0] = '\0';
        parsedTag[0] = '\0';

        uint32_t n = curr->childNum;
        for(uint32_t i = 0; i < n; i++){
            if(do_build_json(curr->children[i], file, depth + 1)) return 1;

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
        temp[0] = '\0';
    }
    else{   // Primitive type
        if(depth == 0){
            strcat(buffer, "{ ");
        }
        for(uint32_t i = 0; i < depth; i++){
            strcat(buffer, "  ");
        }
        
        if(parse_value(curr->tag, curr->length, curr->content, parsedTag, parsedValue)) return 1;
        sprintf(temp, "\"%s\" : %s", parsedTag, parsedValue);
        strcat(buffer, temp);
        if(depth == 0){
            strcat(buffer, " }");
        }

        fputs(buffer, file);
        buffer[0] = '\0';
        temp[0] = '\0';
        parsedValue[0] = '\0';
        parsedTag[0] = '\0';
    }

    return 0;
}


uint8_t build_json(ParseTree *parseTree, FILE *file){
    return do_build_json(parseTree->root->children[0], file, 0);
}