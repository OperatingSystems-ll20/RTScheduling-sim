#include <stdio.h>
#include <stdlib.h>
#include <objects.h>
#include <consts.h>
#include <schedule.h>


int rm_checkSchedulingError(){
    int result = 0;
    for(int i=0; i<_martianAmount; i++){
        Martian *martian = (Martian*)_martians.array[i];
        if( (martian->ready && martian->currentEnergy != 0) || 
            (martian->flagExec == 0 && martian->periodCounter > 1) ){
            result = 1;
            break;
        }
    }
    return result;
}

int rm_getShortestPeriod(){
    int candidate = -1;
    for(int i=0; i<_martianAmount; i++){
        Martian *martian = (Martian*)_martians.array[i];
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

int rm_nextShortestPeriodIgnore(const int pIndexToIgnore){
    int candidate = -1;
    int first = 1;
    for(int i=0; i<_martianAmount; i++){
        Martian *martian = (Martian*)_martians.array[i];
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

int rm_checkMartianState(const int pIndex){
    Martian *martian = (Martian*)_martians.array[pIndex];
    if(martian->ready && martian->currentEnergy == 0)
        return READY; //Martian is ready
    else if(!martian->ready && martian->currentEnergy != 0)
        return EXECUTING; //Martian is executing
    else if(!martian->ready && martian->currentEnergy == 0)
        return IDLE; //Martian is idle
}

void rm_shchedule(int *pCurrentState, int *pNextMartianIdx, int *pWait){
    *pCurrentState = rm_checkMartianState(*pNextMartianIdx);
    // printf("Martian %s ready=%d energy=%d\n", ((Martian*)_martians.array[nextMartianIdx])->title,
    // ((Martian*)_martians.array[nextMartianIdx])->ready, ((Martian*)_martians.array[nextMartianIdx])->currentEnergy);
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
    // if(nextMartianIdx != prevMartian) {
    //     frameCounter--; //Skip tick on each change
    //     // printf("Substract to frameCounter!!! -----> frameCounter=%d\n", frameCounter);
    // }
    // prevMartian = nextMartianIdx;
}

int edf_checkSchedulingError(){
    int result = 0;
    for(int i=0; i<_martianAmount; i++){
        Martian *martian = (Martian*)_martians.array[i];
        if( martian->ready && (martian->currentEnergy != 0 || martian->executed == 0) ) {
            result = 1;
            break;
        }
    }
    return result;
}

int edf_getShortestExecution(int pSecTimer){
    int candidate = -1;
    for(int i=0; i<_martianAmount; i++){
        Martian *martian = (Martian*)_martians.array[i];
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

int edf_nextShortestExecutionIgnore(const int pIndexToIgnore, int pSecTimer){
    int candidate = -1;
    int first = 1;
    for(int i=0; i<_martianAmount; i++){
        Martian *martian = (Martian*)_martians.array[i];
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

void schedule(int *pScheduleError, int *pExecuteSchedule, int *pCurrentState, int *pNextMartianIdx, int *pWait, int pSecTimer){
    *pScheduleError = checkSchedulingError(_options._schedulingAlgorithm);
    if(*pScheduleError){
        *pExecuteSchedule = 0;
        _options._errorPopUp = 1;
        // stopAllThreads();
        printf("Scheduling error!!!\n");
        return;
    }
    // pthread_mutex_unlock(&_mutex);

    // pthread_mutex_lock(&_mutex);
    switch(_options._schedulingAlgorithm){
        case RM:
            rm_shchedule(pCurrentState, pNextMartianIdx, pWait);
            break;
        case EDF:
            edf_schedule(pCurrentState, pNextMartianIdx, pWait, pSecTimer);
            break;
    }
}

void allowExecution(Martian *pNextMartian, const int pCurrentState, const int pNextMartianIdx){
    pNextMartian = ((Martian*)_martians.array[pNextMartianIdx]);
    // printf("%s period Counter = %d ------\n", nextMartian->title, nextMartian->periodCounter);
    switch(pCurrentState){
        case READY: //Ready
            pNextMartian->currentEnergy = pNextMartian->maxEnergy;
            pNextMartian->doWork = 1;
            // pthread_cond_signal(&nextMartian->cond);
            // printf("Start of %s at second %d\n", nextMartian->title, _secTimer);
            break;
        case EXECUTING: //Executing
            pNextMartian->doWork = 1;
            // pthread_cond_signal(&nextMartian->cond);
            // printf("Continue executing %s with energy=%d at second %d\n", nextMartian->title, nextMartian->currentEnergy, _secTimer);
            break;
    }
}

void updateThreadTimers(){
    for(int i=0; i<_martianAmount; i++){
        ((Martian*)_martians.array[i])->update = 1;
        while(((Martian*)_martians.array[i])->update == 1); //Wait until thread is updated
        // pthread_cond_signal(&((Martian*)_martians.array[i])->cond);
    }
}
