#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define FILENAME "Text.txt"

int main()
{   
    //need read and writing 
    FILE *fp = NULL;
    FILE *fp1 = NULL;

    char ch = '';

    fp = fopen(FILENAME, "r");
    if (fp == NULL)
    {
        return -1;
    }
    fp1 = fopen("temp.txt", "w");
    if(fp1 = NULL)
    {
        return -1;
    }

    while((ch = fgetc(fp)) != EOF)
    {
        if (islower(ch))
        {
            ch = ch - 32;
        }
        fputc(ch, fp1);
    }
    fclose(fp);
    fclose(fp1);

    rename("temp.txt", FILENAME);

    remove("temp.txt");

    fp = fopen(FILENAME, "r");
    if (fp == NULL)
    {
        return -1;
    }
    while((ch = fgetc(fp)) != EOF)
        printf("%c", ch);

    fclose(fp);
    fp = NULL;
    fp1 = NULL;

    return 0;
}