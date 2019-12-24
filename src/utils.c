/**
 * Implementation of utils.h.
 */
#include "utils.h"
#include <stdlib.h>
#include <string.h>

unsigned int max(unsigned int x, unsigned int y) {
    return x > y ? x : y;
}

unsigned int min(unsigned int x, unsigned int y) {
    return x < y ? x : y;
}

unsigned int num_occurrences(const char *s, char c) {
    unsigned int n = 0;
    for (unsigned int i = 0; s[i] != '\0'; ++i) {
        if (s[i] == c) ++n;
    }
    return n;
}

int mod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}

char *strdupli(const char *s) {
    char *t = malloc((strlen(s) + 1) * sizeof(char));
    unsigned int i;
    for (i = 0; s[i] != '\0'; ++i) t[i] = s[i];
    t[i] = '\0';
    return t;
}
