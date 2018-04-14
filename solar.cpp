//#include "GL/freeglut.h"
//#include 

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include "GL/gl.h"
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "GL/freeglut.h"
#endif
#include <math.h>
#include "globals.h"

#include <typeinfo>
#include <iostream>
// #define WindowWidth 800
// #define WindowHeight 800
using namespace std;

float hOrbit = 180;
float vOrbit = -40;
int polyMode = 0;

// -1 means no planet should scale. 
int toScale = -1;
bool analisysMode = false;






GLuint LoadTexture( const char * filename, int width, int height );

GLuint sunTex;
GLuint earthTex;
GLuint marsTex;
GLuint moonTex;
GLuint mercuryTex;
GLuint venusTex;
GLuint jupiterTex;

void drawCircle(float radius) {
	glPushMatrix();
    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 360; i++) {
        float degInRad = i * deg2rad;
        glColor4i(1, 1, 1, 1);
        glVertex3f(cosf(degInRad) * radius, 0, sinf(degInRad) * radius);
    }
    glEnd();
    glPopMatrix();
}

void setMaterialColor(float r, float g, float b) {
    GLfloat qaBlack[] = { 0, 0, 0, 1 };
    GLfloat color[] = { r, g, b, 1 };
    GLfloat qaWhite[] = { 1, 1, 1, 1 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
}

void drawEarthMoon() {
	GLUquadricObj * sphere = gluNewQuadric();
	glActiveTexture(GL_TEXTURE0);
	gluQuadricTexture(sphere, GL_TRUE);

    glPushMatrix();
	    glRotatef(earthTranslation * 8, 0, 1, 0);
	    // setMaterialColor(1, 1, 1);
	    glPushMatrix();
		    drawCircle(earthRadius + (earthRadius * 0.27) * 2);
		    glTranslatef(0.0, 0.0, earthRadius + (earthRadius * 0.27) * 2);

		    glRotatef(90.0, 1, 0, 0);
		    //glutSolidSphere(earthRadius * 0.27, verticalLines, horizontalLines);
		    glEnable ( GL_TEXTURE_2D );
		    glBindTexture ( GL_TEXTURE_2D, moonTex);
		    gluSphere(sphere, earthRadius * 0.27, verticalLines, horizontalLines);
		    glDisable ( GL_TEXTURE_2D );
	    glPopMatrix();
    glPopMatrix();
}

void drawEarth() {
	GLUquadricObj * sphere = gluNewQuadric();
	glActiveTexture(GL_TEXTURE0);
	gluQuadricTexture(sphere, GL_TRUE);

    glPushMatrix();
    glRotatef(earthTranslation, 0, 1, 0);
    glPushMatrix();
	    drawCircle(au);
	    glTranslatef(0.0, 0.0, au);

	    drawEarthMoon();

	    if (toScale == 3) { glScalef(2.0, 2.0, 2.0); }

	    // Axial tilt of 23.44 -> 90-23.44
	    glRotatef(66.56, 1, 0, 0);

	    //Translation around self axis
	    glRotatef(zRotated / 50, 0, 0, 1);
 		
 		glEnable ( GL_TEXTURE_2D );
	    glBindTexture ( GL_TEXTURE_2D, earthTex);
	    gluSphere(sphere, earthRadius, verticalLines, horizontalLines);
	    glDisable ( GL_TEXTURE_2D );
    glPopMatrix();
    glPopMatrix();
}

void drawSun() {
    GLUquadricObj * sphere = gluNewQuadric();
    glActiveTexture(GL_TEXTURE0);
	gluQuadricTexture(sphere, GL_TRUE);


    // setMaterialColor(1, 1, 0);
    glPushMatrix();
	    glRotatef(90.0, 1, 0, 0);
	    glRotatef(zRotated / 360, 0, 0, 1);
	    if (toScale == 0) glScalef(2.0, 2.0, 2.0);

	    glEnable ( GL_TEXTURE_2D );
	    glBindTexture ( GL_TEXTURE_2D, sunTex);
	    gluSphere(sphere, sunRadius, verticalLines, horizontalLines);
	    glDisable ( GL_TEXTURE_2D );

    glPopMatrix();
}

void drawMars() {

	GLUquadricObj * sphere = gluNewQuadric();
	glActiveTexture(GL_TEXTURE0);
	gluQuadricTexture(sphere, GL_TRUE);

    glPushMatrix();
	    glRotatef(earthTranslation * 1.3, 0, 1, 0);
	    // setMaterialColor(0.8, 0.2, 0.1);

	    glPushMatrix();
	    	drawCircle(marsAU);
		    glTranslatef(0.0, 0.0, marsAU);

		    // Axial tilt of 25.19 -> 90-25.19
		    glRotatef(64.81, 1, 0, 0);

		    glRotatef(zRotated / 10, 0, 0, 1);
		    if (toScale == 4) { glScalef(2.0, 2.0, 2.0); }
		    // glutSolidSphere(earthRadius * 0.7, verticalLines, horizontalLines);

		    glEnable ( GL_TEXTURE_2D );
		    glBindTexture ( GL_TEXTURE_2D, marsTex);
		    gluSphere(sphere, marsRadius, verticalLines, horizontalLines);
		    glDisable ( GL_TEXTURE_2D );

	    glPopMatrix();
    glPopMatrix();
}

void drawVenus() {
	GLUquadricObj * sphere = gluNewQuadric();
	glActiveTexture(GL_TEXTURE0);
	gluQuadricTexture(sphere, GL_TRUE);

    glPushMatrix();
	    glRotatef(earthTranslation * 0.7, 0, 1, 0);

	    // setMaterialColor(0.0, 0.7, 0.1);
	    glPushMatrix();
	    	drawCircle(venusAU);
		    glTranslatef(0.0, 0.0, venusAU);
		    glRotatef(90.0, 1, 0, 0);
		    glRotatef(zRotated / 10, 0, 0, 1);
		    if (toScale == 2) { glScalef(2.0, 2.0, 2.0); }

		    glEnable ( GL_TEXTURE_2D );
		    glBindTexture ( GL_TEXTURE_2D, venusTex);
		    gluSphere(sphere, venusRadius, verticalLines, horizontalLines);
		    glDisable ( GL_TEXTURE_2D );
	    glPopMatrix();
    glPopMatrix();
}

void drawMercury() {
	GLUquadricObj * sphere = gluNewQuadric();
	glActiveTexture(GL_TEXTURE0);
	gluQuadricTexture(sphere, GL_TRUE);

	glPushMatrix();
	    glRotatef(earthTranslation*1.8, 0, 1, 0);

	    // setMaterialColor(0.0, 0.7, 0.1);
	    glPushMatrix();
	    	drawCircle(mercuryAU);
		    glTranslatef(0.0, 0.0, mercuryAU);
		    glRotatef(90.0, 1, 0, 0);
		    glRotatef(zRotated / 10, 0, 0, 1);
		    if (toScale == 1) glScalef(2.0, 2.0, 2.0);
		    glEnable ( GL_TEXTURE_2D );
		    glBindTexture ( GL_TEXTURE_2D, mercuryTex);
		    gluSphere(sphere, mercuryRadius, verticalLines, horizontalLines);
		    glDisable ( GL_TEXTURE_2D );
	    glPopMatrix();
    glPopMatrix();
}

void drawJupiter() {
	GLUquadricObj * sphere = gluNewQuadric();
	glActiveTexture(GL_TEXTURE0);
	gluQuadricTexture(sphere, GL_TRUE);

	glPushMatrix();
	    glRotatef(earthTranslation*3, 0, 1, 0);

	    // setMaterialColor(0.0, 0.7, 0.1);
	    glPushMatrix();
	    	drawCircle(jupiterAU);
		    glTranslatef(0.0, 0.0, jupiterAU);
		    glRotatef(90.0, 1, 0, 0);
		    glRotatef(zRotated / 10, 0, 0, 1);
		    if (toScale == 1) glScalef(2.0, 2.0, 2.0);
		    glEnable ( GL_TEXTURE_2D );
		    glBindTexture ( GL_TEXTURE_2D, jupiterTex);
		    gluSphere(sphere, jupiterRadius, verticalLines, horizontalLines);
		    glDisable ( GL_TEXTURE_2D );
	    glPopMatrix();
    glPopMatrix();
}



void reshapeFunc(int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(40.0, (GLdouble) x / (GLdouble) y, 0.5, 280.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, x, y);

    // disable see through
    glEnable(GL_DEPTH_TEST);

}

void draw(void) {

    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glLoadIdentity();
	float eyeX = cameraDistance * -sinf(hOrbit*(M_PI/180)) * cosf((vOrbit)*(M_PI/180));
	float eyeY = cameraDistance * -sinf((vOrbit)*(M_PI/180));
	float eyeZ = -cameraDistance * cosf((hOrbit)*(M_PI/180)) * cosf((vOrbit)*(M_PI/180));

	gluLookAt(eyeX, eyeY, eyeZ,
	          0.0, 0.0, 0.0, 
	          0.0, 1.0, 0.0);

    drawSun();
	drawMercury();
	drawVenus();
    drawEarth();
    drawMars();
    drawJupiter();
    
    
    glutSwapBuffers();
}

void idleFunc(void) {
    zRotated += earthDay;
    if (!analisysMode)
    	earthTranslation += (earthDay / 365);
    glutPostRedisplay();
}

void keyboard(int key, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    if (key == GLUT_KEY_LEFT) {
        hOrbit -= 10;
    } else if (key == GLUT_KEY_RIGHT) {
        hOrbit += 10;
    }
    if (key == GLUT_KEY_UP) {
        vOrbit += 10;
    } else if (key == GLUT_KEY_DOWN) {
        vOrbit -= 10;
    }
    // Request display update
    glutPostRedisplay();
}

void keyboardManager(unsigned char key, int x, int y) {
	if (isdigit(key)) {
        int value = key - '0';
        if (toScale == value) {
            toScale = -1;
        } else {
            toScale = value;
        }
    }

    switch (key) {
    case 'f':
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case 'l':
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case 'p':
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;

	// Camera distance
    case 'z':
        cameraDistance += 1;
        break;
    case 'x':
        cameraDistance -= 1;
        break;
    }

    glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        polyMode = ++polyMode % 3;

        if (polyMode == 0) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else if (polyMode == 1) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    	analisysMode = !analisysMode;
    }
}

void init() {
    // Lighting set up
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Set lighting intensity and color
    GLfloat qaAmbientLight[] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat qaDiffuseLight[] = { 1, 1, 1, 1 };
    GLfloat qaSpecularLight[]	= {0.8, 0.8, 0.8, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

    // Set the light position
    GLfloat qaLightPosition[] = { 0, 0, 0, 1 };

    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
    glEnable(GL_DEPTH_TEST);
}

void loadAllTextures() {
	sunTex = LoadTexture( "Bitmaps/sunmap.bmp", 1024, 512 );
	earthTex = LoadTexture( "Bitmaps/earthmap.bmp", 1000, 500 );
	marsTex = LoadTexture( "Bitmaps/marsmap.bmp", 1024, 512 );

	moonTex = LoadTexture( "Bitmaps/moonmap.bmp", 1024, 512 );
	mercuryTex = LoadTexture( "Bitmaps/mercurymap.bmp", 1024, 512 );
	venusTex = LoadTexture( "Bitmaps/venusmap.bmp", 1024, 512 );
	jupiterTex = LoadTexture( "Bitmaps/jupitermap.bmp", 1024, 512 );
}

int main(int argc, char * * argv) {
    glutInit( & argc, argv);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800, 700);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("You Spin me right round, baby right round");

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutMouseFunc(mouseButton);
    init();
    glutSpecialFunc(keyboard);
    glutKeyboardFunc(keyboardManager);
    glutDisplayFunc(draw);
    glutReshapeFunc(reshapeFunc);
    glutIdleFunc(idleFunc);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    loadAllTextures();
    glutMainLoop();

    return 0;
}

GLuint LoadTexture( const char * filename, int width, int height ) {

  GLuint texture;

  // int width, height;

  unsigned char * data;

  FILE * file;

  file = fopen( filename, "rb" );

  if ( file == NULL ) return 0;
  data = (unsigned char *)malloc( width * height * 3 );
  //int size = fseek(file,);
  fread( data, width * height * 3, 1, file );
  fclose( file );

	for(int i = 0; i < width * height ; ++i) {
	   int index = i*3;
	   unsigned char B,R;
	   B = data[index];
	   R = data[index+2];

	   data[index] = R;
	   data[index+2] = B;
	}
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );


	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
	free( data );

	return texture;
}