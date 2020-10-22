#ifndef _SCHEDULE_H
#define _SCHEDULE_H

#include <pthread.h>
#include <dynamicArray.h>

enum SHEDULING_ALGORITHM { RM, EDF};
enum MARTIAN_STATE {READY, EXECUTING, IDLE};

extern int martianAmount;
extern DynamicArray _martians;



int rm_checkSchedulingError();
int rm_getShortestPeriod();
int rm_nextShortestPeriodIgnore(const int pIndexToIgnore);
int rm_checkMartianState(const int pIndex);
void rm_shchedule(int *pCurrentState, int *pNextMartianIdx, int *pWait);

int checkSchedulingError(const int pAlgorithm);



#endif
