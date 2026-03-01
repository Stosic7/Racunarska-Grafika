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

	lamp = LoadTexture("slike/lamp.jpg");
	pozadina[0] = LoadTexture("slike/front.jpg");
	pozadina[1] = LoadTexture("slike/left.jpg");
	pozadina[2] = LoadTexture("slike/right.jpg");
	pozadina[3] = LoadTexture("slike/back.jpg");
	pozadina[4] = LoadTexture("slike/top.jpg");
	pozadina[5] = LoadTexture("slike/bot.jpg");
	pozadina[6] = LoadTexture("slike/side.jpg");

	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(
		eyex, eyey, eyez,
		0.0, 4.0, 0.0,
		0.0, 1.0, 0.0
	);

	glPushMatrix();
	{
		glTranslatef(0, 50, 0);
		DrawEnvCube(100.0);
	}
	glPopMatrix();

	DrawAxes();
	glPushMatrix();
	{
		glTranslatef(0.0, 50.0, 0.0);
		DrawEnvCube(100.0);
	}
	glPopMatrix();

	DrawLamp();

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
	gluPerspective(50, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glDeleteTextures(1, &lamp);
	glDeleteTextures(7, pozadina);
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::RotateView(double dXY, double dXZ)
{
	XY += dXY;
	XZ += dXZ;

	if (XZ > 90)
		XZ = 90;
	if (XY < -90)
		XY = -90;

	UpdateCameraPosition();
}

void CGLRenderer::ZoomView(double dR)
{
	R += dR;

	if (R < 2)
		R = 2;
	if (R > 50)
		R = 50;

	UpdateCameraPosition();
}

void CGLRenderer::UpdateCameraPosition()
{
	double rXY = ToRad(XY);
	double rXZ = ToRad(XZ);

	eyex = R * cos(rXZ) * cos(rXY);
	eyey = R * sin(rXZ);
	eyez = R * cos(rXZ) * sin(rXY);
}

void CGLRenderer::DrawAxes()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	{
		// x osa
		glColor3f(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex3d(50, 0, 0);

		// y osa
		glColor3f(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 50, 0);

		// z osa
		glColor3f(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, 50);
	}
	glEnd();
	glLineWidth(1.0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
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

void CGLRenderer::DrawEnvCube(double a)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	//front
	glBindTexture(GL_TEXTURE_2D, pozadina[0]);
	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);

		glTexCoord2f(0, 0); glVertex3d(-a / 2, a / 2, -a / 2);
		glTexCoord2f(0, 1); glVertex3d(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(1, 1); glVertex3d(a / 2, -a / 2, -a / 2);
		glTexCoord2f(1, 0); glVertex3d(a / 2, a / 2, -a / 2);
	}
	glEnd();

	// left
	glBindTexture(GL_TEXTURE_2D, pozadina[1]);
	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);

		glTexCoord2f(0, 0); glVertex3d(-a / 2, a / 2, a / 2);
		glTexCoord2f(0, 1); glVertex3d(-a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 1); glVertex3d(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(1, 0); glVertex3d(-a / 2, a / 2, -a / 2);
	}
	glEnd();

	//right
	glBindTexture(GL_TEXTURE_2D, pozadina[2]);
	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);

		glTexCoord2f(0, 0); glVertex3d(a / 2, a / 2, -a / 2);
		glTexCoord2f(0, 1); glVertex3d(a / 2, -a / 2, -a / 2);
		glTexCoord2f(1, 1); glVertex3d(a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 0); glVertex3d(a / 2, a / 2, a / 2);
	}
	glEnd();

	//back
	glBindTexture(GL_TEXTURE_2D, pozadina[3]);
	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);

		glTexCoord2f(0, 0); glVertex3d(a / 2, a / 2, a / 2);
		glTexCoord2f(0, 1); glVertex3d(a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 1); glVertex3d(-a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 0); glVertex3d(-a / 2, a / 2, a / 2);
	}
	glEnd();

	//top
	glBindTexture(GL_TEXTURE_2D, pozadina[4]);
	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);

		glTexCoord2f(0, 0); glVertex3d(-a / 2, a / 2, a / 2);
		glTexCoord2f(0, 1); glVertex3d(-a / 2, a / 2, -a / 2);
		glTexCoord2f(1, 1); glVertex3d(a / 2, a / 2, -a / 2);
		glTexCoord2f(1, 0); glVertex3d(a / 2, a / 2, a / 2);
	}
	glEnd();

	// top
	glBindTexture(GL_TEXTURE_2D, pozadina[5]);
	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);

		glTexCoord2f(0, 0); glVertex3d(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(0, 1); glVertex3d(-a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 1); glVertex3d(a / 2, -a / 2, a / 2);
		glTexCoord2f(1, 0); glVertex3d(a / 2, -a / 2, -a / 2);
	}
	glEnd();

	glEnable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool bIsOpen)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	double step = 360.0 / nSeg;

	if (texMode == 0) {
		glPushMatrix();
		{
			glColor3f(1.0, 1.0, 1.0);
			glBindTexture(GL_TEXTURE_2D, lamp);

			if (bIsOpen == false) {
				// donja osnova
				glBegin(GL_TRIANGLE_FAN);
				{
					glTexCoord2f(0.5, 0.25);
					glVertex3d(0, 0, 0);
					for (int i = 0; i <= 360; i += step) {
						double rad = ToRad(i);
						glTexCoord2f(0.5 + 0.5 * cos(rad), 0.25 + 0.25 * sin(rad));
						glVertex3d(r1 * cos(rad), 0, r1 * sin(rad));
					}
				}
				glEnd();

				// gornja osnova
				glBegin(GL_TRIANGLE_FAN);
				{
					glTexCoord2f(0.5, 0.25);
					glVertex3d(0, h, 0);
					for (int i = 0; i <= 360; i += step) {
						double rad = ToRad(i);
						glTexCoord2f(0.5 + 0.5 * cos(rad), 0.25 + 0.25 * sin(rad));
						glVertex3d(r2 * cos(rad), h, r2 * sin(rad));
					}
				}
				glEnd();
			}

			// omotac
			glBegin(GL_QUAD_STRIP);
			{
				for (int i = 0; i <= 360; i += step) {
					double rad = ToRad(i);
					double u = i / 360.0;

					glTexCoord2f(u, 0.0);
					glVertex3d(r2 * cos(rad), h, r2 * sin(rad));

					glTexCoord2f(u, 0.5);
					glVertex3d(r1 * cos(rad), 0, r1 * sin(rad));
				}
			}
			glEnd();
		}
		glPopMatrix();
	}
	else if (texMode == 1) {
		glPushMatrix();
		{
			glColor3f(1.0, 1.0, 1.0);
			glBindTexture(GL_TEXTURE_2D, lamp);

			if (bIsOpen == false) {
				// donja osnova
				glBegin(GL_TRIANGLE_FAN);
				{
					glTexCoord2f(0.5, 0.25);
					glVertex3d(0, 0, 0);
					for (int i = 0; i <= 360; i += step) {
						double rad = ToRad(i);
						glTexCoord2f(0.5 + 0.5 * cos(rad), 0.25 + 0.25 * sin(rad));
						glVertex3d(r1 * cos(rad), 0, r1 * sin(rad));
					}
				}
				glEnd();

				// gornja osnova
				glBegin(GL_TRIANGLE_FAN);
				{
					glTexCoord2f(0.5, 0.25);
					glVertex3d(0, h, 0);
					for (int i = 0; i <= 360; i += step) {
						double rad = ToRad(i);
						glTexCoord2f(0.5 + 0.5 * cos(rad), 0.25 + 0.25 * sin(rad));
						glVertex3d(r2 * cos(rad), h, r2 * sin(rad));
					}
				}
				glEnd();
			}

			// omotac
			glBegin(GL_QUAD_STRIP);
			{
				for (int i = 0; i <= 360; i += step) {
					double rad = ToRad(i);
					double u = i / 360.0;

					glTexCoord2f(u, 0.5);
					glVertex3d(r2 * cos(rad), h, r2 * sin(rad));

					glTexCoord2f(u, 1.0);
					glVertex3d(r1 * cos(rad), 0, r1 * sin(rad));
				}
			}
			glEnd();
		}
		glPopMatrix();
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawLampBase()
{
	glPushMatrix();
	{
		DrawCylinder(8, 7, 2, 15, 1, false);
	}
	glPopMatrix();
}

void CGLRenderer::DrawLampArm()
{
	glPushMatrix();
	{
		// prvi cilindar
		glPushMatrix();
		{
			glTranslatef(0, 2, -1);
			glRotatef(90, 1, 0, 0);
			DrawCylinder(3, 3, 2, 30, 1, false);
		}
		glPopMatrix();

		// drugi cilindar
		glPushMatrix();
		{
			DrawCylinder(1, 1, 15, 30, 1, false);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void CGLRenderer::DrawLampHead()
{
	glPushMatrix();
	{
		// prvi cilindar
		glPushMatrix();
		{
			DrawCylinder(6, 3, 5, 30, 0, true);
		}
		glPopMatrix();

		// drugi cilindar
		glPushMatrix();
		{
			glTranslatef(0.0, 5.0, 0.0);
			DrawCylinder(3, 3, 2, 30, 0, false);
		}
		glPopMatrix();

		// treci cilindar
		glPushMatrix();
		{
			glTranslatef(0.0, 7.0, 0.0);
			DrawCylinder(3, 2, 1, 30, 0, false);
		}
		glPopMatrix();

		// cetvrti cilindar
		glPushMatrix();
		{
			glTranslatef(-3.0, 6.0, 0.0);
			glRotatef(90.0, 0.0, 0.0, 1.0);
			DrawCylinder(2, 2, 2, 30, 0, false);

		}
		glPopMatrix();
	}
	glPopMatrix();
}

void CGLRenderer::DrawLamp()
{
	glPushMatrix();
	{
		// postolje
		DrawLampBase();

		glTranslatef(0.0, 0.0, 0);

		// prva ruka
		glRotatef(m_rot1, 0.0, 0.0, 1.0);
		DrawLampArm();

		glTranslatef(-1.0, 15.0, 0.0);

		// druga ruka
		glRotatef(m_rot2, 0.0, 0.0, 1.0);
		DrawLampArm();

		glTranslatef(0.0, 15.0, 0.0);

		// glava
		glRotatef(-m_rot3, 0.0, 0.0, 1.0);
		glTranslatef(3.0, -6.0, 0.0);
		DrawLampHead();
	}
	glPopMatrix();
}


