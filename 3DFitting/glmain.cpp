#include "glparameters.h"
#include "BuildClothModel.h"
#include "particles.h"
#include "effect.h"
#include "ManModel.h"
#include <windows.h>
#include <GL/glut.h>
#include <GL/glext.h>
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zRot = 0.0f;
static GLfloat step = 5.0f;
const int refreshStep = 1;



//LIGHT0
GLfloat ambientLight0[] = {0.3f, 0.3f, 0.3f, 1.0f};
GLfloat diffuseLight0[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat specular0[] = {0.1f, 0.1f, 0.1f, 1.0f};
GLfloat position0[] = {-1000.0f, 1000.0f, 100.0f};

void preDraw(void) {
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);

	glEnable(GL_LINE_WIDTH);
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex4f(-1000.0f, 0.0f, 0.0f, 1.0f);
	glVertex4f(1000.0f, 0.0f, 0.0f, 1.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex4f(0.0f, -1000.0f, 0.0f, 1.0f);
	glVertex4f(0.0f, 1000.0f, 0.0f, 1.0f);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex4f(0.0f, 0.0f, -1000.0f, 1.0f);
	glVertex4f(0.0f, 0.0f, 1000.0f, 1.0f);
	glEnd();

}
void drawManModel(void) {
	glColor4f(0.8f, 0.8f, 0.8f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef( manModel.head.center[0], manModel.head.center[1], manModel.head.center[2] );
	glutSolidSphere( manModel.head.radius, 20, 30);
	glPopMatrix();
	//glPushMatrix();
	//glTranslatef( manModel.neck.center[0], manModel.neck.center[1], manModel.neck.center[2] );
	//glutSolidSphere( manModel.neck.radius, 20, 30);
	//glPopMatrix();
	glPushMatrix();
	glTranslatef( manModel.leftShoulder.center[0]/2, manModel.leftShoulder.center[1], manModel.leftShoulder.center[2] );
	glutSolidSphere( manModel.leftShoulder.radius, 20, 30);
	glPopMatrix();
	glPushMatrix();
	glTranslatef( manModel.rightShoulder.center[0]/2, manModel.rightShoulder.center[1], manModel.rightShoulder.center[2] );
	glutSolidSphere( manModel.rightShoulder.radius, 20, 30);
	glPopMatrix();
	glPushMatrix();
	glTranslatef( manModel.body.center[0], manModel.body.center[1], manModel.body.center[2] );
	glBegin(GL_QUADS);
	glVertex3f( manModel.body.xlength/2, manModel.body.ylength/2, manModel.body.zlength/2);
	glVertex3f( manModel.body.xlength/2, -manModel.body.ylength/2, manModel.body.zlength/2);
	glVertex3f( -manModel.body.xlength/2, -manModel.body.ylength/2, manModel.body.zlength/2);
	glVertex3f( -manModel.body.xlength/2, manModel.body.ylength/2, manModel.body.zlength/2);

	glVertex3f( manModel.body.xlength/2, manModel.body.ylength/2, -manModel.body.zlength/2);
	glVertex3f( manModel.body.xlength/2, -manModel.body.ylength/2, -manModel.body.zlength/2);
	glVertex3f( -manModel.body.xlength/2, -manModel.body.ylength/2, -manModel.body.zlength/2);
	glVertex3f( -manModel.body.xlength/2, manModel.body.ylength/2, -manModel.body.zlength/2);

	glVertex3f( manModel.body.xlength/2, manModel.body.ylength/2, manModel.body.zlength/2);
	glVertex3f( manModel.body.xlength/2, manModel.body.ylength/2, -manModel.body.zlength/2);
	glVertex3f( manModel.body.xlength/2, -manModel.body.ylength/2, -manModel.body.zlength/2);
	glVertex3f( manModel.body.xlength/2, -manModel.body.ylength/2, manModel.body.zlength/2);

	glVertex3f( -manModel.body.xlength/2, manModel.body.ylength/2, manModel.body.zlength/2);
	glVertex3f( -manModel.body.xlength/2, manModel.body.ylength/2, -manModel.body.zlength/2);
	glVertex3f( -manModel.body.xlength/2, -manModel.body.ylength/2, -manModel.body.zlength/2);
	glVertex3f( -manModel.body.xlength/2, -manModel.body.ylength/2, manModel.body.zlength/2);
	glEnd();
	glPopMatrix();
	GLUquadricObj *quadratic;
	quadratic=gluNewQuadric();
	
	glPushMatrix();
	glTranslatef(manModel.body.xlength/2,0.0f,0.0f );	
	glRotatef(90, 0.0f,-1.0f, 0.0f);
	gluCylinder(quadratic,manModel.shoulder.radius,manModel.shoulder.radius,manModel.body.xlength,32,32);//»­Ô²Öù
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.0f,0.0f,0.0f );	
	glRotatef(90, -1.0f, 0.0f, 0.0f);
	gluCylinder(quadratic,manModel.neck.radius,manModel.neck.radius,manModel.head.radius,32,32);//»­Ô²Öù
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-manModel.body.xlength/2,0.0f,0.0f );	
	glRotatef(90, 0.0f,-1.0f, 0.0f);
	glRotatef(45, 1.0f,0.0f, 0.0f);
	gluCylinder(quadratic,manModel.shoulder.radius - 5.0f,manModel.shoulder.radius - 5.0f,manModel.body.xlength * 1.2,32,32);//»­Ô²Öù
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(manModel.body.xlength/2,0.0f,0.0f );	
	glRotatef(90, 0.0f,1.0f, 0.0f);
	glRotatef(45, 1.0f,0.0f, 0.0f);
	gluCylinder(quadratic,manModel.shoulder.radius - 5.0f,manModel.shoulder.radius-5.0f,manModel.body.xlength * 1.2,32,32);//»­Ô²Öù
	glPopMatrix();

	glPushMatrix();
	glTranslatef(manModel.body.xlength * 1.4, -manModel.body.xlength * 0.9, 0.0f);
	glutSolidSphere( manModel.rightShoulder.radius, 20, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-manModel.body.xlength * 1.4, -manModel.body.xlength * 0.9, 0.0f );
	glutSolidSphere( manModel.rightShoulder.radius, 20, 30);
	glPopMatrix();

	//glVertex3f( manModel.body.width/2, manModel.body.height/2, manModel.body.thickness/2);
	//glVertex3f( manModel.body.width/2, manModel.body.height/2, -manModel.body.thickness/2);
	//glVertex3f( -manModel.body.width/2, manModel.body.height/2, -manModel.body.thickness/2);
	//glVertex3f( -manModel.body.width/2, manModel.body.height/2, manModel.body.thickness/2);

	//glVertex3f( manModel.body.width/2, -manModel.body.height/2, manModel.body.thickness/2);
	//glVertex3f( manModel.body.width/2, -manModel.body.height/2, -manModel.body.thickness/2);
	//glVertex3f( -manModel.body.width/2, -manModel.body.height/2, -manModel.body.thickness/2);
	//glVertex3f( -manModel.body.width/2, -manModel.body.height/2, manModel.body.thickness/2);
	
}

void RenderSceneTest(void) {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//DrawCloth();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	//glTranslatef(0.0f, 0.0f, -1000.0f);
	gluLookAt(1000.0f, 1000.0f, 1000.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, -1.0f);
	preDraw();
	drawManModel();
	drawClothModel();

	glPopMatrix();

	glutSwapBuffers();
}

void ChangeSize(int w, int h) {
	if (h == 0)
		h = 1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(50.0f, w/h, 1.0f, 3000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void TimerFunc(int value) {
	//sewClothModel();
	simulation();
	//system("pause");
	glutPostRedisplay();
	glutTimerFunc(refreshStep, TimerFunc, 1);
}

void SpecialKeys(int key, int x, int y)
{
	if(key == GLUT_KEY_F1)
		force1 = !force1;
	if(key == GLUT_KEY_F2)
		force2 = !force2;
	if(key == GLUT_KEY_UP)
		xRot -= step;
	if(key == GLUT_KEY_DOWN)
		xRot += step;
	if(key == GLUT_KEY_LEFT)
		yRot -= step;
	if(key == GLUT_KEY_RIGHT)
		yRot += step;
	if(key == GLUT_KEY_PAGE_UP)
		zRot -= step;
	if(key == GLUT_KEY_PAGE_DOWN)
		zRot += step;
	if(xRot >= 356.0f)
		xRot = 0.0f;
	if(xRot <= -1.0f)
		xRot = 355.0f;
	if(yRot >= 356.0f)
		yRot = 0.0f;
	if(yRot <= -1.0f)
		yRot = 355.0f;
	if(zRot >= 356.0f)
		zRot = 0.0f;
	if(zRot <= -1.0f)
		zRot = 355.0f;

	glutPostRedisplay();
}

void SetupRC()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
	glLightfv(GL_LIGHT0, GL_POSITION, position0);

	//glFrontFace(GL_CCW);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_NORMALIZE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int main(void) {
	initEnvironment();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(900, 50);
	glutCreateWindow("opengl_example");
	init();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderSceneTest);
	glutTimerFunc(250, TimerFunc, 1);
	glutSpecialFunc(SpecialKeys);
	SetupRC();
	glutMainLoop();
	return 0;
}