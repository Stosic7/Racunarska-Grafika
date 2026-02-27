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

	// helperi
	void RotateView(double dXY, double dXZ);
	void ZoomView(double dR);
	void UpdateCameraPosition();
	double R(double a, double alpha, int n, double h);
	void NormCrossProd(double x1, double y1, double z1, double x2, double y2, double z2, double& x, double& y, double& z);

	// funkcije za zadatak
	UINT LoadTexture(char* fileName);
	void SetMetalicMaterial();
	void DrawWall(double a);
	void DrawGear(double a, double alpha, int n, double h, double d);

public:
	HGLRC	 m_hrc; //OpenGL Rendering Context

	double m_cameraR = 20.0;
	double m_angleXY =  0.0; 
	double m_angleXZ = 25.0;

	double m_eyex, m_eyey, m_eyez;

	// za zadatak
	UINT bg;
	double m_gearAngle = 0.0;

	const double PI = 3.14;
	inline double ToRad(double angle) { return angle * PI / 180.0f; }
};
