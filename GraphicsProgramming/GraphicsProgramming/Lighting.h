#pragma once
#include "ProjectIncludes.h"
#include "Model.h"
using namespace std;
class Lighting{
public:
	Lighting();
	Vector3 selectColor(string color);

	void spotLt(float x, float y, float z, string color, GLenum lightNum, float tilt);
	void directionalLt(float x, float y, float z, string color);
	void pointLt(float x, float y, float z, string color);
	void ambientLt(float x, float y, float z, string color);
	void lightBulb(float x, float y, float z);
	void twinkleLight(float x, float y, float z, string color, float r, GLenum lightn, float tilt);
	Model sphere;
	Model sphere1;
	Model sphere2;
	Model sphereBase;
	Model sphereBase1;

private:
	GLuint myTexture;
	Vector3 colorVector;
	Vector3 retColorVector;





};


