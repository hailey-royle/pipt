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

struct item {
    char connection[MAX_ITEM_COUNT][MAX_ITEM_DATA_LENGTH];
    char body[MAX_ITEM_BODY_LINES][MAX_ITEM_DATA_LENGTH];
    char title[MAX_ITEM_DATA_LENGTH];
    char top[MAX_ITEM_DATA_LENGTH];
    char bottom[MAX_ITEM_DATA_LENGTH];
    int connected[MAX_ITEM_COUNT];
    int width;
    int height;
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

//==============================================================
//  Stack
//==============================================================

int StackRemaining(struct stack stack) {
    return stack.capacity - stack.top;
}

void StackPush(struct stack* stack, int item){
    if (StackRemaining(*stack) == 0) {
        printf("stack overflow\n");
        abort();
        return;
    }
    stack->top++;
    stack->item[stack->top] = item;
}

int StackPop(struct stack* stack) {
    if (stack->top < 0) {
        printf("stack underflow\n");
        abort();
        return -1;
    }
    stack->top--;
    return stack->top;
}

//==============================================================
//  Input
//==============================================================

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

void LoadFileData(const char* arg) {
    FILE *file;
    file = fopen(arg, "r");
    char rawData[MAX_ITEM_DATA_LENGTH];

    if (file == NULL) {
        printf("file %s not found\n", arg);
        return;
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
            pipt.item[pipt.itemCount].width = strlen(pipt.item[pipt.itemCount].title);
        }
        else if (rawData[0] == BODY_MARK) {
            strcpy(pipt.item[pipt.itemCount].body[pipt.item[pipt.itemCount].bodyLineCount], rawData);
            if (pipt.item[pipt.itemCount].width < strlen(pipt.item[pipt.itemCount].body[pipt.item[pipt.itemCount].bodyLineCount])) {
                pipt.item[pipt.itemCount].width = strlen(pipt.item[pipt.itemCount].body[pipt.item[pipt.itemCount].bodyLineCount]);
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
    for (int i = strlen(pipt.item[itemNumber].top) - 1; i <= pipt.item[itemNumber].width - 2; i++) {
        pipt.item[itemNumber].top[i] = HORIZONTAL_CHAR;
    }
    pipt.item[itemNumber].top[pipt.item[itemNumber].width - 1] = CORRNER_CHAR;
}

void FormatBody(const int itemNumber) {
    for (int i = 0; i <= pipt.item[itemNumber].height - 2; i++) {
        pipt.item[itemNumber].body[i][0] = VERTICAL_CHAR;
        pipt.item[itemNumber].body[i][1] = SPACE_CHAR;
        for (int j = strlen(pipt.item[itemNumber].body[i]) - 1; j <= pipt.item[itemNumber].width - 2; j++) {
            pipt.item[itemNumber].body[i][j] = SPACE_CHAR;
        }
        pipt.item[itemNumber].body[i][pipt.item[itemNumber].width - 1] = VERTICAL_CHAR;
    }
}

void FormatBottom(const int itemNumber) {
    pipt.item[itemNumber].bottom[0] = CORRNER_CHAR;
    for (int i = 1; i <= pipt.item[itemNumber].width - 2; i++) {
        pipt.item[itemNumber].bottom[i] = HORIZONTAL_CHAR;
    }
    pipt.item[itemNumber].bottom[pipt.item[itemNumber].width - 1] = CORRNER_CHAR;
}

void FormatItems() {
    for (int i = 0; i <= pipt.itemCount; i++) {
        pipt.item[i].height = pipt.item[i].bodyLineCount + 2;
        pipt.item[i].width += 1;
        FormatTop(i);
        FormatBody(i);
        FormatBottom(i);
    }
}

//==============================================================
//  PossitionItems
//==============================================================

int ValidConnectionY(struct stack* stackY) {
    for (int i = 0; i < pipt.item[stackY->item[stackY->top]].connectionCount; i++) {
        if (pipt.item[pipt.item[stackY->item[stackY->top]].connected[i]].y == 0) {
            return i;
        }
    }
    return -1;
}

void PossitionItemsY() {
    int currentY = 1;
    struct stack stackY;
    stackY.top = -1;
    stackY.capacity = pipt.itemCount;

    StackPush(&stackY, 0);
    while (stackY.top >= 0) {
        int connection = ValidConnectionY(&stackY);

        if (connection != -1) {
            currentY += pipt.item[stackY.item[stackY.top]].height + 1;
            StackPush(&stackY, pipt.item[stackY.item[stackY.top]].connected[connection]);
        }
        else {
            pipt.item[stackY.item[stackY.top]].y = currentY;
            StackPop(&stackY);
            currentY -= pipt.item[stackY.item[stackY.top]].height + 1;
        }
    }
}

//==============================================================
//  PrintItems
//==============================================================

void PrintItems() {
    for (int i = 0; i <= pipt.itemCount; i++) {
        printf("\nx:%d y:%d w:%d h:%d blc:%d\n", pipt.item[i].x, pipt.item[i].y, pipt.item[i].width, pipt.item[i].height, pipt.item[i].bodyLineCount);
        for (int j = 0; j < pipt.item[i].connectionCount; j++) {
            printf("connection:%d\n", pipt.item[i].connected[j]);
        }
        printf("%s\n", pipt.item[i].top);
        for (int j = 0; j < pipt.item[i].height - 2; j++) {
            printf("%s\n", pipt.item[i].body[j]);
        }
        printf("%s\n", pipt.item[i].bottom);
    }
}

//==============================================================
//  main
//==============================================================

int main(int argc, char* argv[]) {
    const int argvPath = VerifyArgs(argc);
    if (argvPath != 1) return -1;

    LoadFileData(argv[argvPath]);

    ConnectItems();

    FormatItems();

    PossitionItemsY();

    PrintItems();

    return 0;
}
