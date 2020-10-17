#include <stdio.h>
#include <stdlib.h>
#include <array.h>

int arrayInit(Array *pArray, unsigned int pInitialSize, unsigned int pElementSize){
    pArray->array = malloc(pInitialSize * sizeof(void*));
    if(!pArray->array) return -1;

    pArray->elements = 0;
    pArray->size = pInitialSize;
    pArray->elementSize = pElementSize;
    return 0;
}


int arrayInsert(Array *pArray, void *pElement){
    if(pArray->elements == pArray->size){
        pArray->size *= 2;
        pArray->array = realloc(pArray->array, pArray->size * sizeof(void*));
        if(!pArray->array) return -1;
    }

    pArray->array[pArray->elements++] = pElement;
    return 0;
}

void arrayFree(Array *pArray){
    for(int i=0; i<pArray->elements; i++)
        free(pArray->array[i]);
    free(pArray->array);
    pArray->array = NULL;
    pArray->elements = pArray->size = pArray->elementSize = 0;
}
