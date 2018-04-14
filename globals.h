
const GLfloat PI = 3.1415926535;
const float deg2rad = PI/180;

float cameraDistance = 22; 


float definition = 1;
float verticalLines = 20*definition;
float horizontalLines = 50*definition;
 
float xRotated = 90.0, yRotated = 0.0, zRotated = 0.0;
float earthTranslation = 0.0;
float earthDay = 70;

// Radius
float earthRadius = 0.5;
float sunRadius = earthRadius*5;
float marsRadius = earthRadius*0.5;
float venusRadius = earthRadius*0.95;

float au = 4 + sunRadius + earthRadius;

// Distances to sun
float marsAU = au*1.524 + marsRadius;
float venusAU = au*0.723 + venusRadius;
