#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//-----------------------------Things for movement-----------------------------------------------
//angle of rotation
float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle=0.0;

float cRadius = 10.0f; //radius distance from the character

float lastx, lasty; //used to remember previous x and y positions

//positions of the cubes
float positionz[10];
float positionx[10];

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
	glMatrixMode(GL_PROJECTION); //set the matrix to projection

	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 100.0); //set the perspective (angle of sight, width, height, , depth)
	glMatrixMode(GL_MODELVIEW); //set the matrix back to model
}

//-----------------------------------------------------------------------------------------------

//-----------------------------      Classes       -----------------------------------------------

class Block {
private:
	//Shape location
	float xPos;
	float yPos;
	float zPos;

	//Sizes
	float length;

	//Orientation of the block, X or Z
	char orient;

	bool isKeyBlock;

public:
	//Constructors
	Block(){
		xPos = 0;
		yPos = 0;
		zPos = 0;
		isKeyBlock = false;
	}

	void set(float x, float y, float z, float inputLength, bool isKey, bool orientX){
		xPos = x;
		yPos = y;
		zPos = z;
		length = inputLength;
		isKeyBlock = isKey;
		if (orientX) {orient = 'x';}
		else {orient = 'z';}
	}

	//Getters
	float getXPos(){
		return xPos;
	}

	float getYPos(){
		return yPos;
	}

	float getZPos(){
		return zPos;
	}

	float getLength(){
		return length;
	}

	//Functions
	void draw(){
		glPushMatrix();

		glTranslated(xPos,yPos,zPos);
		
		//Change to brown later
		if (isKeyBlock) {glColor3d(1, 0, 0);}
 		else glColor3d(1, 0.5, 0.5);

 		//Scale cube to create block in correct oriantaion and length
 		if (orient == 'x')
 			glScaled(length, 1, 1);
 		else 
 			glScaled(1, 1, length);

 		//Default size 1
		glutSolidCube(1);

		glPopMatrix(); 
	}
	

	//Move object in specified direction
	void move(float moveX, float moveY, float moveZ){
		xPos += moveX;
		yPos += moveY;
		zPos += moveZ;
	}


	void increaseSize(float changeAmount){
		length += changeAmount;
	}

	void decreaseSize(float changeAmount){
		length -= changeAmount;
	}

};

//-----------------------------------------------------------------------------------------------




//float angle = 0.0f;
float ver[8][3] = 
{
    {-1.0,-1.0,1.0},
    {-1.0,1.0,1.0},
    {1.0,1.0,1.0},
    {1.0,-1.0,1.0},
    {-1.0,-1.0,-1.0},
    {-1.0,1.0,-1.0},
    {1.0,1.0,-1.0},
    {1.0,-1.0,-1.0},
};

GLfloat color[8][3] = 
{
    {0.0,0.0,0.0},
    {1.0,0.0,0.0},
    {1.0,1.0,0.0},
    {0.0,1.0,0.0},
    {0.0,0.0,1.0},
    {1.0,0.0,1.0},
    {1.0,1.0,1.0},
    {0.0,1.0,1.0},
};

void quad(int a,int b,int c,int d,float scaleX,float scaleY,float scaleZ )
{
	int i;
	GLfloat d1[] = { 0.1, 0.1, 0.1, 1.0 };
	//glMaterialfv(GL_FRONT,GL_DIFFUSE,d1);
    glBegin(GL_QUADS);
    //glNormal3f(0,0,1);
    	//glColor3fv(color[a]);
    	glVertex3f(ver[a][0]*scaleX,ver[a][1]*scaleY,ver[a][2]*scaleZ);
//glNormal3f(0,0,1);
    	//glColor3fv(color[b]);
		glVertex3f(ver[b][0]*scaleX,ver[b][1]*scaleY,ver[b][2]*scaleZ);
//glNormal3f(0,0,1);
		//glColor3fv(color[c]);
		glVertex3f(ver[c][0]*scaleX,ver[c][1]*scaleY,ver[c][2]*scaleZ);
//glNormal3f(0,0,1);
		//glColor3fv(color[d]);
		glVertex3f(ver[d][0]*scaleX,ver[d][1]*scaleY,ver[d][2]*scaleZ);

    glEnd();
}

void room1()
{
	glColor3fv(color[6]);
    quad(0,3,2,1,10,5,11);
    glColor3fv(color[1]);
    quad(2,3,7,6,10,5,11);
 	glColor3fv(color[2]);
    quad(0,4,7,3,10,5,11);
    glColor3fv(color[3]);
    quad(1,2,6,5,10,5,11);
    glColor3fv(color[4]);
    quad(4,5,6,7,10,5,11);
    glColor3fv(color[5]);
    quad(0,1,5,4,10,5,11);
}

void init(void) {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST); //enable the depth testing

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glEnable(GL_LIGHTING); //enable the lighting
	glEnable(GL_LIGHT0); //enable LIGHT0, our Diffuse Light


	GLfloat qaAmbientLight[]	= {0.2, 0.2, 0.2, 1.0};
	GLfloat qaDiffuseLight[]	= {0.8, 0.8, 0.8, 1.0};
	GLfloat qaSpecularLight[]	= {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

	// Set the light position
	GLfloat qaLightPosition[]	= {.5, .5, 0.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);

	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH); //set the shader to smooth shader
}

void renderScene(void) {
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	//temp character 
	glTranslatef(0.0f, -1.0f, -cRadius);
	glRotatef(xrot,1.0,0.0,0.0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidCube(1); //Our character to follow
	//used for character movement
	glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
	glTranslated(-xpos,0.0f,-zpos); //translate the screen to the position of our camera
	glColor3f(1.0f, 1.0f, 1.0f);
	
	// Set the camera
	gluLookAt(	0.0f, 0.0f, 10.0f,
				0.0f, 0.0f,  0.0f,
				0.0f, 20.0f,  0.0f);

	//glRotatef(angle, 0.0f, 1.0f, 0.0f);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	room1();
	//angle+=0.5f;
	
	glutSwapBuffers();
	angle++; //increase the angle
}

void changeSize(int w, int h) { 

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,100);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
	if(key=='q'){ //rotate view up
	xrot += 1;
	if(xrot >360) xrot -= 360;
	}

	if(key=='z'){ //rotate view down
	xrot -= 1;
	if(xrot < -360) xrot += 360;
	}
	//w,a,s,d movement keys
	if(key=='w'){
	float xrotrad, yrotrad;
	yrotrad = (yrot / 180 * 3.141592654f);
	xrotrad = (xrot / 180 * 3.141592654f); 
	xpos += float(sin(yrotrad));
	zpos -= float(cos(yrotrad));
	ypos -= float(sin(xrotrad));
	}

	if(key=='s'){
	float xrotrad, yrotrad;
	yrotrad = (yrot / 180 * 3.141592654f);
	xrotrad = (xrot / 180 * 3.141592654f); 
	xpos -= float(sin(yrotrad));
	zpos += float(cos(yrotrad));
	ypos += float(sin(xrotrad));
	}

	if(key=='d'){
	float yrotrad;
	yrotrad = (yrot / 180 * 3.141592654f);
	xpos += float(cos(yrotrad)) * 0.2;
	zpos += float(sin(yrotrad)) * 0.2;
	}

	if(key=='a'){
	float yrotrad;
	yrotrad = (yrot / 180 * 3.141592654f);
	xpos -= float(cos(yrotrad)) * 0.2;
	zpos -= float(sin(yrotrad)) * 0.2;
	}

	if(key==27){ // esc key to quit
	exit(0);
	}
}

void mouseMovement(int x, int y) {
	int diffx=x-lastx; //check the difference between the current x and the last x position
	int diffy=y-lasty; //check the difference between the current y and the last y position
	lastx=x; //set lastx to the current x position
	lasty=y; //set lasty to the current y position
	xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
	yrot += (float) diffx;    //set the xrot to yrot with the addition of the difference in the x position
}

int main(int argc, char **argv) {
	// init GLUT and create window
	int windowSizeWidth = 1280;
	int windowSizeHeight = 720;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-windowSizeWidth)/2,
                      	   (glutGet(GLUT_SCREEN_HEIGHT)-windowSizeHeight*1.3)/2); //initial window position is centered and slightly up(cuz laptop screen so I dont have to look down)
	glutInitWindowSize(windowSizeWidth,windowSizeHeight);
	glutCreateWindow("Unblock3D - 3GC3 Final");
	init(); //calls enable functions

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);


	// here is the idle func registration
	glutIdleFunc(renderScene);
	
	//from movement code
	//glutReshapeFunc(reshape);
	glutPassiveMotionFunc(mouseMovement); //check for mouse movement
	glutKeyboardFunc(keyboard);

	// enter GLUT event processing loop
	glutMainLoop();

	return 1;
}
