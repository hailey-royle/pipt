#include <stdio.h>
#include <string.h>

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

struct piptItem {
    char connection[MAX_ITEM_COUNT][MAX_ITEM_DATA_LENGTH];
    char body[MAX_ITEM_BODY_LINES][MAX_ITEM_DATA_LENGTH];
    char title[MAX_ITEM_DATA_LENGTH];
    char top[MAX_ITEM_DATA_LENGTH];
    char bottom[MAX_ITEM_DATA_LENGTH];
    int connected[MAX_ITEM_COUNT];
    int connectionCount;
    int width;
    int height;
    int x;
    int y;
};
struct piptItem piptItem[MAX_ITEM_COUNT];

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
        return;
    }
    stack->top++;
    stack->item[stack->top] = item;
}

int StackPop(struct stack* stack) {
    if (stack->top < 0) {
        printf("stack underflow\n");
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

int LoadFileData(const char* arg) {
    FILE *file;
    file = fopen(arg, "r");
    char rawData[MAX_ITEM_DATA_LENGTH];

    if (file == NULL) {
        printf("file %s not found\n", arg);
        return -1;
    }

    int itemCount = -1;

    while(fgets(rawData, MAX_ITEM_DATA_LENGTH, file)) {
        static int lineNumber = 0;
        static int bodyNumber = 0;
        static int connectionNumber = 0;
        lineNumber++;
        if (strlen(rawData) >= MAX_ITEM_DATA_LENGTH - 1) {
            printf("data overflow on line %d\n", lineNumber);
            return -1;
        }
        if (bodyNumber >= MAX_ITEM_BODY_LINES) {
            printf("item %d has to many lines in its body, max is %d", itemCount, MAX_ITEM_BODY_LINES);
        }
        if (rawData[0] == '\n') {
            continue;
        }
        if (rawData[0] == TITLE_MARK) {
            itemCount++;
            bodyNumber = 0;
            connectionNumber = 0;
            strcpy(piptItem[itemCount].title, rawData);
        }
        else if (rawData[0] == BODY_MARK) {
            strcpy(piptItem[itemCount].body[bodyNumber], rawData);
            bodyNumber++;
        }
        else if (rawData[0] == CONNECTION_MARK) {
            strcpy(piptItem[itemCount].connection[connectionNumber], rawData);
            piptItem[itemCount].connectionCount++;
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

//==============================================================
//  ConnectItems
//==============================================================

void FindConnection(const int itemCount, const int itemNumber, const int connectionNumber) {
    piptItem[itemNumber].connection[connectionNumber][0] = TITLE_MARK;
    for (int i = 0; i < itemCount; i++) {
        if (piptItem[i].title[0] == '\0') {
            return;
        }
        if (strcmp(piptItem[itemNumber].connection[connectionNumber], piptItem[i].title) == 0) {
            piptItem[itemNumber].connected[connectionNumber] = i;
        }
    }
}

void ConnectConnections(const int itemCount, const int itemNumber) {
    for (int i = 0; i < itemCount; i++) {
        FindConnection(itemCount, itemNumber, i);
    }
}

void ConnectItems(const int itemCount) {
    for (int i = 0; i < itemCount; i++) {
        ConnectConnections(itemCount, i);
    }
}

//==============================================================
//  FormatItems
//==============================================================

int FindItemHeight(const int itemNumber) {
    int height = 0;
    for (int i = 0; i < MAX_ITEM_BODY_LINES; i++) {
        if (piptItem[itemNumber].body[i][0] == '\0') {
            break;
        }
        height++;
    }
    return height + 2;
}

int FindItemWidth(const int itemNumber) {
    long unsigned int width = strlen(piptItem[itemNumber].title);
    for (int i = 0; i < MAX_ITEM_BODY_LINES; i++) {
        if (piptItem[itemNumber].body[i][0] == '\0') {
            break;
        }
        if (width < strlen(piptItem[itemNumber].body[i])) {
            width = strlen(piptItem[itemNumber].body[i]);
        }
    }
    return width + 1;
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
    piptItem[itemNumber].bottom[0] = CORRNER_CHAR;
    for (int i = 1; i <= piptItem[itemNumber].width - 2; i++) {
        piptItem[itemNumber].bottom[i] = HORIZONTAL_CHAR;
    }
    piptItem[itemNumber].bottom[piptItem[itemNumber].width - 1] = CORRNER_CHAR;
}

void FormatItems(const int itemCount) {
    for (int i = 0; i < itemCount; i++) {
        piptItem[i].height = FindItemHeight(i);
        piptItem[i].width = FindItemWidth(i);
        FormatTop(i);
        FormatBody(i);
        FormatBottom(i);
    }
}

//==============================================================
//  PossitionItems
//==============================================================

void PossitionItemsY() {
    int currentY = 1;
    struct stack stackY;
    stackY.top = -1;
    stackY.capacity = MAX_ITEM_COUNT;

    StackPush(&stackY, 0);
    while (stackY.top >= 0) {
        int connection = -1;
        if (connection >= 0) {
            currentY += piptItem[stackY.top].height + 1;
            StackPush(&stackY, connection);
        }
        if (connection == -1) {
            piptItem[stackY.top].y = currentY;
            printf("item: %s, y %d\n", piptItem[stackY.top].top, piptItem[stackY.top].y);
            currentY -= piptItem[stackY.top].height;
            StackPop(&stackY);
        }
    }
    printf("\n\n");
}

//==============================================================
//  PrintItems
//==============================================================

void PrintItems(const int itemCount) {
    for (int i = 0; i < itemCount; i++) {
        printf("\nx:%d y:%d\n", piptItem[i].x, piptItem[i].y);
        for (int j = 0; j < piptItem[i].connectionCount; j++) {
            printf("connection:%d\n", piptItem[i].connected[j]);
        }
        printf("%s\n", piptItem[i].top);
        for (int j = 0; j < piptItem[i].height - 2; j++) {
            printf("%s\n", piptItem[i].body[j]);
        }
        printf("%s\n", piptItem[i].bottom);
    }
}


//==============================================================
//  main
//==============================================================

int main(int argc, char* argv[]) {
    const int argvPath = VerifyArgs(argc);
    if (argvPath != 1) return -1;

    const int itemCount = LoadFileData(argv[argvPath]);
    if (itemCount <= 0) return -1;

    ConnectItems(itemCount);

    FormatItems(itemCount);

    //PossitionItemsY();

    PrintItems(itemCount);

    return 0;
}
