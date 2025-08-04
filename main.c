#include <stdio.h>
#include <string.h>

#define MAX_ITEM_WIDTH 32
#define MAX_ITEM_HEIGHT 8
#define MAX_ITEM_DATA MAX_ITEM_WIDTH
#define MAX_ITEM_COUNT 16
#define TITLE_MARK '#'
#define BODY_MARK '>'
#define CONNECTION_MARK '-'
#define MARK_LENGTH 2
#define CORRNER_CHAR '+'
#define HORIZONTAL_CHAR '-'
#define VERTICAL_CHAR '|'
#define SPACE_CHAR ' '
#define HORIZONTAL_BUFFER 2

struct piptItem {
    char connection[MAX_ITEM_COUNT][MAX_ITEM_DATA];
    char title[MAX_ITEM_DATA];
    char body[MAX_ITEM_COUNT][MAX_ITEM_DATA];
    char bottom[MAX_ITEM_DATA];
    int width;
    int height;
};
struct piptItem piptItem[MAX_ITEM_COUNT];

//----------------------------------------------------------------
// Content functions, do a specific job directly related to program functioning
//----------------------------------------------------------------

int VerifyArgs(const int argc) {
    if (argc < 2) {
        printf("no arguments given\n");
        return -1;
    }
    else if (argc > 2) {
        printf("too many arguments\n");
        return -1;
    }
    return 1;
}

int LoadFileData(const char* arg) {
    FILE *file;
    file = fopen(arg, "r");
    char rawData[MAX_ITEM_DATA];

    if (file == NULL) {
        printf("file %s not found\n", arg);
        return -1;
    }

    int lineNumber = 0;
    int itemCount = -1;
    int bodyNumber = 0;
    int connectionNumber = 0;

    while(fgets(rawData, MAX_ITEM_DATA, file)) {
        lineNumber++;
        if (strlen(rawData) >= MAX_ITEM_DATA - 1) {
            printf("data overflow on line %d\n", lineNumber);
            return -1;
        }
        if (rawData[0] == '\n') {
            continue;
        }
        else if (rawData[0] == TITLE_MARK) {
            itemCount++;
            bodyNumber = 0;
            connectionNumber = 0;
            rawData[0] = CORRNER_CHAR;
            rawData[1] = HORIZONTAL_CHAR;
            strcpy(piptItem[itemCount].title, rawData);
            piptItem[itemCount].width = strlen(rawData);
            piptItem[itemCount].height++;
        }
        else if (rawData[0] == BODY_MARK) {
            rawData[0] = VERTICAL_CHAR;
            rawData[1] = SPACE_CHAR;
            strcat(piptItem[itemCount].body[bodyNumber], rawData);
            piptItem[itemCount].height++;
            bodyNumber++;
        }
        else if (rawData[0] == CONNECTION_MARK) {
            rawData[0] = CORRNER_CHAR;
            rawData[1] = HORIZONTAL_CHAR;
            strcpy(piptItem[itemCount].connection[connectionNumber], rawData);
            connectionNumber++;
        }
        else {
            printf("data on line %d could not be prosessed\n", lineNumber);
            printf("line must start line with %c : title, %c : body, or %c : connection\n", TITLE_MARK, BODY_MARK, CONNECTION_MARK);
            printf("line data : %s\n", rawData);
            return -1;
        }
    }

    fclose(file);
    return itemCount + 1;
}

void FormatItem(int itemNumber) {
    piptItem[itemNumber].title[strlen(piptItem[itemNumber].title) - 1] = HORIZONTAL_CHAR;
    piptItem[itemNumber].title[piptItem[itemNumber].width] = CORRNER_CHAR;
}

void DrawItem(int itemNumber) {
    printf("%d\n", piptItem[itemNumber].width);
    printf("%d\n", piptItem[itemNumber].height);
    printf("%s\n", piptItem[itemNumber].title);
    for (int i = 0; i < piptItem[itemNumber].height; i++) {
        printf("%s\n", piptItem[itemNumber].body[i]);
    }

    printf("\n");
}

int main(int argc, char* argv[]) {
    const int argvPath = VerifyArgs(argc);

    if (argvPath < 0) {
        return 0;
    }

    const int itemCount = LoadFileData(argv[argvPath]);

    for (int i = 0; i < itemCount; i++) {
        FormatItem(i);
        DrawItem(i);
    }

    return 0;
}
