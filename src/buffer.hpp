#include <cstring>
#include <cstdlib>

static char *str; // The string currntly being built

/// Clear the current string being built to a
/// blank string.
void clearBuffer() {
    str = "";
}

/// Add a character to the string buffer
/// handles string resizing.
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

/// Append a series of characters to the
/// string buffer, handles string resizing.
void appendBufferStr(char *str2) {
    int currentLen = strlen(str);
    int lenWithNullTerminator = currentLen + 1;
    int newLen = lenWithNullTerminator + strlen(str2);
    char *result = (char *)malloc(sizeof(char) * newLen);
    strcpy(result, str);
    strcat(result, str2);
    str = result;
}

/// Return a deep copy of the current buffer.
char *copyBuffer() {
    char *copy = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(copy, str);
    return copy;
}