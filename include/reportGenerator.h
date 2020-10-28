#ifndef _REPORTGENERATOR_H
#define _REPORTGENERATOR_H

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <regex.h>
#include <dynamicArray.h>

static int BMP_HEIGHT;
static int LEFT_OFFSET;
static int SECOND_WIDTH;
static int SECOND_HEIGHT;

static int _simTime;

FILE *_fp;
ALLEGRO_BITMAP *_reportBMP;

static int getTimeSpan(FILE *pFP);
static int parseLine(regex_t *pRegex, char *pLine, int *pEventType, int *pMartianId, int *pTime); 
static void assignRandomColors();
static void createBitmap();
void createGrid(ALLEGRO_FONT *pFont);

void saveReport();
ALLEGRO_BITMAP* generateReport(DynamicArray pColors, ALLEGRO_FONT *pFont);



#endif