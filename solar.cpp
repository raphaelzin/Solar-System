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

#include <iostream>
#define width 800
#define height 800
using namespace std;

float hOrbit = 180;
float vOrbit = -40;
int polyMode = 0;

// -1 means no planet should scale. 
int toScale = -1;


void drawCircle(float radius) {
    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 360; i++) {
        float degInRad = i * deg2rad;
        glColor3f(1, 1, 1);
        glVertex3f(cos(degInRad) * radius, 0, sin(degInRad) * radius);
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
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
}

void drawEarthMoon() {
    glPushMatrix();
    glRotatef(earthTranslation * 8, 0, 1, 0);
    setMaterialColor(1, 1, 1);
    glPushMatrix();
    glTranslatef(0.0, 0.0, earthRadius + (earthRadius * 0.27) * 2);

    glRotatef(90.0, 1, 0, 0);
    glutSolidSphere(earthRadius * 0.27, verticalLines, horizontalLines);
    glPopMatrix();
    glPopMatrix();
}

void drawEarth() {

    glPushMatrix();

    glRotatef(earthTranslation, 0, 1, 0);

    glPushMatrix();
    drawCircle(au);
    glTranslatef(0.0, 0.0, au);

    drawEarthMoon();

    setMaterialColor(0.1, 0.2, 0.8);

    // Axial tilt of 23.44 -> 90-23.44
    glRotatef(66.56, 1, 0, 0);

    //Translation around self axis
    glRotatef(zRotated / 50, 0, 0, 1);

    if (toScale == 3) {
        glScalef(2.0, 2.0, 2.0);
    }

    glutSolidSphere(earthRadius, verticalLines, horizontalLines);

    glPopMatrix();
    glPopMatrix();
}

void drawSun() {
    GLUquadricObj * sphere = gluNewQuadric();
    gluQuadricTexture(sphere, GL_TRUE);

    setMaterialColor(1, 1, 0);
    glPushMatrix();
    glRotatef(90.0, 1, 0, 0);
    glRotatef(zRotated / 360, 0, 0, 1);
    gluSphere(sphere, sunRadius, verticalLines, horizontalLines);
    glPopMatrix();
}

void drawMars() {
    glPushMatrix();
    glRotatef(earthTranslation * -1.3, 0, 1, 0);

    setMaterialColor(0.8, 0.2, 0.1);

    glPushMatrix();
    glTranslatef(0.0, 0.0, marsAU);

    // Axial tilt of 25.19 -> 90-25.19
    glRotatef(64.81, 1, 0, 0);

    glRotatef(zRotated / 10, 0, 0, 1);
    glutSolidSphere(earthRadius * 0.7, verticalLines, horizontalLines);
    glPopMatrix();
    glPopMatrix();
}

void drawVenus() {
    glPushMatrix();
    glRotatef(earthTranslation * 0.7, 0, 1, 0);

    setMaterialColor(0.0, 0.7, 0.1);
    glPushMatrix();
    glTranslatef(0.0, 0.0, venusAU);
    glRotatef(90.0, 1, 0, 0);
    glRotatef(zRotated / 10, 0, 0, 1);
    glutSolidSphere(venusRadius * 0.7, verticalLines, horizontalLines);
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

void Draw_Spheres(void) {

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
    drawEarth();
    drawMars();
    drawVenus();
    glutSwapBuffers();
}

void idleFunc(void) {
    zRotated += earthDay;
    earthTranslation += (earthDay / 365);
    glutPostRedisplay();
}

void keyboard(int key, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    if (key == GLUT_KEY_LEFT) {
        hOrbit += 10;
    } else if (key == GLUT_KEY_RIGHT) {
        hOrbit -= 10;
    }
    if (key == GLUT_KEY_UP) {
        vOrbit += 10;
    } else if (key == GLUT_KEY_DOWN) {
        vOrbit -= 10;
    }
    // Request display update
    glutPostRedisplay();
}

/* Função callback chamada para gerenciar eventos de teclado */
void GerenciaTeclado(unsigned char key, int x, int y) {
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
    case '3':
        toScale = 3;
        break;
    }


    if (isdigit(key)) {
        int value = (int) key;
        if (toScale == value) {
            toScale = -1;
        } else {
            toScale = value;
        }
    }

    glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        cout << "left";
        polyMode = ++polyMode % 3;

        if (polyMode == 0) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else if (polyMode == 1) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }

    }
}

void init() {
    // Lighting set up
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Set lighting intensity and color
    GLfloat qaAmbientLight[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat qaDiffuseLight[] = { 1, 1, 1, 1 };
    // GLfloat qaSpecularLight[]	= {0.3, 0.3, 0.3, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
    // glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

    // Set the light position
    GLfloat qaLightPosition[] = { 0, 0, 0, 1 };

    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
    glEnable(GL_DEPTH_TEST);
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
    glutKeyboardFunc(GerenciaTeclado);
    glutDisplayFunc(Draw_Spheres);
    glutReshapeFunc(reshapeFunc);
    glutIdleFunc(idleFunc);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glutMainLoop();
    return 0;
}