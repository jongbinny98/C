#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main()
{
    int primes[50] = {0};
    int primesIndex = 2;

    primes[0] = 1;
    primes[1] = 3;

    bool x;

    for (int p = 5; p <= 100; p = p + 2)
    {
        x = true;
        
        /* if primes^2 is less than p(odd number) 
        -> checking if odd number is primes^2 or greater than compare with next prime number */
        for (int i = 1; x && p / primes[i] >= primes[i]; i++)
            if (p % primes[i] == 0)
                x = false;

        if (x == true)
        {
            primes[primesIndex] = p;
            primesIndex++;
        }
    }
    for (int i = 0; i < primesIndex; i++)
        printf("%i ", primes[i]);

    return 0;
}

