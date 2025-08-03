#include <stdio.h>

#define MAX_ITEM_TITLE 32
#define MAX_ITEM_BODY 128
#define MAX_ITEM_DATA MAX_ITEM_BODY
#define MAX_ITEM_COUNT 16
#define TITLE_MARK '#'
#define BODY_MARK '>'
#define CONNECTION_MARK '-'
#define MARK_LENGTH 2
#define CORRNER_CHAR "+"
#define HORIZONTAL_CHAR "-"
#define VERTICAL_CHAR "|"

struct piptItem {
    char connection[MAX_ITEM_COUNT][MAX_ITEM_TITLE];
    char title[MAX_ITEM_TITLE];
    char body[MAX_ITEM_BODY];
    int totalTitleLength;
};
struct piptItem piptItem[MAX_ITEM_COUNT];

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

int CopyData(char* read, char* write, const int max) {
    for (int i = 2; i <= max; i++) {
        if (read[i] == '\n' || read[i] == '\0') {
            return i;
        }
        write[i - 2] = read[i];
    }
    return -1;
}

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

void BuildItemTitle(char* rawData, int itemNumber) {
    RemoveNewLine(rawData);

    char* title = piptItem[itemNumber].title;
    int titleLength = 0;

    titleLength += StrCpy(title, CORRNER_CHAR, titleLength, 0, MAX_ITEM_TITLE - titleLength);
    titleLength += StrCpy(title, HORIZONTAL_CHAR, titleLength, 0, MAX_ITEM_TITLE - titleLength);
    titleLength += StrCpy(title, rawData, titleLength, MARK_LENGTH, MAX_ITEM_TITLE - titleLength);
    titleLength += StrCpy(title, HORIZONTAL_CHAR, titleLength, 0, MAX_ITEM_TITLE - titleLength);
    titleLength += StrCpy(title, CORRNER_CHAR, titleLength, 0, MAX_ITEM_TITLE - titleLength);
}

void BuildItemBody(char* rawData, int itemNumber) {
    char* body = piptItem[itemNumber].body;
    CopyData(rawData, body, MAX_ITEM_BODY);
}

int LoadFileData(const char* arg) {
    FILE *file;
    file = fopen(arg, "r");
    char fileData[MAX_ITEM_DATA];

    if (file == NULL) {
        printf("file %s not found\n", arg);
        return -1;
    }

    int lineNumber = -1;
    int itemCount = -1;
    int connectionNumber = -1;

    while(fgets(fileData, MAX_ITEM_DATA, file)) {
        lineNumber++;

        if (fileData[0] == TITLE_MARK) {
            connectionNumber = -1;
            itemCount++;

            BuildItemTitle(fileData, itemCount);
        }
        else if (fileData[0] == BODY_MARK) {
            BuildItemBody(fileData, itemCount);
        }
        else if (fileData[0] == CONNECTION_MARK) {
            connectionNumber++;

            char* fileDataDestination = piptItem[itemCount].connection[connectionNumber];
            CopyData(fileData, fileDataDestination, MAX_ITEM_TITLE);
        }
        else if (fileData[0] == '\n') {
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

int main(int argc, char* argv[]) {
    const int argvPath = VerifyArgs(argc);

    if (argvPath < 0) {
        return 0;
    }

    const int itemCount = LoadFileData(argv[argvPath]);

    printf("%d\n", itemCount);

    return 0;
}
