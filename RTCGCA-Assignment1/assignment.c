/*
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 *  assignment.c
 *  This program is one assignment of RTCGCA course.
 */
#include <windows.h>
#include <GL/GL.h>
#include <glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static float angle[7] = {0.0};

#define FRAME_WIDTH  800
#define FRAME_HEIGHT 600
int keyChar[8];

GLUnurbs* glunurbs;
GLfloat ctlpoints[4][4][3];
GLfloat knots[8] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};

void init(void)
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do

	memset(keyChar, 0, 8 * sizeof(int));

	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};

	GLfloat light_position[] = {2.0, 2.0, 3.0, 0.0}; 

	GLfloat light_ambient[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glunurbs = gluNewNurbsRenderer();

	// Init nurbs
	int u, v;
	for (u = 0; u < 4; ++u) {
		for (v = 0; v < 4; ++v) {
			ctlpoints[u][v][0] = 2.0 * ((GLfloat)u - 1.5);
			ctlpoints[u][v][1] = 2.0 * ((GLfloat)v - 1.5);
			if ((u == 1 || u == 2) && (v == 1 || v == 2))
				ctlpoints[u][v][2] = 3.0f;
			else
				ctlpoints[u][v][2] = -3.0f;
		}
	}
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -6.0f);
		glScalef(4.0f, 4.0f, 4.0f);
		glBegin(GL_QUADS);
			glColor3f(0.3f, 0.3f, 0.3f);
			glVertex3d(-0.5, -0.5, -0.5);
			glVertex3d(0.5, -0.5, -0.5);
			glVertex3d(0.5, 0.5, -0.5);
			glVertex3d(-0.5, 0.5, -0.5);
		glEnd();
		glBegin(GL_QUADS);
			glColor3f(0.3f, 0.3f, 0.5f);
			glVertex3d(0.5, -0.5, -0.5);
			glVertex3d(0.5, 0.5, -0.5);
			glVertex3d(0.5, 0.5, 0.5);
			glVertex3d(0.5, -0.5, 0.5);
		glEnd();
		glBegin(GL_QUADS);
			glColor3f(0.3f, 0.4f, 0.6f);
			glVertex3d(-0.5, -0.5, -0.5);
			glVertex3d(-0.5, -0.5, 0.5);
			glVertex3d(-0.5, 0.5, 0.5);
			glVertex3d(-0.5, 0.5, -0.5);
		glEnd();
		glBegin(GL_QUADS);
			glColor3f(0.4f, 0.4f, 0.7f);
			glVertex3d(-0.5, 0.5, -0.5);
			glVertex3d(0.5, 0.5, -0.5);
			glVertex3d(0.5, 0.5, 0.5);
			glVertex3d(-0.5, 0.5, 0.5);
		glEnd();
		glBegin(GL_QUADS);
			glColor3f(0.4f, 0.4f, 0.6f);
			glVertex3d(-0.5, -0.5, -0.5);
			glVertex3d(0.5, -0.5, -0.5);
			glVertex3d(0.5, -0.5, 0.5);
			glVertex3d(-0.5, -0.5, 0.5);
		glEnd();
	glPopMatrix();

	glColor3f(0.5f, 0.0f, 0.0f);
	glPushMatrix();
		glTranslatef(-1.0f, 1.0f, -6.0f);
		glRotatef(angle[1], 0.0f, 1.0f, 0.0f);
		glutSolidTorus(0.1f, 0.3f, 50, 50);
	glPopMatrix();

	glColor3f(0.0f, 0.0f, 0.5f);
	glPushMatrix();
		glTranslatef(-0.6f, 0.2f, -5.0f);
		glRotatef(angle[2], 1.0f, 0.0f, 0.0f);
		glutSolidTeapot(0.3f);
	glPopMatrix();

	glColor3f(0.5f, 0.0f, 0.5f);
	glPushMatrix();
		glTranslatef(-0.9f, -0.8f, -6.0f);
		glScalef(3.0f + sin(angle[3] / 90), 3.0f + sin(angle[3] / 90), 3.0f + sin(angle[3] / 90));
		glutSolidSphere(0.1f, 50, 50);
	glPopMatrix();

	glColor3f(0.0f, 0.5f, 0.0f);
	glPushMatrix();
		glTranslatef(1.0f, 1.0f, -6.0f);
		glRotatef(270 + angle[4], 1.0f, 0.0f, 0.0f);
		glutSolidCone(0.2f, 0.4f, 50, 50);
	glPopMatrix();

	glColor3f(0.0f, 0.5f, 0.5f);
	glPushMatrix();
		glTranslatef(1.1f, -0.1f, -6.0f);
		glRotatef(angle[5], 0.0f, 1.0f, 0.0f);
		glRotatef(270 + angle[6], 1.0f, 0.0f, 0.0f);
		glScalef(0.1f, 0.1f, 0.1f);
		gluBeginSurface(glunurbs);
			gluNurbsSurface(glunurbs,
				8, knots, 8, knots,
				4 * 3, 3, &ctlpoints[0][0][0],
				4, 4, GL_MAP2_VERTEX_3);
		gluEndSurface(glunurbs);
	glPopMatrix();

	glColor3f(0.5f, 0.5f, 0.0f);
	glPushMatrix();
		glTranslatef(0.7f, -0.95f, -6.0f + sin(angle[7] / 90));
		glutSolidCube(0.5);
	glPopMatrix();

	glutSwapBuffers();
	for (int i = 1; i <= 7; ++i)
		if (keyChar[i])
			angle[i] += 1.0f;
}

void reshape(int width, int height)
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

														// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	case 'q':
		memset(keyChar, 0, 8 * sizeof(int));
		break;
	case '1':
		keyChar[1] = !keyChar[1];
		break;
	case '2':
		keyChar[2] = !keyChar[2];
		break;
	case '3':
		keyChar[3] = !keyChar[3];
		break;
	case '4':
		keyChar[4] = !keyChar[4];
		break;
	case '5':
		keyChar[5] = !keyChar[5];
		break;
	case '6':
		keyChar[6] = !keyChar[6];
		break;
	case '7':
		keyChar[7] = !keyChar[7];
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(FRAME_WIDTH, FRAME_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment 1 - Gao Tong");

	const GLubyte* name = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
	const GLubyte* biaoshifu = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
	const GLubyte* OpenGLVersion = glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
	const GLubyte* gluVersion = gluGetString(GLU_VERSION); //返回当前GLU工具库版本

	printf("OpenGL实现厂商的名字：%s\n", name);
	printf("渲染器标识符：%s\n", biaoshifu);
	printf("OpenGL实现的版本号：%s\n", OpenGLVersion);
	printf("GLU工具库版本：%s\n", gluVersion);

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);
	glutMainLoop();
	return 0;
}
