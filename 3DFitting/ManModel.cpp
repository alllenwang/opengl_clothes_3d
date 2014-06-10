#include "ManModel.h"

using namespace std;

ManModel manModel;
const M3DVector3f headCenter = { 0.0f, 150.0f, 0.0f };
//const M3DVector3f neckCenter = {0.0f, 50.0f, 0.0f};
//const M3DVector3f bodyCenter = { 0.0f, 0.0f, 0.0f };
const GLfloat headRadius = 100.0f;
const GLfloat shoulderWidth = 800.0f;
const GLfloat shoulderRadius = 50.0f;
const GLfloat neckRadius = 20.0f;
const GLfloat armRadius = 60.0f;
const GLfloat bodyHeight = 2400.0f;

//const M3DVector3f shoulderWidth = { 

void initManModel() {
	m3dLoadVector3( manModel.head.center, headCenter[0], headCenter[1], headCenter[2]);
	manModel.head.radius = headRadius;

	m3dLoadVector3( manModel.neck.endpoint2, 0.0f,headCenter[1] + headRadius ,0.0f);
	m3dLoadVector3( manModel.neck.endpoint1, 0.0f,-headCenter[1] - headRadius , 0.0f);
	manModel.neck.radius = neckRadius;

	m3dLoadVector3( manModel.shoulder.endpoint1, shoulderWidth/2, 0.0f, 0.0f);
	m3dLoadVector3( manModel.shoulder.endpoint2, -shoulderWidth/2, 0.0f, 0.0f);
	manModel.shoulder.radius = shoulderRadius;
	
	m3dLoadVector3( manModel.leftShoulder.center, shoulderWidth/2, 0.0f, 0.0f );
	manModel.leftShoulder.radius = shoulderRadius;
	m3dLoadVector3( manModel.rightShoulder.center, -shoulderWidth/2, 0.0f, 0.0f );
	manModel.rightShoulder.radius = shoulderRadius;

	m3dLoadVector3( manModel.body.center, 0.0f, -bodyHeight/2, 0.0f );
	manModel.body.ylength = bodyHeight;
	manModel.body.xlength = shoulderWidth/2 - 20.0f;
	manModel.body.zlength = shoulderRadius*2 - 50.0f;

}

void adjustPositionByCollision(const M3DVector3f curPos, M3DVector3f nextPos) {
		adjustPositionByCollisionCylinder( curPos, nextPos, manModel.shoulder.endpoint1, manModel.shoulder.endpoint2, manModel.shoulder.radius + 2.0f);
		adjustPositionByCollisionSphere( curPos, nextPos, manModel.leftShoulder.center, manModel.leftShoulder.radius + 2.0f);
		adjustPositionByCollisionSphere( curPos, nextPos, manModel.rightShoulder.center, manModel.rightShoulder.radius + 2.0f);
		adjustPositionByCollisionCylinder( curPos, nextPos, manModel.neck.endpoint1, manModel.neck.endpoint2, manModel.neck.radius + 2.0f);
		adjustPositionByCollisionCube( curPos, nextPos, manModel.body.center, manModel.body.xlength, manModel.body.ylength, manModel.body.zlength );
}

