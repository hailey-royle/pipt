#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITEM_DATA_LENGTH 32
#define MAX_ITEM_COUNT 16
#define MAX_ITEM_BODY_LINES 8
#define TITLE_MARK '#'
#define BODY_MARK '>'
#define CONNECTION_MARK '-'
#define CORRNER_CHAR '+'
#define HORIZONTAL_CHAR '-'
#define VERTICAL_CHAR '|'
#define SPACE_CHAR ' '
#define BACKGROUND_CHAR '.'
#define CANVAS_GAP 1
#define ITEM_GAP 1

struct item {
    char connection[MAX_ITEM_COUNT][MAX_ITEM_DATA_LENGTH];
    char body[MAX_ITEM_BODY_LINES][MAX_ITEM_DATA_LENGTH];
    char title[MAX_ITEM_DATA_LENGTH];
    char top[MAX_ITEM_DATA_LENGTH];
    char bottom[MAX_ITEM_DATA_LENGTH];
    int connected[MAX_ITEM_COUNT];
    int itemW;
    int itemH;
    int totalW;
    int totalH;
    int x;
    int y;
    int connectionCount;
    int bodyLineCount;
};

struct pipt {
    struct item item[MAX_ITEM_COUNT];
    int itemCount;
};
struct pipt pipt;

struct stack {
    int top;
    int capacity;
    int item[MAX_ITEM_COUNT];
};
struct stack stackY;
struct stack stackX;

//==============================================================
//  Stack
//==============================================================

void StackPush(struct stack* stack, int item){
    if ((stack->capacity - stack->top) == 0) {
        printf("stack overflow\n");
        exit(1);
    }
    stack->top++;
    stack->item[stack->top] = item;
}

void StackPop(struct stack* stack) {
    if (stack->top < 0) {
        printf("stack underflow\n");
        exit(1);
    }
    stack->top--;
}

//==============================================================
//  Input
//==============================================================

void VerifyArgs(const int argc) {
    if (argc < 2) {
        printf("no arguments given\n");
        exit(1);
    }
    if (argc > 2) {
        printf("too many arguments\n");
        exit(1);
    }
}

void LoadFileData(const char* arg) {
    FILE *file;
    file = fopen(arg, "r");
    char rawData[MAX_ITEM_DATA_LENGTH];

    if (file == NULL) {
        printf("file %s not found\n", arg);
        exit(1);
    }

    pipt.itemCount = -1;

    while(fgets(rawData, MAX_ITEM_DATA_LENGTH, file)) {
        if (strlen(rawData) >= MAX_ITEM_DATA_LENGTH - 1) {
            printf("data overflow on line \"%s\"\n", rawData);
            return;
        }
        if (pipt.item[pipt.itemCount].bodyLineCount >= MAX_ITEM_BODY_LINES) {
            printf("item %d has to many lines in its body, max is %d", pipt.itemCount, MAX_ITEM_BODY_LINES);
        }
        if (rawData[0] == '\n') {
            continue;
        }
        if (rawData[0] == TITLE_MARK) {
            pipt.itemCount++;
            strcpy(pipt.item[pipt.itemCount].title, rawData);
            pipt.item[pipt.itemCount].itemW = strlen(pipt.item[pipt.itemCount].title);
        }
        else if (rawData[0] == BODY_MARK) {
            strcpy(pipt.item[pipt.itemCount].body[pipt.item[pipt.itemCount].bodyLineCount], rawData);
            if (pipt.item[pipt.itemCount].itemW < strlen(pipt.item[pipt.itemCount].body[pipt.item[pipt.itemCount].bodyLineCount])) {
                pipt.item[pipt.itemCount].itemW = strlen(pipt.item[pipt.itemCount].body[pipt.item[pipt.itemCount].bodyLineCount]);
            }
            pipt.item[pipt.itemCount].bodyLineCount++;
        }
        else if (rawData[0] == CONNECTION_MARK) {
            strcpy(pipt.item[pipt.itemCount].connection[pipt.item[pipt.itemCount].connectionCount], rawData);
            pipt.item[pipt.itemCount].connectionCount++;
        }
        else {
            printf("data on line \"%s\" could not be prosessed\n", rawData);
            printf("line must start line with %c : title, %c : body, or %c : connection\n", TITLE_MARK, BODY_MARK, CONNECTION_MARK);
            printf("line data : %s\n", rawData);
            return;
        }
    }
    fclose(file);

    if (pipt.itemCount < 0) {
        printf("error loading file data, item count:%d\n", pipt.itemCount);
        exit(1);
    }
}

//==============================================================
//  ConnectItems
//==============================================================

void FindConnection(const int itemNumber, const int connectionNumber) {
    pipt.item[itemNumber].connection[connectionNumber][0] = TITLE_MARK;
    for (int i = 0; i <= pipt.itemCount; i++) {
        if (pipt.item[i].title[0] == '\0') {
            return;
        }
        if (strcmp(pipt.item[itemNumber].connection[connectionNumber], pipt.item[i].title) == 0) {
            pipt.item[itemNumber].connected[connectionNumber] = i;
        }
    }
}

void ConnectItems() {
    for (int i = 0; i <= pipt.itemCount; i++) {
        for (int j = 0; j <= pipt.itemCount; j++) {
            FindConnection(i, j);
        }
    }
}

//==============================================================
//  FormatItems
//==============================================================

void FormatTop(const int itemNumber) {
    strcpy(pipt.item[itemNumber].top, pipt.item[itemNumber].title);
    pipt.item[itemNumber].top[0] = CORRNER_CHAR;
    pipt.item[itemNumber].top[1] = HORIZONTAL_CHAR;
    for (int i = strlen(pipt.item[itemNumber].top) - 1; i <= pipt.item[itemNumber].itemW - 2; i++) {
        pipt.item[itemNumber].top[i] = HORIZONTAL_CHAR;
    }
    pipt.item[itemNumber].top[pipt.item[itemNumber].itemW - 1] = CORRNER_CHAR;
}

void FormatBody(const int itemNumber) {
    for (int i = 0; i <= pipt.item[itemNumber].itemH - 2; i++) {
        pipt.item[itemNumber].body[i][0] = VERTICAL_CHAR;
        pipt.item[itemNumber].body[i][1] = SPACE_CHAR;
        for (int j = strlen(pipt.item[itemNumber].body[i]) - 1; j <= pipt.item[itemNumber].itemW - 2; j++) {
            pipt.item[itemNumber].body[i][j] = SPACE_CHAR;
        }
        pipt.item[itemNumber].body[i][pipt.item[itemNumber].itemW - 1] = VERTICAL_CHAR;
    }
}

void FormatBottom(const int itemNumber) {
    pipt.item[itemNumber].bottom[0] = CORRNER_CHAR;
    for (int i = 1; i <= pipt.item[itemNumber].itemW - 2; i++) {
        pipt.item[itemNumber].bottom[i] = HORIZONTAL_CHAR;
    }
    pipt.item[itemNumber].bottom[pipt.item[itemNumber].itemW - 1] = CORRNER_CHAR;
}

void FormatItems() {
    for (int i = 0; i <= pipt.itemCount; i++) {
        pipt.item[i].itemH = pipt.item[i].bodyLineCount + 2;
        pipt.item[i].itemW += 1;
        FormatTop(i);
        FormatBody(i);
        FormatBottom(i);
    }
}

//==============================================================
//  SizeItems
//==============================================================

void SizeItem(const int itemNumber) {
    pipt.item[itemNumber].totalW = 1;
    for (int i = 0; i < pipt.item[itemNumber].connectionCount; i++) {
        if (pipt.item[pipt.item[itemNumber].connected[i]].totalW == 0) {
            SizeItem(pipt.item[itemNumber].connected[i]);
        }
    }
    for (int i = 0; i < pipt.item[itemNumber].connectionCount; i++) {
        pipt.item[itemNumber].totalW += pipt.item[pipt.item[itemNumber].connected[i]].totalW;
    }
    pipt.item[itemNumber].totalW = pipt.item[itemNumber].itemW + (ITEM_GAP * 2);
    for (int i = 0; i < pipt.item[itemNumber].connectionCount; i++) {
        if (pipt.item[itemNumber].totalH < pipt.item[pipt.item[itemNumber].connected[i]].totalH) {
            pipt.item[itemNumber].totalH = pipt.item[pipt.item[itemNumber].connected[i]].totalH;
        }
    }
    pipt.item[itemNumber].totalH += pipt.item[itemNumber].itemH + (ITEM_GAP * 2);

}

//==============================================================
//  PossitionItems
//==============================================================

void PossitionItem(const int itemNumber, const int totalY) {
    pipt.item[itemNumber].y = totalY + ITEM_GAP;
    pipt.item[itemNumber].x = (pipt.item[itemNumber].totalW / 2) - (pipt.item[itemNumber].itemW / 2);
    for (int i = 0; i < pipt.item[itemNumber].connectionCount; i++) {
        if (pipt.item[pipt.item[itemNumber].connected[i]].x == 0) {
            PossitionItem(pipt.item[itemNumber].connected[i], totalY + pipt.item[itemNumber].totalH);
        }
    }
    printf("x: %d, y: %d, iw: %d, ih: %d, tw: %d, th: %d, item: %s", pipt.item[itemNumber].x, pipt.item[itemNumber].y, pipt.item[itemNumber].itemW, pipt.item[itemNumber].itemH, pipt.item[itemNumber].totalW, pipt.item[itemNumber].totalH, pipt.item[itemNumber].title);
}

//==============================================================
//  main
//==============================================================

int main(int argc, char* argv[]) {
    VerifyArgs(argc);

    LoadFileData(argv[1]);

    ConnectItems();

    FormatItems();

    SizeItem(0);

    PossitionItem(0, ITEM_GAP);

    return 0;
}
