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

#define MAX_ITEM_DATA_LENGTH 255
#define MAX_CONNECTIONS 4
#define MAX_ITEMS 16

#define TITLE_MARKER '#'
#define BODY_MARKER '-'
#define CONNECTION_MARKER '>'

struct PiptItem {
    char connections[MAX_CONNECTIONS][MAX_ITEM_DATA_LENGTH];
    char title[MAX_ITEM_DATA_LENGTH];
    char body[MAX_ITEM_DATA_LENGTH];
};
struct PiptItem piptItems[MAX_ITEMS];

void LoadFile(char** path) {

        FILE *filePointer;
        filePointer = fopen(*path, "r");
        char rawFileData[MAX_ITEM_DATA_LENGTH];

        if(filePointer != NULL) {

            int currentItem = -1;
            int currentItemConnection = 0;
            int currentItemBodyPosition = 0;
            int currentLineNumber = 0;

            while (fgets(rawFileData, MAX_ITEM_DATA_LENGTH, filePointer)) {

                currentLineNumber++;

                if (rawFileData[0] == TITLE_MARKER) {

                    currentItem++;

                    for (int i = 0; i < MAX_ITEM_DATA_LENGTH; i++) {
                        if (rawFileData[i] == '\n') {
                            break;
                        }
                        piptItems[currentItem].title[i] = rawFileData[i];
                    }

                    currentItemConnection = 0;
                    currentItemBodyPosition = 0;

                } else if (rawFileData[0] == CONNECTION_MARKER) {

                    for (int i = 0; i < MAX_ITEM_DATA_LENGTH; i++) {
                        if (rawFileData[i] == '\n') {
                            break;
                        }
                        piptItems[currentItem].connections[currentItemConnection][i] = rawFileData[i];
                    }

                    currentItemConnection++;

                } else if (rawFileData[0] == BODY_MARKER) {

                    for (int i = 0; i < MAX_ITEM_DATA_LENGTH; i++) {
                        if (rawFileData[i] == '\n') {
                            break;
                        }
                        piptItems[currentItem].body[currentItemBodyPosition] = rawFileData[i];
                        currentItemBodyPosition++;
                    }

                } else if (rawFileData[0] == '\n') {

                } else {

                    printf("data on line %d could not be prosessed, starts with %c\n", currentLineNumber, rawFileData[0]);

                }

            }

            fclose(filePointer);

        } else {

            printf("file %s not found\n.", *path);

        }
}

int main(int argc, char* argv[]) {

    if (argc == 2) {

        LoadFile(&argv[1]);

    } else if (argc > 2) {

        puts("too many arguments");

    } else if (argc < 2) {

        puts("no file given");

    }

    for (int i = 0; i < MAX_ITEMS; i++) {

        if (piptItems[i].title[0] == '\0') {
            break;
        }

        printf("%s\n", piptItems[i].title);
        printf("%s\n", piptItems[i].body);

        for (int j = 0; j < MAX_CONNECTIONS; j++) {

            if (piptItems[i].connections[j][0] == '\0') {
                break;
            }

            printf("%s\n", piptItems[i].connections[j]);
        }
    }

    return 0;

}
