/*
 * program will take in file
 * then will generate and print a graph
 * a file will contain items
 * items will contain title, body, and connections
 * title is the title
 * body is the content/description
 * connections graphicly link items together
 */

#include <stdio.h>

int main(int argc, char* argv[]) {

    if (argc < 2) {

        puts("no file given");

    } else if (argc > 2) {

        puts("too many arguments");

    } else {

        printf("input argument:%s\n", argv[1]);

    }

    return 0;

}

    
