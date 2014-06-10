#pragma once

#ifndef __MANMODEL_H__
#define __MANMODEL_H__

#include "particles.h"
#include <fstream>

struct Sphere {
	M3DVector3f center;
	GLfloat radius;
};

struct Cylinder {
	M3DVector3f endpoint1;
	M3DVector3f endpoint2;
	GLfloat radius;
};

struct Cube {
	M3DVector3f center;
	GLfloat ylength;
	GLfloat xlength;
	GLfloat zlength;
};

struct ManModel {
	Sphere head;
	Cylinder neck;
	Sphere leftShoulder;
	Sphere rightShoulder;
	Cylinder shoulder;
	Cylinder leftArm;
	Cylinder rightArm;
	Cube body;

};

extern ManModel manModel;
extern const M3DVector3f headCenter;
extern const GLfloat headRadius ;
extern const GLfloat shoulderWidth ;
extern const GLfloat shoulderRadius;
extern const GLfloat neckRadius ;
extern const GLfloat armRadius ;

void initManModel(void);
void adjustPositionByCollision(const M3DVector3f, M3DVector3f );

#endif