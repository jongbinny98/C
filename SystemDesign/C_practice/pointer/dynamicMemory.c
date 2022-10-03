#include <stdio.h>
#include <stdlib.h>

int main()
{   
    //user enter the size
    int size;
    char *text = NULL;

    printf("Enter limits of bytes: \n");
    scanf("%d", &size);

    //n is the number of the characters
    //calloc doesnt need it cause two parameter instead of multiplication
    text = (char *)malloc(size * sizeof(char));
    
    if (*text)
    {
        printf("Enter some text: \n");
        scanf(" ");
        gets(text);
        printf("inputted text is %s\n", text);
    }
    free(text);
    text = NULL;

    return 0;
}