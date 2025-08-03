#include <stdio.h>

#define MAX_ITEM_DATA 255
#define MAX_ITEM_COUNT 16
#define TITLE_MARK '#'
#define BODY_MARK '>'
#define CONNECTION_MARK '-'

struct piptItem {
    char title[MAX_ITEM_DATA];
    char body[MAX_ITEM_DATA];
    char* connection[MAX_ITEM_COUNT];
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

void CopyData(char* read, char* write) {
    for (int i = 2; i <= MAX_ITEM_DATA; i++) {
        write[i - 2] = read[i];
    }
}

int MakeGraph(const char* arg) {
    FILE *file;
    file = fopen(arg, "r");
    char fileData[MAX_ITEM_DATA];

    if (file == NULL) {
        printf("file %s not found\n", arg);
        return -1;
    }

    int lineNumber = 0;
    int itemCount = 0;

    while(fgets(fileData, MAX_ITEM_DATA, file)) {
        lineNumber++;

        if (fileData[0] == TITLE_MARK) {
            itemCount++;

            char* fileDataDestination = piptItem[itemCount].title;
            CopyData(fileData, fileDataDestination);
        }
        else if (fileData[0] == BODY_MARK) {
            char* fileDataDestination = piptItem[itemCount].body;
            CopyData(fileData, fileDataDestination);
        }
        else if (fileData[0] == CONNECTION_MARK) {
            //god why
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
    return 0;
}

int main(int argc, char* argv[]) {
    const int argvPath = VerifyArgs(argc);

    if (argvPath > 0) {
        const int nodeCount = MakeGraph(argv[argvPath]);
        printf("%d\n", nodeCount);
    }

    return 0;
}
