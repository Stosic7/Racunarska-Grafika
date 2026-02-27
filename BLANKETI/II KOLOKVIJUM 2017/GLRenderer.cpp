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

	bg = LoadTexture("slike/T1.jpg");
	
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

	GLfloat light1_pos[] = { 0.0, 5,0, -10, 1 };
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, white_light);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT1, GL_SPECULAR, ambient_light);
	glEnable(GL_LIGHT1);

	glEnable(GL_LIGHTING);

	glPushMatrix();
	{
		DrawWall(100.0);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0.0, 5.0, -10.0);

		GLfloat emission_color[] = { 1, 1, 1, 1 };
		glMaterialfv(GL_FRONT, GL_EMISSION, emission_color);

		glutSolidSphere(0.5, 32, 32);

		GLfloat no_emission[] = { 0, 0, 0, 1 };
		glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
	}
	glPopMatrix();

	SetMetalicMaterial();

	glPushMatrix();
	{
		glRotatef(m_gearAngle, 0.0f, 0.0f, 1.0f);
		DrawGear(0.5, 1.0, 40, 0.5, 0.5);
	}
	glPopMatrix();

	glPushMatrix();
	{
		double r1 = R(0.5, 1.0, 40, 0.5);
		double r2 = R(0.5, 2.0, 20, 0.5);
		double distance = r1 + r2 - 0.5;

		glTranslatef(distance, 0.0f, 0.0f);
		glRotatef(30.0f, 0.0f, 0.0f, 1.0f);

		glRotatef(-m_gearAngle * 2.0, 0.0f, 0.0f, 1.0f);

		DrawGear(0.5, 2.0, 20, 0.5, 0.5);
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
	gluPerspective(45, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glDeleteTextures(1, &bg);
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

double CGLRenderer::R(double a, double alpha, int n, double h)
{
	double alphaRad = alpha * 3.1415926535 / 180.0;
	double slopeWidth = h * tan(alphaRad);
	double perimeter = n * (a + a + 2 * slopeWidth);
	return perimeter / (2 * 3.1415926535);
}

void CGLRenderer::NormCrossProd(double x1, double y1, double z1, double x2, double y2, double z2, double& x, double& y, double& z)
{
	x = y1 * z2 - z1 * y2;
	y = z1 * x2 - x1 * z2;
	z = x1 * y2 - y1 * x2;
	double length = sqrt(x * x + y * y + z * z);
	if (length > 0) {
		x /= length;
		y /= length;
		z /= length;
	}
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

void CGLRenderer::SetMetalicMaterial()
{
	GLfloat ambient[] = { 0.05f, 0.05f, 0.2f, 1.0f };
	GLfloat diffuse[] = { 0.1f, 0.1f, 0.5f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shininess = 120.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void CGLRenderer::DrawWall(double a)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	// FRONT
	glBindTexture(GL_TEXTURE_2D, bg);
	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);
		glTexCoord2f(0.0, 0.0); glVertex3d(-a / 2, a / 2, -a / 2);
		glTexCoord2f(0.0, 1.0); glVertex3d(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(1.0, 1.0); glVertex3d(a / 2, -a / 2, -a / 2);
		glTexCoord2f(1.0, 0.0); glVertex3d(a / 2, a / 2, -a / 2);
	}
	glEnd();

	// LEFT
	glBindTexture(GL_TEXTURE_2D, bg);
	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);
		glTexCoord2f(1.0, 0.0); glVertex3d(-a / 2, a / 2, a / 2);
		glTexCoord2f(1.0, 1.0); glVertex3d(-a / 2, -a / 2, a / 2);
		glTexCoord2f(0.0, 1.0); glVertex3d(-a / 2, -a / 2, -a / 2);
		glTexCoord2f(0.0, 0.0); glVertex3d(-a / 2, a / 2, -a / 2);
	}
	glEnd();

	// RIGHT
	glBindTexture(GL_TEXTURE_2D, bg);
	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);
		glTexCoord2f(1.0, 0.0); glVertex3d(a / 2, a / 2, -a / 2);
		glTexCoord2f(1.0, 1.0); glVertex3d(a / 2, -a / 2, -a / 2);
		glTexCoord2f(0.0, 1.0); glVertex3d(a / 2, -a / 2, a / 2);
		glTexCoord2f(0.0, 0.0); glVertex3d(a / 2, a / 2, a / 2);
	}
	glEnd();

	// BACK
	glBindTexture(GL_TEXTURE_2D, bg);
	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);
		glTexCoord2f(0.0, 0.0); glVertex3d(a / 2, a / 2, a / 2);
		glTexCoord2f(0.0, 1.0); glVertex3d(a / 2, -a / 2, a / 2);
		glTexCoord2f(1.0, 1.0); glVertex3d(-a / 2, -a / 2, a / 2);
		glTexCoord2f(1.0, 0.0); glVertex3d(-a / 2, a / 2, a / 2);
	}
	glEnd();

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawGear(double a, double alpha, int n, double h, double d)
{
	double R_outer = R(a, alpha, n, h);
	double R_inner = R_outer - h;

	double alphaRad = alpha * 3.1415926535 / 180.0;
	double segmentAngle = 2 * 3.1415926535 / n;

	double slopeWidth = h * tan(alphaRad);
	double totalWidth = a + a + 2 * slopeWidth;

	double dThetaTop = (a / totalWidth) * segmentAngle;
	double dThetaSlope = (slopeWidth / totalWidth) * segmentAngle;
	double dThetaBot = (a / totalWidth) * segmentAngle;

	int numPoints = 4 * n;
	POINTF* profile = new POINTF[numPoints];

	double currentAngle = 0;
	for (int i = 0; i < n; i++)
	{
		profile[4 * i + 0].x = R_outer * cos(currentAngle);
		profile[4 * i + 0].y = R_outer * sin(currentAngle);
		currentAngle += dThetaTop;

		profile[4 * i + 1].x = R_outer * cos(currentAngle);
		profile[4 * i + 1].y = R_outer * sin(currentAngle);
		currentAngle += dThetaSlope;

		profile[4 * i + 2].x = R_inner * cos(currentAngle);
		profile[4 * i + 2].y = R_inner * sin(currentAngle);
		currentAngle += dThetaBot;

		profile[4 * i + 3].x = R_inner * cos(currentAngle);
		profile[4 * i + 3].y = R_inner * sin(currentAngle);
		currentAngle += dThetaSlope;
	}

	glNormal3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3f(0.0f, 0.0f, d / 2);
		for (int i = 0; i < numPoints; i++) {
			glVertex3f(profile[i].x, profile[i].y, d / 2);
		}
		glVertex3f(profile[0].x, profile[0].y, d / 2);
	}
	glEnd();

	glNormal3f(0.0f, 0.0f, -1.0f);
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3f(0.0f, 0.0f, -d / 2);
		for (int i = numPoints - 1; i >= 0; i--) {
			glVertex3f(profile[i].x, profile[i].y, -d / 2);
		}
		glVertex3f(profile[numPoints - 1].x, profile[numPoints - 1].y, -d / 2);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		for (int i = 0; i < numPoints; i++)
		{
			int next = (i + 1) % numPoints;

			double v1x = 0.0;
			double v1y = 0.0;
			double v1z = -d;

			double v2x = profile[next].x - profile[i].x;
			double v2y = profile[next].y - profile[i].y;
			double v2z = 0.0;

			double nx, ny, nz;
			NormCrossProd(v1x, v1y, v1z, v2x, v2y, v2z, nx, ny, nz);
			glNormal3f(nx, ny, nz);
			glVertex3f(profile[i].x, profile[i].y, d / 2);
			glVertex3f(profile[i].x, profile[i].y, -d / 2);
			glVertex3f(profile[next].x, profile[next].y, -d / 2);
			glVertex3f(profile[next].x, profile[next].y, d / 2);
		}
	}
	glEnd();

	delete[] profile;
}


