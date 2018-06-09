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
#include "BitmapLoader.h"
#include <iostream>

using namespace std;

/*

	HOW TO PLAY

	- Arrows move the camera, though it's fixed to the center of the sun
	- Z and X move the camera back and forth from the center
	- Keys 1 to 5 scale the planets by 200%. Ordered by proximity to the sun
	- Mouse right button and keys L, P and F change poligon mode
	- Mouse left button enter in analisys mode, halt rotation and keep its translation so the user can zoom in.


	Sun is not scaled, though the planets are.
*/

void setMaterialColor(float r, float g, float b);

float hOrbit = 180;
float vOrbit = -20;
int polyMode = 0;

// -1 means no planet should scale. 
int toScale = -1;
bool analisysMode = false;

GLuint sunTex;
GLuint earthTex;
GLuint marsTex;
GLuint moonTex;
GLuint mercuryTex;
GLuint venusTex;
GLuint jupiterTex;
GLuint starsTex;

float windowWidth = 800;
float windowHeight = 700;

void drawCircle(float radius) {
    glBegin(GL_LINE_LOOP);
		//  Create line loop with vertexes according to parametric equation of a circle
    	//	x = r * cos(alpha)
    	//	y = r * sin(alpha)
		for (int i = 0; i < 360; i++) {
		    float degInRad = i * deg2rad;
		    glVertex3f(cosf(degInRad) * radius, 0, sinf(degInRad) * radius);
		}
    glEnd();
}

void setMaterialColor(float r, float g, float b) {
    GLfloat qaBlack[] = { 0, 0, 0, 1 };
    GLfloat color[] = { r, g, b, 1 };
    GLfloat qaWhite[] = { 1, 1, 1, 1 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
}

void drawEarthMoon() {
	// Since moon is being drawn inside earth's inner push and pop matrixes it's rotation and 
	// translations will consider the earth as it's center
	
	GLUquadricObj * sphere = gluNewQuadric();
	glActiveTexture(GL_TEXTURE0);
	gluQuadricTexture(sphere, GL_TRUE);

    glPushMatrix();
	    glRotatef(earthTranslation * 8, 0, 1, 0);
	    drawCircle(earthRadius + (earthRadius * 0.27) * 2);
	    glTranslatef(0.0, 0.0, earthRadius + (earthRadius * 0.27) * 2);
	    glRotatef(90.0, 1, 0, 0);
	    glRotatef(-zRotated / 50, 0, 0, 1);
	    glEnable ( GL_TEXTURE_2D );
	    glBindTexture ( GL_TEXTURE_2D, moonTex);
	    gluSphere(sphere, earthRadius * 0.27, verticalLines, horizontalLines);
	    glDisable ( GL_TEXTURE_2D );
    glPopMatrix();
}

void drawEarth() {
	GLUquadricObj * sphere = gluNewQuadric();
	glActiveTexture(GL_TEXTURE0);
	gluQuadricTexture(sphere, GL_TRUE);

    glPushMatrix(); 

	    //Rotation around the sun
	    glRotatef(earthTranslation, 0, 1, 0);

    	// Draws the planet's orbit
	    drawCircle(au);

	    // Translate planet to it's distance from the sun (in Astronomic Units)
	    glTranslatef(0.0, 0.0, au);

	    // Draw earth's moon
	    drawEarthMoon();

	    // If the planet selected is 3, double its scale
	    if (toScale == 3) glScalef(2.0, 2.0, 2.0);

	    // Axial tilt of 23.44 -> -90+23.44
	    glRotatef(-66.56, 1, 0, 0);

	    //Translation around self axis
	    glRotatef(zRotated / 50, 0, 0, 1);
 		
 		glEnable ( GL_TEXTURE_2D );
 		//Apply texture to next drawn object
	    glBindTexture ( GL_TEXTURE_2D, earthTex);

	    // Create the sphere, set its radius, slices and stacks
	    gluSphere(sphere, earthRadius, verticalLines, horizontalLines);
	    glDisable ( GL_TEXTURE_2D );
    glPopMatrix();
}

void drawSun() {
    GLUquadricObj * sphere = gluNewQuadric();
    glActiveTexture(GL_TEXTURE0);
	gluQuadricTexture(sphere, GL_TRUE);
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

    	drawCircle(marsAU);
	    glTranslatef(0.0, 0.0, marsAU);

	    // Axial tilt of 25.19 -> -90+25.19
	    glRotatef(-64.81, 1, 0, 0);

	    glRotatef(zRotated / 10, 0, 0, 1);
	    if (toScale == 4) glScalef(2.0, 2.0, 2.0); 

	    glEnable ( GL_TEXTURE_2D );
	    glBindTexture ( GL_TEXTURE_2D, marsTex);
	    gluSphere(sphere, marsRadius, verticalLines, horizontalLines);
	    glDisable ( GL_TEXTURE_2D );

    glPopMatrix();
}

void drawVenus() {
	GLUquadricObj * sphere = gluNewQuadric();
	glActiveTexture(GL_TEXTURE0);
	gluQuadricTexture(sphere, GL_TRUE);

    glPushMatrix();
	    glRotatef(earthTranslation * 0.7, 0, 1, 0);
    	drawCircle(venusAU);
	    glTranslatef(0.0, 0.0, venusAU);
	    glRotatef(90.0, 1, 0, 0);
	    glRotatef(zRotated / 10, 0, 0, 1);
	    if (toScale == 2)  glScalef(2.0, 2.0, 2.0); 
	    glEnable ( GL_TEXTURE_2D );
	    glBindTexture ( GL_TEXTURE_2D, venusTex);
	    gluSphere(sphere, venusRadius, verticalLines, horizontalLines);
	    glDisable ( GL_TEXTURE_2D );
    glPopMatrix();
}

void drawMercury() {
	GLUquadricObj * sphere = gluNewQuadric();
	glActiveTexture(GL_TEXTURE0);
	gluQuadricTexture(sphere, GL_TRUE);

	glPushMatrix();
	    glRotatef(earthTranslation*1.8, 0, 1, 0);
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
}

void drawJupiter() {
	GLUquadricObj * sphere = gluNewQuadric();
	glActiveTexture(GL_TEXTURE0);
	gluQuadricTexture(sphere, GL_TRUE);

	glPushMatrix();
	    glRotatef(earthTranslation*3, 0, 1, 0);

		drawCircle(jupiterAU);
	    glTranslatef(0.0, 0.0, jupiterAU);
	    glRotatef(90.0, 1, 0, 0);
	    glRotatef(zRotated / 10, 0, 0, 1);
	    if (toScale == 5) glScalef(2.0, 2.0, 2.0);
	    glEnable ( GL_TEXTURE_2D );
	    glBindTexture ( GL_TEXTURE_2D, jupiterTex);
	    gluSphere(sphere, jupiterRadius, verticalLines, horizontalLines);
	    glDisable ( GL_TEXTURE_2D );
    glPopMatrix();
}

// Draws a sphere and add texture as backgroud stars.
void stars() {
	GLUquadricObj * sphere = gluNewQuadric();
	glActiveTexture(GL_TEXTURE0);
	gluQuadricTexture(sphere, GL_TRUE);

	glPushMatrix();
	    glRotatef(90.0, 1, 0, 0);

	    //Rotate the stars so the universe doesn't look static
		glRotatef(zRotated/8000, 0, 0, 1);

	    glEnable ( GL_TEXTURE_2D );
	    glBindTexture ( GL_TEXTURE_2D, starsTex);

	    // float radius = cameraDistance+10;
	    // if (radius < jupiterAU) radius = jupiterAU;
	    // the sphere is always a bit bigger than the observer's distance to the center
	    gluSphere(sphere, cameraDistance+10, verticalLines, horizontalLines);
	    glDisable ( GL_TEXTURE_2D );

    glPopMatrix();
}


// set and update perspective and viewport
void init(int x, int y) {
    // Enter projection matrix mode to set perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLdouble) x / (GLdouble) y, 0.5, (cameraDistance+10)*2);
    glMatrixMode(GL_MODELVIEW);

    // update viewport if necessery
    glViewport(0, 0, x, y);
    windowWidth = x;
    windowHeight = y;
}


void draw(void) {

    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glLoadIdentity();

    // Specify camera position
	float eyeX = cameraDistance * -sinf(hOrbit*deg2rad) * cosf((vOrbit)*deg2rad);
	float eyeY = cameraDistance * -sinf((vOrbit)*deg2rad);
	float eyeZ = -cameraDistance * cosf((hOrbit)*deg2rad) * cosf((vOrbit)*deg2rad);

	gluLookAt(eyeX, eyeY, eyeZ,
	          0.0, 0.0, 0.0, 
	          0.0, 1.0, 0.0);
	setMaterialColor(0.8,0.8,0.8);

	// Draw planets
    drawSun();
	drawMercury();
	drawVenus();
    drawEarth();
    drawMars();
    drawJupiter();
    stars();
    
    glutSwapBuffers();
}

void idleFunc(void) {
    zRotated += earthDay;
    // if not in analysis mode, rotate around sun
    if (!analisysMode)
    	earthTranslation += (earthDay / 365);
    glutPostRedisplay();
}

void keyboard(int key, int x, int y) {

	// Rotate camera
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

    cout << "H: " << hOrbit << " - V: " << vOrbit << " - R: " << cameraDistance << endl;
    
    glutPostRedisplay();
}

void keyboardManager(unsigned char key, int x, int y) {
	// Set planet to scale
	if (isdigit(key)) {
        int value = key - '0';
        if (toScale == value) {
            toScale = -1;
        } else {
            toScale = value;
        }
    }
    // Change poligon modes
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
        cameraDistance += 4;
        init(windowWidth, windowHeight);
        break;
    case 'x':
        cameraDistance -= 4;
        init(windowWidth, windowHeight);
        break;
    }

    glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y) {

	// Change poligon mode
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
    	// Enter analisys mode Dolores
    	analisysMode = !analisysMode;
    }
}

void setupLighting() {
    // enable light
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Set lighting intensity and color
    GLfloat qaAmbientLight[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat qaDiffuseLight[] = { 0.2, 0.2, 0.2, 1 };
    GLfloat qaSpecularLight[]	= {0.8, 0.8, 0.8, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

    // Set the light position as center of the sun
    GLfloat qaLightPosition[] = { 0, 0, 0, 1 };

    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
}

// Load all textures
void loadAllTextures() {
	sunTex = LoadTexture("Bitmaps/sun.bmp", 2048, 1024);
	earthTex = LoadTexture( "Bitmaps/earthmap.bmp", 1000, 500 );
	marsTex = LoadTexture( "Bitmaps/marsmap.bmp", 1024, 512 );

	moonTex = LoadTexture( "Bitmaps/moonmap.bmp", 1024, 512 );
	mercuryTex = LoadTexture( "Bitmaps/mercurymap.bmp", 1024, 512 );
	venusTex = LoadTexture( "Bitmaps/venusmap.bmp", 1024, 512 );
	jupiterTex = LoadTexture( "Bitmaps/jupitermap.bmp", 1024, 512 );
	starsTex = LoadTexture( "Bitmaps/stars.bmp", 4096, 2048 );
}

int main(int argc, char * * argv) {
    glutInit( & argc, argv);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(windowWidth, windowHeight);


    glutInitWindowPosition(0, 0);
    glutCreateWindow("You Spin me right round, baby right round");

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutMouseFunc(mouseButton);
    
    glutSpecialFunc(keyboard);
    glutKeyboardFunc(keyboardManager);
    glutDisplayFunc(draw);
    glutReshapeFunc(init);
    glutIdleFunc(idleFunc);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    loadAllTextures();
    setupLighting();
    glutMainLoop();

    return 0;
}

