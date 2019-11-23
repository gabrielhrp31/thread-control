//
// Created by winchester on 22/11/2019.
//

#include <stdio.h>
#include <stdlib.h>

int parse_arguments(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Arguments invalid\n");
        exit(1);
    }

    return atoi(argv[1]);
}


