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
	void DrawAxes();

	// za zadatak
	UINT LoadTexture(char* fileName);
	void FillVA(float* buff, float a, float b, float h);
	void FillVANorm(float* buff, float a, float b, float h);
	void FillVATex(float* buff, float x1, float x2);


protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	double m_cameraR = 20.0;
	double m_angleXY = 0.0;
	double m_angleXZ = 20.0;

	double m_eyex, m_eyey, m_eyez;

	// za zadatak
	UINT towerParts[4];
	float* m_VA[4];
	float* m_VANorm[4];
	float* m_VATex[4];

	const double PI = 3.14;
	inline double ToRad(double angle) { return angle * PI / 180.0; }

};
