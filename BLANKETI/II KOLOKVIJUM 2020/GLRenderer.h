#pragma once
#include "DImage.h"

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	// helper za crtanje
	void DrawAxes(double width);

	// helperi
	void RotateView(double dXY, double dXZ);
	void ZoomView(double dR);
	void UpdateCameraPosition();

	// funkcije za zadatak
	UINT LoadTexture(char* fileName);
	void DrawEnvCube(double a);
	void DrawSphere(double r, int nSeg, double texU, double texV, double texR);
	void DrawCone(double r, double h, int nSeg, double texU, double texV, double texR);
	void DrawSpiderBody();
	void DrawLegSegment(double r, double h, int nSeg);
	void DrawLeg();
	void DrawSpider();

public:
	HGLRC	 m_hrc; //OpenGL Rendering Context

	double m_cameraR = 50.0;
	double m_angleXY = 0.0;
	double m_angleXZ = 20.0; 

	// promenljive za zadatak
	UINT m_texSpider;
	UINT m_texEnv[6];

	double m_eyex, m_eyey, m_eyez;
	const double PI = 3.14159265358979323846;
	inline double ToRad(double angle) { return angle * PI / 180.0; }
};
