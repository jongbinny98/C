#include <stdio.h>
#include <ctype.h>

int main()
{
    char buf[100];
    int nLetter = 0;
    int nDigits = 0;
    int nPunct = 0;

    printf("Enter an interesting string of less than %d characters:\n", 100);
    scanf("%s", buf);

    int i = 0;
    while(buf[i] != '\0')
    {
        if(isalpha(buf[i]))
            nLetter++;
        else if(isdigit(buf[i]))
            nDigits++;
        else if(ispunct(buf[i]))
            nPunct++;
        i++;
    }
    printf("\nYour string contained %d letters, %d digits, %d punctuation character", nLetter, nDigits, nPunct);

    return 0;
}