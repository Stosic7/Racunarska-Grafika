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

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	towerParts[0] = LoadTexture("slike/Eiffel-1a.bmp");
	towerParts[1] = LoadTexture("slike/Eiffel-2a.bmp");
	towerParts[2] = LoadTexture("slike/Eiffel-3a.bmp");
	towerParts[3] = LoadTexture("slike/Eiffel-4a.bmp");

	for (int i = 0; i < 4; i++) {
		m_VA[i] = new float[48];
		m_VANorm[i] = new float[48];
		m_VATex[i] = new float[32];
	}

	FillVA(m_VA[0], 4.0f, 2.4f, 1.5f);
	FillVANorm(m_VANorm[0], 4.0f, 2.4f, 1.5f);
	FillVATex(m_VATex[0], 0.215f, 0.0f);

	FillVA(m_VA[1], 2.4f, 1.2f, 1.5f);
	FillVANorm(m_VANorm[1], 2.4f, 1.2f, 1.5f);
	FillVATex(m_VATex[1], 0.215f, 0.0f);

	FillVA(m_VA[2], 1.08f, 0.0f, 8.0f);
	FillVANorm(m_VANorm[2], 1.08f, 0.0f, 8.0f);
	FillVATex(m_VATex[2], 0.45f, 0.05f);

	FillVA(m_VA[3], 2.4f, 2.64f, 0.25f);
	FillVANorm(m_VANorm[3], 2.4f, 2.64f, 0.25f);
	FillVATex(m_VATex[3], 0.0f, 0.0f);

	glEnable(GL_TEXTURE_2D);

	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(
		m_eyex, m_eyey, m_eyez,
		0.0, 4.0, 0.0,
		0.0, 1.0, 0.0
	);

	float light0_pos[] = { 10.0f, 15.0f, 10.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	float mat_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_white);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_white);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glPushMatrix();
	glTranslatef(0.0f, 0.75f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, towerParts[0]);
	glVertexPointer(3, GL_FLOAT, 0, m_VA[0]);
	glNormalPointer(GL_FLOAT, 0, m_VANorm[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, m_VATex[0]);
	glDrawArrays(GL_QUADS, 0, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 1.625f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, towerParts[3]);
	glVertexPointer(3, GL_FLOAT, 0, m_VA[3]);
	glNormalPointer(GL_FLOAT, 0, m_VANorm[3]);
	glTexCoordPointer(2, GL_FLOAT, 0, m_VATex[3]);
	glDrawArrays(GL_QUADS, 0, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 2.25f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, towerParts[1]);
	glVertexPointer(3, GL_FLOAT, 0, m_VA[1]);
	glNormalPointer(GL_FLOAT, 0, m_VANorm[1]);
	glTexCoordPointer(2, GL_FLOAT, 0, m_VATex[1]);
	glDrawArrays(GL_QUADS, 0, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 3.125f, 0.0f);
	glScalef(0.5f, 1.0f, 0.5f);
	glBindTexture(GL_TEXTURE_2D, towerParts[3]);
	glVertexPointer(3, GL_FLOAT, 0, m_VA[3]);
	glNormalPointer(GL_FLOAT, 0, m_VANorm[3]);
	glTexCoordPointer(2, GL_FLOAT, 0, m_VATex[3]);
	glDrawArrays(GL_QUADS, 0, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 7.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, towerParts[2]);
	glVertexPointer(3, GL_FLOAT, 0, m_VA[2]);
	glNormalPointer(GL_FLOAT, 0, m_VANorm[2]);
	glTexCoordPointer(2, GL_FLOAT, 0, m_VATex[2]);
	glDrawArrays(GL_QUADS, 0, 16);
	glPopMatrix();

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	DrawAxes();

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
	gluPerspective(45, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	glDeleteTextures(4, towerParts);

	for (int i = 0; i < 4; i++) {
		if (m_VA[i]) delete[] m_VA[i];
		if (m_VANorm[i]) delete[] m_VANorm[i];
		if (m_VATex[i]) delete[] m_VATex[i];
	}

	wglMakeCurrent(NULL, NULL);

	if (m_hrc)
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

void CGLRenderer::DrawAxes()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	{
		// x osa
		glColor3f(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(50, 0, 0);

		// y osa
		glColor3f(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 50, 0);

		// z osa
		glColor3f(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, 50);
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

void CGLRenderer::FillVA(float* buff, float a, float b, float h)
{
	float a2 = a / 2.0f;
	float b2 = b / 2.0f;
	float h2 = h / 2.0f;

	int i = 0;

	buff[i++] = -a2; 
	buff[i++] = -h2; 
	buff[i++] = a2;
	buff[i++] = a2; 
	buff[i++] = -h2; 
	buff[i++] = a2;
	buff[i++] = b2; 
	buff[i++] = h2; 
	buff[i++] = b2;
	buff[i++] = -b2; 
	buff[i++] = h2; 
	buff[i++] = b2;

	buff[i++] = a2; 
	buff[i++] = -h2; 
	buff[i++] = a2;
	buff[i++] = a2;
	buff[i++] = -h2; 
	buff[i++] = -a2;
	buff[i++] = b2; 
	buff[i++] = h2; 
	buff[i++] = -b2;
	buff[i++] = b2; 
	buff[i++] = h2; 
	buff[i++] = b2;

	buff[i++] = a2; 
	buff[i++] = -h2; 
	buff[i++] = -a2;
	buff[i++] = -a2; 
	buff[i++] = -h2; 
	buff[i++] = -a2;
	buff[i++] = -b2; 
	buff[i++] = h2; 
	buff[i++] = -b2;
	buff[i++] = b2; 
	buff[i++] = h2; 
	buff[i++] = -b2;

	buff[i++] = -a2; 
	buff[i++] = -h2; 
	buff[i++] = -a2;
	buff[i++] = -a2; 
	buff[i++] = -h2; 
	buff[i++] = a2;
	buff[i++] = -b2; 
	buff[i++] = h2; 
	buff[i++] = b2;
	buff[i++] = -b2; 
	buff[i++] = h2; 
	buff[i++] = -b2;
}

void CGLRenderer::FillVANorm(float* buff, float a, float b, float h)
{
	float dy = (a - b) / 2.0f;
	float len = sqrt(dy * dy + h * h);
	float ny = dy / len;
	float nxz = h / len;

	int i = 0;

	for (int j = 0; j < 4; j++) {
		buff[i++] = 0.0f;
		buff[i++] = ny;
		buff[i++] = nxz;
	}

	for (int j = 0; j < 4; j++) {
		buff[i++] = nxz;
		buff[i++] = ny;
		buff[i++] = 0.0f;
	}

	for (int j = 0; j < 4; j++) {
		buff[i++] = 0.0f;
		buff[i++] = ny;
		buff[i++] = -nxz;
	}

	for (int j = 0; j < 4; j++) {
		buff[i++] = -nxz;
		buff[i++] = ny;
		buff[i++] = 0.0f;
	}
}

void CGLRenderer::FillVATex(float* buff, float x1, float x2)
{
	int i = 0;

	for (int j = 0; j < 4; j++) {
		buff[i++] = x2;
		buff[i++] = 1.0f;

		buff[i++] = 1.0f - x2;
		buff[i++] = 1.0f;

		buff[i++] = 1.0f - x1;
		buff[i++] = 0.0f;

		buff[i++] = x1;
		buff[i++] = 0.0f;
	}
}


