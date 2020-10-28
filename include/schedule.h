#ifndef _SCHEDULE_H
#define _SCHEDULE_H

#include <pthread.h>
#include <dynamicArray.h>

enum THREAD_STATE {READY, EXECUTING, IDLE};

extern int _martianAmount;
extern DynamicArray _martians;
extern Options _options;


int rm_checkSchedulingError();
int rm_getShortestPeriod();
int rm_nextShortestPeriodIgnore(const int pIndexToIgnore);
int rm_checkMartianState(const int pIndex);
void rm_shchedule(int *pCurrentState, int *pNextMartianIdx, int *pWait);

int edf_checkSchedulingError();
int edf_getShortestExecution(int pSecTimer);
int edf_nextShortestExecutionIgnore(const int pIndexToIgnore, int pSecTimer);
void edf_schedule(int *pCurrentState, int *pNextMartianIdx, int *pWait, int pSecTimer);

int checkSchedulingError(const int pAlgorithm);
void schedule(int *pScheduleError, int *pExecuteSchedule, int *pCurrentState, int *pNextMartianIdx, int *pWait, int pSecTimer);
void allowExecution(Martian *pNextMartian, const int pCurrentState, const int pNextMartianIdx);
void updateThreadTimers();



#endif
