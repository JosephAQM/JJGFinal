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

int currentLevel;

//Camera stuff (from A2, might be useful)
float pos[] = {0,0,0};
float rot[] = {0, 1, 0};
float headRot[] = {0, 0, 0};
float camPos[] = {7, 7, 7};
float angle = 0.0f;

//light stuff (from A2, might be useful)
float lightpos[4] = {0, 2, 0, 0};
float amb[4] = {1, 1, 1, 1};
float diff[4] = {2, 2,2, 2};
float spec[4] = {0, 0, 1, 1};
bool useLight = true;

/***************************************************************************************
FUNCTIONS
***************************************************************************************/

//Possible breakdown of classes

class Player {

	//Player location
	float xPos;
	float yPos;
	float zPos;

	//Possible state for jumping
	bool jumping;

	void draw(){

	}

	//Move in the direction the player is currently facing
	void move(){

	}

	//Rotate character, use string input to determine CW or CCW?
	void rotate(string direction){

	}

	//Does the player jump?
	void jump(){

	}


};

//Blocks to be pushed by player
class Block {

	//Block dimensions
	float xDim;
	float yDim;
	float zDim;

	//Block positions (can be defined as the center of the box for simplicity)
	float xPos;
	float yPos;
	float zPos;

	//Draws the block
	void draw(){

	}

	//Moves the block, should blocks move in set intervals? aka have a sort of grid system
	//within the map
	void move(){

	}
};

//The general map that the player and objects are placed in
//Should the map just be hardcoded?
class Map {
	void draw(){

	}
};

//Responsible for loading the individual puzzlbes
//Block placement, checking win conditions, etc
//one puzzle object = one set of block placements
class Puzzle{


	bool checkWin(){
		
	}


};


//Keyboard controls
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

//Special key entries
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


// display function - GLUT display callback function 
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

// main function - program entry point
int main(int argc, char** argv)
{

	printf("\nWelcome to our final project!\n");

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