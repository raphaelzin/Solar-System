
const GLfloat PI = 3.1415926535;
const float deg2rad = PI/180;

float cameraDistance = 48; 


float definition = 1;
float verticalLines = 20*definition;
float horizontalLines = 50*definition;
 
float xRotated = 90.0, yRotated = 0.0, zRotated = 0.0;
float earthTranslation = 0.0;
float earthDay = 140;

// Radius
float earthRadius = 0.5;
float sunRadius = earthRadius*10;
float marsRadius = earthRadius*0.5;
float venusRadius = earthRadius*0.95;
float jupiterRadius = earthRadius*11.2;
float saturnRadius = earthRadius*9.45;
float uranusRadius = earthRadius*4;
float neptuneRadius = earthRadius*3.88;
float mercuryRadius = earthRadius*0.38;

float au = 10 + sunRadius + earthRadius;

// Distances to sun
float mercuryAU = au*0.39 + mercuryRadius;
float marsAU = au*1.524 + marsRadius;
float venusAU = au*0.723 + venusRadius;
float jupiterAU = au*5.2 + jupiterRadius;
float saturnAU = au*9.54 + saturnRadius;
float uranusAU = au*19.2 + uranusRadius;
float neptuneAU = au*30.1 + neptuneRadius;
// float plutoAU = au*39.4;