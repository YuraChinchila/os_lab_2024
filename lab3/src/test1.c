#include "find_min_max.h"
#include <stdio.h>

int main() {
    int a[5] = {3, 2, 6, 1, 4};
    struct MinMax m = GetMinMax(a, 0, 5);
    printf("%d %d\n", m.max, m.min);
    return 0;
}