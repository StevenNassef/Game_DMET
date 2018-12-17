#pragma once
#include "pch.h"

vector<GameObject *> objects;
Camera camera;
Player player;
EnemySpawner spawner;
UICanvas canvas;
AmmoCrate crate;

// Dummy Objects
WoodChair chair;
House house;
Tree tree;
GroundGrass grass;

int screenX = 1400;
int screenY = 900;

float light0Direction = 0.0;
float light0Increment = 0.05;
float light0Position = 0.0;

//float light1Direction = 0.0;
//float light1Increment = 0.05;
//float light1Position = 0.0;
//
//float light2Direction = 0.0;
//float light2Increment = 0.05;
//float light2Position = 0.0;


float lastXPos;
float lastYPos;
float cameraSpeed = 1;

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
	canvas = UICanvas(&player);
	crate = AmmoCrate(&player);
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
	addObject(&canvas, false);
	addObject(&crate, false);
}

void RestartGame()
{
	player.RestartGame();
	spawner.RestartGame();
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
	GLfloat l0Position[] = { light0Position, 100.0f, 0.0f, s };
	GLfloat l0Direction[] = { light0Direction, -100.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0Diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0Ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, l0Position);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 90.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);

	GLfloat l1Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l1Spec[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	GLfloat l1Ambient[] = { 0.0f, 0.1f, 0.0f, 1.0f };
	GLfloat l1Position[] = { 0.0f, 5.0f, 0.0f, s };
	GLfloat l1Direction[] = { 0.0, -5.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l1Diffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, l1Ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, l1Position);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 128.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l1Direction);

	GLfloat l2Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l2Spec[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	GLfloat l2Ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l2Position[] = { -10.0, 10.0f, -10.0f, s };
	GLfloat l2Direction[] = { 10.0, -10.0, 10.0 };
	glLightfv(GL_LIGHT2, GL_DIFFUSE, l2Diffuse);
	glLightfv(GL_LIGHT2, GL_AMBIENT, l2Ambient);
	glLightfv(GL_LIGHT2, GL_POSITION, l2Position);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 90.0);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, l2Direction);


}

void Display()
{
	printf("%f\n", light0Direction);
	setupLights();
	light0Position -= 0.0001;
	if (light0Direction <= -20.0)
		light0Increment = 0.2;
	else if (light0Direction >= 0.0)
		light0Increment = -0.2;
	light0Direction += light0Increment;

	/*if (light1Direction <= -5.0)
		light1Increment = 0.05;
	else if (light1Direction >= 0.0)
		light1Increment = -0.05;
	light1Direction += light1Increment;

	if (light2Direction <= -5.0)
		light2Increment = 0.05;
	else if (light2Direction >= 0.0)
		light0Increment = -0.05;
	light2Direction += light2Increment;
*/
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
	//glEnable(GL_LIGHT2);
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


