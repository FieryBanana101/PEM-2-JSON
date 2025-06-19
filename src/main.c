
#include <stdio.h>

#include <pem-parser.h>
#include <der-parser.h>
#include <build-json.h>


int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "[ERROR] Invalid usage.\nUsage: ./pem2json <file1> <file2> <file3> ...\n\nConvert PEM formatted file into JSON\n");
        return 1;
    }

    base64_value_init();

    for(uint32_t i = 1; i < argc; i++){
        FILE *file = fopen(argv[i], "r");
        if(file == NULL){
            fprintf(stderr, "[ERROR] File %s does not exist.\n", argv[i]);
            continue;
        }

        ParseTree parseTree;

        if(init_parse_tree(&parseTree) || parse_pem(file, &parseTree)){
            fprintf(stderr, "[ERROR] Failed to parse %s.\n", argv[i]);
        }
        else{
            printf("\n");
            visualize_parse_tree(&parseTree);

            FILE *out = fopen("test.json", "w");
            if(out == NULL){
                fprintf(stderr, "[ERROR] Unable to create json file '%s' for PEM file '%s'.\n", "test.json", argv[i]);
                continue;
            }
            build_json(&parseTree, out);
            
            free_parse_tree(&parseTree);
            printf("\nParsed: %s\n\n", argv[i]);
        }
    }
    
    return 0;
}