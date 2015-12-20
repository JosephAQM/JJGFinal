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
#include <vector>
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

//----------------------------------------- CLASSES ----------------------------------------------

class Block {
private:
	//Shape location
	float xPos;
	float yPos;
	float zPos;

	//Sizes
	float length;
	float defaultSideLength;

	//Orientation of the block, X or Z
	char orient;

	bool isKeyBlock;
	bool isGrabbed;

public:
	//Constructors
	Block(){
		xPos = 0;
		yPos = 0;
		zPos = 0;
		isKeyBlock = false;
		isGrabbed = false;
	}

	void set(float x, float y, float z, float inputLength, bool isKey, bool orientX){
		xPos = x;
		yPos = y;
		zPos = z;
		length = inputLength;
		defaultSideLength = 2; //Built in scaling for block
		isKeyBlock = isKey;
		isGrabbed = false;
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
		return length/2;
	}

	float getXLength(){
		if (orient == 'X')
			return length;
		else
			return 1;
	}

	float getZLength(){
		if (orient == 'Z')
			return length;
		else
			return 1;
	}

	char getOrient(){
		return orient;
	}

	//Functions
	void draw(){
		glPushMatrix();

		//Move block origin to appropriate location
		glTranslated(xPos,yPos,zPos);
		float m_amb[] = {0.33, 0.22, 0.03, 1.0};
		float m_dif[] = {0.78, 0.57, 0.11, 1.0};
		float m_spec[] = {0.99, 0.91, 0.81, 1.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 27);

		
		//Change to brown later
		if (isKeyBlock) {glColor3d(1, 0, 0);}
 		else glColor3d(1, 0.5, 0);

 		//Scale cube to create block in correct oriantaion and length
 		if (orient == 'x')
 			glScaled(length, 1, 1);
 		else 
 			glScaled(1, 1, length);

 		//Default size 2x2x2 * scaling
 		//Size of 2 is equivalent to 1 unit
		glutSolidCube(defaultSideLength);

		glPopMatrix(); 
	}
	
	//Checks if the given point is within the shape's hitbox
	bool isInHitBox(float x, float y, float z){

		float blockWidth = 2;

		float x1 = xPos;
		float z1 = zPos;
		float x1length;
		float z1length;
		float y1 = yPos;
		float y1length = blockWidth;

		if (orient == 'x'){
			x1length = length/2.0;
			z1length = blockWidth;
	   }

	   else if (orient == 'z'){
			x1length = blockWidth;
			z1length = length/2.0;
	   }

		if ((x <= x1+x1length) && (x >= x1-x1length)){
			if ((z <= z1+z1length) && (z >= z1-z1length)){
				if ((y <= y1 + y1length) && (y >= y1-y1length)){
					return true;
				}
			}	
		}
			
		return false;
	}

	//Input block to check collision with, and direction THIS block is moving in (pos 1, neg 1)
	//Input block to check collision with, and direction THIS block is moving in (pos 1, neg 1)
	bool collisionCheck(Block block, float amount){
		//Once again, generate points on each end of this block

		//To calculate where the block wants to move
		float moveAmnt = amount;

		float blockWidth = 2;

		float x1 = xPos;
		float z1 = zPos;
		float x1length;
		float z1length;

		if (orient == 'x'){
			x1+=moveAmnt;
			x1length = length/2.0;
			z1length = blockWidth;
	   }

	   else if (orient == 'z'){
	   		z1+=moveAmnt;
			x1length = blockWidth;
			z1length = length/2.0;
	   }

	   	float x2 = block.getXPos();
		float z2 = block.getZPos();
		float x2length;
		float z2length;

	   if (block.getOrient() == 'x'){
			x2length = block.getLength()/2.0;
			z2length = blockWidth;
	   }
	   else if (block.getOrient() == 'z'){
			x2length = blockWidth;
			z2length = block.getLength()/2.0;
	   }

	   if (block.getOrient() == orient){
	   		return false;
	   }
	    return (((x1 - x1length) <= (x2 + x2length)) && ((x1 + x1length) >= (x2 - x2length)) &&
      ((z1 + z1length) >= (z2 - z2length)) && ((z1 - z1length) <= (z2 + z2length)));
			
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

	void grab(){
		isGrabbed = true;
	}

	void unGrab(){
		isGrabbed = false;
	}

	bool getGrabbed(){
		return isGrabbed;
	}

	bool isKey(){
		return isKeyBlock;
	}

};

class SolidSphere
{
protected:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;

public:
    SolidSphere(float radius, unsigned int rings, unsigned int sectors)
    {
        float const R = 1./(float)(rings-1);
        float const S = 1./(float)(sectors-1);
        int r, s;

        vertices.resize(rings * sectors * 3);
        normals.resize(rings * sectors * 3);
        texcoords.resize(rings * sectors * 2);
        std::vector<GLfloat>::iterator v = vertices.begin();
        std::vector<GLfloat>::iterator n = normals.begin();
        std::vector<GLfloat>::iterator t = texcoords.begin();
        for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
                float const y = sin( -M_PI_2 + M_PI * r * R );
                float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
                float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

                *t++ = s*S;
                *t++ = r*R;

                *v++ = x * radius;
                *v++ = y * radius;
                *v++ = z * radius;

                *n++ = x;
                *n++ = y;
                *n++ = z;
        }

        indices.resize(rings * sectors * 4);
        std::vector<GLushort>::iterator i = indices.begin();
        for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
                *i++ = r * sectors + s;
                *i++ = r * sectors + (s+1);
                *i++ = (r+1) * sectors + (s+1);
                *i++ = (r+1) * sectors + s;
        }
    }

    void draw(GLfloat x, GLfloat y, GLfloat z)
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(x,y,z);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
        glNormalPointer(GL_FLOAT, 0, &normals[0]);
        glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
        glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
        glPopMatrix();
    }
};


//-----------------------------------------------------------------------------------------------


//----------------------------------------------- GLOBALS ---------------------------------------

SolidSphere sphere(0.3, 24, 24);
//Array of objects containing all of the blocks (arbritrary limit of 20)
Block sceneBlocks[20];
//Contains whether the blocks are currently being used or not
bool activeBlocks[20];

bool grabbing = false;
int toSelect;

//Test coordinates for grabbing mechanic
float test1x;
float test1z;

int windowSizeWidth = 1280;
int windowSizeHeight = 720;

//an array for iamge data
GLubyte* tex;
int width, height, max;

//Coordinates of mouse click in the world
float worldX;
float worldY;
float worldZ;

//camPos coords when click occurs
float cX;
float cY;
float cZ;

//Mouse ray slopes (d for delta)
float deltaY;
float deltaX;
float deltaZ;



//-----------------------------------------------------------------------------------------------

void drawTestPoint(int X, int Z){
	glPushMatrix();

	//Move block origin to appropriate location
	glTranslated(X,-4,Z);

	{glColor3d(1, 0, 0);}

	glutSolidCube(0.5);

	glPopMatrix(); 
}

void drawRay(){
	glBegin(GL_LINES);
		glColor3f(1, 1, 1);
		glVertex3f(cX, cY, cZ);
		glVertex3f(worldX + deltaX * 10, worldY + deltaY * 10, worldZ + deltaZ * 10);
	glEnd();

	glutPostRedisplay();
}



void mouseToWorld(int mouseX, int mouseY, int mouseZ){
    GLint viewport[4];
    GLdouble modelMatrix[16];   
    GLdouble projectionMatrix[16];  

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);

    float winY = float(viewport[3] - mouseY);

    double x, y, z;
    gluUnProject((double)mouseX, winY, mouseZ, 
        modelMatrix, projectionMatrix, viewport,
        &x, &y, &z);
    
    worldX = x;
    worldY = y;
    worldZ = z;
}

//Cycles through all the points in a line, checks if each object on the list interects with it
int rayCasting(int mouseX, int mouseY){
	mouseToWorld(1280/2, 720/2, 1);

	//0.0f, -4.0f, 5.0f
	//xpos+0.1, zpos+5
	//Calculate slopes for ray

	float wtfX = xpos+0.1;
	float wtfY = ypos-4;
	float wtfZ = zpos+5;

	deltaX = (worldX - wtfX);
	deltaY = (worldY - wtfY);
	deltaZ = (worldZ - wtfZ);

	//Save camera position for ray drawing
	cX = wtfX;
	cY = wtfY;
	cZ = wtfZ;

	//Go through points in the ray
	for (int i = 0; i < 100000; i++){ // lol overkill resolution
		float linePointX = cX + (worldX + deltaX)*(float)i*0.00001;
		float linePointY = cY + (worldY + deltaY)*(float)i*0.00001;
		float linePointZ = cZ + (worldZ + deltaZ)*(float)i*0.00001;

		//Loop through every active shape in the array
		for (int j = 0; j < 20; j++){
			if (activeBlocks[j]){
				if(sceneBlocks[j].isInHitBox(linePointX, linePointY, linePointZ)){
					return j;
				}
			}
		}
	}

	return -1;
}


//Grab block closest to given x,z coords
void grabNearestBlock(float x, float z){
	int closestBlock = 0;
	float distanceToClosest = 99999; //Big ass number

	for (int i = 0; i < 20; i++){
		if (activeBlocks[i]){
			//Calculates a point on each end of the block (depends on orientation)
			float point1x;
			float point1z;

			float point2x;
			float point2z;

			//Can be cleaner
			if (sceneBlocks[i].getOrient() == 'x'){
				point1x = sceneBlocks[i].getXPos() + sceneBlocks[i].getLength();
				point1z = sceneBlocks[i].getZPos();

				point2x = sceneBlocks[i].getXPos() - sceneBlocks[i].getLength();
				point2z = sceneBlocks[i].getZPos();
			}

			else{
				point1x = sceneBlocks[i].getXPos();
				point1z = sceneBlocks[i].getZPos() + sceneBlocks[i].getLength();

				point2x = sceneBlocks[i].getXPos();
				point2z = sceneBlocks[i].getZPos() - sceneBlocks[i].getLength();
			}

			//Calculate distance between the character and each point
			float distance1 = sqrt(pow(point1x - x, 2) + pow(point1z - z, 2));
			float distance2 = sqrt(pow(point2x - x, 2) + pow(point2z - z, 2));

			//Check if either of the two points are closer than any previous block points
			if (distance1 < distanceToClosest){
				closestBlock = i;
				distanceToClosest = distance1;
			}
			if (distance2 < distanceToClosest){
				closestBlock = i;
				distanceToClosest = distance2;				
			}

		}
	}

	printf("Grabbed %i\n", closestBlock);
	sceneBlocks[closestBlock].grab();
}

void ungrabAll(){
	for (int i = 0; i < 20; i++){
		if (activeBlocks[i]){
			sceneBlocks[i].unGrab();
		}
	}
}

bool checkBlockCollisions(float amount){
	for (int i = 0; i < 20; i++){
		if (activeBlocks[i] && sceneBlocks[i].getGrabbed()){
			for (int j = 0; j < 20; j++)
				if ((i != j) && activeBlocks[j] && sceneBlocks[i].collisionCheck(sceneBlocks[j], amount)){
					printf("Collision between %i and %i\n",i ,j);
					return true;
				}
		}
	}
}

//Loops through block list, and moves the grabbed block(s) depending on its orientation
void moveGrabbedBlock(float moveX, float moveY, float moveZ){
	for (int i = 0; i < 20; i++){
		if (activeBlocks[i] && sceneBlocks[i].getGrabbed() ){
			if ((sceneBlocks[i].getOrient() == 'x')){
				if (((moveX > 0) && !checkBlockCollisions(moveX)) || ((moveX < 0) && !checkBlockCollisions(moveX))){
					sceneBlocks[i].move(moveX, 0, 0);
					xpos+=moveX;

				}
			}
			else 
				if (((moveZ > 0) && !checkBlockCollisions(moveZ)) || ((moveZ < 0) && !checkBlockCollisions(moveX))){
					sceneBlocks[i].move(0,0,moveZ);
					zpos+=moveZ;
				}
		}
	}
}

void clearBlocks(){
	for (int i = 0; i < 20; i++){
		activeBlocks[i] = false;
	}
}

bool checkWin(){
	for (int i = 0; i < 20; i++){
		if(activeBlocks[i] && sceneBlocks[i].isKey()){
			if (sceneBlocks[i].getXPos() + sceneBlocks[i].getLength() > 12){
				return true;
			}
		}
	}
}

void generateLevel(int level){
	if (level ==1){
		sceneBlocks[0].set(-6.8, -4 , 0, 2, true, true);
		sceneBlocks[1].set(-7.8, -4 , 4, 3, false, false);
		sceneBlocks[2].set(-4.7, -4 , 6, 2, false, true);
		sceneBlocks[3].set(-3.7, -4 , -2, 3, false, false);
		sceneBlocks[4].set(0.4, -4 , -4, 3, false, true);
		sceneBlocks[5].set(-2.7, -4 , 2.1, 2, false, true);
		sceneBlocks[6].set(0.4, -4 , 5.1, 2, false, false);
		sceneBlocks[7].set(2.5, -4 , 4.1, 3, false, false);

		for (int i = 0; i < 8; i++)
			activeBlocks[i] = true;

	}

	else if (level == 2){
		//x = horizontal, z vertical, 0,0 is middle, -7,-7 is top left (all relative to initial camera)
		//                 x    y    z  length  key     ox
		sceneBlocks[0].set(-7, -4 , -7, 2, false, false);
		sceneBlocks[1].set(-7, -4 , -2, 2, true, true);
		sceneBlocks[2].set(-6, -4 , 1, 3, false, true);
		sceneBlocks[3].set(-7, -4 , 5, 2, false, true);
		sceneBlocks[4].set(-3.7, -4 , 5, 2, false, false);
		sceneBlocks[5].set(-0.3, -4 , 5, 2, false, true);
		sceneBlocks[6].set(5.8, -4 , -6, 4, false, true);
		sceneBlocks[7].set(8.2, -4 , -1, 3, false, false);
		sceneBlocks[8].set(5.7, -4 , 0.08, 2, false, false);
		sceneBlocks[9].set(3.2, -4 , -1.92, 2, false, false);
		sceneBlocks[10].set(5.7, -4 , 5, 2, false, true);

		for (int i = 0; i < 20; i++)
			activeBlocks[i] = true;
	}
}

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



void quad(int a,int b,int c,int d,float scaleX,float scaleY,float scaleZ )
{
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(ver[a][0]*scaleX,ver[a][1]*scaleY,ver[a][2]*scaleZ);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(ver[b][0]*scaleX,ver[b][1]*scaleY,ver[b][2]*scaleZ);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(ver[c][0]*scaleX,ver[c][1]*scaleY,ver[c][2]*scaleZ);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(ver[d][0]*scaleX,ver[d][1]*scaleY,ver[d][2]*scaleZ);
    glEnd();
}
void drawBase(float xp,float yp,float zp,float xr,float yr,float zr,float size){
		glPushMatrix();
		//Translate object
		glTranslated(xp,yp,zp);
		//Rotate Object
		glRotatef(xr, 1.0, 0.0, 0.0);
		glRotatef(yr, 0.0, 1.0, 0.0);
		glRotatef(zr, 0.0, 0.0, 1.0);
		glutSolidCone(size , size*3, 50, 50);
		glPopMatrix(); 
}
void drawTorches(){
	glColor3f(0.98f, 0.625f, 0.12f);
	sphere.draw(3.5, -2, 10.5);
	sphere.draw(-3.5, -2, 10.5);
	sphere.draw(3.5, -2, -10.5);
	sphere.draw(-3.5, -2, -10.5);
	sphere.draw(-9.5, -2, 5);
	sphere.draw(-9.5, -2, -5);
	sphere.draw(9.5, -2, 5);
	sphere.draw(9.5, -2, -5);
	glColor3f( 0.36f, 0.25f, 0.2f);
	drawBase(3.5,-2.2,10.6,70,0,45,0.4);
	drawBase(-3.5,-2.2,10.6,70,0,45,0.4);
	drawBase(3.5,-2.2,-10.6,110,0,45,0.4);
	drawBase(-3.5,-2.2,-10.6,110,0,45,0.4);
	drawBase(-9.5,-2.2,5,90,-22,45,0.4);
	drawBase(-9.5,-2.2,-5,90,-22,45,0.4);
	drawBase(9.5,-2.2,5,90,22,45,0.4);
	drawBase(9.5,-2.2,-5,90,22,45,0.4);
}
void room1()
{
    glColor3f(0.5,0.5,0.5);
    quad(0,3,2,1,10,5,11);
    //glColor3fv(color[1]);
    quad(2,3,7,6,10,5,11);
    //glColor3fv(color[2]);
    quad(0,4,7,3,10,5,11);
    //glColor3fv(color[3]);
    quad(1,2,6,5,10,5,11);
    //glColor3fv(color[4]);
    quad(4,5,6,7,10,5,11);
    //glColor3fv(color[5]);
    quad(0,1,5,4,10,5,11);
}

/* LoadPPM -- loads the specified ppm file, and returns the image data as a GLubyte 
 *  (unsigned byte) array. Also returns the width and height of the image, and the
 *  maximum colour value by way of arguments
 *  usage: GLubyte myImg = LoadPPM("myImg.ppm", &width, &height, &max);
 */
GLubyte* LoadPPM(char* file, int* width, int* height, int* max)
{
	GLubyte* img;
	FILE *fd;
	int n, m;
	int  k, nm;
	char c;
	int i;
	char b[100];
	float s;
	int red, green, blue;
	
	fd = fopen(file, "r");
	fscanf(fd,"%[^\n] ",b);
	if(b[0]!='P'|| b[1] != '3')
	{
		printf("%s is not a PPM file!\n",file); 
		exit(0);
	}
	printf("%s is a PPM file\n", file);
	fscanf(fd, "%c",&c);
	while(c == '#') 
	{
		fscanf(fd, "%[^\n] ", b);
		printf("%s\n",b);
		fscanf(fd, "%c",&c);
	}
	ungetc(c,fd); 
	fscanf(fd, "%d %d %d", &n, &m, &k);

	printf("%d rows  %d columns  max value= %d\n",n,m,k);

	nm = n*m;

	img = (GLubyte*)(malloc(3*sizeof(GLuint)*nm));

	s=255.0/k;


	for(i=0;i<nm;i++) 
	{
		fscanf(fd,"%d %d %d",&red, &green, &blue );
		img[3*nm-3*i-3]=red*s;
		img[3*nm-3*i-2]=green*s;
		img[3*nm-3*i-1]=blue*s;
	}

	*width = n;
	*height = m;
	*max = k;

	return img;
}

void init(void) {
	//glutSetCursor(GLUT_CURSOR_NONE); 
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST); //enable the depth testing

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glEnable(GL_LIGHTING); //enable the lighting
	glEnable(GL_LIGHT0); //enable LIGHT0, our Diffuse Light
	glEnable(GL_NORMALIZE);

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
	
	

	//load specified ppm file for use as texture
	tex = LoadPPM("bricks2.ppm", &width, &height, &max);
}

void drawAllBlocks(){
	for (int i = 0; i < 20; i++){//(sizeof(sceneShapes)/sizeof(*sceneShapes)); i++) {
		if (activeBlocks[i]) {
			glTexCoord2f(1, 1);
			sceneBlocks[i].draw();
		}
	}
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
	gluLookAt(	0.0f, -4.0f, 5.0f,
				0.0f, -4.0f,  0.0f,
				0.0f, 20.0f,  0.0f);


	drawRay();
	//glRotatef(angle, 0.0f, 1.0f, 0.0f);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	room1();
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	drawTorches();
	drawAllBlocks();
	//--------------------------------------set texture------------------------------------
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 
					0, GL_RGB, GL_UNSIGNED_BYTE, tex);


	//set texture proterties
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//angle+=0.5f;
	//drawTestPoint(test1x, test1z);
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
	if(key=='q'){ //rotate view down
	xrot += 1;
	if(xrot >45) xrot = 45;
	}

	if(key=='z'){ //rotate view up
	xrot -= 1;
	if(xrot < -30) xrot = -30;
	}
	//w,a,s,d movement keys
	if(key=='w'){
	float xrotrad, yrotrad;
	yrotrad = (yrot / 180 * 3.141592654f);
	xrotrad = (xrot / 180 * 3.141592654f); 
	moveGrabbedBlock(float(sin(yrotrad) * 0.3), -float(cos(yrotrad) * 0.3), -float(sin(xrotrad)) * 0.3);

	if (!grabbing){
		xpos += float(sin(yrotrad)) * 0.3;
		zpos -= float(cos(yrotrad)) * 0.3;
		ypos -= float(sin(xrotrad)) * 0.3;
		}
	}

	if(key=='s'){
	float xrotrad, yrotrad;
	yrotrad = (yrot / 180 * 3.141592654f);
	xrotrad = (xrot / 180 * 3.141592654f); 
	moveGrabbedBlock(-float(sin(yrotrad) * 0.3), float(cos(yrotrad) * 0.3), float(sin(xrotrad)) * 0.3);
	if (!grabbing){
		xpos -= float(sin(yrotrad)) * 0.3;
		zpos += float(cos(yrotrad)) * 0.3;
		ypos += float(sin(xrotrad)) * 0.3;
	}
	}

	if(key=='d'){
	float yrotrad;
	yrotrad = (yrot / 180 * 3.141592654f);
	moveGrabbedBlock(float(cos(yrotrad)) * 0.3, 0, float(sin(yrotrad)) * 0.3);

	if (!grabbing){
		xpos += float(cos(yrotrad)) * 0.3;
		zpos += float(sin(yrotrad)) * 0.3;
	}
	
	}

	if(key=='a'){
	float yrotrad;
	yrotrad = (yrot / 180 * 3.141592654f);
	moveGrabbedBlock(-float(cos(yrotrad)) * 0.3, 0, -float(sin(yrotrad)) * 0.3);
	if (!grabbing){
		xpos -= float(cos(yrotrad)) * 0.3;
		zpos -= float(sin(yrotrad)) * 0.3;
	}
	}

	if(key==27){ // esc key to quit
	exit(0);
	}
}

void mouse(int btn, int state, int x, int y){
	switch(btn){
		case GLUT_LEFT_BUTTON:
			if(state==GLUT_DOWN){
				ungrabAll();
				grabbing = true;
				
				grabNearestBlock(xpos+0.1, zpos+5); //CHARACTER POSITION??
				int toSelect = rayCasting(x, y);

				printf("%i\n",toSelect);
				}
			else if(state==GLUT_UP){
				ungrabAll();
				grabbing = false;
			}
			break;

	}
}

void mouseMovement(int x, int y) {
	int diffx=x-lastx; //check the difference between the current x and the last x position
	int diffy=y-lasty; //check the difference between the current y and the last y position
	lastx=x; //set lastx to the current x position
	lasty=y; //set lasty to the current y position
	 //set the xrot to xrot with the addition of the difference in the y position
	
	yrot += (float) diffx;    //set the xrot to yrot with the addition of the difference in the x position
	if(xrot < 70){
		xrot += (float) diffy;
	}else{
		xrot = 70;
		
	}
	if(xrot > -10){
		xrot += (float) diffy;
	}else{
		xrot = -10;
		
	}

	
	if(x >= windowSizeWidth-100){
		
		glutWarpPointer(windowSizeWidth-101,y);
		yrot += 3;
		
	}else if(x <= 100){
		
		glutWarpPointer(101,y);
		yrot -= 3;
		
	}

	if(y >= windowSizeHeight-100){
		glutWarpPointer(x,windowSizeHeight-101);
	}else if(y <= 100){
		glutWarpPointer(x,100);
	}

}

int main(int argc, char **argv) {
	// init GLUT and create window

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
	glutWarpPointer(windowSizeWidth/2, windowSizeHeight/2);
	glutPassiveMotionFunc(mouseMovement); //check for mouse movement
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	generateLevel(1);
	//                 x,y,z pos length key orient x
	// sceneBlocks[0].set(-6.0, -3 , -6.0, 2, false, true);
	// activeBlocks[0] = true;

	// sceneBlocks[1].set(-5.0, -3 , -4.0, 3, false, false);
	// activeBlocks[1] = true;

	//sceneBlocks[0].grab();
	// enter GLUT event processing loop
	glutMainLoop();

	return 1;
}
