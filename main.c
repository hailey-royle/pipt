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

#define MAX_ITEM_DATA_LENGTH 64
#define MAX_CONNECTIONS 4
#define MAX_ITEM_LIST 16

struct PiptItem {
    char connections[MAX_CONNECTIONS][MAX_ITEM_DATA_LENGTH];
    char title[MAX_ITEM_DATA_LENGTH];
    char body[MAX_ITEM_DATA_LENGTH];
};
struct PiptItem piptItems[MAX_ITEM_LIST];

int main(int argc, char* argv[]) {

    if (argc == 2) {

        FILE *filePointer;
        char rawFileData[MAX_ITEM_DATA_LENGTH];
        filePointer = fopen(argv[1], "r");

        if(filePointer != NULL) {

            while (fgets(rawFileData, MAX_ITEM_DATA_LENGTH, filePointer)) {

                if (rawFileData[0] == '#') {
                    //title
                } else if (rawFileData[0] == '-') {
                    //connection
                } else if (rawFileData[0] == '\n') {
                    //nothing
                } else {
                    //body
                }

                printf("%s", rawFileData);

            }

        fclose(filePointer);

        } else {

            printf("file %s not found\n.", argv[1]);

        }

    } else if (argc > 2) {

        puts("too many arguments");

    } else if (argc < 2) {

        puts("no file given");

    }

    return 0;

}
