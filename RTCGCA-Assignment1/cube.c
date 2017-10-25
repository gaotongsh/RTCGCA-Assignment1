/*
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 *  cube.c
 *  This program demonstrates a single modeling transformation,
 *  glScalef() and a single viewing transformation, gluLookAt().
 *  A wireframe cube is rendered.
 */
#include <windows.h>
#include <gl.h>
#include <glut.h>
#include <stdio.h>
#include <stdlib.h>

//void GenerateIndexName(char* placeChar,int num,int placeFlag);
boolean SaveDIB24(const char* lpszFileName, DWORD dwWidth, DWORD dwHeight, void* lpvBits);


static float angle = 0.0;
char bmpFilename[50];
static int frameNum = 0;
char idxName[3];

#define FRAME_WIDTH  320
#define FRAME_HEIGHT 240

#define EXIT_PLACE -1
#define UNITS_PLACE 0
#define TENS_PLACE  1
#define HUNDREDS_PLACE 2
#define THOUSANDS_PLACE 3
int ratio[4];
char numStr[5];
int keyChar[4];


unsigned char colorBuf[FRAME_WIDTH*FRAME_HEIGHT*3];
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 150.0 };
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_diffuse[] = { 0.7f, 0.1, 0.2, 1.0 };


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
         c = ((((i&0x8)==0)^((j&0x8))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
         checkImage[i][j][3] = (GLubyte) 255;
         c = ((((i&0x10)==0)^((j&0x10))==0))*255;
         otherImage[i][j][0] = (GLubyte) c;
         otherImage[i][j][1] = (GLubyte) 0;
         otherImage[i][j][2] = (GLubyte) 0;
         otherImage[i][j][3] = (GLubyte) 255;
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

   ratio[UNITS_PLACE]= 1;
   ratio[TENS_PLACE]= 10;
   ratio[HUNDREDS_PLACE]= 100;
   ratio[THOUSANDS_PLACE]= 1000;

   keyChar[0] = 1;
   keyChar[1] = 0;
   keyChar[2] = 1;
   keyChar[3] = 1;

   makeCheckImages();
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   glGenTextures(2, texName);
   glBindTexture(GL_TEXTURE_2D, texName[0]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                   GL_NEAREST);
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
   
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();					// Reset The Current Modelview Matrix
    glTranslatef(0.0f,0.0f,-6.0f);				// Move Right 1.5 Units And Into The Screen 6.0
	glRotatef(angle,1.0f,1.0f,0.0f);			// Rotate The Quad On The X axis
	glColor3f(0.5f,0.5f,1.0f);							// Set The Color To Blue One Time Only

	if (keyChar[0])
	{
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	}
	
	if (keyChar[1])
	{
	   glEnable(GL_LIGHTING);
	   glEnable(GL_LIGHT0);
	}
	else 
	{
		glDisable(GL_LIGHTING);	
	    glDisable(GL_LIGHT0);
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
		glColor3f(0.0f,1.0f,0.0f);			// Set The Color To Blue
		glNormal3f(0.0,1.0,0.0);
	    
		glTexCoord2f(0.0, 0.0); 	glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Top)
		glTexCoord2f(0.0, 1.0);		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Top)
		glTexCoord2f(1.0, 1.0);		glVertex3f(-1.0f, 1.0f, 1.0f);			// Bottom Left Of The Quad (Top)
		glTexCoord2f(1.0, 0.0);		glVertex3f( 1.0f, 1.0f, 1.0f);			// Bottom Right Of The Quad (Top)
	glEnd();						// Done Drawing The Quad
		
	glBegin(GL_QUADS);									// Draw A Quad
		glColor3f(1.0f,0.5f,0.0f);			// Set The Color To Orange
		glNormal3f(0.0,1.0,0.0);
		glTexCoord2f(0.0, 0.0); 	glVertex3f( 1.0f,-1.0f, 1.0f);			// Top Right Of The Quad (Bottom)
		glTexCoord2f(0.0, 1.0);		glVertex3f(-1.0f,-1.0f, 1.0f);			// Top Left Of The Quad (Bottom)
		glTexCoord2f(1.0, 1.0);		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Bottom)
		glTexCoord2f(1.0, 0.0);		glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Bottom)
	glEnd();						// Done Drawing The Quad
		
	glBegin(GL_QUADS);									// Draw A Quad
		glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
		glNormal3f(0.0,0.0,1.0);
		glTexCoord2f(0.0, 0.0); 	glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Front)
		glTexCoord2f(0.0, 1.0);		glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Front)
		glTexCoord2f(1.0, 1.0);		glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Front)
		glTexCoord2f(1.0, 0.0);		glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Front)
	glEnd();						// Done Drawing The Quad
	
    glBindTexture(GL_TEXTURE_2D, texName[0]);
	
	glBegin(GL_QUADS);									// Draw A Quad
		glColor3f(1.0f,1.0f,0.0f);			// Set The Color To Yellow
		glNormal3f(0.0,0.0,1.0);
		glTexCoord2f(0.0, 0.0); 	glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Back)
		glTexCoord2f(0.0, 1.0);		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Back)
		glTexCoord2f(1.0, 1.0);		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Back)
		glTexCoord2f(1.0, 0.0);		glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Back)
	glEnd();						// Done Drawing The Quad
		
	glBegin(GL_QUADS);									// Draw A Quad
		glColor3f(0.0f,0.0f,1.0f);			// Set The Color To Blue
		glNormal3f(1.0,0.0,0.0);
		glTexCoord2f(0.0, 0.0); 	glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Left)
		glTexCoord2f(0.0, 1.0);		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Left)
		glTexCoord2f(1.0, 1.0);		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Left)
		glTexCoord2f(1.0, 0.0);		glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Left)
	glEnd();						// Done Drawing The Quad
		
	glBegin(GL_QUADS);									// Draw A Quad
		glColor3f(1.0f,0.0f,1.0f);			// Set The Color To Violet
		glNormal3f(1.0,0.0,0.0);	
		glTexCoord2f(0.0, 0.0); 	glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Right)
		glTexCoord2f(0.0, 1.0);		glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Right)
		glTexCoord2f(1.0, 1.0);		glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Right)
		glTexCoord2f(1.0, 0.0);		glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Right)
	glEnd();						// Done Drawing The Quad
  												// Done Drawing The Quad
   
   //读取帧缓存
   frameNum++;
   if ((keyChar[2])&&(frameNum < 180))
   {
	   glReadBuffer(GL_BACK);
	   glReadPixels(0,0,FRAME_WIDTH,FRAME_HEIGHT,GL_BGR,GL_UNSIGNED_BYTE,colorBuf);
   
	   sprintf_s(numStr,100,"%03d",frameNum);
	   numStr[4] = '\0';
	   sprintf_s(bmpFilename,100,"%s%s.bmp","./result/frame",numStr);
	   SaveDIB24(bmpFilename,FRAME_WIDTH,FRAME_HEIGHT,colorBuf);
	   
   }

   glutSwapBuffers();
   angle += 1.0f;
}

void reshape (int width, int height)
{
  if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
	  case 's':
			keyChar[0] = !keyChar[0];
		  break;
      case 'o':
			keyChar[1] = !keyChar[1];
		  break;
	  case 'r':
		    if (keyChar[2])
				frameNum = 0;
			break;
	  case 'g':
		    keyChar[2] = !keyChar[2];
			break;
	  case 't':
		    keyChar[3] = !keyChar[3];
			break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (FRAME_WIDTH, FRAME_HEIGHT); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);

   const GLubyte* name = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
   const GLubyte* biaoshifu = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
   const GLubyte* OpenGLVersion = glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
   const GLubyte* gluVersion = gluGetString(GLU_VERSION); //返回当前GLU工具库版本

   printf("OpenGL实现厂商的名字：%s\n", name);
   printf("渲染器标识符：%s\n", biaoshifu);
   printf("OpenGL实现的版本号：%s\n", OpenGLVersion);
   printf("OGLU工具库版本：%s\n", gluVersion);
   //如果是在VS上执行，需要在return前加上：system("pause");

   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutIdleFunc( display );
   glutMainLoop();
   return 0;
}

boolean SaveDIB24(const char* lpszFileName, DWORD dwWidth, DWORD dwHeight, void* lpvBits)
{
	HANDLE hFile = NULL;

	BOOL bOK;
	DWORD dwNumWritten;
	DWORD dwWidthAlign;
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

	__try {

		hFile = CreateFile(
			lpszFileName,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			0,
			NULL
		);
		if (hFile==INVALID_HANDLE_VALUE) return 0;

		dwWidthAlign = ((dwWidth*sizeof(RGBTRIPLE)+3)/4)*4;

		// BITMAPFILEHEADDER
		bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
		bmfh.bfType = ('B'|'M'<<8);
		bmfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
		bmfh.bfSize = bmfh.bfOffBits + dwWidthAlign*dwHeight;

		bOK = WriteFile(
			hFile,
			&bmfh,
			sizeof(bmfh),
			&dwNumWritten,
			NULL
		);
		if (!bOK) return 0;


		bmih.biSize = sizeof(BITMAPINFOHEADER);
		bmih.biWidth = dwWidth;
		bmih.biHeight = dwHeight;
		bmih.biPlanes = 1;
		bmih.biBitCount = 24;
		bmih.biCompression = 0;
		bmih.biSizeImage = 0;
		bmih.biXPelsPerMeter = 0;
		bmih.biYPelsPerMeter = 0;
		bmih.biClrUsed = 0;
		bmih.biClrImportant = 0;

		bOK = WriteFile(
			hFile,
			&bmih,
			sizeof(bmih),
			&dwNumWritten,
			NULL
		);
		if (!bOK) return 0;

		bOK = WriteFile(
			hFile,
			lpvBits,
			dwWidthAlign*dwHeight,
			&dwNumWritten,
			NULL
		);
		if (!bOK) return 0;
	
	} __finally {

		CloseHandle(hFile);

	}
	
	return 1;
}

/*
 *----------------------------------------------
 * 设定帧数范围0000--9999
 *----------------------------------------------
void GenerateIndexName(char* placeChar,int num,int placeFlag)
{
	int remainder;
	int quotient;
	char value[5];

	if (placeFlag > EXIT_PLACE)
	{
		quotient = num / ratio[placeFlag];
		remainder = num - quotient*ratio[placeFlag];
		*placeChar = *(_itoa(quotient,value,5));
		GenerateIndexName(++placeChar,remainder,--placeFlag);
	}
	
}
 */

