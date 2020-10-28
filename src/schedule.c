#include <stdio.h>
#include <stdlib.h>
#include <objects.h>
#include <consts.h>
#include <schedule.h>

/**
 * @brief Checks if there is an error during the RM Scheduling
 * 
 * @return int  1->Error in scheduling
 *              0->No error found
 */
int rm_checkSchedulingError(){
    int result = 0;
    for(int i=0; i<_martianAmount; i++){
        Martian *martian = (Martian*)_martians.array[i];
        if(martian->martianState == DONE) continue;
        if( (martian->ready && martian->currentEnergy != 0) || 
            (martian->flagExec == 0 && martian->periodCounter > 1) ){
            result = 1;
            break;
        }
    }
    return result;
}


/**
 * @brief Returns the index of the active martian with the shortest period
 * 
 * @return int  Index.
 *              If there are no active martians -1 is returned
 */
int rm_getShortestPeriod(){
    int candidate = -1;
    for(int i=0; i<_martianAmount; i++){
        Martian *martian = (Martian*)_martians.array[i];
        if(martian->martianState == DONE) continue;
        if((!martian->ready && martian->currentEnergy == 0)){
            continue;
        }

        else if(candidate < 0){
            if(((martian->ready && martian->currentEnergy == 0) ||
                (!martian->ready && martian->currentEnergy != 0))){
                candidate = i;
            }
        }

        else{
            if(((martian->ready && martian->currentEnergy == 0) ||
                (!martian->ready && martian->currentEnergy != 0))) {
                if(martian->period < ((Martian*)_martians.array[candidate])->period)
                    candidate = i;
            }
        }
    }
    return candidate;
}


/**
 * @brief Returns the index of the martian with the shortest period, ignoring the
 *        index passed by parameter
 * 
 * @param pIndexToIgnore   Index to ignore
 * @return int             Index of martian. 
 *                         If there are no active martians -1 is returned              
 */
int rm_nextShortestPeriodIgnore(const int pIndexToIgnore){
    int candidate = -1;
    int first = 1;
    for(int i=0; i<_martianAmount; i++){
        Martian *martian = (Martian*)_martians.array[i];
        if(martian->martianState == DONE) continue;
        if(i == pIndexToIgnore) continue;

        else{
            if(first){
                if(((martian->ready && martian->currentEnergy == 0) ||
                    (!martian->ready && martian->currentEnergy != 0))) {
                    candidate = i;
                    first = 0;
                }
            }
            else{
                if(((martian->ready && martian->currentEnergy == 0) ||
                    (!martian->ready && martian->currentEnergy != 0))) {
                    if(martian->period < ((Martian*)_martians.array[candidate])->period)
                        candidate = i;
                }
            }
        }
    }
    return candidate;
}


/**
 * @brief Checks the current a martian (Ready, Executing or Idle)
 * 
 * @param pIndex Index of martian to check
 * @return int   State
 */
int rm_checkMartianState(const int pIndex){
    Martian *martian = (Martian*)_martians.array[pIndex];
    if(martian->martianState == DONE) return IDLE;
    if(martian->ready && martian->currentEnergy == 0)
        return READY; //Martian is ready
    else if(!martian->ready && martian->currentEnergy != 0)
        return EXECUTING; //Martian is executing
    else if(!martian->ready && martian->currentEnergy == 0)
        return IDLE; //Martian is idle
}

/**
 * @brief Based on the state of the current executing martian, determines
 *        the index of the next martian that needs to be executed
 * 
 * @param pCurrentState     State of the current executing martian
 * @param pNextMartianIdx   Index of the next martian that needs execution
 * @param pWait             Flag set in case any martian needs execution
 */
void rm_shchedule(int *pCurrentState, int *pNextMartianIdx, int *pWait){
    *pCurrentState = rm_checkMartianState(*pNextMartianIdx);
    switch(*pCurrentState){
        case READY: //Ready
        case EXECUTING: //Executing
            *pWait = 0;
            *pNextMartianIdx = rm_getShortestPeriod();
            *pCurrentState = rm_checkMartianState(*pNextMartianIdx);
            break;

        case IDLE: //Idle
            *pWait = 0;
            int test = rm_nextShortestPeriodIgnore(*pNextMartianIdx);
            if(test < 0) {
                *pWait = 1;
                break;
            }
            *pNextMartianIdx = test;
            *pCurrentState = rm_checkMartianState(*pNextMartianIdx);
            break;
    }
}

/**
 * @brief Checks for errors in the EDF Scheduling
 * 
 * @return int  0->No errors found
 *              1->Error in scheduling
 */
int edf_checkSchedulingError(){
    int result = 0;
    for(int i=0; i<_martianAmount; i++){
        Martian *martian = (Martian*)_martians.array[i];
        if(martian->martianState == DONE) continue;
        if( martian->ready && (martian->currentEnergy != 0 || martian->executed == 0) ) {
            result = 1;
            break;
        }
    }
    return result;
}


/**
 * @brief Returns the index of the martian that has the closest deadline
 * 
 * @param pSecTimer Current second timer
 * @return int      Index.
 *                  In case any martian needs execution -1 is returned
 */
int edf_getShortestExecution(int pSecTimer){
    int candidate = -1;
    for(int i=0; i<_martianAmount; i++){
        Martian *martian = (Martian*)_martians.array[i];
        if(martian->martianState == DONE) continue;
        if((!martian->ready && martian->currentEnergy == 0)){ //Ignore idle
            continue;
        }

        else if(candidate < 0){
            if(((martian->ready && martian->currentEnergy == 0) ||
                (!martian->ready && martian->currentEnergy != 0))){
                candidate = i;
            }
        }

        else{
            int testCurrent = abs((pSecTimer-martian->arrivalTime) - (martian->period * martian->periodCounter));
            int testCandidate = abs((pSecTimer-((Martian*)_martians.array[candidate])->arrivalTime) -
                ( ((Martian*)_martians.array[candidate])->period * ((Martian*)_martians.array[candidate])->periodCounter ) );

            if(testCurrent == testCandidate){
                if(!martian->ready && martian->currentEnergy != 0)//Select already active martian
                    candidate = i;
            }
            else if(testCurrent < testCandidate)
                candidate = i;
        }
    }
    return candidate;
}


/**
 * @brief Returns the index of the martian that has the closest deadline, ignoring
 *        the index of the martian passed by parameter
 * 
 * @param pIndexToIgnore Index of the martian to ignore
 * @param pSecTimer      Current second timer
 * @return int           Index.
 *                       In case any martian needs execution -1 is returned.
 */
int edf_nextShortestExecutionIgnore(const int pIndexToIgnore, int pSecTimer){
    int candidate = -1;
    int first = 1;
    for(int i=0; i<_martianAmount; i++){
        Martian *martian = (Martian*)_martians.array[i];
        if(martian->martianState == DONE) continue;
        if((i == pIndexToIgnore) || (!martian->ready && martian->currentEnergy == 0))
            continue;

        else{
            if(first){
                if(((martian->ready && martian->currentEnergy == 0) ||
                    (!martian->ready && martian->currentEnergy != 0)))
                {
                    candidate = i;
                    first = 0;
                }
            }
            else {
                int testCurrent = abs((pSecTimer-martian->arrivalTime) - (martian->period * martian->periodCounter));
                int testCandidate = abs((pSecTimer-((Martian*)_martians.array[candidate])->arrivalTime) -
                    ( ((Martian*)_martians.array[candidate])->period * ((Martian*)_martians.array[candidate])->periodCounter ) );

                if(testCurrent == testCandidate){
                    if(!martian->ready && martian->currentEnergy != 0)//Select already active martian
                        candidate = i;
                }
                else if(testCurrent < testCandidate)
                    candidate = i;
            }
        }
    }
    return candidate;
}


/**
 * @brief Based on the state of the current executing martian, the index of the next
 *        martian that needs execution is determined using the EDF scheduling
 * 
 * @param pCurrentState    State of the current executing martian
 * @param pNextMartianIdx  Reference to the next martian index
 * @param pWait            Reference to the wait flag (in case any martian needs execution)
 * @param pSecTimer        Current second timer
 */
void edf_schedule(int *pCurrentState, int *pNextMartianIdx, int *pWait, int pSecTimer){
    *pCurrentState = rm_checkMartianState(*pNextMartianIdx);
    switch(*pCurrentState){
        case READY: //Ready
        case EXECUTING: //Executing
            *pWait = 0;
            *pNextMartianIdx = edf_getShortestExecution(pSecTimer);
            *pCurrentState = rm_checkMartianState(*pNextMartianIdx);
            break;

        case IDLE: //Idle
            *pWait = 0;
            int test = edf_nextShortestExecutionIgnore(*pNextMartianIdx, pSecTimer);
            if(test < 0) {
                *pWait = 1;
                break;
            }
            *pNextMartianIdx = test;
            *pCurrentState = rm_checkMartianState(*pNextMartianIdx);
            break;
    }
}


/**
 * @brief Checks if there are any scheduling errors calling the 
 *        appropriate function based on the scheduling algorithm used
 * 
 * @param pAlgorithm Scheduling algorithm used
 * @return int       0->No errors founds
 *                   1->Error in scheduling
 */
int checkSchedulingError(const int pAlgorithm){
    int result = 0;
    switch(pAlgorithm){
        case RM:
            result = rm_checkSchedulingError();
            break;
        case EDF:
            result = edf_checkSchedulingError();
            break;
    }
    return result;
}


/**
 * @brief Calls the appropriate functions to schedule the execution of the martians
 *        based on the scheduling algorithm selected.
 * 
 * @param pScheduleError    Schedule error flag
 * @param pExecuteSchedule  Execute schedule flag 
 * @param pCurrentState     Current state of the martian in execution
 * @param pNextMartianIdx   Index of the next martian
 * @param pWait             Wait flag
 * @param pSecTimer         Current second timer
 */
void schedule(int *pScheduleError, int *pExecuteSchedule, int *pCurrentState, int *pNextMartianIdx, int *pWait, int pSecTimer){
    *pScheduleError = checkSchedulingError(_options._schedulingAlgorithm);
    if(*pScheduleError){
        *pExecuteSchedule = 0;
        _options._errorPopUp = 1;
        printf("Scheduling error!!!\n");
        return;
    }

    switch(_options._schedulingAlgorithm){
        case RM:
            rm_shchedule(pCurrentState, pNextMartianIdx, pWait);
            break;
        case EDF:
            edf_schedule(pCurrentState, pNextMartianIdx, pWait, pSecTimer);
            break;
    }
}


/**
 * @brief Based on the state of the martian, resets the energy level and allow its execution
 * 
 * @param pNextMartian      Reference to struct that holds the info of the martian
 * @param pCurrentState     Current state of the martian
 * @param pNextMartianIdx   Index of the martian that needs execution
 */
void allowExecution(Martian *pNextMartian, const int pCurrentState, const int pNextMartianIdx){
    pNextMartian = ((Martian*)_martians.array[pNextMartianIdx]);
    switch(pCurrentState){
        case READY: //Ready
            pNextMartian->currentEnergy = pNextMartian->maxEnergy;
            pNextMartian->doWork = 1;
            break;
        case EXECUTING: //Executing
            pNextMartian->doWork = 1;
            break;
    }
}


/**
 * @brief Updates the timer of all the martian threads and waits until
 *        all are updated
 * 
 */
void updateThreadTimers(){
    for(int i=0; i<_martianAmount; i++){
        if(((Martian*)_martians.array[i])->martianState == DONE) continue;
        ((Martian*)_martians.array[i])->update = 1;
        while(((Martian*)_martians.array[i])->update == 1); //Wait until thread is updated
    }
}
