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

	// funkcije za zadatak
	void DrawAxes();
	UINT LoadTextures(char* fileName);
	void PolarToCartesian(double R, double phi, double theta, double& x, double& y, double& z);
	void DrawSphere(float R, int n, int m);
	void CalcRotAxis(double x1, double y1, double z1, double x2, double y2, double z2, double& x3, double& y3, double& z3);
	void DrawSphFlower(float R, int n, int m, float factor,
		unsigned char R1, unsigned char G1,
		unsigned char B1, unsigned char R2,
		unsigned char G2, unsigned char B2);
	void DrawFlower();

public:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	double m_cameraR = 40.0;
	double m_angleXY = 0.0;
	double m_angleXZ = 20.0;

	// promenljive za zadatak
	UINT bg;
	double m_flowerAngle = 0.0;

	double m_eyex, m_eyey, m_eyez;

	const double PI = 3.14;
	inline double ToRad(double angle) { return angle * PI / 180.0; }
};
