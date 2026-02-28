#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	m_hrc = NULL;
	m_rot1 = 0.0;
	m_rot2 = 0.0;
	m_rot3 = 0.0;

	UpdateCameraPosition();
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(0.6, 0.6, 0.6, 0.6);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	logo = LoadTexture("slike/OpenGL.bmp");

	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	SetWhiteLight();

	GLfloat light1_pos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat light1_dir[] = { 0.0f, 0.0f, -1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_dir);

	gluLookAt(
		m_eyex, m_eyey, m_eyez,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0
	);

	GLfloat light0_pos[] = { 5.0f, 20.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);

	DrawAxes();

	glPushMatrix();
	{
		glTranslatef(-7.75f, -7.75f, -7.75f);
		DrawRubikCube(5.0, 3);
	}
	glPopMatrix();

	glFlush();
	SwapBuffers(pDC->m_hDC);

	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glDeleteTextures(1, &logo);
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxes()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glLineWidth(2.0);
	glBegin(GL_LINES);
	{
		// x osa
		glColor3f(1.0, 0.0, 0.0);
		glVertex3d(0, 0, 0);
		glVertex3d(50, 0, 0);

		// y osa
		glColor3f(0.0, 1.0, 0.0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 50, 0);

		// z osa
		glColor3f(0.0, 0.0, 1.0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, 50);
	}
	glEnd();
	glLineWidth(1.0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}

void CGLRenderer::RotateView(double dXY, double dXZ)
{
	m_angleXY += dXY;
	m_angleXZ += dXZ;

	if (m_angleXZ > 90)
		m_angleXZ = 90;
	if (m_angleXY < -90)
		m_angleXY = -90;

	UpdateCameraPosition();
}

void CGLRenderer::ZoomView(double dR)
{
	m_cameraR += dR;

	if (m_cameraR < 2)
		m_cameraR = 2;
	if (m_cameraR > 50)
		m_cameraR = 50;

	UpdateCameraPosition();
}

void CGLRenderer::UpdateCameraPosition()
{
	double radXY = ToRad(m_angleXY);
	double radXZ = ToRad(m_angleXZ);

	m_eyex = m_cameraR * cos(radXZ) * cos(radXY);
	m_eyey = m_cameraR * sin(radXZ);
	m_eyez = m_cameraR * cos(radXZ) * sin(radXY);
}

void CGLRenderer::DrawCube(float a, int x, int y, int z, int count)
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, logo);

	double c = (double)count;

	double uF0 = x / c;
	double uF1 = (x + 1) / c;
	double vF0 = 1.0 - y / c;
	double vF1 = 1.0 - (y + 1) / c;

	double uT0 = x / c;
	double uT1 = (x + 1) / c;
	double vT0 = z / c;
	double vT1 = (z + 1) / c;

	double uR0 = 1.0 - (z + 1) / c;
	double uR1 = 1.0 - z / c;
	double vR0 = 1.0 - y / c;
	double vR1 = 1.0 - (y + 1) / c; 

	glPushMatrix();
	{
		glBegin(GL_QUADS);
		{
			SetMaterial(1.0f, 0.0f, 0.0f);
			glNormal3d(0, 0, 1);
			glTexCoord2d(uF0, vF0); glVertex3d(0, 0, a);
			glTexCoord2d(uF1, vF0); glVertex3d(a, 0, a);
			glTexCoord2d(uF1, vF1); glVertex3d(a, a, a);
			glTexCoord2d(uF0, vF1); glVertex3d(0, a, a);

			SetMaterial(1.0f, 1.0f, 1.0f);
			glNormal3d(0, 1, 0);
			glTexCoord2d(uT0, vT0); glVertex3d(0, a, 0);
			glTexCoord2d(uT1, vT0); glVertex3d(a, a, 0);
			glTexCoord2d(uT1, vT1); glVertex3d(a, a, a);
			glTexCoord2d(uT0, vT1); glVertex3d(0, a, a);

			SetMaterial(0.0f, 0.0f, 1.0f);
			glNormal3d(1, 0, 0);
			glTexCoord2d(uR1, vR0); glVertex3d(a, 0, 0);
			glTexCoord2d(uR0, vR0); glVertex3d(a, 0, a);
			glTexCoord2d(uR0, vR1); glVertex3d(a, a, a);
			glTexCoord2d(uR1, vR1); glVertex3d(a, a, 0);

			SetMaterial(1.0f, 0.65f, 0.0f);
			glNormal3d(0, 0, -1);
			glTexCoord2d(0, 1); glVertex3d(0, 0, 0);
			glTexCoord2d(1, 1); glVertex3d(a, 0, 0);
			glTexCoord2d(1, 0); glVertex3d(a, a, 0);
			glTexCoord2d(0, 0); glVertex3d(0, a, 0);

			SetMaterial(0.0f, 1.0f, 0.0f);
			glNormal3d(-1, 0, 0);
			glTexCoord2d(0, 1); glVertex3d(0, 0, 0);
			glTexCoord2d(1, 1); glVertex3d(0, 0, a);
			glTexCoord2d(1, 0); glVertex3d(0, a, a);
			glTexCoord2d(0, 0); glVertex3d(0, a, 0);

			SetMaterial(1.0f, 1.0f, 0.0f);
			glNormal3d(0, -1, 0);
			glTexCoord2d(0, 1); glVertex3d(0, 0, 0);
			glTexCoord2d(1, 1); glVertex3d(a, 0, 0);
			glTexCoord2d(1, 0); glVertex3d(a, 0, a);
			glTexCoord2d(0, 0); glVertex3d(0, 0, a);
		}
		glEnd();
	}
	glPopMatrix();
}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texID;
	DImage img;
	img.Load(CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());
	return texID;
}

void CGLRenderer::DrawRubikCube(double a, int count)
{
	double gap = a * 0.05;
	double step = a + gap;
	double center = (count * step - gap) / 2.0;

	for (int x = 0; x < count; x++)
	{
		glPushMatrix();

		glTranslatef(0.0, center, center);

		if (x == 0) 
			glRotatef(m_rot1, 1.0, 0.0, 0.0);
		if (x == 1) 
			glRotatef(m_rot2, 1.0, 0.0, 0.0);
		if (x == 2) 
			glRotatef(m_rot3, 1.0, 0.0, 0.0);

		glTranslatef(0.0, -center, -center);

		for (int y = 0; y < count; y++)
		{
			for (int z = 0; z < count; z++)
			{
				glPushMatrix();
				{
					glTranslatef(x * step, y * step, z * step);
					DrawCube(a, x, y, z, count);
				}
				glPopMatrix();
			}
		}
		glPopMatrix();
	}
}

void CGLRenderer::SetWhiteLight()
{
	GLfloat white_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat no_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT0, GL_AMBIENT, no_ambient);

	GLfloat point_light_pos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, point_light_pos);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT1, GL_AMBIENT, no_ambient);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 13.0f);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0f);

	glEnable(GL_LIGHT1);

	GLfloat global_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

void CGLRenderer::SetMaterial(float r, float g, float b)
{
	GLfloat ambient[] = { r * 0.2f, g * 0.2f, b * 0.2f, 1.0f };
	GLfloat diffuse[] = { r, g, b, 1.0f };
	GLfloat specular[] = { r, g, b, 1.0f };
	GLfloat emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat shininess = 64.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}


