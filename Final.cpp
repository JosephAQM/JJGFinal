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


/***************************************************************************************
GLOBALS
***************************************************************************************/

//Terrain heightmap
float terrain[300][300];

//Parallel array to terrain[][] that signies if the point should be used in
//the circles algorithm.
bool toCircle[300][300];

//0 = solid, 1 = wireframe, 2 = both
int drawMode = 0;

//Terrain dimensions
int terrainSizeX = 0;
int terrainSizeZ = 0;

float terrainCircleSize = 30;

//Camera stuff
float pos[] = {0,0,0};
float rot[] = {0, 1, 0};
float headRot[] = {0, 0, 0};
float camPos[] = {7, 7, 7};
float angle = 0.0f;

//light stuff
float lightpos[4] = {0, 2, 0, 0};
float amb[4] = {1, 1, 1, 1};
float diff[4] = {2, 2,2, 2};
float spec[4] = {0, 0, 1, 1};
bool useLight = true;

//selected quad
int selX = 0;
int selZ = 0;



void keyboard(unsigned char key, int x, int y)
{

	switch (key)
	{	
		//Exit program
		case 'q':
		case 27:
			exit (0);
			break;
	}
	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	//Move camera
	switch(key)
	{
		case GLUT_KEY_LEFT:
			//camPos[2]-=0.1;
			break;

	}
	glutPostRedisplay();
}

void init(void)
{
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45, 1, 1, 100);
}


/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void)
{
	float origin[3] = {0,0,0};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
	glColor3f(1,1,1);


	glutSwapBuffers();
}

/* main function - program entry point */
int main(int argc, char** argv)
{

	printf("\nWelcome to Joseph's Terrain Assignment!\n\nControls:\nArrow Keys -> Camera movement\n'e' -> Toggle wireframe modes'\n'f' -> Toggle lighting\n'i,j,k,l' -> Move terrain selection (red square)\n'y' -> Increase height of selected terrain\n'q' -> Quit\n\n");


	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("3GC3 Final   John Zhang/Gabriel Lopez/Joseph Manalo");	//creates the window
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	glEnable(GL_DEPTH_TEST);
	init();

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}