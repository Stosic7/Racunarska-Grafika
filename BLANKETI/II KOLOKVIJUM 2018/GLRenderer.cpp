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

	m_cameraR = 25.0;
	m_cameraAngleXY = 0.0;
	m_cameraAngleXZ = 20.0;

	m_beta = 0.0;
	m_alpha = 0.0;
	m_dist = 5.0;
	m_bLight = false;

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
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	m_texShip = LoadTexture("slike/ShipT1.png");
	m_texSpace[0] = LoadTexture("slike/front.jpg");
	m_texSpace[1] = LoadTexture("slike/left.jpg");
	m_texSpace[2] = LoadTexture("slike/right.jpg");
	m_texSpace[3] = LoadTexture("slike/back.jpg");
	m_texSpace[4] = LoadTexture("slike/top.jpg");
	m_texSpace[5] = LoadTexture("slike/bot.jpg");

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

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glRotated(m_beta, 1.0, 0.0, 0.0);
	glRotated(m_alpha, 0.0, 1.0, 0.0);
	DrawSpaceCube(1.0);

	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

	glTranslatef(0, 0, -m_dist);
	glRotated(m_beta, 1.0, 0.0, 0.0);
	glRotated(m_alpha, 0.0, 1.0, 0.0);

	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	DrawAxis(10.0);
	glEnable(GL_TEXTURE_2D);

	if (m_bLight)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	double d = sqrt(5.8 * 5.8 + 2.15 * 2.15);
	glTranslatef(-d / 2., 0, 0);
	glRotatef(90, 1, 0, 0);
	DrawShip();

	//---------------------------------
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
	gluPerspective(50, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glDeleteTextures(1, &m_texShip);
	glDeleteTextures(6, m_texSpace);
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxis(double width)
{
	glLineWidth(2.0);
	glBegin(GL_LINES);
	{
		// x osa
		glColor3f(1.0, 0.0, 0.0);
		glVertex3d(0, 0, 0);
		glVertex3d(width, 0, 0);

		// y osa
		glColor3f(0.0, 1.0, 0.0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, width, 0);

		// z osa
		glColor3f(0.0, 0.0, 1.0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, 1.0);
	}
	glEnd();
	glLineWidth(1.0);
}

void CGLRenderer::RotateView(double dXY, double dXZ)
{
	m_cameraAngleXY += dXY;
	m_cameraAngleXZ += dXZ;

	if (m_cameraAngleXZ > 90)
		m_cameraAngleXZ = 90;
	if (m_cameraAngleXY < -90)
		m_cameraAngleXZ = -90;

	UpdateCameraPosition();
}

void CGLRenderer::ZoomView(double dR)
{
	m_cameraR += dR;
	if (m_cameraR < 2.0)
		m_cameraR = 2.0;
	if (m_cameraR > 50.0)
		m_cameraR = 50.0;

	UpdateCameraPosition();
}

void CGLRenderer::UpdateCameraPosition()
{
	double radXY = ToRad(m_cameraAngleXY);
	double radXZ = ToRad(m_cameraAngleXZ);

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

void CGLRenderer::DrawTriangle(float d1, float d2, float rep)
{
	double a1 = atan2(d2, d1); // ugao izmedju d1 i d3
	double d3 = sqrt(d1 * d1 + d2 * d2); // hipotenuza
	double y = d1 * cos(a1) / d3;
	double x = d1 * sin(a1) / d3;

	int n = (int)rep;

	double u_bot = (0.5 + x) * rep; // Tekstura u pravom uglu (dole desno)
	double v_bot = y * rep;
	double u_top = 0.5 * rep;       // Tekstura na vrhu (gore desno)
	double v_top = 1.0 * rep;

	glColor3f(1.0, 1.0, 1.0);
	glNormal3f(0.0, 0.0, 1.0);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < n; i++) {
		double t0 = (double)i / n;
		double t1 = (double)(i + 1) / n;

		// 1. tacka trougla
		glTexCoord2d(0.5 * rep, 0.0);
		glVertex3d(0.0, 0.0, 0.0);

		// 2. tacka trougla
		double u2 = u_bot + t0 * (u_top - u_bot);
		double v2 = v_bot + t0 * (v_top - v_bot);
		glTexCoord2d(u2, v2);
		glVertex3d(d1, t0 * d2, 0.0);

		// 3. tacka trougla
		double u3 = u_bot + t1 * (u_top - u_bot);
		double v3 = v_bot + t1 * (v_top - v_bot);
		glTexCoord2d(u3, v3);
		glVertex3d(d1, t1 * d2, 0.0);
	}
	glEnd();
}

void CGLRenderer::DrawShip()
{
	glBindTexture(GL_TEXTURE_2D, m_texShip);
	double a1 = atan2(2.15, 5.8);
	float rep = 3.0;

	// gornji levo
	glPushMatrix();
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-ToDeg(a1), 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();

	// gornji desno
	glPushMatrix();
	glScalef(1, -1, 1);
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-ToDeg(a1), 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();

	// donji deo
	glPushMatrix();
	glRotatef(180, 1, 0, 0);

	// levo
	glPushMatrix();
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-ToDeg(a1), 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();

	// desno
	glPushMatrix();
	glScalef(1, -1, 1);
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-ToDeg(a1), 0, 0, 1);
	DrawTriangle(5.8, 2.15, rep);
	glPopMatrix();
	glPopMatrix();
}

void CGLRenderer::DrawSpaceCube(double a)
{
	// 1. PREDNJA STRANA (Front) - Gleda u -Z osu
	glBindTexture(GL_TEXTURE_2D, m_texSpace[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3d(-a / 2, a / 2, -a / 2); // Gore levo
	glTexCoord2f(0.0, 0.0); glVertex3d(-a / 2, -a / 2, -a / 2); // Dole levo
	glTexCoord2f(1.0, 0.0); glVertex3d(a / 2, -a / 2, -a / 2);  // Dole desno
	glTexCoord2f(1.0, 1.0); glVertex3d(a / 2, a / 2, -a / 2);   // Gore desno
	glEnd();

	// 2. LEVA STRANA (Left) - Gleda u -X osu
	glBindTexture(GL_TEXTURE_2D, m_texSpace[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3d(-a / 2, a / 2, a / 2);   // Gore levo
	glTexCoord2f(0.0, 0.0); glVertex3d(-a / 2, -a / 2, a / 2);  // Dole levo
	glTexCoord2f(1.0, 0.0); glVertex3d(-a / 2, -a / 2, -a / 2); // Dole desno
	glTexCoord2f(1.0, 1.0); glVertex3d(-a / 2, a / 2, -a / 2);  // Gore desno
	glEnd();

	// 3. DESNA STRANA (Right) - Gleda u +X osu
	glBindTexture(GL_TEXTURE_2D, m_texSpace[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3d(a / 2, a / 2, -a / 2);   // Gore levo (spaja se sa prednjom)
	glTexCoord2f(0.0, 0.0); glVertex3d(a / 2, -a / 2, -a / 2);  // Dole levo (spaja se sa prednjom)
	glTexCoord2f(1.0, 0.0); glVertex3d(a / 2, -a / 2, a / 2);   // Dole desno
	glTexCoord2f(1.0, 1.0); glVertex3d(a / 2, a / 2, a / 2);    // Gore desno
	glEnd();

	// 4. ZADNJA STRANA (Back) - Gleda u +Z osu
	glBindTexture(GL_TEXTURE_2D, m_texSpace[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3d(a / 2, a / 2, a / 2);    // Gore levo (spaja se sa desnom)
	glTexCoord2f(0.0, 0.0); glVertex3d(a / 2, -a / 2, a / 2);   // Dole levo
	glTexCoord2f(1.0, 0.0); glVertex3d(-a / 2, -a / 2, a / 2);  // Dole desno (spaja se sa levom)
	glTexCoord2f(1.0, 1.0); glVertex3d(-a / 2, a / 2, a / 2);   // Gore desno
	glEnd();

	// 5. GORNJA STRANA (Top) - Gleda u +Y osu
	glBindTexture(GL_TEXTURE_2D, m_texSpace[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3d(-a / 2, a / 2, a / 2);   // Gore levo (spaja se sa zadnjom)
	glTexCoord2f(0.0, 0.0); glVertex3d(-a / 2, a / 2, -a / 2);  // Dole levo (spaja se sa prednjom)
	glTexCoord2f(1.0, 0.0); glVertex3d(a / 2, a / 2, -a / 2);   // Dole desno (spaja se sa prednjom)
	glTexCoord2f(1.0, 1.0); glVertex3d(a / 2, a / 2, a / 2);    // Gore desno (spaja se sa zadnjom)
	glEnd();

	// 6. DONJA STRANA (Bot) - Gleda u -Y osu
	glBindTexture(GL_TEXTURE_2D, m_texSpace[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3d(-a / 2, -a / 2, -a / 2); // Gore levo (spaja se sa prednjom)
	glTexCoord2f(0.0, 0.0); glVertex3d(-a / 2, -a / 2, a / 2);  // Dole levo (spaja se sa zadnjom)
	glTexCoord2f(1.0, 0.0); glVertex3d(a / 2, -a / 2, a / 2);   // Dole desno (spaja se sa zadnjom)
	glTexCoord2f(1.0, 1.0); glVertex3d(a / 2, -a / 2, -a / 2);  // Gore desno (spaja se sa prednjom)
	glEnd();
}


