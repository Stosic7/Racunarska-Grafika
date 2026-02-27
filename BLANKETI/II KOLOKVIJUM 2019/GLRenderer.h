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
	void DrawAxes(int width);

	// funkcije za zadatak
	UINT LoadTexture(char* fileName);
	void DrawEnvCube(double a);
	void DrawPolygon(POINTF* points, POINTF* texCoords, int n);
	void DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b);
	void DrawBase();
	void DrawBody();
	void DrawArm(double zh);
	void DrawFork();
	void DrawExcavator();

public:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	double m_cameraR = 25.0;
	double m_angleXY = 0;
	double m_angleXZ = 25.0;

	double m_eyex, m_eyey, m_eyez;

	// promenljive za zadatak
	UINT bager;
	UINT pozadina[6];

	double m_rot1;
	double m_rot2;
	double m_rot3;
	double m_rot4;

	const double PI = 3.14;
	inline double ToRad(double angle) { return angle * PI / 180.0f; }
};
