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

    if (argc == 1) {

        puts("no file given");

    } else if (argc == 2) {

        printf("input argument:%s\n", argv[1]);

    } else if (argc >= 3) {

        puts("too many arguments");

    }

    return 0;

}

    
