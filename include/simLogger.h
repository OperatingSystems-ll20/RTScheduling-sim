#ifndef _SIMLOGGER_H
#define _SIMLOGGER_H

#include <stdio.h>

static FILE *_fileLog;

int initLogger();
void closeLogger();

void logSimEvent(const int pEventType, const int pSecTimer, const char *pEvent);
void logMartianEvent(const int pEventType, const int pMartianId, const int pSecTimer, const char *pEvent);


#endif