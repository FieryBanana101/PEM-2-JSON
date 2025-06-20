
#include <stdio.h>
#include <string.h>

#include <pem-parser.h>
#include <der-parser.h>
#include <build-json.h>


int main(int argc, char *argv[]){
    if(argc < 3 || argc > 4){
        fprintf(stderr, "[ERROR] Invalid usage.\n\nUsage:\n   ./pem2json --tree <in> <out>\nUsage:\n   ./pem2json --json <in> <out>\n<in> : input PEM file\n<out> : optional output file\n\nConvert PEM formatted file into JSON or its tree representation\nUse '--tree' to visualize the parse tree or '--json' to visualize JSON structure\n\n");
        return 1;
    }

    uint8_t visualJson;
    if(strcmp(argv[1], "--json") == 0){
        visualJson = 1;
    }
    else if(strcmp(argv[1], "--tree") == 0){
        visualJson = 0;
    }
    else{
        fprintf(stderr, "[ERROR] Invalid usage.\n\nUsage:\n   ./pem2json --tree <in> <out>\nUsage:\n   ./pem2json --json <in> <out>\n<in> : input PEM file\n<out> : optional output file\n\nConvert PEM formatted file into JSON or its tree representation\nUse '--tree' to visualize the parse tree or '--json' to visualize JSON structure\n\n");
        return 1;
    }

    base64_value_init();

    FILE *in = fopen(argv[2], "r");
    if(in == NULL){
        fprintf(stderr, "[ERROR] File %s does not exist.\n", argv[2]);
        return 1;
    }

    ParseTree parseTree;

    if(init_parse_tree(&parseTree) || parse_pem(in, &parseTree)){
        fprintf(stderr, "[ERROR] Failed to parse %s.\n", argv[2]);
    }
    else{
        FILE *out = (argc > 3 ? fopen(argv[3], "w") : stdout);
        if(out == NULL){
            fprintf(stderr, "[ERROR] Unable to create output file '%s' for PEM file '%s'.\n", argv[3], argv[2]);
            return 1;
        }

        fprintf(out, "\n");
        if(visualJson){
            build_json(&parseTree, out);
            
        }
        else{
            visualize_parse_tree(&parseTree, out);
        }
        fprintf(out, "\n");

        free_parse_tree(&parseTree);
        printf("\nParsed: %s\n\n", argv[2]);
    }
    
    return 0;
}