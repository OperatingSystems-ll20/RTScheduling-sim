#include <stdio.h>
#include <stdlib.h>
#include <dynamicArray.h>

int arrayInit(DynamicArray *pArray, unsigned int pInitialSize, unsigned int pElementSize){
    pArray->array = malloc(pInitialSize * sizeof(void*));
    if(!pArray->array) return 0;

    pArray->elements = 0;
    pArray->size = pInitialSize;
    pArray->elementSize = pElementSize;
    return 1;
}


int arrayInsert(DynamicArray *pArray, void *pElement){
    if(pArray->elements == pArray->size){
        pArray->size *= 2;
        pArray->array = realloc(pArray->array, pArray->size * sizeof(void*));
        if(!pArray->array) return -1;
    }

    pArray->array[pArray->elements++] = pElement;
    return 0;
}

void arrayFree(DynamicArray *pArray){
    for(int i=0; i<pArray->elements; i++)
        free(pArray->array[i]);
    free(pArray->array);
    pArray->array = NULL;
    pArray->elements = pArray->size = pArray->elementSize = 0;
}
