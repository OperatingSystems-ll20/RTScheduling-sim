#ifndef _DYNAMICARRAY_H
#define _DYNAMICARRAY_H

typedef struct {
    void **array;
    unsigned int elements;
    unsigned int elementSize;
    unsigned int size;
} DynamicArray;

int arrayInit(DynamicArray *pArray, unsigned int pInitialSize, unsigned int pElementSize);
int arrayInsert(DynamicArray *pArray, void *pElement);
void arrayFree(DynamicArray *pArray, int pFreeElements);



#endif
