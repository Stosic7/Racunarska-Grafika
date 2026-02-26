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

	bg = LoadTextures("slike/Env.jpg"); // 10 poena

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

	float globAmb[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);

	float light0_pos[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	float light0_amb[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float light0_diff[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);
	glEnable(GL_LIGHT0);
	
	gluLookAt(
		m_eyex, m_eyey, m_eyez,
		0.0, 4.0, 0.0,
		0.0, 1.0, 0.0
	);

	float light1_pos[] = { 0.0f, 1.0f, 1.0f, 0.0f };
	float light1_amb[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float light1_diff[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diff);
	glEnable(GL_LIGHT1);

	glEnable(GL_LIGHTING);

	DrawSphere(1000.0f, 36, 36);
	DrawAxes();
	DrawFlower();

	glPushMatrix();
	{
		glTranslatef(9.0f, 0.0f, 0.0f);
		DrawFlower();
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0.0f, 0.0f, 9.0f);
		DrawFlower();
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
	gluPerspective(50, (double)w / (double)h, 0.1, 2000);
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

void CGLRenderer::DrawAxes()
{
	glDisable(GL_LIGHTING);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	{
		// x ose
		glColor3f(0.0, 0.0, 1.0);
		glVertex3d(0, 0, 0);
		glVertex3d(50, 0, 0);

		// y ose
		glColor3f(1.0, 0.0, 0.0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 50, 0);

		// z ose
		glColor3f(0.0, 1.0, 0.0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, 50);
	}
	glEnd();
	glLineWidth(1.0);
	glEnable(GL_LIGHTING);
}

UINT CGLRenderer::LoadTextures(char* fileName)
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

void CGLRenderer::PolarToCartesian(double R, double phi, double theta, double& x, double& y, double& z)
{
	double phiRad = ToRad(phi);
	double thetaRad = ToRad(theta);

	x = R * cos(thetaRad) * cos(phiRad);
	y = R * sin(thetaRad);
	z = R * cos(thetaRad) * sin(phiRad);
}

void CGLRenderer::DrawSphere(float R, int n, int m)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glPushMatrix();
	{
		glTranslated(m_eyex, m_eyey, m_eyez);
		glBindTexture(GL_TEXTURE_2D, bg);

		double ang1, ang2;
		double dAng1, dAng2;

		dAng1 = PI / (double)m;
		dAng2 = 2.0 * PI / (double)n;
		ang1 = -PI / 2;

		for (int i = 0; i < m; i++) {
			ang2 = 0;
			glBegin(GL_QUAD_STRIP);
			for (int j = 0; j < n + 1; j++) {

				// koordinate za teksturu
				double u = (double)j / n; // sirina
				double v1 = (double)i / m; // visina donjeg temena
				double v2 = (double)(i + 1) / m; // visina gornjeg temena (na spratu iznad)

				double x1 = R * cos(ang1) * cos(ang2);
				double y1 = R * sin(ang1);
				double z1 = R * cos(ang1) * sin(ang2);

				glTexCoord2f(u, v1);
				glVertex3d(x1, y1, z1);

				double x2 = R * cos(ang1 + dAng1) * cos(ang2);
				double y2 = R * sin(ang1 + dAng1);
				double z2 = R * cos(ang1 + dAng1) * sin(ang2);

				glTexCoord2f(u, v2);
				glVertex3d(x2, y2, z2);

				ang2 += dAng2;
			}
			glEnd();
			ang1 += dAng1;
		}
		glEnable(GL_TEXTURE_2D);
		glPopMatrix();
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
	}
}

void CGLRenderer::CalcRotAxis(double x1, double y1, double z1, double x2, double y2, double z2, double& x3, double& y3, double& z3)
{
	// pravimo vektor V koji ide od tacke 1 do tacke 2 (V = P2 - P1)
	double vx = x2 - x1;
	double vy = y2 - y1;
	double vz = z2 - z1;

	// racunamo duzinu (intenzitet) tog vektora primenom Pitagorine teoreme u 3D
	double length = sqrt(vx * vx + vy * vy + vz * vz);

	// normalizujemo vektor
	if (length > 0.0)
	{
		x3 = vx / length;
		y3 = vy / length;
		z3 = vz / length;
	}
	else
	{
		x3 = 0.0;
		y3 = 0.0;
		z3 = 0.0;
	}
}

void CGLRenderer::DrawSphFlower(float R, int n, int m, float factor,
								unsigned char R1, unsigned char G1,
								unsigned char B1, unsigned char R2,
								unsigned char G2, unsigned char B2)
{
	glDisable(GL_TEXTURE_2D); // Na sferu se ne primenjuju teksture

	float amb[] = { R1 * 0.2f / 255.0f, G1 * 0.2f / 255.0f, B1 * 0.2f / 255.0f, 1.0f }; // ambijentalna komponenta 20% boje juznog pola
	float spec[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // boja refleksije

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);

	double dAng1 = 180.0 / m;
	double dAng2 = 360.0 / n;

	double ang2 = 0.0;

	for (int j = 0; j < n; j++) {
		glPushMatrix();
		{
			double ang1 = -90;

			for (int i = 0; i < m; i++) {

				// rascunanje koordinata
				double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;

				PolarToCartesian(R, ang2, ang1, x1, y1, z1);                 // dole levo
				PolarToCartesian(R, ang2 + dAng2, ang1, x2, y2, z2);         // dole desno
				PolarToCartesian(R, ang2 + dAng2, ang1 + dAng1, x3, y3, z3); // gore desno
				PolarToCartesian(R, ang2, ang1 + dAng1, x4, y4, z4);         // gore levo

				// racunanje ose rotacije kroz donja temena
				// kada se rotira samo donja osnovica pravougaonika to daje utisak "sirenja"
				double ax, ay, az;
				CalcRotAxis(x1, y1, z1, x2, y2, z2, ax, ay, az);

				// primena rotacije
				// rotiramo za m_flowerAngel * factor
				glTranslated(x1, y1, z1);
				glRotated(-m_flowerAngle * factor, ax, ay, az);
				glTranslated(-x1, -y1, -z1);

				// interpolacija boja
				// t1 trenutni sprat, t2 sprat iznad
				float t1 = (float)i / m;
				float t2 = (float)(i + 1) / m;

				// difuzne komponente
				float r_diff1 = (R1 + t1 * (R2 - R1)) / 255.0f;
				float g_diff1 = (G1 + t1 * (G2 - G1)) / 255.0f;
				float b_diff1 = (B1 + t1 * (B2 - B1)) / 255.0f;

				float r_diff2 = (R1 + t2 * (R2 - R1)) / 255.0f;
				float g_diff2 = (G1 + t2 * (G2 - G1)) / 255.0f;
				float b_diff2 = (B1 + t2 * (B2 - B1)) / 255.0f;

				float diff1[] = { r_diff1, g_diff1, b_diff1, 1.0f };
				float diff2[] = { r_diff2, g_diff2, b_diff2, 1.0f };

				// iscrtavanje segmenata
				glBegin(GL_QUADS);
				{
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff1);
					glNormal3d(x1 / R, y1 / R, z1 / R);
					glVertex3d(x1, y1, z1);

					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff1);
					glNormal3d(x2 / R, y2 / R, z2 / R);
					glVertex3d(x2, y2, z2);

					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff2);
					glNormal3d(x3 / R, y3 / R, z3 / R);
					glVertex3d(x3, y3, z3);

					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff2);
					glNormal3d(x4 / R, y4 / R, z4 / R);
					glVertex3d(x4, y4, z4);
				}
				glEnd();
				ang1 += dAng1;
			}
		}
		glPopMatrix();
		ang2 += dAng2;
	}
	glEnable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawFlower()
{
	glPushMatrix();
	{
		glPushMatrix();
		{
			// srednji
			glTranslatef(0, 0, 0);
			DrawSphFlower(2.0, 36, 18, 1.0, 0, 192, 0, 0, 255, 0);
		}
		glPopMatrix();

		glPushMatrix();
		{
			// spoljasnji
			glTranslatef(0.0, -0.5, 0.0);
			DrawSphFlower(1.5, 36, 18, 0.5, 0, 0, 255, 255, 0, 0);
		}
		glPopMatrix();

		glPushMatrix();
		{
			// unutrasnji
			glTranslatef(0.0, -1.0, 0.0);
			DrawSphFlower(1.0, 36, 18, 0.25, 192, 192, 0, 192, 192, 192);
		}
		glPopMatrix();
	}
	glPopMatrix();
}


