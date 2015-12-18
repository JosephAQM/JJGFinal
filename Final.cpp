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

float angle = 0.0f;
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
	glBegin(GL_QUADS);
		glColor3fv(color[a]);
		glVertex3f(ver[a][0]*scaleX,ver[a][1]*scaleY,ver[a][2]*scaleZ);
		
		glColor3fv(color[b]);
		glVertex3f(ver[b][0]*scaleX,ver[b][1]*scaleY,ver[b][2]*scaleZ);
		
		glColor3fv(color[c]);
		glVertex3f(ver[c][0]*scaleX,ver[c][1]*scaleY,ver[c][2]*scaleZ);
		
		glColor3fv(color[d]);
		glVertex3f(ver[d][0]*scaleX,ver[d][1]*scaleY,ver[d][2]*scaleZ);
	
	glEnd();
}

void room1()
{
    quad(0,3,2,1,10,5,11);
    quad(2,3,7,6,10,5,11);
    quad(0,4,7,3,10,5,11);
    quad(1,2,6,5,10,5,11);
    quad(4,5,6,7,10,5,11);
    quad(0,1,5,4,10,5,11);
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

void renderScene(void) {


	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(	0.0f, 0.0f, 10.0f,
				0.0f, 0.0f,  0.0f,
				0.0f, 1.0f,  0.0f);

	glRotatef(angle, 0.0f, 1.0f, 0.0f);
//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	room1();
	angle+=0.5f;

	glutSwapBuffers();
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

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);


	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	// here is the idle func registration
	glutIdleFunc(renderScene);

	// enter GLUT event processing loop
	glutMainLoop();

	return 1;
}
