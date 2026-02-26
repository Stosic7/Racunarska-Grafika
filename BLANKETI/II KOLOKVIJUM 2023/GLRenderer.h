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
	UINT LoadTexture(char* fileName);
	void DrawEnvCube(double a);
	void DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool bIsOpen);
	void DrawLampBase();
	void DrawLampArm();
	void DrawLampHead();
	void DrawLamp();

public:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	double m_cameraR = 50.0;
	double m_angleXY = 45.0;
	double m_angleXZ = 15.0;

	// promenljive za zadatak
	UINT lamp;
	UINT pozadina[7];

	double m_rot1;
	double m_rot2;
	double m_rot3;

	double m_eyex, m_eyey, m_eyez;

	const double PI = 3.14;
	inline double ToRad(double angle) { return angle * PI / 180.0f; }
};
