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
#define BACKGROUND_CHAR '.'

struct piptItem {
    char connection[MAX_ITEM_COUNT][MAX_ITEM_DATA];
    char title[MAX_ITEM_DATA];
    char top[MAX_ITEM_DATA];
    char body[MAX_ITEM_COUNT][MAX_ITEM_DATA];
    char bottom[MAX_ITEM_DATA];
    int width;
    int height;
    int x;
    int y;
};
struct piptItem piptItem[MAX_ITEM_COUNT];

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
            piptItem[itemCount].width = strlen(rawData) + 1;
            piptItem[itemCount].height++;
        }
        else if (rawData[0] == BODY_MARK) {
            strcpy(piptItem[itemCount].body[bodyNumber], rawData);
            int lineWidth = strlen(rawData) + 1;
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
    for (int i = strlen(piptItem[itemNumber].top) - 1; i <= piptItem[itemNumber].width - 2; i++) {
        piptItem[itemNumber].top[i] = HORIZONTAL_CHAR;
    }
    piptItem[itemNumber].top[piptItem[itemNumber].width - 1] = CORRNER_CHAR;
}

void FormatBody(const int itemNumber) {
    for (int i = 0; i <= piptItem[itemNumber].height - 2; i++) {
        piptItem[itemNumber].body[i][0] = VERTICAL_CHAR;
        piptItem[itemNumber].body[i][1] = SPACE_CHAR;
        for (int j = strlen(piptItem[itemNumber].body[i]) - 1; j <= piptItem[itemNumber].width - 2; j++) {
            piptItem[itemNumber].body[i][j] = SPACE_CHAR;
        }
        piptItem[itemNumber].body[i][piptItem[itemNumber].width - 1] = VERTICAL_CHAR;
    }
}

void FormatBottom(const int itemNumber) {
    piptItem[itemNumber].height++;
    piptItem[itemNumber].bottom[0] = CORRNER_CHAR;
    for (int i = 1; i <= piptItem[itemNumber].width - 2; i++) {
        piptItem[itemNumber].bottom[i] = HORIZONTAL_CHAR;
    }
    piptItem[itemNumber].bottom[piptItem[itemNumber].width - 1] = CORRNER_CHAR;
}

void FormatItem(const int itemNumber) {
    FormatTop(itemNumber);
    FormatBody(itemNumber);
    FormatBottom(itemNumber);
}

int FindCanvasHeight(const int itemCount) {
    int canvasHeight = 1;
    for (int i = 0; i <= itemCount - 1; i++) {
        canvasHeight += piptItem[i].height;
        canvasHeight++;
    }
    return canvasHeight;
}

int FindCanvasWidth(const int itemCount) {
    int canvasWidth = 1;
    canvasWidth += piptItem[0].width;
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

void InitCanvas(char* canvas, const int canvasHeight, const int canvasWidth) {
    for (int i = 0; i < canvasHeight * canvasWidth; i++) {
        canvas[i] = BACKGROUND_CHAR;
    }
    for (int i = canvasWidth - 1; i < canvasHeight * canvasWidth; i += canvasWidth) {
        canvas[i] = '\n';
    }
    canvas[(canvasHeight * canvasWidth) - 1] = '\0';
}

void DrawItem(char* canvas, int itemNumber, int canvasWidth) {
    for (int i = 0; i < piptItem[itemNumber].height; i++) {
        if (i == 0) {
            for (int j = 0; j <= piptItem[itemNumber].width - 1; j++) {
                canvas[((piptItem[itemNumber].y * canvasWidth) + piptItem[itemNumber].x) + j] = piptItem[itemNumber].top[j];
            }
        }
        else if (i == piptItem[itemNumber].height - 1) {
            for (int j = 0; j <= piptItem[itemNumber].width - 1; j++) {
                canvas[(((piptItem[itemNumber].y + i) * canvasWidth) + piptItem[itemNumber].x) + j] = piptItem[itemNumber].bottom[j];
            }
        }
        else {
            for (int j = 0; j <= piptItem[itemNumber].width - 1; j++) {
                canvas[(((piptItem[itemNumber].y + i) * canvasWidth) + piptItem[itemNumber].x) + j] = piptItem[itemNumber].body[i - 1][j];
            }
        }
    }
}

int FindConnection(int itemNumber, int connectionNumber) {
    piptItem[itemNumber].connection[connectionNumber][0] = TITLE_MARK;
    for (int i = 0; i < MAX_ITEM_COUNT; i++) {
        if (piptItem[i].title[0] == '\0') {
            return -1;
        }
        if (strcmp(piptItem[itemNumber].connection[connectionNumber], piptItem[i].title) == 0) {
            return i;
        }
    }
    return -1;
}

int PushItem(int itemNumber, int yPush, int xPush) {
    piptItem[itemNumber].y += yPush;
    piptItem[itemNumber].x += xPush;
    return piptItem[itemNumber].width;
}

void PlaceItem(char* canvas, int itemNumber, int canvasWidth) {
    DrawItem(canvas, itemNumber, canvasWidth);

    for (int i = 0; i < MAX_ITEM_COUNT; i++) {
        if (piptItem[itemNumber].connection[i][0] == '\0') {
            break;
        }

        int connectionItem = FindConnection(itemNumber, i);
        if (connectionItem == -1) {
            printf("connection not found for %s", piptItem[itemNumber].connection[i]);
        }

        static int xOffset = 0;
        xOffset = PushItem(connectionItem, piptItem[itemNumber].height + 2, xOffset + 1);
        xOffset++;

        PlaceItem(canvas, connectionItem, canvasWidth);
    }
}

int main(int argc, char* argv[]) {
    const int argvPath = VerifyArgs(argc);

    if (argvPath != 1) {
        return 0;
    }

    const int itemCount = LoadFileData(argv[argvPath]);

    if (itemCount < 1) {
        return 0;
    }

    for (int i = 0; i < itemCount; i++) {
        FormatItem(i);
    }

    int canvasHeight = FindCanvasHeight(itemCount);
    int canvasWidth = FindCanvasWidth(itemCount);
    canvasWidth++;
    char canvas[canvasHeight * canvasWidth];
    InitCanvas(canvas, canvasHeight, canvasWidth);

    PushItem(0, 1, 1);
    PlaceItem(canvas, 0, canvasWidth);

    printf("%s\n", canvas);

    return 0;
}
