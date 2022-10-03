#include <stdio.h>

void square(int *const x);

int main()
{
    int num = 9;
    square(&num);
    printf("The square of the given number is %d\n", num);

    return 0;
}
//you cannot change the value of address
void square(int *const x)
{
    *x = (*x) *(*x);
}