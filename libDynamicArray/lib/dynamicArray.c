#include <stdio.h>
#include <stdlib.h>
#include <dynamicArray.h>

/**
 * @brief Initializes the array struct with the initial size
 * 
 * @param pArray        Struct of type DynamicArray
 * @param pInitialSize  Initial size of the array
 * @param pElementSize  Size of the elements in the array
 * @return int          0-> Initialization successful
 *                      1-> Error
 */
int arrayInit(DynamicArray *pArray, unsigned int pInitialSize, unsigned int pElementSize){
    pArray->array = malloc(pInitialSize * sizeof(void*));
    if(!pArray->array) return 0;

    pArray->elements = 0;
    pArray->size = pInitialSize;
    pArray->elementSize = pElementSize;
    return 1;
}


/**
 * @brief Inserts a new element in the array and change the size if needed
 * 
 * @param pArray    Struct of type DynamicArray
 * @param pElement  Element to insert
 * @return int      0-> Successful
 *                 -1-> Error during insertion
 */
int arrayInsert(DynamicArray *pArray, void *pElement){
    if(pArray->elements == pArray->size){
        pArray->size *= 2;
        pArray->array = realloc(pArray->array, pArray->size * sizeof(void*));
        if(!pArray->array) return -1;
    }

    pArray->array[pArray->elements++] = pElement;
    return 0;
}


/**
 * @brief Free the memory allocated for the array
 * 
 * @param pArray        Struct of type DynamicArray
 * @param pFreeElements Indicates if the elements inside the array also need to be freed
 */
void arrayFree(DynamicArray *pArray, int pFreeElements){
    if(pFreeElements){
        for(int i=0; i<pArray->elements; i++)
            free(pArray->array[i]);
    }
    free(pArray->array);
    pArray->array = NULL;
    pArray->elements = pArray->size = pArray->elementSize = 0;
}
