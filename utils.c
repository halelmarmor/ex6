#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "game.h"

// Helper functions for reading user input

int getInt(const char* prompt) {
    printf("%s", prompt);
    int num;
    if (scanf("%d",&num) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    while (getchar() != '\n');
    return num;
}

char* getString(const char* prompt) {
    printf("%s", prompt);
    int initialC;
    while ((initialC = getchar()) == '\n' || initialC == ' ' || initialC == '\r');
    if (initialC == EOF) return NULL;
    int capacity = 1;
    int length = 0;
    char *str = malloc(capacity * sizeof(char));
    if (str == NULL) {
        return NULL;
    }
    str[length++] = (char)initialC;
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (length + 1 >= capacity) {
            capacity *= 2;
            char *temp = realloc(str, capacity * sizeof(char));
            if (temp == NULL) {
                free(str);
                return NULL;
            }
            str = temp;
        }
        str[length++] = c;
    }
    str[length] = '\0';
    return str;
}