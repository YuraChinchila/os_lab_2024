#include <unistd.h>
#include <stdio.h>

int main() {
    execl("sequential_min_max", "sequential_min_max", "42", "100", NULL);
    return 1;
}