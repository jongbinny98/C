#include <stdio.h>

int main()
{
    int num = 150;
    int *pNum = NULL;

    pNum = &num;

    printf("address of Num: %p\n", &num);
    printf("address of pNum: %p\n", &pNum);

    printf("Value of pNum: %p\n", pNum); 
    printf("what value pNum is pointing to: %d", *pNum);
    return 0;

}