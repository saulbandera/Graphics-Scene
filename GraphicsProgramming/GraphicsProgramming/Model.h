#ifndef _MODEL_H_
#define _MODEL_H_
#include "ProjectIncludes.h"
class Model {
public:
	bool load(char* modelFilename, char* textureFilename);
	void render();

private:
	void loadTexture(char*);
	bool loadModel(char*);

	// model texture
	GLuint texture;
	// Stoagre for sorted data
	vector<Vector3> vertex, normals, texCoords; //vertex, normals and texcoords arrays
};

#endif