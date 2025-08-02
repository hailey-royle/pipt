#include <stdio.h>

int MakeGraph(int* count, char* args) {
    printf("%d\n", *count);
    return *count;
}

int main(int argc, char* argv[]) {
    int nodeCount = MakeGraph(&argc, *argv);

    printf("%d\n", nodeCount);
    return 0;
}
