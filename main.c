#include <stdio.h>
#include <string.h>

#define MAX_ITEM_TITLE 64
#define MAX_ITEM_BODY 256
#define MAX_ITEM_DATA MAX_ITEM_BODY
#define MAX_ITEM_COUNT 16
#define TITLE_MARK '#'
#define BODY_MARK '>'
#define CONNECTION_MARK '-'
#define MARK_LENGTH 2
#define CORRNER_CHAR "+"
#define HORIZONTAL_CHAR "-"
#define VERTICAL_CHAR "|"
#define SPACE_CHAR " "
#define HORIZONTAL_BUFFER 2

struct piptItem {
    char connection[MAX_ITEM_COUNT][MAX_ITEM_TITLE];
    char rawTitle[MAX_ITEM_TITLE];
    char title[MAX_ITEM_TITLE];
    char body[MAX_ITEM_BODY];
    char bottom[MAX_ITEM_BODY];
    int width;
};
struct piptItem piptItem[MAX_ITEM_COUNT];

//----------------------------------------------------------------
// Utility Functions, for various string functions and other like things
//----------------------------------------------------------------

int RemoveNewLine(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            str[i] = '\0';
            return 1;
        }
        i++;
    }
    return 0;
}

int StrCpy(char* writeStr, char* readStr, int writeStart, int readStart, int max) {
    for (int i = 0; i <= max; i++) {
        if (readStr[readStart + i] == '\0') {
            return i;
        }
        writeStr[writeStart + i] = readStr[readStart + i];
    }
    return -1;
}

int StrLen(char* str) {
    int i = 0;
    while (str[i]) {
        i++;
    }
    return i;
}

int StrNln(char* str, int start) {
    if (str[start] == '\0') {
        return 0;
    }
    int i = start;
    while (str[i]) {
        if (str[i] == '\n' || str[i] == '\0') {
            break;
        }
        i++;
    }
    return i;
}

int LongestLine(char* str) {
    int i = 0;
    int currentLine = 0;
    int longestLine = 0;
    while (str[i]) {
        if (str[i] == '\n') {
            if (longestLine < currentLine) {longestLine = currentLine;}
            currentLine = 0;
            i++;
            continue;
        }
        i++;
        currentLine++;
    }
    if (longestLine < currentLine) {
        longestLine = currentLine;
    }
    return longestLine;
}

//----------------------------------------------------------------
// Content functions, do a specific job directly related to program functioning
//----------------------------------------------------------------

int VerifyArgs(const int argc) {
    if (argc < 2) {
        printf("no arguments given\n");
        return -1;
    }

    if (argc > 2) {
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
    int connectionNumber = 0;

    while(fgets(rawData, MAX_ITEM_DATA, file)) {
        if (rawData[0] == TITLE_MARK) {
            itemCount++;
            connectionNumber = 0;
            strcpy(piptItem[itemCount].title, rawData);
        }
        else if (rawData[0] == BODY_MARK) {
            strcat(piptItem[itemCount].title, rawData);
        }
        else if (rawData[0] == CONNECTION_MARK) {
            char* connection = piptItem[itemCount].connection[connectionNumber];
            StrCpy(connection, rawData, 0, MARK_LENGTH, MAX_ITEM_TITLE);
            connectionNumber++;
        }
        else if (rawData[0] == '\n') {
        } 
        else {
            printf("data on line %d could not be prosessed, must start line with %c : body, %c : body, or %c : connection\n",
                    lineNumber, TITLE_MARK, BODY_MARK, CONNECTION_MARK);
            return -1;
        }
        lineNumber++;
    }

    fclose(file);
    return itemCount + 1;
}

void FormatItemTitle(int itemNumber) {
    char* title = piptItem[itemNumber].title;
    for (int i = StrLen(title); i < piptItem[itemNumber].width; i++) {
        StrCpy(title, HORIZONTAL_CHAR, i, 0, 1);
    }
    StrCpy(title, CORRNER_CHAR, piptItem[itemNumber].width - 1, 0, 1);
}

void FormatItemBottom(int itemNumber) {
    char* bottom = piptItem[itemNumber].bottom;
    for (int i = 0; i < piptItem[itemNumber].width; i++) {
        StrCpy(bottom, HORIZONTAL_CHAR, i, 0, 1);
    }
    StrCpy(bottom, CORRNER_CHAR, 0, 0, 1);
    StrCpy(bottom, CORRNER_CHAR, piptItem[itemNumber].width - 1, 0, 1);
}

void FormatItem(int itemNumber) {
    char* title = piptItem[itemNumber].title;
    char* body = piptItem[itemNumber].body;

    int width = StrLen(title);
    int bodyLineLen = LongestLine(body);
    if (width < bodyLineLen) {
        width = bodyLineLen;
    }
    width = width + HORIZONTAL_BUFFER;
    piptItem[itemNumber].width = width;

    FormatItemTitle(itemNumber);
    FormatItemBottom(itemNumber);
}

void DrawItem(int itemNumber) {
    printf("%s\n", piptItem[itemNumber].title);
    printf("%s\n", piptItem[itemNumber].body);
    printf("%s\n", piptItem[itemNumber].bottom);
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
    }

    for (int i = 0; i < itemCount; i++) {
        DrawItem(i);
    }

    return 0;
}
