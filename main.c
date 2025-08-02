#include <stdio.h>

#define MAX_ITEM_DATA 255

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
    int nodeCount = MakeGraph(argv[1]);

    printf("%d\n", nodeCount);
    return 0;
}
