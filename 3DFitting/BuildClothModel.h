#pragma once

#ifndef __BUILSCLOTHMODEL_H__
#define __BUILSCLOTHMODEL_H__

#include "glparameters.h"

#define HEIGHTNUMBER 60
#define WIDNUMBER 40
#define EDGELENGTH 12
#define MODELX 0
#define MODELY 0
#define MODELZ 0
#define GAPLENGTH 150
#define COLLARRADIUS 25
#define CUFFLENGTH 20

using namespace std;

extern int number;
extern const GLfloat edge;
extern const int line;
extern M3DVector3f ClothCenter;

void fileOut(void);
int outFileClothModel(void);
void inFileClothSquare(void);
void inFileClothModel(void);
void  shiftClothModelPosition(M3DVector3f);
void getClothModelCenter(void);

#endif