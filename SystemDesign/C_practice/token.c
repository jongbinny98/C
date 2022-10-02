#include <stdio.h>
#include <string.h>

int main()
{
    char str[80] = "hello how are you - my name is - jongbin";
    const char s[2] = "-"; //delimeter 
    char *token;    //pointer will return our each token

    //get the first token
    token = strtok(str, s);
    printf("%s\n", token);
    

    //walk through other token
    while(token != NULL)
    {
        printf("%s\n", token);
        token = strtok(NULL, s);
    }
    return 0;
}