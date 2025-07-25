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

const int MAX_DATA_LENGTH = 255;

int main(int argc, char* argv[]) {

    if (argc < 2) {

        puts("no file given");

    } else if (argc > 2) {

        puts("too many arguments");

    } else {

        FILE *filePointer;
        char rawFileData[MAX_DATA_LENGTH];
        filePointer = fopen(argv[1], "r");

        if(filePointer != NULL) {

            while(fgets(rawFileData, MAX_DATA_LENGTH, filePointer)) {
                printf("%s", rawFileData);
            }

        fclose(filePointer);

        } else {

            printf("file %s not found\n.", argv[1]);

        }

    }

    return 0;

}
