#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int stringLength(const char string[]);
void concat(char result[], const char str[], const char str1[]);

int main()
{

    const char word1[] = "jason";
    const char word2[] = "ok";
    char result[50];

    printf("length for jason:%d, ok:%d", stringLength(word1), stringLength(word2));
    concat(result, word1, word2);
    printf("\nconcat: %s", result);
    return 0;
}

int stringLength(const char string[])
{
    int count = 0;
    while (string[count] != '\0')
        count++;

    return count;
}

void concat(char result[], const char str[], const char str1[])
{
    int i, j;
    for (i = 0; str[i] != '\0'; i++)
    {
        result[i] = str[i];
    }
    for (j = 0; str1[j] != '\0'; j++)
    {
        result[i + j] = str1[j];
    }
    result[i + j] = '\0'; //add null term
}