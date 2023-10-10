#pragma once
#include "ProjectIncludes.h"

class GeoModels{
public: 
	GeoModels();
	void plane(float x, float y, float z, float width, float length);
	void cube(float x, float y, float z, float rotation);
	void pyramid(float x, float y, float z, float rotation);

private:
	GLuint textureStone;
	GLuint texturePlane;	

	GLfloat  pyrVerts[36] = {
		//front
		1, 0, 1,
		0, 1, 0,
		-1, 0, 1,
		//right
		1, 0, 1,
		0, 1, 0,
		1, 0, -1,
		//back
		1, 0, -1,
		0, 1, 0,
		-1, 0, -1,
		//left
		-1, 0, 1,
		0, 1, 0,
		-1, 0, -1,


		// Vertex #3
	};

	GLfloat  pyrNormals[36] = {
							   0, 0, -1,
								0, 0, -1,
								0, 0, -1,

								-1,0,0,
								-1,0,0,
								-1,0,0,

								 0,0,1,
								0,0,1,
								0,0,1,

								1,0,0,
								1,0,0,
								1,0,0,




	};

	GLfloat pyrTexcoords[24] = {     1,0,
									0,1,
								   -1,0,

									1,0,
									0,1,
								  -1,0,

									1,0,
									0,1,
								  -1,0,

									1,0,
									0,1,
								  -1,0,
	};

};

