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
	m_rot2 = 45.0;
	m_rot3 = -30.0;
	m_rot4 = -20.0;

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

	bager = LoadTexture("slike/excavator.png");
	pozadina[0] = LoadTexture("slike/front.jpg");
	pozadina[1] = LoadTexture("slike/left.jpg");
	pozadina[2] = LoadTexture("slike/right.jpg");
	pozadina[3] = LoadTexture("slike/back.jpg");
	pozadina[4] = LoadTexture("slike/top.jpg");
	pozadina[5] = LoadTexture("slike/bot.jpg");

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
		m_eyex, m_eyey, m_eyez,
		0.0, 4.0, 0.0,
		0.0, 1.0, 0.0
	);

	DrawAxes(50);

	// pozadina
	glPushMatrix();
	{
		glTranslatef(0, 50, 0);
		DrawEnvCube(100.0);
	}
	glPopMatrix();

	glPushMatrix();
	{
		DrawExcavator();
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
	gluPerspective(55, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glDeleteTextures(1, &bager);
	glDeleteTextures(6, pozadina);
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

void CGLRenderer::DrawAxes(int width)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	{
		// x osa
		glColor3f(0.0, 0.0, 1.0);
		glVertex3d(0, 0, 0);
		glVertex3d(width, 0, 0);

		// y osa
		glColor3f(1.0, 0.0, 0.0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, width, 0);

		// z osa
		glColor3f(0.0, 1.0, 0.0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, width);
	}
	glEnd();
	glLineWidth(1.0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
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

	// front
	glBindTexture(GL_TEXTURE_2D, pozadina[0]);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3d(-a / 2, a / 2, -a / 2);
		glTexCoord2f(0.0, 1.0); glVertex3d(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(1.0, 1.0); glVertex3d(a / 2, -a / 2, -a / 2);
		glTexCoord2f(1.0, 0.0); glVertex3d(a / 2, a / 2, -a / 2);
	}
	glEnd();

	// left
	glBindTexture(GL_TEXTURE_2D, pozadina[1]);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3d(-a / 2, a / 2, a / 2);
		glTexCoord2f(0.0, 1.0); glVertex3d(-a / 2, -a / 2, a / 2);
		glTexCoord2f(1.0, 1.0); glVertex3d(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(1.0, 0.0); glVertex3d(-a / 2, a / 2, -a / 2);
	}
	glEnd();

	// right
	glBindTexture(GL_TEXTURE_2D, pozadina[2]);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3d(a / 2, a / 2, -a / 2);
		glTexCoord2f(0.0, 1.0); glVertex3d(a / 2, -a / 2, -a / 2);
		glTexCoord2f(1.0, 1.0); glVertex3d(a / 2, -a / 2, a / 2);
		glTexCoord2f(1.0, 0.0); glVertex3d(a / 2, a / 2, a / 2);
	}
	glEnd();

	// back
	glBindTexture(GL_TEXTURE_2D, pozadina[3]);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3d(a / 2, a / 2, a / 2);
		glTexCoord2f(0.0, 1.0); glVertex3d(a / 2, -a / 2, a / 2);
		glTexCoord2f(1.0, 1.0); glVertex3d(-a / 2, -a / 2, a / 2);
		glTexCoord2f(1.0, 0.0); glVertex3d(-a / 2, a / 2, a / 2);
	}
	glEnd();

	// top 
	glBindTexture(GL_TEXTURE_2D, pozadina[4]);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3d(-a / 2, a / 2, a / 2);
		glTexCoord2f(0.0, 1.0); glVertex3d(-a / 2, a / 2, -a / 2);
		glTexCoord2f(1.0, 1.0); glVertex3d(a / 2, a / 2, -a / 2);
		glTexCoord2f(1.0, 0.0); glVertex3d(a / 2, a / 2, a / 2);
	}
	glEnd();

	// bottom
	glBindTexture(GL_TEXTURE_2D, pozadina[5]);
	glBegin(GL_QUADS);
	{
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3d(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(0.0, 1.0); glVertex3d(-a / 2, -a / 2, a / 2);
		glTexCoord2f(1.0, 1.0); glVertex3d(a / 2, -a / 2, a / 2);
		glTexCoord2f(1.0, 0.0); glVertex3d(a / 2, -a / 2, -a / 2);
	}
	glEnd();

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawPolygon(POINTF* points, POINTF* texCoords, int n)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glBegin(GL_TRIANGLE_FAN);
	{
		for (int i = 0; i < n; i++) {
			glTexCoord2f(texCoords[i].x, texCoords[i].y);
			glVertex3d(points[i].x, points[i].y, 0.0);
		}
	}
	glEnd();

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b)
{
	// pravi pocetni 2d poligon bez dubine (zh = 0)
	DrawPolygon(points, texCoords, n);

	// pravi zavrsni 2d poligon sa dubinom (-zh)
	glPushMatrix();
	{
		glTranslatef(0.0f, 0.0f, -zh);
		DrawPolygon(points, texCoords, n);
	}
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glColor3f(r, g, b);


	// sada popunjavamo prostor izmedju pocetnog i krajnjeg poligona
	glBegin(GL_QUAD_STRIP);
	{
		for (int i = 0; i <= n; i++)
		{
			int index = i % n;

			// teme na prednjoj osnovi
			glVertex3f(points[index].x, points[index].y, 0.0f);

			// teme na zadnjoj osnovi
			glVertex3f(points[index].x, points[index].y, -zh);
		}
	}
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawBase()
{
	glEnable(GL_TEXTURE_2D);

	POINTF points[6];
	POINTF texCoords[6];

	points[0].x = 1.0f; points[0].y = 0.0f;
	texCoords[0].x = 2.0f / 16.0f;
	texCoords[0].y = 1.0f - (0.0f / 16.0f);

	points[1].x = 7.0f; points[1].y = 0.0f;
	texCoords[1].x = 14.0f / 16.0f;
	texCoords[1].y = 1.0f - (0.0f / 16.0f);

	points[2].x = 8.0f; points[2].y = 1.0f;
	texCoords[2].x = 16.0f / 16.0f;
	texCoords[2].y = 1.0f - (2.0f / 16.0f);

	points[3].x = 7.0f; points[3].y = 2.0f;
	texCoords[3].x = 14.0f / 16.0f;
	texCoords[3].y = 1.0f - (4.0f / 16.0f);

	points[4].x = 1.0f; points[4].y = 2.0f;
	texCoords[4].x = 2.0f / 16.0f;
	texCoords[4].y = 1.0f - (4.0f / 16.0f);

	points[5].x = 0.0f; points[5].y = 1.0f;
	texCoords[5].x = 0.0f / 16.0f;
	texCoords[5].y = 1.0f - (2.0f / 16.0f);

	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, bager);

		DrawExtrudedPolygon(points, texCoords, 6, 5.0f, 0.0f, 0.0f, 0.0f);
	}
	glPopMatrix();
}

void CGLRenderer::DrawBody()
{
	glEnable(GL_TEXTURE_2D);

	POINTF points[5];
	POINTF texCoords[5];

	points[0].x = 0.0f;
	points[0].y = 0.0f;
	texCoords[0].x = 8.0 / 16.0;
	texCoords[0].y = 1 - (8.0 / 16.0);

	points[1].x = 4.0f; 
	points[1].y = 0.0f;
	texCoords[1].x = 16.0f / 16.0f;
	texCoords[1].y = 1.0f - (8.0f / 16.0f);

	points[2].x = 4.0f; 
	points[2].y = 1.0f;
	texCoords[2].x = 16.0f / 16.0f;
	texCoords[2].y = 1.0f - (10.0f / 16.0f);

	points[3].x = 2.0f; 
	points[3].y = 4.0f;
	texCoords[3].x = 12.0f / 16.0f;
	texCoords[3].y = 1.0f - (16.0f / 16.0f);

	points[4].x = 0.0f; 
	points[4].y = 4.0f;
	texCoords[4].x = 8.0f / 16.0f;
	texCoords[4].y = 1.0f - (16.0f / 16.0f);

	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, bager);
		DrawExtrudedPolygon(points, texCoords, 5, 4.0f, 0.96f, 0.5f, 0.12f);
	}
	glPopMatrix();
}

void CGLRenderer::DrawArm(double zh)
{
	glEnable(GL_TEXTURE_2D);

	POINTF points[7];
	POINTF texCoords[7];

	points[0].x = 0.0f;  
	points[0].y = -1.0f;
	texCoords[0].x = 2.0f / 16.0f;
	texCoords[0].y = 1.0f - (4.0f / 16.0f);

	points[1].x = 6.0f;  
	points[1].y = -0.5f;
	texCoords[1].x = 14.0f / 16.0f;
	texCoords[1].y = 1.0f - (5.0f / 16.0f);

	points[2].x = 6.5f;  
	points[2].y = 0.0f;
	texCoords[2].x = 15.0f / 16.0f;
	texCoords[2].y = 1.0f - (6.0f / 16.0f);

	points[3].x = 6.0f;  
	points[3].y = 0.5f;
	texCoords[3].x = 14.0f / 16.0f;
	texCoords[3].y = 1.0f - (7.0f / 16.0f);

	points[4].x = 0.0f;  
	points[4].y = 1.0f;
	texCoords[4].x = 2.0f / 16.0f;
	texCoords[4].y = 1.0f - (8.0f / 16.0f);

	points[5].x = -1.0f; 
	points[5].y = 0.5f;
	texCoords[5].x = 0.0f / 16.0f;
	texCoords[5].y = 1.0f - (7.0f / 16.0f);

	points[6].x = -1.0f; 
	points[6].y = -0.5f;
	texCoords[6].x = 0.0f / 16.0f;
	texCoords[6].y = 1.0f - (5.0f / 16.0f);

	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, bager);
		DrawExtrudedPolygon(points, texCoords, 7, zh, 0.96f, 0.5f, 0.12f);
	}
	glPopMatrix();
}

void CGLRenderer::DrawFork()
{
	glEnable(GL_TEXTURE_2D);

	POINTF points[6];
	POINTF texCoords[6];

	points[0].x = -1.0f; 
	points[0].y = -0.5f;
	texCoords[0].x = 0.0f / 16.0f;
	texCoords[0].y = 1.0f - (10.0f / 16.0f);

	points[1].x = -0.5f; 
	points[1].y = -1.0f;
	texCoords[1].x = 1.0f / 16.0f;
	texCoords[1].y = 1.0f - (9.0f / 16.0f);

	points[2].x = 2.5f;  
	points[2].y = -1.0f;
	texCoords[2].x = 7.0f / 16.0f;
	texCoords[2].y = 1.0f - (9.0f / 16.0f);

	points[3].x = 2.5f;  
	points[3].y = 2.0f;
	texCoords[3].x = 7.0f / 16.0f;
	texCoords[3].y = 1.0f - (15.0f / 16.0f);

	points[4].x = -0.5f; 
	points[4].y = 2.0f;
	texCoords[4].x = 1.0f / 16.0f;
	texCoords[4].y = 1.0f - (15.0f / 16.0f);

	points[5].x = -1.0f; 
	points[5].y = 1.5f;
	texCoords[5].x = 0.0f / 16.0f;
	texCoords[5].y = 1.0f - (14.0f / 16.0f);

	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, bager);
		DrawExtrudedPolygon(points, texCoords, 6, 1.0f, 0.7f, 0.7f, 0.7f);
	}
	glPopMatrix();
}

void CGLRenderer::DrawExcavator()
{
	DrawBase();

	glTranslatef(2.0f, 2.0f, -0.5f);

	glPushMatrix();
	{
		glTranslatef(2.0f, 0.0f, -2.0f);
		glRotatef(m_rot1, 0.0f, 1.0f, 0.0f);
		glTranslatef(-2.0f, 0.0f, 2.0f);

		DrawBody();

		glTranslatef(2.0f, 1.0f, 0.0f);
		glRotatef(m_rot2, 0.0f, 0.0f, 1.0f);
		DrawArm(1.0f);

		glTranslatef(6.0f, 0.0f, -1.0f);
		glRotatef(m_rot3, 0.0f, 0.0f, 1.0f);
		DrawArm(1.5f);

		glTranslatef(6.0f, 0.0f, -1.5f);
		glRotatef(m_rot4, 0.0f, 0.0f, 1.0f);
		DrawFork();
	}
	glPopMatrix();
}

