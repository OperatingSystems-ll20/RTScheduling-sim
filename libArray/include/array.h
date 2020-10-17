#ifndef _ARRAY_H
#define _ARRAY_H

typedef struct {
    void **array;
    unsigned int elements;
    unsigned int elementSize;
    unsigned int size;
} Array;

int arrayInit(Array *pArray, unsigned int pInitialSize, unsigned int pElementSize);
int arrayInsert(Array *pArray, void *pElement);
void arrayFree(Array *pArray);



#endif
