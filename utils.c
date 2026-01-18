#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "game.h"

int getInt(const char* prompt) {
    printf("%s", prompt);
    int num;
    scanf("%d",&num);
    while (getchar() != '\n');
    return num;
}

char* getString(const char* prompt) {
    printf("%s", prompt);
    int capacity = 1;
    int length = 0;
    char *str = malloc(capacity * sizeof(char));
    if (str == NULL) {
        return NULL;
    }
    char c;
    while ((c = getchar()) != '\n') {
        if (length + 1 >= capacity) {
            capacity++;
            char *temp = realloc(str, capacity * sizeof(char));
            if (temp == NULL) {
                free(str);
                return NULL;
            }
            str = temp;
        }
        str[length] = c;
        length++;
    }
    str[length] = '\0';
    return str;
}