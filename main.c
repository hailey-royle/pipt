#include <stdio.h>
#include <string.h>

#define MAX_ITEM_WIDTH 32
#define MAX_ITEM_HEIGHT 8
#define MAX_ITEM_DATA MAX_ITEM_WIDTH
#define MAX_ITEM_COUNT 16
#define TITLE_MARK '#'
#define BODY_MARK '>'
#define CONNECTION_MARK '-'
#define CORRNER_CHAR '+'
#define HORIZONTAL_CHAR '-'
#define VERTICAL_CHAR '|'
#define SPACE_CHAR ' '

struct piptItem {
    char connection[MAX_ITEM_COUNT][MAX_ITEM_DATA];
    char title[MAX_ITEM_DATA];
    char top[MAX_ITEM_DATA];
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

    int itemCount = -1;

    while(fgets(rawData, MAX_ITEM_DATA, file)) {
        static int lineNumber = 0;
        static int bodyNumber = 0;
        static int connectionNumber = 0;
        lineNumber++;
        if (strlen(rawData) >= MAX_ITEM_DATA - 1) {
            printf("data overflow on line %d\n", lineNumber);
            return -1;
        }
        if (rawData[0] == '\n') {
            continue;
        }
        if (rawData[0] == TITLE_MARK) {
            itemCount++;
            bodyNumber = 0;
            connectionNumber = 0;
            strcpy(piptItem[itemCount].title, rawData);
            piptItem[itemCount].width = strlen(rawData);
            piptItem[itemCount].height++;
        }
        else if (rawData[0] == BODY_MARK) {
            strcpy(piptItem[itemCount].body[bodyNumber], rawData);
            int lineWidth = strlen(rawData);
            if (piptItem[itemCount].width < lineWidth) {
                piptItem[itemCount].width = lineWidth;
            }
            piptItem[itemCount].height++;
            bodyNumber++;
        }
        else if (rawData[0] == CONNECTION_MARK) {
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

void FormatTop(const int itemNumber) {
    strcpy(piptItem[itemNumber].top, piptItem[itemNumber].title);
    piptItem[itemNumber].top[0] = CORRNER_CHAR;
    piptItem[itemNumber].top[1] = HORIZONTAL_CHAR;
    piptItem[itemNumber].top[strlen(piptItem[itemNumber].top) - 1] = HORIZONTAL_CHAR;
    for (int i = strlen(piptItem[itemNumber].top); i <= piptItem[itemNumber].width - 1; i++) {
        piptItem[itemNumber].top[i] = HORIZONTAL_CHAR;
    }
    piptItem[itemNumber].top[piptItem[itemNumber].width] = CORRNER_CHAR;
}

void FormatBody(const int itemNumber) {
    for (int i = 0; i <= piptItem[itemNumber].height - 2; i++) {
        piptItem[itemNumber].body[i][0] = VERTICAL_CHAR;
        piptItem[itemNumber].body[i][1] = SPACE_CHAR;
        for (int j = strlen(piptItem[itemNumber].body[i]) - 1; j <= piptItem[itemNumber].width - 1; j++) {
            piptItem[itemNumber].body[i][j] = SPACE_CHAR;
        }
        piptItem[itemNumber].body[i][piptItem[itemNumber].width] = VERTICAL_CHAR;
    }
}

void FormatBottom(const int itemNumber) {
    for (int i = 1; i <= piptItem[itemNumber].width - 1; i++) {
        piptItem[itemNumber].bottom[i] = HORIZONTAL_CHAR;
    }
    piptItem[itemNumber].bottom[0] = CORRNER_CHAR;
    piptItem[itemNumber].bottom[piptItem[itemNumber].width] = CORRNER_CHAR;
}

void FormatItem(const int itemNumber) {
    FormatTop(itemNumber);
    FormatBody(itemNumber);
    FormatBottom(itemNumber);
}

void DrawItem(const int itemNumber) {
    printf("%s\n", piptItem[itemNumber].top);
    for (int i = 0; i <= piptItem[itemNumber].height - 2; i++) {
        printf("%s\n", piptItem[itemNumber].body[i]);
    }
    printf("%s\n", piptItem[itemNumber].bottom);
}

int FindCanvasHeight(const int itemCount) {
    int canvasHeight = 1;
    for (int i = 0; i <= itemCount; i++) {
        canvasHeight += piptItem[i].height;
        canvasHeight++;
    }
    return canvasHeight;
}

int FindCanvasWidth(const int itemCount) {
    int canvasWidth = piptItem[0].width;
    if (piptItem[1].width > piptItem[0].width) {
        canvasWidth = piptItem[1].width;
    }
    for (int i = 2; i <= itemCount; i++) {
        canvasWidth += piptItem[i].width;
        canvasWidth++;
    }
    canvasWidth++;
    return canvasWidth;
}

int main(int argc, char* argv[]) {
    const int argvPath = VerifyArgs(argc);

    if (argvPath != 1) {
        printf("argv failure\n");
        return 0;
    }

    const int itemCount = LoadFileData(argv[argvPath]);

    if (itemCount < 1) {
        printf("file %s does not have any data\n", argv[argvPath]);
        return 0;
    }

    for (int i = 0; i < itemCount; i++) {
        FormatItem(i);
        DrawItem(i);
    }

    int canvasHeight = FindCanvasHeight(itemCount);
    int canvasWidth = FindCanvasWidth(itemCount);

/*    char canvas[canvasHeight][canvasWidth + 1];
    for (int i = 0; i <= canvasHeight - 1; i++) {
        canvas[i][canvasWidth] = '\0';
        for (int j = 0; j <= canvasWidth - 1; j++) {
            canvas[i][j] = '_';
        }
    }*/

    printf("%d\n", canvasHeight);
    printf("%d\n", canvasWidth);

    return 0;
}
