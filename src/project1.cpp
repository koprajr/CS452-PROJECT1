/*
 * Jeffrey Kopra and Alan Schay 
 * CS452
 * Graphics
 * Project 1
 *
 * Compile Using:
 *	g++ -Wall -o run-me project1.cpp -lGL -lGLU -lglut
 *
 */
/*
 * Camera movement:
 * 	X-axis: A and D
 * 	Y-axis: S and W
 * 	Z-axis: Q and E
 * 
 * Ball Rolling:
 * 	Arrow keys
 * 
 * requires libraries:
 * 	GL, GLU, and glut
*/

#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

GLUquadricObj* sphere;

GLUquadricObj* hole01;

GLfloat xSphere = 0.0;
GLfloat ySphere = 0.15;
GLfloat zSphere = 0.0;
GLdouble sphereAngles[16];

GLdouble xCamera = 0.0;
GLdouble yCamera = 150.0;
GLdouble zCamera = 4.0;

GLubyte sphereTexture[1024][1024][3];
GLubyte planeTexture[1024][1024][3];

/*
Black Holes
*/
GLfloat xHole_01 = 0.5;
GLfloat yHole_01 = 0.15;
GLfloat zHole_01 = 0.1;
GLdouble hole_01_Angles[16];



void display() {
	GLfloat lightPosition[] = { 0.0, 30.0, 50.0, 0.0 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(xCamera, yCamera, zCamera, 0, 0, 0, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	/*
	 * Build the Sphere
	 */
	glPushMatrix();

	glTexImage2D(GL_TEXTURE_2D,0,3,1024,1024,0,GL_RGB,GL_UNSIGNED_BYTE, sphereTexture);
	glTranslatef(xSphere, ySphere, zSphere);
	glMultMatrixd(sphereAngles);
	gluSphere(sphere, 0.15, 50, 50);

	glPopMatrix();

	/* Build the surface */
	glPushMatrix();

	glTexImage2D(GL_TEXTURE_2D,0,3,1024,1024,0,GL_RGB,GL_UNSIGNED_BYTE, planeTexture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);
	glVertex3f(1.5, 0, 1.5);
	glTexCoord2f(0, 1.0);
	glVertex3f(1.5, 0, -1.5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.5, 0, -1.5);
	glTexCoord2f(1.0, 0);
	glVertex3f(-1.5, 0, 1.5);
	glEnd();

	glPopMatrix();


	/*
	 * Build the Black Holes
	 */
	glPushMatrix();

	glTexImage2D(GL_TEXTURE_2D,0,3,1024,1024,0,GL_RGB,GL_UNSIGNED_BYTE, sphereTexture);
	glTranslatef(xHole_01, yHole_01, zHole_01);
	glMultMatrixd(hole_01_Angles);
	gluSphere(hole01, 0.15, 50, 50);

	glPopMatrix();


	GLdouble angle = 5.625;
	if( xHole_01 > (GLfloat) -1.35 ) {
		xHole_01 -= 0.0375f;

		glPushMatrix();

		glLoadIdentity();
		glRotated(angle,0,0,1.0);
		glMultMatrixd(hole_01_Angles);
		glGetDoublev(GL_MODELVIEW_MATRIX, hole_01_Angles);

		glPopMatrix();
	} else {
		xHole_01 += 1.0000f;

		glPushMatrix();

		glLoadIdentity();
		glRotated(-angle,0,0,1.0);
		glMultMatrixd(hole_01_Angles);
		glGetDoublev(GL_MODELVIEW_MATRIX, hole_01_Angles);

		glPopMatrix();
	}


	


	glutSwapBuffers();
	glutPostRedisplay();
}

void ball(int key, int x, int y) {

	/*
	 * Equation to calculate angle of change:
	 * x = distance moved / radius
	 * angle = (180 * ((x/2)r)^2)/(180 * (radius^2) )*360
	 */
	GLdouble angle = 5.625;
	if( key == GLUT_KEY_UP && zSphere > (GLfloat) -1.35 ) {
		zSphere -= 0.0375f;

		glPushMatrix();

		glLoadIdentity();
		glRotated(-angle,1.0,0,0);
		glMultMatrixd(sphereAngles);
		glGetDoublev(GL_MODELVIEW_MATRIX, sphereAngles);

		glPopMatrix();
	}
	if( key == GLUT_KEY_DOWN && zSphere < (GLfloat) 1.35 ) {
		zSphere += 0.0375f;

		glPushMatrix();

		glLoadIdentity();
		glRotated(angle,1.0,0,0);
		glMultMatrixd(sphereAngles);
		glGetDoublev(GL_MODELVIEW_MATRIX, sphereAngles);

		glPopMatrix();
	}
	if( key == GLUT_KEY_LEFT && xSphere > (GLfloat) -1.35 ) {
		xSphere -= 0.0375f;

		glPushMatrix();

		glLoadIdentity();
		glRotated(angle,0,0,1.0);
		glMultMatrixd(sphereAngles);
		glGetDoublev(GL_MODELVIEW_MATRIX, sphereAngles);

		glPopMatrix();
	}
	if( key == GLUT_KEY_RIGHT && xSphere < (GLfloat) 1.35 ) {
		xSphere += 0.0375f;

		glPushMatrix();

		glLoadIdentity();
		glRotated(-angle,0,0,1.0);
		glMultMatrixd(sphereAngles);
		glGetDoublev(GL_MODELVIEW_MATRIX, sphereAngles);

		glPopMatrix();
	}

}

void camera(unsigned char key, int x, int y) {

	if( key == 'a' ) {
		xCamera -= 0.1;
	}
	else if( key == 'd' ) {
		xCamera += 0.1;
	}
	else if( key == 's' ) {
		yCamera -= 0.1;
	}
	else if( key == 'w' ) {
		yCamera += 0.1;
	}
	else if( key == 'q' ) {
		zCamera -= 0.1;
	}
	else if( key == 'e' ) {
		zCamera += 0.1;
	}

}

void init() {
	int i, j;
	ifstream fs;
	char temp[80];

	GLfloat diffuseLighting[] = { 1, 1, 1, 1 };
	GLfloat ambientLighting[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat specularLighting[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat specter[] = { 1.0, 1.0, 1.0, 1.0 };

	glClearColor( 0, 0, 0, 0 );
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLighting);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLighting);

	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLighting);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specter);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	fs.open("earth.ppm");
	fs.getline(temp, 80);
	fs.getline(temp, 80);
	fs.getline(temp, 80);
	for( i = 0; i < 1024; i++ ) {
		for( j = 0; j < 1024; j++ ) {
			sphereTexture[i][1023-j][0] = (GLubyte) fs.get();
			sphereTexture[i][1023-j][1] = (GLubyte) fs.get();
			sphereTexture[i][1023-j][2] = (GLubyte) fs.get();
		}
	}
	fs.close();

	fs.open("space.ppm");
	fs.getline(temp, 80);
	fs.getline(temp, 80);
	fs.getline(temp, 80);
	for( i = 0; i < 1024; i++ ) {
		for( j = 0; j < 1024; j++ ) {
			planeTexture[i][1023-j][0] = (GLubyte) fs.get();
			planeTexture[i][1023-j][1] = (GLubyte) fs.get();
			planeTexture[i][1023-j][2] = (GLubyte) fs.get();
		}
	}
	fs.close();

	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricTexture(sphere, GL_TRUE);

	glPushMatrix();

	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, sphereAngles);

	glPopMatrix();




	hole01 = gluNewQuadric();
	gluQuadricDrawStyle(hole01, GLU_FILL);
	gluQuadricNormals(hole01, GLU_SMOOTH);
	gluQuadricTexture(hole01, GL_TRUE);

	glPushMatrix();

	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, hole_01_Angles);

	glPopMatrix();


}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(1.0f, (float) 1024 / (float) 1024, 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 1024);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Project 01");

	init();

	glutSpecialFunc(ball);
	glutKeyboardFunc(camera);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMainLoop();

	return 0;
}
