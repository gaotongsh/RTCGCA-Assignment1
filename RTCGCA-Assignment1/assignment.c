/*
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 *  assignment.c
 *  This program is one assignment of RTCGCA course.
 */
#include <windows.h>
#include <gl.h>
#include <glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static float angle[7] = {0.0};

#define FRAME_WIDTH  800
#define FRAME_HEIGHT 600

#define UNITS_PLACE 0
#define TENS_PLACE  1
#define HUNDREDS_PLACE 2
#define THOUSANDS_PLACE 3
int ratio[4];
int keyChar[8];


GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};

GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess[] = {150.0};
GLfloat mat_ambient[] = {0.7, 0.7, 0.7, 1.0};
GLfloat mat_diffuse[] = {0.7f, 0.1, 0.2, 1.0};


#define	checkImageWidth 64
#define	checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLubyte otherImage[checkImageHeight][checkImageWidth][4];

static GLuint texName[2];

void makeCheckImages(void)
{
	int i, j, c;

	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
			c = ((((i & 0x10) == 0) ^ ((j & 0x10)) == 0)) * 255;
			otherImage[i][j][0] = (GLubyte)c;
			otherImage[i][j][1] = (GLubyte)0;
			otherImage[i][j][2] = (GLubyte)0;
			otherImage[i][j][3] = (GLubyte)255;
		}
	}
}

void init(void)
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	ratio[UNITS_PLACE] = 1;
	ratio[TENS_PLACE] = 10;
	ratio[HUNDREDS_PLACE] = 100;
	ratio[THOUSANDS_PLACE] = 1000;

	memset(keyChar, 0, 8 * sizeof(int));

	makeCheckImages(); // ???
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Generate the two textures
	glGenTextures(2, texName);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
		checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		checkImage);

	glBindTexture(GL_TEXTURE_2D, texName[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
		checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		otherImage);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();					// Reset The Current Modelview Matrix

	glColor3f(0.5f, 0.25f, 0.0f);
	glPushMatrix();
		glTranslatef(1.0f, -1.0f, -6.0f + sin(angle[7] / 90));				// Move Right 1.5 Units And Into The Screen 6.0
		glutSolidCube(0.5);
	glPopMatrix();

	/*
		if (keyChar[0])
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (keyChar[3])
	{
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, texName[1]);
	glBegin(GL_QUADS);									// Draw A Quad
	glColor3f(0.0f, 1.0f, 0.0f);			// Set The Color To Blue
	glNormal3f(0.0, 1.0, 0.0);

	glTexCoord2f(0.0, 0.0); 	glVertex3f(1.0f, 1.0f, -1.0f);			// Top Right Of The Quad (Top)
	glTexCoord2f(0.0, 1.0);		glVertex3f(-1.0f, 1.0f, -1.0f);			// Top Left Of The Quad (Top)
	glTexCoord2f(1.0, 1.0);		glVertex3f(-1.0f, 1.0f, 1.0f);			// Bottom Left Of The Quad (Top)
	glTexCoord2f(1.0, 0.0);		glVertex3f(1.0f, 1.0f, 1.0f);			// Bottom Right Of The Quad (Top)
	glEnd();						// Done Drawing The Quad

	glBegin(GL_QUADS);									// Draw A Quad
	glColor3f(1.0f, 0.5f, 0.0f);			// Set The Color To Orange
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0, 0.0); 	glVertex3f(1.0f, -1.0f, 1.0f);			// Top Right Of The Quad (Bottom)
	glTexCoord2f(0.0, 1.0);		glVertex3f(-1.0f, -1.0f, 1.0f);			// Top Left Of The Quad (Bottom)
	glTexCoord2f(1.0, 1.0);		glVertex3f(-1.0f, -1.0f, -1.0f);			// Bottom Left Of The Quad (Bottom)
	glTexCoord2f(1.0, 0.0);		glVertex3f(1.0f, -1.0f, -1.0f);			// Bottom Right Of The Quad (Bottom)
	glEnd();						// Done Drawing The Quad

	glBegin(GL_QUADS);									// Draw A Quad
	glColor3f(1.0f, 0.0f, 0.0f);			// Set The Color To Red
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 0.0); 	glVertex3f(1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Front)
	glTexCoord2f(0.0, 1.0);		glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Front)
	glTexCoord2f(1.0, 1.0);		glVertex3f(-1.0f, -1.0f, 1.0f);			// Bottom Left Of The Quad (Front)
	glTexCoord2f(1.0, 0.0);		glVertex3f(1.0f, -1.0f, 1.0f);			// Bottom Right Of The Quad (Front)
	glEnd();						// Done Drawing The Quad

	glBindTexture(GL_TEXTURE_2D, texName[0]);

	glBegin(GL_QUADS);									// Draw A Quad
	glColor3f(1.0f, 1.0f, 0.0f);			// Set The Color To Yellow
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 0.0); 	glVertex3f(1.0f, -1.0f, -1.0f);			// Bottom Left Of The Quad (Back)
	glTexCoord2f(0.0, 1.0);		glVertex3f(-1.0f, -1.0f, -1.0f);			// Bottom Right Of The Quad (Back)
	glTexCoord2f(1.0, 1.0);		glVertex3f(-1.0f, 1.0f, -1.0f);			// Top Right Of The Quad (Back)
	glTexCoord2f(1.0, 0.0);		glVertex3f(1.0f, 1.0f, -1.0f);			// Top Left Of The Quad (Back)
	glEnd();						// Done Drawing The Quad

	glBegin(GL_QUADS);									// Draw A Quad
	glColor3f(0.0f, 0.0f, 1.0f);			// Set The Color To Blue
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); 	glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Left)
	glTexCoord2f(0.0, 1.0);		glVertex3f(-1.0f, 1.0f, -1.0f);			// Top Left Of The Quad (Left)
	glTexCoord2f(1.0, 1.0);		glVertex3f(-1.0f, -1.0f, -1.0f);			// Bottom Left Of The Quad (Left)
	glTexCoord2f(1.0, 0.0);		glVertex3f(-1.0f, -1.0f, 1.0f);			// Bottom Right Of The Quad (Left)
	glEnd();						// Done Drawing The Quad

	glBegin(GL_QUADS);									// Draw A Quad
	glColor3f(1.0f, 0.0f, 1.0f);			// Set The Color To Violet
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); 	glVertex3f(1.0f, 1.0f, -1.0f);			// Top Right Of The Quad (Right)
	glTexCoord2f(0.0, 1.0);		glVertex3f(1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Right)
	glTexCoord2f(1.0, 1.0);		glVertex3f(1.0f, -1.0f, 1.0f);			// Bottom Left Of The Quad (Right)
	glTexCoord2f(1.0, 0.0);		glVertex3f(1.0f, -1.0f, -1.0f);			// Bottom Right Of The Quad (Right)
	glEnd();						// Done Drawing The Quad
									// Done Drawing The Quad

									//读取帧缓存
	frameNum++;
	if ((keyChar[2]) && (frameNum < 180))
	{
		glReadBuffer(GL_BACK);
		glReadPixels(0, 0, FRAME_WIDTH, FRAME_HEIGHT, GL_BGR, GL_UNSIGNED_BYTE, colorBuf);

		sprintf_s(numStr, 100, "%03d", frameNum);
		numStr[4] = '\0';
		sprintf_s(bmpFilename, 100, "%s%s.bmp", "./result/frame", numStr);
		SaveDIB24(bmpFilename, FRAME_WIDTH, FRAME_HEIGHT, colorBuf);

	}
	*/
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
