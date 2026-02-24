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

	//helper za crtanje
	void DrawAxis(double width);

	// helperi za kameru
	void RotateView(double dXY, double dXZ);
	void ZoomView(double dR);
	void UpdateCameraPosition();

	// funkcije za zadatak
	UINT LoadTexture(char* fileName);
	void DrawTriangle(float d1, float d2, float rep);
	void DrawShip();
	void DrawSpaceCube(double a);

public:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	double m_cameraR;
	double m_cameraAngleXY;
	double m_cameraAngleXZ;

	// promenljive za zadatak
	UINT m_texShip;
	UINT m_texSpace[6];
	double m_beta;
	double m_alpha;
	double m_dist;
	bool m_bLight;

	double m_eyex, m_eyey, m_eyez;

	const double PI = 3.14159265358979323846;
	inline double ToRad(double angle) { return angle * PI / 180.0f; }
	inline double ToDeg(double angle) { return angle * 180.0 / PI; }
};
