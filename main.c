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

void FormatRawTitle(char* rawData, int itemNumber) {
    RemoveNewLine(rawData);

    char* rawTitle = piptItem[itemNumber].rawTitle;
    StrCpy(rawTitle, rawData, 0, MARK_LENGTH, MAX_ITEM_TITLE);

    char* title = piptItem[itemNumber].title;
    int titleLength = 0;

    titleLength += StrCpy(title, CORRNER_CHAR, titleLength, 0, MAX_ITEM_TITLE - titleLength);
    titleLength += StrCpy(title, HORIZONTAL_CHAR, titleLength, 0, MAX_ITEM_TITLE - titleLength);
    titleLength += StrCpy(title, rawData, titleLength, MARK_LENGTH, MAX_ITEM_TITLE - titleLength);
}

int FormatRawBody(char* rawData, int itemNumber, int bodyLength) {
    RemoveNewLine(rawData);

    char* body = piptItem[itemNumber].body;

    if (bodyLength > 0) {
        bodyLength += StrCpy(body, "\n", bodyLength, 0, MAX_ITEM_BODY - bodyLength);
    }

    bodyLength += StrCpy(body, VERTICAL_CHAR, bodyLength, 0, MAX_ITEM_BODY - bodyLength);
    bodyLength += StrCpy(body, SPACE_CHAR, bodyLength, 0, MAX_ITEM_BODY - bodyLength);
    bodyLength += StrCpy(body, rawData, bodyLength, MARK_LENGTH, MAX_ITEM_BODY - bodyLength);

    return bodyLength;
}

int LoadFileData(const char* arg) {
    FILE *file;
    file = fopen(arg, "r");
    char rawData[MAX_ITEM_DATA];

    if (file == NULL) {
        printf("file %s not found\n", arg);
        return -1;
    }

    int lineNumber = -1;
    int itemCount = -1;
    int connectionNumber = -1;
    int itemBodyLength = 0;

    while(fgets(rawData, MAX_ITEM_DATA, file)) {
        lineNumber++;

        if (rawData[0] == TITLE_MARK) {
            connectionNumber = -1;
            itemBodyLength = 0;
            itemCount++;

            FormatRawTitle(rawData, itemCount);
        }
        else if (rawData[0] == BODY_MARK) {
            itemBodyLength = FormatRawBody(rawData, itemCount, itemBodyLength);
        }
        else if (rawData[0] == CONNECTION_MARK) {
            connectionNumber++;

            char* connection = piptItem[itemCount].connection[connectionNumber];
            StrCpy(connection, rawData, 0, MARK_LENGTH, MAX_ITEM_TITLE);
        }
        else if (rawData[0] == '\n') {
        } 
        else {
            printf("data on line %d could not be prosessed, must start line with %c : body, %c : body, or %c : connection\n",
                    lineNumber, TITLE_MARK, BODY_MARK, CONNECTION_MARK);
            return -1;
        }
    }

    fclose(file);
    return itemCount + 1;
}

void FormatItemTitle(int itemNumber) {
    char* title = piptItem[itemNumber].title;
    for (int i = strlen(title); i < piptItem[itemNumber].width; i++) {
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

    int width = strlen(title);
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
