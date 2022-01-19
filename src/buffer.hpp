#include <cstring>
#include <cstdlib>

static char *str;

void clearBuffer() {
    str = "";
}

void appendBuffer(char c) {
    int currentLen = strlen(str);
    int lenWithNullTerminator = currentLen + 1;
    int newLen = lenWithNullTerminator + 1;

    char *newStr = (char *)malloc(sizeof(char) * newLen);
    strcpy(newStr, str);
    newStr[newLen - 1] = '\0';
    newStr[newLen - 2] = c;
    str = newStr;
}

void appendBufferStr(char *str2) {
    int currentLen = strlen(str);
    int lenWithNullTerminator = currentLen + 1;
    int newLen = lenWithNullTerminator + strlen(str2);
    char *result = (char *)malloc(sizeof(char) * newLen);
    strcpy(result, str);
    strcat(result, str2);
    str = result;
}

char *copyBuffer() {
    char *copy = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(copy, str);
    return copy;
}