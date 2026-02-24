#pragma once
#include "DImage.h"
class GLRenderer
{
public:
	GLRenderer(void);
	virtual ~GLRenderer(void);

	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);
	void DrawScene(CDC* pDC);
	void DestroyScene(CDC* pDC);

	// helperi za crtanje
	void DrawAxis(double width);

	// helperi za kameru
	void RotateView(double dXY, double dXZ);
	void ZoomView(double dR);
	void UpdateCameraPosition();

	// funkcije za zadatak
	UINT LoadTexture(char* fileName);
	void DrawPatch(double R, int n);
	void inverseTSC(double x, double y, double& phi, double& theta) {
		phi = atan(x);
		theta = atan(y * cos(phi));
	}
	void DrawEarth(double R, int tes);
	void DrawMoon(double R, int tes);
	void DrawSpace(double R, int tes);

public:
	HGLRC m_hrc;

	// slike
	UINT T[6];
	UINT S[6];
	UINT M[6];

	double m_cameraR;
	double m_cameraAngleXY;
	double m_cameraAngleXZ;
	double m_beta;
	double m_alpha;
	bool m_bLight;
	double m_dist;
	double m_moonRot;

	double m_eyex, m_eyey, m_eyez;

	const double PI = 3.14159265358979323846;
	inline double ToRad(double angle) { return angle * PI / 180.0; }
};

