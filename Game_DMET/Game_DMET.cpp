#pragma once
#include "pch.h"

vector<GameObject *> objects;
Camera camera;
Player player;
EnemySpawner spawner;

// Dummy Objects
WoodChair chair;
House house;
Tree tree;
GroundGrass grass;

int screenX = 1400;
int screenY = 900;

float lastXPos;
float lastYPos;
float cameraSpeed = 1;

int currentAmmo = 30;
int maxAmmo = 30;
int score = 0;
int hp = 100;

Transform Transform::defaultParentTranform = Transform();

bool s = true;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
GLTexture tex_ground;


void RenderGround()
{
	glPushMatrix();
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	tex_ground.Use();

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
	glPopMatrix();
}

void printText(int x, int y, const char *string, float c1, float c2, float c3)
{
	//Assume we are in MODEL_VIEW already
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluOrtho2D(0, viewport[2], viewport[3], 0);

	glDepthFunc(GL_ALWAYS);
	glColor3f(c1, c2, c3);
	glRasterPos2f(x, y);
	for (int i = 0; string[i] != '\0'; ++i)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	glDepthFunc(GL_LESS);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void drawLine(float x1, float y1, float x2, float y2, float c1, float c2, float c3) {
	glPushMatrix();
	glColor3f(c1, c2, c3);
	glBegin(GL_LINES);
	glVertex3d(x1, y1, 0);
	glVertex3d(x2, y2, 0);
	glEnd();
	glPopMatrix();
}

void printLine(int x, int y, float length, bool isVertical, float c1, float c2, float c3)
{
	//Assume we are in MODEL_VIEW already
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluOrtho2D(0, viewport[2], viewport[3], 0);

	glDepthFunc(GL_ALWAYS);
	glColor3f(c1, c2, c3);
	glRasterPos2f(x, y);

	if(isVertical)
		drawLine(x, y - (length/2), x, y + (length/2), c1, c2, c3);
	else
		drawLine(x - (length/2), y, x + (length/2), y, c1, c2, c3);

	glDepthFunc(GL_LESS);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void printRect(int x, int y, float width, float height, bool startVertexCenter, float c1, float c2, float c3, float cb1, float cb2, float cb3)
{
	//Assume we are in MODEL_VIEW already
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluOrtho2D(0, viewport[2], viewport[3], 0);

	glDepthFunc(GL_ALWAYS);
	glColor3f(c1, c2, c3);
	glRasterPos2f(x, y);

	glPushMatrix();
	glBegin(GL_POLYGON);
	if (startVertexCenter) {
		glVertex3d(x - (width / 2), y - (height / 2), 0);
		glVertex3d(x + (width / 2), y - (height / 2), 0);
		glVertex3d(x + (width / 2), y + (height / 2), 0);
		glVertex3d(x - (width / 2), y + (height / 2), 0);
	}
	else {
		glVertex3d(x, y, 0);
		glVertex3d(x + width, y, 0);
		glVertex3d(x + width, y - height, 0);
		glVertex3d(x, y - height, 0);
	}
	glEnd();
	glPopMatrix();

	if (startVertexCenter) {
		drawLine(x - (width / 2), y - (height / 2), x + (width / 2), y - (height / 2), cb1, cb2, cb3);
		drawLine(x + (width / 2), y - (height / 2), x + (width / 2), y + (height / 2), cb1, cb2, cb3);
		drawLine(x + (width / 2), y + (height / 2), x - (width / 2), y + (height / 2), cb1, cb2, cb3);
		drawLine(x - (width / 2), y + (height / 2), x - (width / 2), y - (height / 2), cb1, cb2, cb3);
	}
	else {
		drawLine(x, y, x + width, y, cb1, cb2, cb3);
		drawLine(x + width, y, x + width, y - height, cb1, cb2, cb3);
		drawLine(x + width, y - height, x, y - height, cb1, cb2, cb3);
		drawLine(x, y - height, x, y, cb1, cb2, cb3);
	}

	glDepthFunc(GL_LESS);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void printSolidCircle(int x, int y, float r, float c1, float c2, float c3, float cb1, float cb2, float cb3)
{
	//Assume we are in MODEL_VIEW already
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluOrtho2D(0, viewport[2], viewport[3], 0);

	glDepthFunc(GL_ALWAYS);
	glColor3f(c1, c2, c3);
	glRasterPos2f(x, y);

	glColor3f(c1, c2, c3);

	glBegin(GL_POLYGON);
	for (int i = 0; i <= 360; i++)
	{
		float degInRad = i * 3.14159 / 180;
		glVertex3f(x + (cos(degInRad)*r), y + (sin(degInRad)*r), 0);
	}
	glEnd();

	glColor3f(cb1, cb2, cb3);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i <= 360; i++)
	{
		float degInRad = i * 3.14159 / 180;
		glVertex3f(x + (cos(degInRad)*r), y + (sin(degInRad)*r), 0);
	}
	glEnd();

	glDepthFunc(GL_LESS);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void printHallowCircle(int x, int y, float r, float c1, float c2, float c3)
{
	//Assume we are in MODEL_VIEW already
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluOrtho2D(0, viewport[2], viewport[3], 0);

	glDepthFunc(GL_ALWAYS);
	glColor3f(c1, c2, c3);
	glRasterPos2f(x, y);

	glColor3f(c1, c2, c3);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i <= 360; i++)
	{
		float degInRad = i * 3.14159 / 180;
		glVertex3f(x + (cos(degInRad)*r), y + (sin(degInRad)*r), 0);
	}
	glEnd();

	glDepthFunc(GL_LESS);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void addObject(GameObject *obj, bool started)
{
	if (!started)
		obj->Start();
	objects.push_back(obj);
}

void Start()
{
	spawner = EnemySpawner();
	player = Player(&spawner);
	spawner.player = &player;
	camera = Camera(&player.transform);

	addObject(&camera , false);
	addObject(&player, false);
	addObject(&spawner, false);

	// Dummy Objects and Furnitures
	chair = WoodChair();
	house = House();
	tree = Tree();
	grass = GroundGrass();

	addObject(&chair, false);
	addObject(&house, false);
	addObject(&tree, false);
	addObject(&grass, false);
}

void drawWall(double thickness)
{
	glPushMatrix();
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}
void drawTableLeg(double thick, double len)
{
	glPushMatrix();
	glTranslated(0, len / 2, 0);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}
void drawJackPart()
{
	glPushMatrix();
	glScaled(0.2, 0.2, 1.0);
	glutSolidSphere(1, 15, 15);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, 1.2);
	glutSolidSphere(0.2, 15, 15);
	glTranslated(0, 0, -2.4);
	glutSolidSphere(0.2, 15, 15);
	glPopMatrix();
}
void drawJack()
{
	glPushMatrix();
	drawJackPart();
	glRotated(90.0, 0, 1, 0);
	drawJackPart();
	glRotated(90.0, 1, 0, 0);
	drawJackPart();
	glPopMatrix();
}
void drawTable(double topWid, double topThick, double legThick, double legLen)
{
	glPushMatrix();
	glTranslated(0, legLen, 0);
	glScaled(topWid, topThick, topWid);
	glutSolidCube(1.0);
	glPopMatrix();

	double dist = 0.95 * topWid / 2.0 - legThick / 2.0;
	glPushMatrix();
	glTranslated(dist, 0, dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(-2 * dist, 0, 2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glPopMatrix();
}

//void setupLights()
//{
//	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
//	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
//	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
//	GLfloat shininess[] = { 50 };
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
//	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
//
//	GLfloat lightIntensity[] = { 1, 1.0f, 1, 1.0f };
//	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
//	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
//}

void setupLights() {


	GLfloat lmodel_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	GLfloat l0Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l0Spec[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	GLfloat l0Ambient[] = { 0.1f, 0.0f, 0.0f, 1.0f };
	GLfloat l0Position[] = { 0.0f, 10.0f, 0.0f, s };
	GLfloat l0Direction[] = { 0.0, -10.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0Diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0Ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, l0Position);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 180.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);

	GLfloat l1Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l1Spec[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	GLfloat l1Ambient[] = { 0.0f, 0.1f, 0.0f, 1.0f };
	GLfloat l1Position[] = { 10.0f, 10.0f, 10.0f, s };
	GLfloat l1Direction[] = { -10.0, -10.0, -10.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l1Diffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, l1Ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, l1Position);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 180.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l1Direction);

	GLfloat l2Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l2Spec[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	GLfloat l2Ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l2Position[] = { -100.0f, 10.0f, -100.0f, s };
	GLfloat l2Direction[] = { 100.0, -10.0, 100.0 };
	glLightfv(GL_LIGHT2, GL_DIFFUSE, l2Diffuse);
	glLightfv(GL_LIGHT2, GL_AMBIENT, l2Ambient);
	glLightfv(GL_LIGHT2, GL_POSITION, l2Position);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 180.0);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, l2Direction);


}

void Display()
{
	setupLights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (GameObject *obj : objects)
	{
		obj->mainFunc();
	}

	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslated(5, 0 , 0);
	glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslated(-5, 0, 0);
	glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, 5);
	glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, -5);
	glutSolidCube(1.0);
	glPopMatrix();

	char score_toString[32];
	sprintf_s(score_toString, "%d", score);
	char score_print[100] = "Score: ";
	strcat_s(score_print, score_toString);
	
	char hp_toString[32];
	sprintf_s(hp_toString, "%d", hp);
	char hp_print[100] = "HP: ";
	strcat_s(hp_print, hp_toString);

	char ammo_toString[32];
	sprintf_s(ammo_toString, "%d", currentAmmo);
	char ammo_print[100] = "Ammo: ";
	strcat_s(ammo_print, ammo_toString);
	sprintf_s(ammo_toString, "%d", maxAmmo);
	strcat_s(ammo_print, "/");
	strcat_s(ammo_print, ammo_toString);

	printRect(screenX * 0.01, screenY * 0.07, screenX * 0.07, screenY * 0.05, false, 0.2, 0.2, 0.2, 0, 0, 0);
	printText(screenX * 0.02, screenY * 0.05, score_print, 1, 1, 1);
	printRect(screenX * 0.89, screenY * 0.07, screenX * 0.07, screenY * 0.05, false, 0.2, 0.2, 0.2, 0, 0, 0);
	printText(screenX * 0.90, screenY * 0.05, hp_print, 0.7, 0.1, 0.1);
	printRect(screenX * 0.89, screenY * 0.97, screenX * 0.10, screenY * 0.05, false, 0.2, 0.2, 0.2, 0, 0, 0);
	printText(screenX * 0.90, screenY * 0.95, ammo_print, 1, 1, 1);
	printHallowCircle(screenX * 0.50, screenY * 0.50, screenX * 0.01, 0, 0, 0);
	printLine(screenX * 0.50, screenY * 0.50, screenX * 0.01, true, 0, 0, 0);
	printLine(screenX * 0.50, screenY * 0.50, screenX * 0.01, false, 0, 0, 0);

	glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y)
{
	float d = cameraSpeed;

	if (key == 'w')
	{
		player.movementDirection = player.movementDirection | FORWARD;
	}
	if (key == 's')
	{
		player.movementDirection = player.movementDirection | BACKWARD;
	}
	if (key == 'd')
	{
		player.movementDirection = player.movementDirection | RIGHT;
	}
	if (key == 'a')
	{
		player.movementDirection = player.movementDirection | LEFT;
	}

	switch (key)
	{
	case 'v':
		camera.toggleViewMode();
		break;

	case 'o':
		camera.defaultPos();
		break;
	case '+':
		cameraSpeed -= 10.0f;
		break;
	case '-':
		cameraSpeed += 10.0f;
		break;
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	//glutPostRedisplay();
}

void KeyboardUp(unsigned char key, int x, int y)
{
	if (key == 'w')
	{
		player.movementDirection = player.movementDirection & (~FORWARD);
	}
	if (key == 's')
	{
		player.movementDirection = player.movementDirection & (~BACKWARD);
	}
	if (key == 'd')
	{
		player.movementDirection = player.movementDirection & (~RIGHT);
	}
	if (key == 'a')
	{
		player.movementDirection = player.movementDirection & (~LEFT);
	}
}
void Special(int key, int x, int y)
{
	float a = 5.0;

	switch (key)
	{
	case GLUT_KEY_UP:
		player.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		player.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		player.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		player.rotateY(-a);
		break;
	}

	//glutPostRedisplay();
}

void MousePassive(int x, int y)
{
	/*camera.center.x += ((x - screenX * 0.5f) * 2.0f);
	camera.center.y += (-(y - screenY * 0.5f) * 2.0f);*/
	player.rotateY(-(x - screenX * 0.5f) * 0.5f);
	player.rotateX(-(y - screenY * 0.5f) * 0.5f);
	glutWarpPointer(screenX * 0.5f, screenY * 0.5f);
}

void MouseActive(int c,int state ,  int x, int y)
{
	player.rotateY(-(x - screenX * 0.5f) * 0.5f);
	player.rotateX(-(y - screenY * 0.5f) * 0.5f);
	glutWarpPointer(screenX * 0.5f, screenY * 0.5f);
	if (c == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		player.Shoot();
	}
	
}

void frame(int)
{
	glutTimerFunc(15, frame, 0);
	glutPostRedisplay();
}

void initializeScene(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(screenX, screenY);
	glutInitWindowPosition(50, 50);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("DMET507 Game");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(Special);
	glutMouseFunc(MouseActive);
	glutTimerFunc(15, frame, 0);

	glClearColor(0.2f, 0.38f, 0.5f, 0.0f);
	glutPassiveMotionFunc(MousePassive);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);
	glutSetCursor(GLUT_CURSOR_NONE);
	srand(time(NULL));
	Start();
}

int main(int argc, char **argv)
{
	initializeScene(argc, argv);
//	setupLights();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
	return 0;
}


