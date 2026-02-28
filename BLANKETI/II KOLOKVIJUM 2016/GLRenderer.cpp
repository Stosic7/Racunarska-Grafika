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
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	env = LoadTexture("slike/env.png");
	brick = LoadTexture("slike/brick.png");
	
	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	GLfloat light0_pos[] = { 0, 0, 1, 0 };
	GLfloat white_light[] = { 1, 1, 1, 1 };
	GLfloat ambient_light[] = { 0.2, 0.2, 0.2, 0.2 };

	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ambient_light);
	glEnable(GL_LIGHT0);

	gluLookAt(
		m_eyex, m_eyey, m_eyez,
		0.0, 4.0, 0.0,
		0.0, 1.0, 0.0
	);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	GLfloat light1_pos[] = { 0.0, 5.0, -10, 1 };
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, white_light);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT1, GL_SPECULAR, ambient_light);
	glEnable(GL_LIGHT1);

	glEnable(GL_LIGHTING);

	glPushMatrix();
	{
		glTranslatef(0, 50, 0);
		DrawCube(100.0);
	}
	glPopMatrix();	

	glPushMatrix();
	{
		glPushMatrix();
			glTranslatef(0, 0, -20); // na 20 jedinica
			DrawTube(2.5, 3.5, 10, 32);
			glTranslatef(0, 10, 0); // na vrhu Tube
			DrawCone(3.8, 2, 32);
			glTranslatef(0, -1, 5);
			glRotatef(m_paddleRot, 0, 0, 1);
			for (int i = 0; i < 4; i++) {
				DrawPaddle(8, 1.5);
				glRotatef(90, 0, 0, 1);
			}
		glPopMatrix();
	}
	glPopMatrix();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
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
	glDeleteTextures(1, &env);
	glDeleteTextures(1, &brick);
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
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

void CGLRenderer::DrawCube(double a)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, env);

	double u0 = 0.0, u1 = 0.25, u2 = 0.5, u3 = 0.75, u4 = 1.0;

	double v0 = 0.0, v1 = 1.0 / 3.0, v2 = 2.0 / 3.0, v3 = 1.0;

	glColor3f(1.0f, 1.0f, 1.0f);

	// FRONT
	glBegin(GL_QUADS);
	{
		glTexCoord2f(u1, v1); glVertex3d(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(u2, v1); glVertex3d(a / 2, -a / 2, -a / 2);
		glTexCoord2f(u2, v2); glVertex3d(a / 2, -a / 2, a / 2);
		glTexCoord2f(u1, v2); glVertex3d(-a / 2, -a / 2, a / 2);
	}
	glEnd();

	// FRONT
	glBegin(GL_QUADS);
	{
		glTexCoord2f(u1, v1); glVertex3d(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(u2, v1); glVertex3d(a / 2, -a / 2, -a / 2);
		glTexCoord2f(u2, v0); glVertex3d(a / 2, a / 2, -a / 2);
		glTexCoord2f(u1, v0); glVertex3d(-a / 2, a / 2, -a / 2);
	}
	glEnd();

	// BACK
	glBegin(GL_QUADS);
	{
		glTexCoord2f(u2, v2); glVertex3d(a / 2, -a / 2, a / 2);
		glTexCoord2f(u1, v2); glVertex3d(-a / 2, -a / 2, a / 2);
		glTexCoord2f(u1, v3); glVertex3d(-a / 2, a / 2, a / 2);
		glTexCoord2f(u2, v3); glVertex3d(a / 2, a / 2, a / 2);
	}
	glEnd();

	// LEFT
	glBegin(GL_QUADS);
	{
		glTexCoord2f(u1, v2); glVertex3d(-a / 2, -a / 2, a / 2);
		glTexCoord2f(u1, v1); glVertex3d(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(u0, v1); glVertex3d(-a / 2, a / 2, -a / 2);
		glTexCoord2f(u0, v2); glVertex3d(-a / 2, a / 2, a / 2);
	}
	glEnd();

	// RIGHT
	glBegin(GL_QUADS);
	{
		glTexCoord2f(u2, v1); glVertex3d(a / 2, -a / 2, -a / 2);
		glTexCoord2f(u2, v2); glVertex3d(a / 2, -a / 2, a / 2);
		glTexCoord2f(u3, v2); glVertex3d(a / 2, a / 2, a / 2);
		glTexCoord2f(u3, v1); glVertex3d(a / 2, a / 2, -a / 2);
	}
	glEnd();

	// TOP 
	glBegin(GL_QUADS);
	{
		glTexCoord2f(u1, v0); glVertex3d(-a / 2, a / 2, -a / 2);
		glTexCoord2f(u2, v0); glVertex3d(a / 2, a / 2, -a / 2);
		glTexCoord2f(u2, v0); glVertex3d(a / 2, a / 2, a / 2);
		glTexCoord2f(u1, v0); glVertex3d(-a / 2, a / 2, a / 2);
	}
	glEnd();

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawTube(double r1, double r2, double h, int n)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	double ang = 360.0 / n;
	
	glPushMatrix();
	{
		glColor3f(1, 1, 1);
		glBindTexture(GL_TEXTURE_2D, brick);

		// omotac
		glBegin(GL_QUAD_STRIP);
		{
			glTexCoord2f(0.0, 1.0);
			for (int i = 0; i <= 360; i += ang) {
				double rad = ToRad(i);
				double u = i / 360.0;

				glTexCoord2f(u, 0.0);
				glVertex3d(r2 * cos(rad), h, r2 * sin(rad));

				glTexCoord2f(u, 1.0);
				glVertex3d(r1 * cos(rad), 0, r1 * sin(rad));
			}
		}
		glEnd();
	}
	glPopMatrix();

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawCone(double r, double h, int n)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	double ang1, dAng1;
	
	glPushMatrix();
	{
		dAng1 = 2 * PI / n;
		ang1 = 0;
		glBegin(GL_TRIANGLE_FAN);
		{
			glColor3f(1, 1, 1);
			glNormal3f(0, 0, 1);
			glVertex3d(0, h, 0);

			for (int i = 0; i < n + 1; i++) {
				double x = r * cos(ang1);
				double y = 0;
				double z = r * sin(ang1);

				glVertex3d(x, y, z);

				ang1 += dAng1;
			}
		}
		glEnd();
	}
	glPopMatrix();
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawPaddle(double length, double width)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	double x0 = 0.0;
	double x1 = length / 8.0;
	double x2 = length / 4.0;
	double x3 = length;

	double w_thin = (width / 8.0) / 2.0; // polovina uskog dela
	double w_full = width / 2.0;         // polovina sirokog dela

	glVertex3d(0, 0, 0); // krecemo bas od dole
	glPushMatrix();
	{
		glColor3f(0.6f, 0.6f, 0.6f);
		// prednja strana krila
		glBegin(GL_POLYGON);
		{
			glNormal3f(0, 0, 1);

			// 1. tacka (gore levo, koordinatni pocetak)
			glVertex3d(x0, 0.0, 0.0);

			// 2. tacka (dole levo, debljina uskog dela)
			glVertex3d(x0, -w_thin, 0.0);

			// 3. tacka (pcetak kosine, kraj uskog dela po X osi)
			glVertex3d(x1, -w_thin, 0.0);

			// 4. tacka (kraj kosine, krilo se rasirilo do pune sirine)
			glVertex3d(x2, -w_full, 0.0);

			// 5. tacka (dole desno, kraj krila pune sirine)
			glVertex3d(x3, -w_full, 0.0);

			// 6. tacka (gore desno, vracamo se na gornju ravnu ivicu)
			glVertex3d(x3, 0.0, 0.0);
			
		}
		glEnd();

	}
	glPopMatrix();

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}

