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

	m_texSpider = LoadTexture("slike/spider.png");

	m_texEnv[0] = LoadTexture("slike/front.jpg");
	m_texEnv[1] = LoadTexture("slike/back.jpg");
	m_texEnv[2] = LoadTexture("slike/left.jpg");
	m_texEnv[3] = LoadTexture("slike/right.jpg");
	m_texEnv[4] = LoadTexture("slike/top.jpg");
	m_texEnv[5] = LoadTexture("slike/bot.jpg");

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
	UpdateCameraPosition();

	gluLookAt(
		m_eyex, m_eyey, m_eyez,
		0.0, 4.0, 0.0,
		0.0, 1.0, 0.0
	);

	glTranslatef(0, -10, 0);
	DrawSpider();
	glPushMatrix();
	{
		glTranslatef(0.0, 50, 0.0);
		DrawEnvCube(100.0);
	}
	
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
	glDeleteTextures(1, &m_texSpider);
	glDeleteTextures(6, m_texEnv);
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxes(double width)
{
	glLineWidth(2.0);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	{
		// x ose
		glColor3f(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(width, 0, 0);

		// y ose
		glColor3f(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, width, 0);

		// z ose
		glColor3f(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, width);
	}
	glEnd();
	glLineWidth(1.0);
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

	if (m_cameraR < 2.0)
		m_cameraR = 2.0;
	if (m_cameraR > 50.0)
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
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());
	return texID;
}

void CGLRenderer::DrawEnvCube(double a)
{
	glEnable(GL_TEXTURE_2D);

	// front
	glBindTexture(GL_TEXTURE_2D, m_texEnv[0]);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3d(-a / 2, a / 2, -a / 2);

		glTexCoord2f(0.0, 0.0);
		glVertex3d(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(1.0, 0.0);
		glVertex3d(a / 2, -a / 2, -a / 2);

		glTexCoord2f(1.0, 1.0);
		glVertex3d(a / 2, a / 2, -a / 2);
	}
	glEnd();

	// left
	glBindTexture(GL_TEXTURE_2D, m_texEnv[2]);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3d(-a / 2, a / 2, a / 2);

		glTexCoord2f(0.0, 0.0);
		glVertex3d(-a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0, 0.0);
		glVertex3d(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(1.0, 1.0);
		glVertex3d(-a / 2, a / 2, -a / 2);
	}
	glEnd();

	// right
	glBindTexture(GL_TEXTURE_2D, m_texEnv[3]);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3d(a / 2, a / 2, -a / 2);

		glTexCoord2f(0.0, 0.0);
		glVertex3d(a / 2, -a / 2, -a / 2);

		glTexCoord2f(1.0, 0.0);
		glVertex3d(a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0, 1.0);
		glVertex3d(a / 2, a / 2, a / 2);
	}
	glEnd();

	// back
	glBindTexture(GL_TEXTURE_2D, m_texEnv[1]);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3d(a / 2, a / 2, a / 2);

		glTexCoord2f(0.0, 0.0);
		glVertex3d(a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0, 0.0);
		glVertex3d(-a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0, 1.0);
		glVertex3d(-a / 2, a / 2, a / 2);
	}
	glEnd();

	// top
	glBindTexture(GL_TEXTURE_2D, m_texEnv[4]);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3d(-a / 2, a / 2, a / 2);

		glTexCoord2f(0.0, 0.0);
		glVertex3d(-a / 2, a / 2, -a / 2);

		glTexCoord2f(1.0, 0.0);
		glVertex3d(a / 2, a / 2, -a / 2);

		glTexCoord2f(1.0, 1.0);
		glVertex3d(a / 2, a / 2, a / 2);
	}
	glEnd();


	// bottom
	glBindTexture(GL_TEXTURE_2D, m_texEnv[5]);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3d(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(0.0, 0.0);
		glVertex3d(-a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0, 0.0);
		glVertex3d(a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0, 1.0);
		glVertex3d(a / 2, -a / 2, -a / 2);
	}
	glEnd();

}

void CGLRenderer::DrawSphere(double r, int nSeg, double texU, double texV, double texR)
{
	double ang1, ang2;
	double dAng1, dAng2;
	dAng1 = PI / (double)nSeg;
	dAng2 = 2 * PI / (double)nSeg;
	ang1 = -PI / 2;

	for (int i = 0; i < nSeg; i++) {
		ang2 = 0;
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j < nSeg + 1; j++) {
			double x1 = r * cos(ang1) * cos(ang2);
			double y1 = r * sin(ang1);
			double z1 = r * cos(ang1) * sin(ang2);

			double x2 = r * cos(ang1 + dAng1) * cos(ang2);
			double y2 = r * sin(ang1 + dAng1);
			double z2 = r * cos(ang1 + dAng1) * sin(ang2);

			double tx1 = texR * x1 / r + texU;
			double ty1 = texR * z1 / r + texV;

			double tx2 = texR * x2 / r + texU;
			double ty2 = texR * z2 / r + texV;

			glTexCoord2f(tx1, ty1);
			glVertex3d(x1, y1, z1);
			glTexCoord2f(tx2, ty2);
			glVertex3d(x2, y2, z2);

			ang2 += dAng2;
		}
		glEnd();
		ang1 += dAng1;
	}
}

void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR)
{
	double ang1, dAng1;
	
	dAng1 = 2 * PI / (double)nSeg;

	ang1 = 0;
	glBegin(GL_TRIANGLE_FAN);

	glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(texU, texV);
	glVertex3d(0.0, h, 0.0);

	for (int i = 0; i < nSeg + 1; i++) {
		double x = r * cos(ang1);
		double y = 0.0;
		double z = r * sin(ang1);

		double tx = texR * x / r + texU;
		double ty = texR * z / r + texV;

		glTexCoord2f(tx, ty);
		glVertex3d(x, y, z);

		ang1 += dAng1;
	}
	glEnd();
}

void CGLRenderer::DrawSpiderBody()
{
	double ang = 45;
	glPushMatrix();
	{
		// center
		glPushMatrix();
		{
			glScalef(1.0, 0.5, 1.0);
			DrawSphere(3, 10, 0.25, 0.25, 0.24);
		}
		glPopMatrix();

		// tail
		glPushMatrix();
		{
			glTranslatef(6.0, 0.0, 0.0);
			glScalef(1.0, 0.8, 1.0);
			DrawSphere(5, 10, 0.25, 0.25, 0.24);
		}
		glPopMatrix();

		//head
		glPushMatrix();
		{
			glTranslatef(-4.0, 0.0, 0.0);
			glScalef(1.0, 0.5, 2.0);
			DrawSphere(2, 10, 0.75, 0.25, 0.24);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void CGLRenderer::DrawLegSegment(double r, double h, int nSeg)
{
	glPushMatrix();
	{
		glTranslated(0, r, 0);
		DrawSphere(r, 2 * nSeg, 0.25, 0.25, 0.24);
		DrawCone(r, h, nSeg, 0.25, 0.25, 0.24);
	}
	glPopMatrix();
}

void CGLRenderer::DrawLeg()
{
	double ang = 85;
	glPushMatrix();
	{
		DrawLegSegment(1, 10, 5);
		glTranslatef(0, 11, 0);
		glRotatef(ang, 1.0, 0.0, 0.0);
		DrawLegSegment(1, 15, 5);
	}
	glPopMatrix();
}

void CGLRenderer::DrawSpider()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texSpider);

	glPushMatrix();
	{
		glTranslatef(0.0, 5.0, 0.0);
		glPushMatrix();
		{
			DrawSpiderBody();
		}
		glPopMatrix();

		for (int i = 0; i < 4; i++) {
			glPushMatrix();
			{
				glRotatef(i * 30 - 45, 0.0, 1.0, 0.0);
				glRotatef(45, 1.0, 0.0, 0.0);
				DrawLeg();
			}
			glPopMatrix();
		}
		
		for (int i = 0; i < 4; i++) {
			glPushMatrix();
			{
				glRotatef(i * 30 - 45 + 180, 0.0, 1.0, 0.0);
				glRotatef(45, 1.0, 0.0, 0.0);
				DrawLeg();
			}
			glPopMatrix();
		}
	}
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
}




