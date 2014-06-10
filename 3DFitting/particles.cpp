#include "particles.h"
#include <fstream>
#include <windows.h>
#include <GL/glut.h>
#include <GL/glext.h>
using namespace std;

PFNGLACTIVETEXTUREARBPROC glActiveTextureARB = NULL;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB = NULL;

// 纹理存储数组
static GLubyte T0[64][64][4];


// 纹理名字
static GLuint Tname0;


const GLfloat CollideCoefficient = 20.0f;
const GLfloat StructStiff = 2.0f;
const GLfloat ShearStiff = 0.8f;
const GLfloat BendStiff = 0.4f;
const GLfloat AccumulateStiff = 0.5f;
const GLfloat DampCoefficient = 0.2f;
const GLfloat Mass = 1.0f;
//const GLfloat collisionA = 1.0f;
//const GLfloat collisionB = 10.0f;

GLfloat sphereRatio = 120.0f; //ratio of the sphere
M3DVector3f sphereCentre = { 400.0f, -sphereRatio, 400.0f }; //centre of the sphere
GLfloat tableAltitude; //height of table surface
GLfloat minForce = 20.0f;
M3DVector3f collideForce;
Particle ClothLnk[PTCAMT];

//bool init=false;
void makeImages()
{
	int i, j, c;

	// 第一个纹理，渐变绿色，*64
	for (i = 0; i < 64; i++) {
		for (j = 0; j < 64; j++) {
			T0[i][j][0] = (GLubyte) 0;
			T0[i][j][1] = (GLubyte) i * 5 + j * 5;
			T0[i][j][2] = (GLubyte) 0;
			T0[i][j][3] = (GLubyte) 255;
		}
	}
}

void init()
{    
	// 获取函数地址
	glActiveTextureARB  = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");

	// 场景初始化
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	makeImages(); // 纹理初始化
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 对其像素字节

	glGenTextures(1, &Tname0); // 获取纹理名字


	// 初始化纹理
	glBindTexture(GL_TEXTURE_2D, Tname0); // 生成指定名字的纹理对象
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, T0); // 设置纹理对象内容
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 设置纹理过滤方式
	
}
void initPtc(Particle *ptc, int n, GLfloat m, M3DVector3f p, int *structa, int *sheara, int *benda) {
	ptc->num = n;
	ptc->Mass = m;
	m3dCopyVector3(ptc->position, p);
	m3dCopyVector3(ptc->pre_position, ptc->position);
	m3dCopyVector3(ptc->next_position, ptc->position);
	//m3dLoadVector3(ptc->velocity, 0.0f, 0.0f, 0.0f);
	m3dLoadVector3(ptc->force, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; i ++)
		ptc->structadj[i] = structa[i];
	for (int i = 0; i < 4; i ++)
		ptc->shearadj[i] = sheara[i];
	for (int i = 0; i < 8; i ++)
		ptc->bendadj[i] = benda[i];
	ptc->structOrg = EDGELENGTH;
	ptc->shearOrg = EDGELENGTH * 1.414f;
	ptc->bendOrg = EDGELENGTH * 2.828f;
}
void initPtc(Particle *ptc, int n, GLfloat m, M3DVector3f p, int *structa, int *sheara, int *benda, bool e) {
	ptc->num = n;
	ptc->Mass = m;
	m3dCopyVector3(ptc->position, p);
	m3dCopyVector3(ptc->pre_position, ptc->position);
	m3dCopyVector3(ptc->next_position, ptc->position);
	//m3dLoadVector3(ptc->velocity, 0.0f, 0.0f, 0.0f);
	m3dLoadVector3(ptc->force, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; i ++)
		ptc->structadj[i] = structa[i];
	for (int i = 0; i < 4; i ++)
		ptc->shearadj[i] = sheara[i];
	for (int i = 0; i < 8; i ++)
		ptc->bendadj[i] = benda[i];
	ptc->structOrg = EDGELENGTH;
	ptc->shearOrg = EDGELENGTH * 1.414f;
	ptc->bendOrg = EDGELENGTH * 2.828f;
	ptc->exist = e;
}

void initClothModel() {
	for (int i = 0; i < PTCAMT; i ++) {
		m3dLoadVector3(ClothLnk[i].force, 0.0f, 0.0f, 0.0f);
		m3dCopyVector3(ClothLnk[i].pre_position, ClothLnk[i].position);
		m3dCopyVector3(ClothLnk[i].next_position, ClothLnk[i].position);
	}
}

void addSpringForce(M3DVector3f curSpringState, M3DVector3f orgSpringState, GLfloat factor, M3DVector3f &force) {
	GLfloat length = m3dGetVectorLength(curSpringState);
	GLfloat delta = m3dGetVectorLength(orgSpringState) - m3dGetVectorLength(curSpringState);
	m3dScaleVector3(curSpringState, -1 * factor * delta / length);
	m3dAddVectors3(force, force, curSpringState);
}

void addSpringForce(const M3DVector3f springHead, const M3DVector3f springTail, const GLfloat orgLength, const GLfloat factor, M3DVector3f &force) {
	M3DVector3f spring;
	m3dSubtractVectors3(spring, springTail, springHead);
	GLfloat length = m3dGetVectorLength(spring);
	GLfloat delta = orgLength - length;
	m3dScaleVector3(spring, factor * delta / length);
	m3dAddVectors3(force, force, spring);
}

void getInternalForce(int currentPtc) {
	//get structual constraint ClothLnk[currentPtc].force
	for (int i = 0; i < 4; i ++) {
		if (ClothLnk[currentPtc].structadj[i] >= 0) {
			addSpringForce(ClothLnk[ClothLnk[currentPtc].structadj[i]].position, ClothLnk[currentPtc].position, ClothLnk[currentPtc].structOrg, StructStiff,  ClothLnk[currentPtc].force);
		}
	}
	//if ((currentPtc>=44 && currentPtc<=46)||(currentPtc>=54 && currentPtc<=56)||(currentPtc>=64 && currentPtc<=66))
	//	printf("	[%d].struct		{%.1f, %.1f, %.1f} \n", currentPtc, temp[0], temp[1], temp[2]);
	//m3dAddVectors3(ClothLnk[currentPtc].force, ClothLnk[currentPtc].force, temp);

	//get shear constraint ClothLnk[currentPtc].force
	for (int i = 0; i < 4; i ++) {
		if (ClothLnk[currentPtc].shearadj[i] >= 0) {
			addSpringForce(ClothLnk[ClothLnk[currentPtc].shearadj[i]].position, ClothLnk[currentPtc].position, ClothLnk[currentPtc].shearOrg, ShearStiff,  ClothLnk[currentPtc].force);
		}
	}

	//get bending constraint ClothLnk[currentPtc].force
	for (int i = 0; i < 8; i ++) {
		if (ClothLnk[currentPtc].bendadj[i] >= 0) {
			addSpringForce(ClothLnk[ClothLnk[currentPtc].bendadj[i]].position, ClothLnk[currentPtc].position, ClothLnk[currentPtc].bendOrg, BendStiff,  ClothLnk[currentPtc].force);
		}
	}
}

void getTensionForce(int currentPtc, const M3DVector3f tensionPoint, GLfloat tensionFactor) {
	M3DVector3f temp;
	m3dSubtractVectors3(temp, ClothLnk[currentPtc].position, tensionPoint);
	GLfloat length = m3dGetVectorLength(temp);
	m3dScaleVector3(temp, tensionFactor*1000.0f/length);
	addForce(currentPtc, temp);
}

//bool isCollision(int currentPtc) {
//	return isCollision(currentPtc, sphereCentre, sphereRatio);
//}

//void getCollisionForce(int currentPtc) {
//	if ( isCollision(currentPtc) ) {
//		M3DVector3f temp;
//		float depth;
//		m3dSubtractVectors3(temp, ClothLnk[currentPtc].position, sphereCentre);
//		(GLfloat)depth = m3dGetVectorLength(temp);
//		m3dScaleVector3(temp, 1/depth); //unit vector of force
//		depth = sphereRatio - depth;
//		m3dScaleVector3(temp, CollideCoefficient * depth);
//		if ((currentPtc>=44 && currentPtc<=46)||(currentPtc>=54 && currentPtc<=56)||(currentPtc>=64 && currentPtc<=66))
//			printf("	[%d].collide		{%.1f, %.1f, %.1f} \n", currentPtc, temp[0], temp[1], temp[2]);
//		m3dAddVectors3(ClothLnk[currentPtc].force, ClothLnk[currentPtc].force, temp);
//	}
//}

bool adjustPositionByCollisionPlane(const M3DVector3f curPos, M3DVector3f nextPos, const M3DVector3f v0, const M3DVector3f v1, const M3DVector3f v2){
	M3DVector3f normal, curDirect, nextDirect, crossPos, S, side1, side2, side3;
	m3dFindNormal(normal, v0, v1, v2);
	m3dSubtractVectors3(curDirect, curPos, v0);
	m3dSubtractVectors3(nextDirect, nextPos, v0);
	m3dSubtractVectors3(side1, v1, v0);
	m3dSubtractVectors3(side2, v2, v0);
	m3dSubtractVectors3(side3, v2, v1);
	GLfloat factor, s0, s1, s2, s3;

	//the point goes through the plane
	if(m3dDotProduct(normal, curDirect) * m3dDotProduct(normal, nextDirect) < 0){
		m3dSubtractVectors3(nextDirect, nextDirect, curDirect);
		factor = - m3dDotProduct(normal, curDirect) / m3dDotProduct(normal, nextDirect);
		m3dScaleVector3(nextDirect, factor);
		m3dAddVectors3(nextDirect, nextDirect, curDirect);
		m3dAddVectors3(crossPos, v0, nextDirect);

		m3dCrossProduct(S, side1, side2);
		s0 = m3dGetVectorLength(S);
		m3dCrossProduct(S, side1, nextDirect);
		s1 = m3dGetVectorLength(S);
		m3dCrossProduct(S, side2, nextDirect);
		s2 = m3dGetVectorLength(S);
		m3dSubtractVectors3(nextDirect, crossPos, v1);
		m3dCrossProduct(S, side3, nextDirect);
		s3 = m3dGetVectorLength(S);

		//the point goes through the triangle
		if(s1 + s2 + s3 - s0 < 0.01){
			//m3dCopyVector3(nextPos, curPos);
			m3dScaleVector3(normal, 0.000001);
			m3dAddVectors3(nextPos, crossPos, normal);
			return true ;
		}
		else
			return false ;
	}
	else
		return false ;
}

//adjust destinate position, return whether colllided
bool adjustPositionByCollisionSphere(const M3DVector3f curPos, M3DVector3f nextPos, const M3DVector3f sphereCenter, const GLfloat radius)
{
	GLfloat distance = m3dGetDistance(nextPos, sphereCenter);
	if (distance < radius) {//collide
		M3DVector3f displace;
		m3dSubtractVectors3(displace, nextPos, sphereCenter);
		m3dScaleVector3(displace, (radius+0.1f)/distance);
		m3dAddVectors3(nextPos, sphereCenter, displace);	
		return true;
	} else {//not collide
		return false;
	}
}

//adjust destinate position, return whether colllided
bool adjustPositionByCollisionCube(const M3DVector3f curPos, M3DVector3f nextPos, const M3DVector3f cubeCenter, const GLfloat xLength, const GLfloat yLength, const GLfloat zLength)
{
	if(abs(nextPos[0] - cubeCenter[0]) > xLength / 2.0 || abs(nextPos[1] - cubeCenter[1]) > yLength / 2.0 || abs(nextPos[2] - cubeCenter[2]) > zLength / 2.0)
		return false;
	M3DVector3f point1 = {-xLength/2 , yLength/2 , zLength/2};
	M3DVector3f point2 = {xLength/2 , yLength/2 , zLength/2};
	M3DVector3f point3 = {xLength/2 , yLength/2 , -zLength/2};
	M3DVector3f point4 = {-xLength/2 , yLength/2 ,-zLength/2};
	M3DVector3f point5 = {-xLength/2 , -yLength/2 , zLength/2};
	M3DVector3f point6 = {xLength/2 , -yLength/2 , zLength/2};
	M3DVector3f point7 = {xLength/2 , -yLength/2 , -zLength/2};
	M3DVector3f point8 = {-xLength/2 , -yLength/2 , -zLength/2};
	m3dAddVectors3(point1, cubeCenter,point1);
	m3dAddVectors3(point2, cubeCenter,point2);
	m3dAddVectors3(point3, cubeCenter,point3);
	m3dAddVectors3(point4, cubeCenter,point4);
	m3dAddVectors3(point5, cubeCenter,point5);
	m3dAddVectors3(point6, cubeCenter,point6);
	m3dAddVectors3(point7, cubeCenter,point7);
	m3dAddVectors3(point8, cubeCenter,point8);

	M3DVector3f v;
	m3dSubtractVectors3(v , nextPos , curPos);
	GLfloat t = (point1[1] - curPos[1])/v[1];
	//printf("%f %f %f \n",point1[0],point1[1],point1[2]);
	//printf("%f %f %f \n",point2[0],point2[1],point2[2]);
	//printf("%f %f %f \n",point3[0],point3[1],point3[2]);
	//printf("%f %f %f \n",point4[0],point4[1],point4[2]);
	//printf("%f %f %f \n",point5[0],point5[1],point5[2]);
	//printf("%f %f %f \n",point6[0],point6[1],point6[2]);
	//printf("%f %f %f \n",point7[0],point7[1],point7[2]);
	//printf("%f %f %f \n",point8[0],point8[1],point8[2]);
	//printf("---------------\n");
	if(t > 0 && t < 1 )
	{
		//	printf("---------------\n");
		m3dScaleVector3(v,t);
		M3DVector3f point;
		m3dAddVectors3(point,curPos,v);
		if(point[0] > point1[0] && point[0] < point2[0] && point[2] > point4[2] && point[2] < point1[2])
		{
			//	printf("%f\n",nextPos[1]);
			nextPos[1] = point1[1] + 1.0f;
			//	printf("%f\n",nextPos[1]);
			return true;
		}
	}

	m3dSubtractVectors3(v , nextPos , curPos);
	t = (point5[1] - curPos[1])/v[1];
	if(t > 0 && t < 1 )
	{

		m3dScaleVector3(v,t);
		M3DVector3f point;
		m3dAddVectors3(point,curPos,v);
		if(point[0] > point5[0] && point[0] < point6[0] && point[2] > point8[2] && point[2] < point5[2])
		{

			nextPos[1] = point5[1] - 0.1f;

			return true;
		}
	}

	m3dSubtractVectors3(v , nextPos , curPos);
	t = (point1[0] - curPos[0])/v[0];
	if(t > 0 && t < 1 )
	{
		m3dScaleVector3(v,t);
		M3DVector3f point;
		m3dAddVectors3(point,curPos,v);
		if(point[1] < point1[1] && point[1] > point5[1] && point[2] > point4[2] && point[2] < point1[2])
		{
			nextPos[0] = point1[0] - 0.1f;
			return true;
		}
	}

	m3dSubtractVectors3(v , nextPos , curPos);
	t = (point2[0] - curPos[0])/v[0];
	if(t > 0 && t < 1 )
	{
		m3dScaleVector3(v,t);
		M3DVector3f point;
		m3dAddVectors3(point,curPos,v);
		if(point[1] > point6[1] && point[1] < point2[1] && point[2] > point7[2] && point[2] < point6[2])
		{
			nextPos[0] = point2[0] + 0.1f;
			return true;
		}
	}

	m3dSubtractVectors3(v , nextPos , curPos);
	t = (point1[2] - curPos[2])/v[2];
	if(t > 0 && t < 1 )
	{
		m3dScaleVector3(v,t);
		M3DVector3f point;
		m3dAddVectors3(point,curPos,v);
		if(point[0] > point1[0] && point[0] < point2[0] && point[1] > point5[1] && point[1] < point1[1])
		{
			nextPos[2] = point1[2]+0.1f;
			return true;
		}
	}

	m3dSubtractVectors3(v , nextPos , curPos);
	t = (point4[2] - curPos[2])/v[2];
	if(t > 0 && t < 1 )
	{
		m3dScaleVector3(v,t);
		M3DVector3f point;
		m3dAddVectors3(point,curPos,v);
		if(point[0] > point8[0] && point[0] < point7[0] && point[1] > point8[1] && point[1] < point4[1])
		{
			nextPos[2] = point4[2]-0.1f;
			return true;
		}
	}
	return false;
}

//void stayOnSurface(int currentPtc) {
//	if ( isCollision(currentPtc) ) {
//		M3DVector3f temp;
//		float length;
//		m3dSubtractVectors3(temp, ClothLnk[currentPtc].position, sphereCentre);
//		(GLfloat)length = m3dGetVectorLength(temp);
//		m3dScaleVector3(temp, sphereRatio/length); //unit vector of force
//		m3dAddVectors3(ClothLnk[currentPtc].next_position, sphereCentre, temp);
//	}
//}

bool adjustPositionByCollisionCylinder(const M3DVector3f curPos, M3DVector3f nextPos, const M3DVector3f endpoint1, const M3DVector3f endpoint2, const GLfloat radius) {
	M3DVector3f v1, v0, v2, v3;
	m3dSubtractVectors3(v0, endpoint2, endpoint1);
	m3dSubtractVectors3(v3, endpoint1, endpoint2);
	m3dSubtractVectors3(v1, nextPos, endpoint1);
	m3dSubtractVectors3(v2, nextPos, endpoint2);
	if(m3dDotProduct(v1,v0) < 0  || m3dDotProduct(v2,v3) < 0 )
		return false;
	GLfloat distance0 = m3dGetDistance(endpoint2, endpoint1);
	GLfloat distance1 = m3dGetDistance(nextPos, endpoint1);
	m3dScaleVector3(v0, 1/distance0);
	m3dScaleVector3(v1, 1/distance1);
	//get deep
	GLfloat angle = m3dGetAngleBetweenVectors( v1, v0);
	GLfloat tempLength = m3dGetDistance(nextPos, endpoint1);
	GLfloat deep = tempLength * sin(angle);

	if (deep < radius) 
	{//collide
		//get ortho

		GLfloat distance = tempLength * cos(angle);
		//GLfloat height = m3dGetVectorLengthSquared(v0);
		m3dScaleVector3(v0, distance);
		M3DVector3f ortho;
		m3dAddVectors3( ortho, v0 , endpoint1);
		//get point
		m3dSubtractVectors3( v1, nextPos, ortho);
		tempLength = m3dGetVectorLength(v1);
		m3dScaleVector3(v1, (radius+0.1f)/tempLength );
		m3dAddVectors3(nextPos, v1, ortho);

		return true;
	} else {//not collide
		return false;
	}
}

//void stayOnSurface(int currentPtc) {
//	if ( isCollision(currentPtc) ) {
//		M3DVector3f temp;
//		float length;
//		m3dSubtractVectors3(temp, ClothLnk[currentPtc].position, sphereCentre);
//		(GLfloat)length = m3dGetVectorLength(temp);
//		m3dScaleVector3(temp, sphereRatio/length); //unit vector of force
//		m3dAddVectors3(ClothLnk[currentPtc].next_position, sphereCentre, temp);
//	}
//}

void addForce(int currentPtc, M3DVector3f f) {
	m3dAddVectors3(ClothLnk[currentPtc].force, ClothLnk[currentPtc].force, f);
}

void drawCloth(void) { //deprecated
	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	//glEnable(GL_POINT_SIZE);
	//glPointSize(5.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glColor3f(0.7f, 0.5f, 0.5f);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < PTCAMT; i ++) {
		if (ClothLnk[i].structadj[1] != -1 && ClothLnk[i].structadj[3] != -1) {
			glVertex3fv(ClothLnk[i].position);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[1]].position);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[3]].position);
			M3DVector3f normal, temp;
			//m3dCrossProduct(normal, ClothLnk[ClothLnk[i].structadj[1]].position, ClothLnk[ClothLnk[i].structadj[0]].position);
			//m3dCrossProduct(temp, ClothLnk[ClothLnk[i].structadj[3]].position, ClothLnk[ClothLnk[i].structadj[2]].position);
			//m3dAddVectors3(normal, normal, temp);
			m3dFindNormal(normal, ClothLnk[i].position, ClothLnk[ClothLnk[i].structadj[1]].position, ClothLnk[ClothLnk[i].structadj[3]].position);
			glNormal3f(normal[0], normal[1], normal[2]);
		}
		if ( i % line != 0 && i >= line && ClothLnk[i].structadj[0] != -1 && ClothLnk[2].structadj[3] != -1) {
			glVertex3fv(ClothLnk[i].position);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[0]].position);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[2]].position);
			M3DVector3f normal;
			//m3dCrossProduct(normal, ClothLnk[ClothLnk[i].structadj[1]].position, ClothLnk[ClothLnk[i].structadj[0]].position);
			//m3dCrossProduct(temp, ClothLnk[ClothLnk[i].structadj[3]].position, ClothLnk[ClothLnk[i].structadj[2]].position);
			//m3dAddVectors3(normal, normal, temp);
			m3dFindNormal(normal, ClothLnk[i].position, ClothLnk[ClothLnk[i].structadj[0]].position, ClothLnk[ClothLnk[i].structadj[2]].position);
			glNormal3f(normal[0], normal[1], normal[2]);
		}

	}
	glEnd();
	glPopMatrix();
}


void drawClothModel(void) {

	

		
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_POINT_SIZE);
	glPointSize(3.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glColor3f(0.7f, 0.5f, 0.5f);

	glActiveTextureARB(GL_TEXTURE0_ARB); // 选中号纹理
	glEnable(GL_TEXTURE_2D); // 使纹理可用
	glBindTexture(GL_TEXTURE_2D, Tname0); // 激活纹理
	glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // 设置纹理贴图方式

	glBegin(GL_TRIANGLES);

	M3DVector3f normal[PTCAMT];// number of vertexs
	M3DVector3f normal1[PTCAMT];// number of vertexs
	M3DVector3f normal2[PTCAMT];// number of vertexs

	for(int i = 0; i < PTCAMT; i++)
		{
		
			if(sqrt(m3dGetDistanceSquared(ClothLnk[i].position,ClothLnk[ClothLnk[i].structadj[3]].position)) > 40.0f
				&& (i % 40 > 18 && i % 40 < 20) && ClothLnk[i].exist && ClothLnk[i].structadj[3] > 0)
			{
			
				
				ClothLnk[number].exist = 1;
				ClothLnk[number].position[0] = ClothLnk[i].position[0] + 20.0f;
				ClothLnk[number].position[1] =  ClothLnk[i].position[1];
				ClothLnk[number].position[2] = ClothLnk[i].position[2];
			
				ClothLnk[number].structadj[0] = ClothLnk[i+1].structadj[0];
				
				ClothLnk[number].structadj[1] = ClothLnk[i].shearadj[1];
				if(number == 4800)
					ClothLnk[number].structadj[1] = -1;
				ClothLnk[number].structadj[2] = i;
				ClothLnk[number].structadj[3] = -1;

				ClothLnk[number].shearadj[0] = ClothLnk[i].structadj[1];
				ClothLnk[number].shearadj[1] = -1;
				ClothLnk[number].shearadj[2] = ClothLnk[i].structadj[0];
				ClothLnk[number].shearadj[3] = -1;

				ClothLnk[number].bendadj[0] = ClothLnk[i-1].bendadj[1];
				ClothLnk[number].bendadj[1] = ClothLnk[i-1].bendadj[2];
				ClothLnk[number].bendadj[2] = -1;
				ClothLnk[number].bendadj[3] = i-1;
				ClothLnk[number].bendadj[4] = -1;
				ClothLnk[number].bendadj[5] = ClothLnk[i+1].bendadj[5];
				ClothLnk[number].bendadj[6] = ClothLnk[i+1].bendadj[6];
				ClothLnk[number].bendadj[7] = -1;
			
				if(ClothLnk[i].shearadj[3] > 0)
				{
					ClothLnk[ClothLnk[i].shearadj[3]].structadj[1] = number;
					ClothLnk[ClothLnk[i].shearadj[3]].bendadj[1] = -1;
				}
				
				if(ClothLnk[i].shearadj[1] > 0)
				{
					ClothLnk[ClothLnk[i].shearadj[1]].structadj[0] = number;
				}
				
				ClothLnk[2580].structadj[0] = 2620;
				ClothLnk[i].structadj[3] = number;
				ClothLnk[i].bendadj[2] = -1;
				ClothLnk[i].bendadj[4] = -1;
				ClothLnk[i].bendadj[7] = -1;

				if(ClothLnk[i].structadj[0] > 0)
				{
					ClothLnk[ClothLnk[i].structadj[0]].shearadj[1] = number;
					ClothLnk[ClothLnk[i].structadj[0]].bendadj[2] = -1;
					ClothLnk[ClothLnk[i].structadj[0]].bendadj[4] = -1;
				}
				if(ClothLnk[i].structadj[1] > 0)
				{
					ClothLnk[ClothLnk[i].structadj[1]].shearadj[3] = number;
					ClothLnk[ClothLnk[i].structadj[1]].bendadj[7] = -1;
					ClothLnk[ClothLnk[i].structadj[1]].bendadj[4] = -1;
				}
				if(ClothLnk[i-1].bendadj[1] > 0)
				ClothLnk[ClothLnk[i-1].bendadj[1]].bendadj[7] = number;

				ClothLnk[i-1].bendadj[4] = number;

				if(ClothLnk[i-1].bendadj[6] > 0)
					ClothLnk[ClothLnk[i-1].bendadj[6]].bendadj[2] = number;

				if(ClothLnk[i-1].bendadj[7] > 0)
					ClothLnk[ClothLnk[i-1].bendadj[7]].bendadj[1] = -1;//////
				if(ClothLnk[i-1].bendadj[2] > 0)
					ClothLnk[ClothLnk[i-1].bendadj[2]].bendadj[6] = number;

			//
				
				if(ClothLnk[i].bendadj[1] > 0)
				ClothLnk[ClothLnk[i].bendadj[1]].bendadj[7] = -1;
				if(ClothLnk[i].bendadj[6] > 0)
				{
					ClothLnk[ClothLnk[i].bendadj[6]].bendadj[2] = -1;
				}


				ClothLnk[i+1].bendadj[0] = -1;
				ClothLnk[i+1].bendadj[3] = -1;
				ClothLnk[i+1].bendadj[5] = -1;
				ClothLnk[i+1].structadj[2] = -1;
				ClothLnk[i+1].shearadj[0] = -1;
				ClothLnk[i+1].shearadj[2] = -1;

				if(ClothLnk[i+2].structadj[1] > 0)
				{
					ClothLnk[ClothLnk[i+2].structadj[1]].bendadj[5] = -1; 
					ClothLnk[ClothLnk[i+2].structadj[1]].bendadj[3] = -1; 
				}

				if(ClothLnk[i+2].structadj[0] > 0)
				{
					ClothLnk[ClothLnk[i+2].structadj[0]].bendadj[0] = -1; 
					ClothLnk[ClothLnk[i+2].structadj[0]].bendadj[3] = -1; 
				}
				if(ClothLnk[i+2].bendadj[1] > 0)
				ClothLnk[ClothLnk[i+2].bendadj[1]].bendadj[5] = -1; 
				if(ClothLnk[i+2].bendadj[6] > 0)
				{
					ClothLnk[ClothLnk[i+2].bendadj[6]].bendadj[0] = -1;
				}

				ClothLnk[i+2].bendadj[0] = -1;
				ClothLnk[i+2].bendadj[3] = -1;
				ClothLnk[i+2].bendadj[5] = -1;

			/*	for(int i = 4800 ; i < number ; i ++ )
					printf("%d: %d %d %d %d--  %d %d %d %d\n",i,ClothLnk[i].structadj[0],ClothLnk[i].structadj[1],
															ClothLnk[i].structadj[2],ClothLnk[i].structadj[3],
															ClothLnk[i].shearadj[0],ClothLnk[i].shearadj[1],
															ClothLnk[i].shearadj[2],ClothLnk[i].shearadj[3]);
				
				printf("=====================================================================\n");
			*/
			for(int i = 2500 ; i < 2900 ; i ++ )
					if( i % 40 == 20)
				printf("%d: %d %d %d %d--  %d %d %d %d\n",i,ClothLnk[i].structadj[0],ClothLnk[i].structadj[1],
															ClothLnk[i].structadj[2],ClothLnk[i].structadj[3],
															ClothLnk[i].shearadj[0],ClothLnk[i].shearadj[1],
															ClothLnk[i].shearadj[2],ClothLnk[i].shearadj[3]);
				printf("=====================================================================\n");
				
				number ++;
				
			}
	}
	for (int i = 0; i < PTCAMT; i ++)
		//first kind of vertex	//
		//shape	   __				//
		//	      | /
		//        |/
		if (ClothLnk[i].structadj[0] != -1 && ClothLnk[i].structadj[3] != -1)
			m3dFindNormal(normal1[i], ClothLnk[i].position, ClothLnk[ClothLnk

			[i].structadj[3]].position, ClothLnk[ClothLnk[i].structadj[0]].position);
		else
			m3dLoadVector3(normal1[i], 0.0f, 0.0f, 0.0f);

	for (int i = 0; i < PTCAMT; i ++)
		//second kind of vertex //
		//shape	   	/|			//
		//	       /_|
		if (ClothLnk[i].structadj[2] != -1 && ClothLnk[i].structadj[1] != -1)
			m3dFindNormal(normal2[i], ClothLnk[i].position, ClothLnk[ClothLnk

			[i].structadj[2]].position, ClothLnk[ClothLnk[i].structadj[1]].position);
		else
			m3dLoadVector3(normal2[i], 0.0f, 0.0f, 0.0f);

	for(int i = 0; i <  PTCAMT; i++){
		if(i == 0){
			m3dCopyVector3(normal[i], normal1[0]);
		}
		else if(i > 0 && i < 39){
			m3dLoadVector3(normal[i], 0.0f, 0.0f, 0.0f);
			m3dAddVectors3(normal[i], normal[i], normal1[i-1]);
			m3dAddVectors3(normal[i], normal[i], normal1[i]);
			m3dAddVectors3(normal[i], normal[i], normal2[i-1]);
		}
		else if(i == 39){
			m3dLoadVector3(normal[i], 0.0f, 0.0f, 0.0f);
			m3dAddVectors3(normal[i], normal[i], normal1[i-1]);
			m3dAddVectors3(normal[i], normal[i], normal2[i-1]);
		}
		else if(i%40 == 0 && i != 1560){
			m3dLoadVector3(normal[i], 0.0f, 0.0f, 0.0f);
			m3dAddVectors3(normal[i], normal[i], normal1[i-40]);
			m3dAddVectors3(normal[i], normal[i], normal2[i-40]);
			m3dAddVectors3(normal[i], normal[i], normal1[i]);
		}
		else if(i == 1560){
			m3dLoadVector3(normal[i], 0.0f, 0.0f, 0.0f);
			m3dAddVectors3(normal[i], normal[i], normal1[i-40]);
			m3dAddVectors3(normal[i], normal[i], normal2[i-40]);
		}
		else if(i%40 == 39 && i != 1599){
			m3dLoadVector3(normal[i], 0.0f, 0.0f, 0.0f);
			m3dAddVectors3(normal[i], normal[i], normal2[i-41]);
			m3dAddVectors3(normal[i], normal[i], normal1[i-1]);
			m3dAddVectors3(normal[i], normal[i], normal2[i]);
		}
		else if(i == 1599){
			m3dCopyVector3(normal[i], normal2[1598]);
		}
		else{
			m3dLoadVector3(normal[i], 0.0f, 0.0f, 0.0f);
			m3dAddVectors3(normal[i], normal[i], normal2[i-1]);
			m3dAddVectors3(normal[i], normal[i], normal2[i-40]);
			m3dAddVectors3(normal[i], normal[i], normal2[i-41]);
			m3dAddVectors3(normal[i], normal[i], normal1[i-1]);
			m3dAddVectors3(normal[i], normal[i], normal1[i-40]);
			m3dAddVectors3(normal[i], normal[i], normal1[i-41]);
		}
	}

	
	
/*			else
			{
				glNormal3fv(normal[i]);
				glVertex3fv(ClothLnk[i].position);	
				glNormal3fv(normal[ClothLnk[i].structadj[3]]);
				glVertex3fv(ClothLnk[ClothLnk[i].structadj[3]].position);
				glNormal3fv(normal[ClothLnk[i].structadj[0]]);
				glVertex3fv(ClothLnk[ClothLnk[i].structadj[0]].position);
			}
		}
		else if((i % WIDNUMBER == WIDNUMBER - 1 && i <WIDNUMBER * HEIGHTNUMBER &&
			ClothLnk[i].structadj[0] != -1 && ClothLnk[i].structadj[3] != -1))
		{
			glNormal3fv(normal[i]);
			glVertex3fv(ClothLnk[i].position);
			glNormal3fv(normal[ClothLnk[i].structadj[0]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[0]].position);		
			glNormal3fv(normal[ClothLnk[i].structadj[3]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[3]].position);

		}
		else if(i % WIDNUMBER == WIDNUMBER - 1 && i >=WIDNUMBER * HEIGHTNUMBER && 
			ClothLnk[i].structadj[1] != -1 && ClothLnk[i].structadj[3] != -1)
		{
			glNormal3fv(normal[i]);
			glVertex3fv(ClothLnk[i].position);
			glNormal3fv(normal[ClothLnk[i].structadj[1]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[1]].position);		
			glNormal3fv(normal[ClothLnk[i].structadj[3]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[3]].position);
		}
	}
*/
/*	for (int i = 0; i < PTCAMT; i ++){
		if (ClothLnk[i].structadj[2] != -1 && ClothLnk[i].structadj[1] != -1
			&&!(i < WIDNUMBER && i >= 0)&&!(i >= WIDNUMBER * HEIGHTNUMBER && i % 
			WIDNUMBER == 0))
		{ 
			if(sqrt(m3dGetDistanceSquared(ClothLnk[i].position,ClothLnk[ClothLnk[i].structadj[2]].position)) > 40.0f
				&& (i % 40 > 10 && i % 40 < 30))
			{
				ClothLnk[i].structadj[2] = -1;
				ClothLnk[i].bendadj[3] = -1;
				ClothLnk[i+1].bendadj[3] = -1;
				ClothLnk[i-40].shearadj[0] = -1;
				if((i>2359 && i < 2400)&&(i>4759 && i < 4800));
				else
					ClothLnk[i+40].shearadj[2] = -1;
				ClothLnk[i-79].bendadj[5] = -1;
					if((i>2319 && i < 2400)&&(i>4719 && i < 4800));
				else
					ClothLnk[i+81].bendadj[0] = -1;
			
			}
			else
			{
				glNormal3fv(normal[i]);
				glVertex3fv(ClothLnk[i].position);
				glNormal3fv(normal[ClothLnk[i].structadj[2]]);
				glVertex3fv(ClothLnk[ClothLnk[i].structadj[2]].position);
				glNormal3fv(normal[ClothLnk[i].structadj[1]]);
				glVertex3fv(ClothLnk[ClothLnk[i].structadj[1]].position);
			}

		}
		else if (ClothLnk[i].structadj[3] != -1 && ClothLnk[i].structadj[1] != -1
			&&(i < WIDNUMBER && i >= 0))
		{
			glNormal3fv(normal[i]);
			glVertex3fv(ClothLnk[i].position);
			glNormal3fv(normal[ClothLnk[i].structadj[3]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[3]].position);
			glNormal3fv(normal[ClothLnk[i].structadj[1]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[1]].position);
		}
		else if (ClothLnk[i].structadj[2] != -1 && ClothLnk[i].structadj[0] != -1
			&&i >= WIDNUMBER * HEIGHTNUMBER && i % WIDNUMBER == 0)
		{
			glNormal3fv(normal[i]);
			glVertex3fv(ClothLnk[i].position);
			glNormal3fv(normal[ClothLnk[i].structadj[2]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[2]].position);
			glNormal3fv(normal[ClothLnk[i].structadj[0]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[0]].position);
		}
	}	
	*/

		
	
//////////////////////useful
	

	for(int i = 0; i < PTCAMT; i++){
		if (ClothLnk[i].structadj[0] != -1 && ClothLnk[i].structadj[3] != -1 &&
			i % WIDNUMBER != WIDNUMBER - 1 && ClothLnk[i].exist)
		{
			//printf("%f\n",sqrt(m3dGetDistanceSquared(ClothLnk[i].position,ClothLnk[ClothLnk[i].structadj[3]].position)));
				glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 0.0, 0.0);/////////////
				glNormal3fv(normal[i]);
				glVertex3fv(ClothLnk[i].position);	

				glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 0.0, 1.0);
				glNormal3fv(normal[ClothLnk[i].structadj[3]]);
				glVertex3fv(ClothLnk[ClothLnk[i].structadj[3]].position);

				glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 1.0, 0.0);
				glNormal3fv(normal[ClothLnk[i].structadj[0]]);
				glVertex3fv(ClothLnk[ClothLnk[i].structadj[0]].position);

		}
		else if((i % WIDNUMBER == WIDNUMBER - 1 && i <WIDNUMBER * HEIGHTNUMBER &&
			ClothLnk[i].structadj[0] != -1 && ClothLnk[i].structadj[3] != -1)&& ClothLnk[i].exist)
		{
			glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 0.0, 0.0);
			glNormal3fv(normal[i]);
			glVertex3fv(ClothLnk[i].position);

			glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 1.0, 0.0);
			glNormal3fv(normal[ClothLnk[i].structadj[0]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[0]].position);

			glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 0.0, 1.0);
			glNormal3fv(normal[ClothLnk[i].structadj[3]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[3]].position);

		}
		else if(i % WIDNUMBER == WIDNUMBER - 1 && i >=WIDNUMBER * HEIGHTNUMBER &&   
			ClothLnk[i].structadj[1] != -1 && ClothLnk[i].structadj[3] != -1&& ClothLnk[i].exist)
		{
			glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 1.0, 0.0);
			glNormal3fv(normal[i]);
			glVertex3fv(ClothLnk[i].position);

			glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 0.0, 0.0);
			glNormal3fv(normal[ClothLnk[i].structadj[1]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[1]].position);	

			glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 1.0, 1.0);
			glNormal3fv(normal[ClothLnk[i].structadj[3]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[3]].position);
		}
	}
	
	////////////////////////
	
	for (int i = 0; i < PTCAMT; i ++){
		if (ClothLnk[i].structadj[2] != -1 && ClothLnk[i].structadj[1] != -1
			&&!(i < WIDNUMBER && i >= 0)&&!(i >= WIDNUMBER * HEIGHTNUMBER && i % 
			WIDNUMBER == 0)&& ClothLnk[i].exist)
		{ 

			glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 1.0, 1.0);
			glNormal3fv(normal[i]);
			glVertex3fv(ClothLnk[i].position);

			glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 1.0, 0.0);
			glNormal3fv(normal[ClothLnk[i].structadj[2]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[2]].position);

			glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 0.0, 1.0);
			glNormal3fv(normal[ClothLnk[i].structadj[1]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[1]].position);

		}
		else if (ClothLnk[i].structadj[3] != -1 && ClothLnk[i].structadj[1] != -1
			&&(i < WIDNUMBER && i >= 0)&& ClothLnk[i].exist)
		{
			glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 1.0, 0.0);
			glNormal3fv(normal[i]);
			glVertex3fv(ClothLnk[i].position);

			glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 1.0, 1.0);
			glNormal3fv(normal[ClothLnk[i].structadj[3]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[3]].position);

			glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 0.0, 0.0);
			glNormal3fv(normal[ClothLnk[i].structadj[1]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[1]].position);
		}
		else if (ClothLnk[i].structadj[2] != -1 && ClothLnk[i].structadj[0] != -1
			&&i >= WIDNUMBER * HEIGHTNUMBER && i % WIDNUMBER == 0 && ClothLnk[i].exist)
		{
			glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 0.0, 1.0);
			glNormal3fv(normal[i]);
			glVertex3fv(ClothLnk[i].position);

			glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 0.0, 0.0);
			glNormal3fv(normal[ClothLnk[i].structadj[2]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[2]].position);

			glMultiTexCoord2fARB (GL_TEXTURE0_ARB, 1.0, 1.0);
			glNormal3fv(normal[ClothLnk[i].structadj[0]]);
			glVertex3fv(ClothLnk[ClothLnk[i].structadj[0]].position);
		}
	}	
	glEnd();

	glDisable(GL_TEXTURE_2D);

	// 反激活纹理，防止干扰
	glActiveTextureARB (GL_TEXTURE0_ARB); 
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB (GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	
	// 同上，绘制正方形，使用号和号纹理
	
	glFlush();
	
	
	//printf("%d\n",number);
/*	for(int i = 0 ; i < number ; i ++)
	{
		if(i/40 == 0)
			glColor3f(0.0,0.0,1.0);
		glBegin(GL_POINTS);
		if(i < 4800)
			glColor3f(0.0,1.0,0.0);
		else
			glColor3f(0.0,0.0,1.0);
		if(i/40 == 0)
			glColor3f(0.0,0.0,1.0);
		if(ClothLnk[i].exist == 1)
			glVertex3f(ClothLnk[i].position[0],ClothLnk[i].position[1],ClothLnk[i].position[2]);
		glEnd();
	}
	*/

		
	
}

//void drawClothModel(void) {
//	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
//	glEnable(GL_POINT_SIZE);
//	glPointSize(2.0f);
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	//glShadeModel(GL_SMOOTH);
//	//glEnable(GL_NORMALIZE);
//	//glColor3f(0.7f, 0.5f, 0.5f);
//
//	//glBegin(GL_POINTS);
//	//for (int i = 0; i < PTCAMT; i ++) {
//	//	if (i == 15)
//	//		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
//	//	glVertex3fv(ClothLnk[i].position);
//	//	//glVertex3fv(ClothLnk[ClothLnk[i].structadj[1]].position);
//	//	//glVertex3fv(ClothLnk[ClothLnk[i].structadj[3]].position);
//	//}
//	//glEnd();
//
//	glBegin(GL_LINES);
//	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
//	for (int i = 0; i < PTCAMT; i ++) {
//		for (int j = 0; j < 8; j ++) {
//			if (ClothLnk[i].structadj[j] >= 0 && j == 0) {
//				glVertex3fv(ClothLnk[i].position);
//				glVertex3fv(ClothLnk[ClothLnk[i].structadj[j]].position);
//			}
//		}
//	}
//		//glVertex3fv(ClothLnk[ClothLnk[i].structadj[1]].position);
//		//glVertex3fv(ClothLnk[ClothLnk[i].structadj[3]].position);
//
//		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
//		for (int i = 0; i < PTCAMT; i ++) {
//			for (int j = 0; j < 8; j ++) {
//				if (ClothLnk[i].structadj[j] >= 0 && j == 1) {
//					glVertex3fv(ClothLnk[i].position);
//					glVertex3fv(ClothLnk[ClothLnk[i].structadj[j]].position);
//				}
//			}
//		}
//		glEnd();
//
//		/*glColor3f(0.7f, 0.5f, 0.5f);
//		glBegin(GL_TRIANGLES);
//		for (int i = 0; i < PTCAMT; i ++) {
//		if (ClothLnk[i].structadj[1] != -1 && ClothLnk[i].structadj[3] != -1) {
//		glVertex3fv(ClothLnk[i].position);
//		glVertex3fv(ClothLnk[ClothLnk[i].structadj[1]].position);
//		glVertex3fv(ClothLnk[ClothLnk[i].structadj[3]].position);
//		//M3DVector3f normal, temp;
//
//		//m3dFindNormal(normal, ClothLnk[i].position, ClothLnk[ClothLnk[i].structadj[1]].position, ClothLnk[ClothLnk[i].structadj[3]].position);
//		//glNormal3f(normal[0], normal[1], normal[2]);
//		}
//		if ( i % line != 0 && i >= line && ClothLnk[i].structadj[0] != -1 && ClothLnk[2].structadj[3] != -1) {
//		glVertex3fv(ClothLnk[i].position);
//		glVertex3fv(ClothLnk[ClothLnk[i].structadj[0]].position);
//		glVertex3fv(ClothLnk[ClothLnk[i].structadj[2]].position);
//		//M3DVector3f normal, temp;
//
//		//m3dFindNormal(normal, ClothLnk[i].position, ClothLnk[ClothLnk[i].structadj[0]].position, ClothLnk[ClothLnk[i].structadj[2]].position);
//		//glNormal3f(normal[0], normal[1], normal[2]);
//		}
//
//		}
//		glEnd();*/
//
//		glPopMatrix();
//		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
//	}