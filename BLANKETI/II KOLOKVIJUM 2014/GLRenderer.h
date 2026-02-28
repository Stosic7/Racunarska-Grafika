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
	void DrawAxes();
	void RotateView(double dXY, double dXZ);
	void ZoomView(double dR);
	void UpdateCameraPosition();

	// za zadatak
	void DrawCube(float a, int x, int y, int z, int count);
	UINT LoadTexture(char* fileName);
	void DrawRubikCube(double a, int count);
	void SetWhiteLight();
	void SetMaterial(float r, float g, float b);
public:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	double m_cameraR = 50.0;
	double m_angleXY = 0.0;
	double m_angleXZ = 20.0;

	double m_eyex, m_eyey, m_eyez;

	// za zadatak
	UINT logo;
	double m_rot1;
	double m_rot2;
	double m_rot3;

	const double PI = 3.14;
	inline double ToRad(double angle) { return angle * PI / 180.0; }

};
