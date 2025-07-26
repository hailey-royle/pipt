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
#include <stdbool.h>

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

int LoadFile(char** path) {

    FILE *filePointer;
    filePointer = fopen(*path, "r");
    char rawFileData[MAX_ITEM_DATA_LENGTH];
    int currentItem = -1;
    int currentItemConnection = 0;
    int currentItemBodyPosition = 0;
    int currentLineNumber = 0;

    if (filePointer != NULL) {

        while (fgets(rawFileData, MAX_ITEM_DATA_LENGTH, filePointer)) {
            currentLineNumber++;

            if (rawFileData[0] == TITLE_MARKER) {
                currentItem++;

                for (int i = 0; i < MAX_ITEM_DATA_LENGTH; i++) {

                    if (rawFileData[i+2] == '\n') {
                        break;
                    }
                    piptItems[currentItem].title[i] = rawFileData[i+2];
                }
                currentItemConnection = 0;
                currentItemBodyPosition = 0;

            } else if (rawFileData[0] == CONNECTION_MARKER) {

                for (int i = 0; i < MAX_ITEM_DATA_LENGTH; i++) {

                    if (rawFileData[i+2] == '\n') {
                        break;
                    }
                    piptItems[currentItem].connections[currentItemConnection][i] = rawFileData[i+2];
                }
                currentItemConnection++;

            } else if (rawFileData[0] == BODY_MARKER) {

                for (int i = 0; i < MAX_ITEM_DATA_LENGTH; i++) {

                    if (rawFileData[i+2] == '\n') {
                        break;
                    }
                    piptItems[currentItem].body[currentItemBodyPosition] = rawFileData[i+2];
                    currentItemBodyPosition++;
                }

            } else if (rawFileData[0] == '\n') {

            } else {
                printf("data on line %d could not be prosessed, starts with %c\n", currentLineNumber, rawFileData[0]);
            }
        }
    fclose(filePointer);
    return currentItem + 1;

    } else {
        printf("file %s not found\n.", *path);
        return 0;
    }
}

bool streq(char str1[], char str2[]) {

    int i = 0;

    while (str1[i] == str2[i]) {

        if (str1[i] == '\0' || str2[i] == '\0') {
            break;
        }
        i++;
    }

    if ( str1[i] == '\0' && str2[i] == '\0') {
        return 1;

    } else {
        return 0;
    }
}

int main(int argc, char* argv[]) {

    int piptItemCount;

    if (argc == 2) {
        piptItemCount = LoadFile(&argv[1]);

    } else if (argc > 2) {
        puts("too many arguments");

    } else if (argc < 2) {
        puts("no file given");
    }

    bool piptItemMatrix[piptItemCount][piptItemCount];

    for (int i = 0; i < piptItemCount; i++) {

        for (int j = 0; j < piptItemCount; j++) {
            piptItemMatrix[i][j] = false;
        }
    }

    for (int i = 0; i < piptItemCount; i++) {

        for (int j = 0; j < piptItemCount; j++) {

            for (int h = 0; h < piptItemCount; h++) {

                if (streq(piptItems[i].connections[j], piptItems[h].title)) {
                    piptItemMatrix[i][h] = true;
                }
            }
        }
    }

    for (int i = 0; i < piptItemCount; i++) {

        for (int j = 0; j < piptItemCount; j++) {

            if (piptItemMatrix[i][j] == true) {
                printf("y");

            } else {
                printf("n");
            }
        }
        printf("\n");
    }


    for (int i = 0; i < piptItemCount; i++) {

        if (piptItems[i].title[0] == '\0') {
            break;
        }
        printf("#:%s\n", piptItems[i].title);
        printf("-:%s\n", piptItems[i].body);

        for (int j = 0; j < MAX_CONNECTIONS; j++) {

            if (piptItems[i].connections[j][0] == '\0') {
                break;
            }
            printf(">:%s\n", piptItems[i].connections[j]);
        }
    }
    return 0;
}
