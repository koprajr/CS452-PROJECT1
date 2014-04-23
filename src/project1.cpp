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

GLfloat xSphere = 0.0;
GLfloat ySphere = 0.15;
GLfloat zSphere = 0.0;
GLdouble sphereAngles[16];

GLdouble xCamera = 0.0;
GLdouble yCamera = 150.0;
GLdouble zCamera = 4.0;

GLubyte sphereTexture[1024][1024][3];
GLubyte planeTexture[1024][1024][3];
GLubyte planeTextureEND[1024][1024][3];
GLubyte holeTexture[1024][1024][3];

/*
Black Holes
*/
GLUquadricObj* hole01;
GLfloat rHole_01 = 0.15;
GLfloat sHole_01 = 0.0175f;
GLfloat xHole_01 = 2.5;
GLfloat yHole_01 = 0.15;
GLfloat zHole_01 = 0.3;
GLdouble hole_01_Angles[16];

GLUquadricObj* hole02;
GLfloat rHole_02 = 0.10;
GLfloat sHole_02 = 0.0105f;
GLfloat xHole_02 = 3.0;
GLfloat yHole_02 = 0.15;
GLfloat zHole_02 = -0.5;
GLdouble hole_02_Angles[16];

GLUquadricObj* hole03;
GLfloat rHole_03 = 0.25;
GLfloat sHole_03 = 0.0125f;
GLfloat xHole_03 = 4.1;
GLfloat yHole_03 = 0.15;
GLfloat zHole_03 = -1.0;
GLdouble hole_03_Angles[16];

GLUquadricObj* hole04;
GLfloat rHole_04 = 0.35;
GLfloat sHole_04 = 0.0125f;
GLfloat xHole_04 = 2.0;
GLfloat yHole_04 = 0.15;
GLfloat zHole_04 = 0.8;
GLdouble hole_04_Angles[16];

bool dead = false;
int score = 0;



//  Just a pointer to a font style..
//  Fonts supported by GLUT are: GLUT_BITMAP_8_BY_13, 
//  GLUT_BITMAP_9_BY_15, GLUT_BITMAP_TIMES_ROMAN_10, 
//  GLUT_BITMAP_TIMES_ROMAN_24, GLUT_BITMAP_HELVETICA_10,
//  GLUT_BITMAP_HELVETICA_12, and GLUT_BITMAP_HELVETICA_18.
GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;

//  printf prints to file. printw prints to window
void printw (float x, float y, float z, char* format, ...);


bool intersect()
{
    
    float X = xSphere;               
    float Y = zSphere;   

	/* Circle equation (x - h)^2 + (y - k)^2 = r^2....Basically you just have to find the distance between the two centers.
Now take the two radii and add them up. If they are less than this distance, they won't intersect. */

	float X_01 = xHole_01 - X;
	float Y_01 = zHole_01 - Y;

	float X_02 = xHole_02 - X;
	float Y_02 = zHole_02 - Y;

	float X_03 = xHole_03 - X;
	float Y_03 = zHole_03 - Y;

	float X_04 = xHole_04 - X;
	float Y_04 = zHole_04 - Y;


	float D_01 = sqrt( (X_01*X_01 + Y_01*Y_01) );   // distance between two centers
	float D_02 = sqrt( (X_02*X_02 + Y_02*Y_02) );
	float D_03 = sqrt( (X_03*X_03 + Y_03*Y_03) );
	float D_04 = sqrt( (X_04*X_04 + Y_04*Y_04) );


	float sumRad_01 = rHole_01 + 0.15;
	float sumRad_02 = rHole_02 + 0.15;
	float sumRad_03 = rHole_03 + 0.15;
	float sumRad_04 = rHole_04 + 0.15;

	// Testing for overlap
	if (D_01 < sumRad_01 || D_02 < sumRad_02 || D_03 < sumRad_03 || D_04 < sumRad_04 )
		return true; // true means they intersect
	else
		return false;
}


void display() {
	GLfloat lightPosition[] = { 0.0, 30.0, 50.0, 0.0 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(xCamera, yCamera, zCamera, 0, 0, 0, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);


	/*
	 * Build the Sphere
	 */
	 if (!dead) {

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

		glTexImage2D(GL_TEXTURE_2D,0,3,1024,1024,0,GL_RGB,GL_UNSIGNED_BYTE, holeTexture);
		glTranslatef(xHole_01, yHole_01, zHole_01);
		glMultMatrixd(hole_01_Angles);
		gluSphere(hole01, rHole_01, 50, 50);

		glPopMatrix();

		glPushMatrix();

		glTexImage2D(GL_TEXTURE_2D,0,3,1024,1024,0,GL_RGB,GL_UNSIGNED_BYTE, holeTexture);
		glTranslatef(xHole_02, yHole_02, zHole_02);
		glMultMatrixd(hole_02_Angles);
		gluSphere(hole02, rHole_02, 50, 50);

		glPopMatrix();

		glPushMatrix();

		glTexImage2D(GL_TEXTURE_2D,0,3,1024,1024,0,GL_RGB,GL_UNSIGNED_BYTE, holeTexture);
		glTranslatef(xHole_03, yHole_03, zHole_03);
		glMultMatrixd(hole_03_Angles);
		gluSphere(hole03, rHole_03, 50, 50);

		glPopMatrix();

		glPushMatrix();

		glTexImage2D(GL_TEXTURE_2D,0,3,1024,1024,0,GL_RGB,GL_UNSIGNED_BYTE, holeTexture);
		glTranslatef(xHole_04, yHole_04, zHole_04);
		glMultMatrixd(hole_04_Angles);
		gluSphere(hole04, rHole_04, 50, 50);

		glPopMatrix();

		dead = intersect();
		GLdouble angle = 15.00;
		if( xHole_01 > (GLfloat) -2.00 ) {
			xHole_01 -= sHole_01;

			glPushMatrix();

			glLoadIdentity();
			glRotated(angle,0,1.0,0);
			glMultMatrixd(hole_01_Angles);
			glGetDoublev(GL_MODELVIEW_MATRIX, hole_01_Angles);

			glPopMatrix();
		} else {
			xHole_01 = 2.0;
			
			zHole_01 = (rand() % (150-0)) / 100.0;
			if ( (rand() % 2) == 0){ 
				zHole_01 = zHole_01 * (-1);
			}

			sHole_01 = (rand() % (80000-10000)) / 1000000.0;

			glPushMatrix();

			glLoadIdentity();
			glRotated(-angle,0,2.0,0);
			glMultMatrixd(hole_01_Angles);
			glGetDoublev(GL_MODELVIEW_MATRIX, hole_01_Angles);

			glPopMatrix();
			score++;
		}

		if( xHole_02 > (GLfloat) -2.00 ) {
			xHole_02 -= sHole_02;

			glPushMatrix();

			glLoadIdentity();
			glRotated(angle,0,1.0,0);
			glMultMatrixd(hole_02_Angles);
			glGetDoublev(GL_MODELVIEW_MATRIX, hole_02_Angles);

			glPopMatrix();
		} else {
			xHole_02 = 2.0;
			
			zHole_02 = (rand() % (150-0)) / 100.0;
			if ( (rand() % 2) == 0){ 
				zHole_02 = zHole_02 * (-1);
			}

			sHole_02 = (rand() % (80000-10000)) / 1000000.0;

			glPushMatrix();

			glLoadIdentity();
			glRotated(-angle,0,2.0,0);
			glMultMatrixd(hole_02_Angles);
			glGetDoublev(GL_MODELVIEW_MATRIX, hole_02_Angles);

			glPopMatrix();
			score++;
		}

		if( xHole_03 > (GLfloat) -2.00 ) {
			xHole_03 -= sHole_03;

			glPushMatrix();

			glLoadIdentity();
			glRotated(angle,0,1.0,0);
			glMultMatrixd(hole_03_Angles);
			glGetDoublev(GL_MODELVIEW_MATRIX, hole_03_Angles);

			glPopMatrix();
		} else {
			xHole_03 = 2.0;
			
			zHole_03 = (rand() % (150-0)) / 100.0;
			if ( (rand() % 2) == 0){ 
				zHole_03 = zHole_03 * (-1);
			}

			sHole_03 = (rand() % (80000-10000)) / 1000000.0;

			glPushMatrix();

			glLoadIdentity();
			glRotated(-angle,0,2.0,0);
			glMultMatrixd(hole_03_Angles);
			glGetDoublev(GL_MODELVIEW_MATRIX, hole_03_Angles);

			glPopMatrix();
			score++;
		}

		if( xHole_04 > (GLfloat) -2.00 ) {
			xHole_04 -= sHole_04;

			glPushMatrix();

			glLoadIdentity();
			glRotated(angle,0,1.0,0);
			glMultMatrixd(hole_04_Angles);
			glGetDoublev(GL_MODELVIEW_MATRIX, hole_04_Angles);

			glPopMatrix();
		} else {
			xHole_04 = 2.0;
			
			zHole_04 = (rand() % (150-0)) / 100.0;
			if ( (rand() % 2) == 0){ 
				zHole_04 = zHole_04 * (-1);
			}

			sHole_04 = (rand() % (80000-10000)) / 1000000.0;

			glPushMatrix();

			glLoadIdentity();
			glRotated(-angle,0,2.0,0);
			glMultMatrixd(hole_04_Angles);
			glGetDoublev(GL_MODELVIEW_MATRIX, hole_04_Angles);

			glPopMatrix();
			score++;
		}

	} else {

		glPushMatrix();

		glTexImage2D(GL_TEXTURE_2D,0,3,1024,1024,0,GL_RGB,GL_UNSIGNED_BYTE, planeTextureEND);
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

		fprintf(stderr,"Game Over, your score is: %d \n",score);

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

	//  Set the frame buffer clear color to black. 
	glClearColor (0.0, 0.0, 0.0, 0.0);

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

	fs.open("blackHole.ppm");
	fs.getline(temp, 80);
	fs.getline(temp, 80);
	fs.getline(temp, 80);
	for( i = 0; i < 1024; i++ ) {
		for( j = 0; j < 1024; j++ ) {
			holeTexture[i][1023-j][0] = (GLubyte) fs.get();
			holeTexture[i][1023-j][1] = (GLubyte) fs.get();
			holeTexture[i][1023-j][2] = (GLubyte) fs.get();
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

	fs.open("spaceEND.ppm");
	fs.getline(temp, 80);
	fs.getline(temp, 80);
	fs.getline(temp, 80);
	for( i = 0; i < 1024; i++ ) {
		for( j = 0; j < 1024; j++ ) {
			planeTextureEND[i][1023-j][0] = (GLubyte) fs.get();
			planeTextureEND[i][1023-j][1] = (GLubyte) fs.get();
			planeTextureEND[i][1023-j][2] = (GLubyte) fs.get();
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

	hole02 = gluNewQuadric();
	gluQuadricDrawStyle(hole02, GLU_FILL);
	gluQuadricNormals(hole02, GLU_SMOOTH);
	gluQuadricTexture(hole02, GL_TRUE);

	glPushMatrix();

	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, hole_02_Angles);

	glPopMatrix();

	hole03 = gluNewQuadric();
	gluQuadricDrawStyle(hole03, GLU_FILL);
	gluQuadricNormals(hole03, GLU_SMOOTH);
	gluQuadricTexture(hole03, GL_TRUE);

	glPushMatrix();

	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, hole_03_Angles);

	glPopMatrix();

	hole04 = gluNewQuadric();
	gluQuadricDrawStyle(hole04, GLU_FILL);
	gluQuadricNormals(hole04, GLU_SMOOTH);
	gluQuadricTexture(hole04, GL_TRUE);

	glPushMatrix();

	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, hole_04_Angles);

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
