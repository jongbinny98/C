#include <stdio.h>

//determine length of the string
int stringLength(const char *string);

int main()
{
    
    printf("%d \n", stringLength("stringLength test"));
    printf("%d \n", stingLength(""));

    return 0;
}
int stringLength(const char *string)
{
    
    const char *lastAddress = string;
    /*
    or while(*lastAddress) because if you dereference the \0 is equal to 0
    and when its false(0) it ends.
    */
    while (*lastAddress)
        ++lastAddress;
    return lastAddress - string;
}