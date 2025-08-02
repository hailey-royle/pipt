#include <stdio.h>

#define MAX_ITEM_DATA 255

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

int MakeGraph(const char* arg) {
    FILE *file;
    file = fopen(arg, "r");

    if (file == NULL) {
        printf("file %s not found\n", arg);
        return -1;
    }

    char fileData[MAX_ITEM_DATA];

    while(fgets(fileData, MAX_ITEM_DATA, file)) {
        printf("%s", fileData);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    int argvPath = VerifyArgs(argc);

    if (argvPath > 0) {
        int nodeCount = MakeGraph(argv[argvPath]);
        printf("%d\n", nodeCount);
    }

    return 0;
}
