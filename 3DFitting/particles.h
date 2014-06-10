#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "glparameters.h"
#include "BuildClothModel.h"

#define PTCAMT 9600


#define RECTRATIO 40
#define MASS 0.3
extern const GLfloat CollideCoefficient;
//coefficient of stiffness
extern const GLfloat StructStiff;
extern const GLfloat ShearStiff;
extern const GLfloat BendStiff;
extern const GLfloat AccumulateStiff;
extern const GLfloat DampCoefficient;
extern const GLfloat Mass;

extern M3DVector3f sphereCentre; //centre of the sphere
extern GLfloat sphereRatio; //ratio of the sphere
extern GLfloat tableAltitude; //height of table surface
extern GLfloat minForce;
//extern const GLfloat collisionA;
//extern const GLfloat collisionB;



struct Particle {
	int num;
	GLfloat Mass;
	M3DVector3f position, pre_position, next_position;
	//M3DVector3f velocity;
	M3DVector3f force;
	int structadj[4]; //structual constraint
	int shearadj[4]; //Shear constraint
	int bendadj[8]; //Bending constraint
	GLfloat structOrg; //origin length of structual spring
	GLfloat shearOrg; //origin length of shear spring
	GLfloat bendOrg; //origin length of bending spring
	bool exist;
};
extern Particle ClothLnk[PTCAMT];
extern M3DVector3f collideForce;

void initPtc(Particle *, int, GLfloat, M3DVector3f, int *, int *, int *);
void initPtc(Particle *, int, GLfloat, M3DVector3f, int *, int *, int *, bool);

void addSpringForce(M3DVector3f, M3DVector3f, GLfloat, M3DVector3f &);
void addSpringForce(const M3DVector3f, const M3DVector3f, const GLfloat, const GLfloat, M3DVector3f &);
void getInternalForce(int);
void getTensionForce(int , const M3DVector3f , GLfloat );
//verlet
inline bool isCollision(int, M3DVector3f, GLfloat);
bool isCollision(int);
void getCollisionForce(int);
bool adjustPositionByCollisionSphere( const M3DVector3f, M3DVector3f, const M3DVector3f , const GLfloat );
bool adjustPositionByCollisionCylinder( const M3DVector3f, M3DVector3f , const M3DVector3f , const M3DVector3f , const GLfloat );
bool adjustPositionByCollisionCube( const M3DVector3f, M3DVector3f , const M3DVector3f , const GLfloat , const GLfloat , const GLfloat );
bool adjustPositionByCollisionPlane(const M3DVector3f , M3DVector3f , const M3DVector3f , const M3DVector3f , const M3DVector3f );
void addForce(int, M3DVector3f);
//void stayOnSurface(int);
void init(void);
void makeImages(void);
inline void clearForce(int currentPtc) {
	m3dLoadVector3(ClothLnk[currentPtc].force, 0.0f, 0.0f, 0.0f);
}

void drawClothModel(void);
void initClothModel(void);

#endif