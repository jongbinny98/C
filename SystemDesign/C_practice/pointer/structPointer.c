#include <stdio.h>
#include <stdlib.h>
//the value that the pointer is pointing to is not changing 
//struct const item *i --> the address will not be changing 
void readItem(const struct item *i);
void printItem(struct item *i);

struct item 
{
    char *itemName;
    int qty;
    float price;
    float amount;
};

int main()
{
    struct item itm;
    struct item *pItem;

    pItem = &itm;

    pItem->itemName = (char *) malloc(30 * sizeof(char));
    //check for null
    if(pItem == NULL)
        exit(-1);

    readItem(pItem);

    free(pItem->itemName);
}

void readItem(const struct item *i)
{
    printf("Enter product name: ");
    scanf("%s",i->itemName);

    printf("\nEnter the price: ");
    scanf("%f", &i->price);

    printf("\nEnter the quantity: ");
    scanf("%d", &i->qty);

    i->amount = (float)i->qty * i->price;
}

