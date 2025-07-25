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
#define MAX_ITEMS 16

struct PiptItem {
    char connections[MAX_CONNECTIONS][MAX_ITEM_DATA_LENGTH];
    char title[MAX_ITEM_DATA_LENGTH];
    char body[MAX_ITEM_DATA_LENGTH];
};
struct PiptItem piptItems[MAX_ITEMS];

int main(int argc, char* argv[]) {

    if (argc == 2) {

        FILE *filePointer;
        filePointer = fopen(argv[1], "r");
        char rawFileData[MAX_ITEM_DATA_LENGTH];

        if(filePointer != NULL) {

            int currentItem = -1;
            int currentItemConnection = 0;
            int currentItemBodyPosition = 0;

            while (fgets(rawFileData, MAX_ITEM_DATA_LENGTH, filePointer)) {

                if (rawFileData[0] == '#') {

                    currentItem++;

                    for (int i = 0; i < MAX_ITEM_DATA_LENGTH; i++) {
                        if (rawFileData[i] == '\n') {
                            break;
                        }
                        piptItems[currentItem].title[i] = rawFileData[i];
                    }

                    currentItemConnection = 0;
                    currentItemBodyPosition = 0;

                } else if (rawFileData[0] == '-') {

                    for (int i = 0; i < MAX_ITEM_DATA_LENGTH; i++) {
                        if (rawFileData[i] == '\n') {
                            break;
                        }
                        piptItems[currentItem].connections[currentItemConnection][i] = rawFileData[i];
                    }

                    currentItemConnection++;

                } else if (rawFileData[0] == '\n') {

                } else {

                    for (int i = 0; i < MAX_ITEM_DATA_LENGTH; i++) {
                        if (rawFileData[i] == '\n') {
                            break;
                        }
                        piptItems[currentItem].body[currentItemBodyPosition] = rawFileData[i];
                        currentItemBodyPosition++;
                    }

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
