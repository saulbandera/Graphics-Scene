#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "model.h"
bool Model::load(char* modelFilename, char* textureFilename) { //// load model data and texture
	bool result;
	result = loadModel(modelFilename);
	if (!result) {
		std::cout << "Model Failed To Load: " << modelFilename << endl;
		return false;
	}
	loadTexture(textureFilename);
	return true;
}

void Model::render() { //// texture and render 
	glMatrixMode(GL_MODELVIEW);
	// specifying data 
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, this->vertex.data());
	glTexCoordPointer(3, GL_FLOAT, 0, this->texCoords.data());
	glNormalPointer(GL_FLOAT, 0, this->normals.data());
	// texturing 
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// rendering vertices
	glDrawArrays(GL_TRIANGLES, 0, this->vertex.size());

	// dereferencing 
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// Modified from a mulit-threaded version by Mark Ropper.
bool Model::loadModel(char* filename){
	vector<Vector3> verts;
	vector<Vector3> norms;
	vector<Vector3> texCs;
	vector<unsigned int> faces;
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		return false;
	}
	while (true)
	{
		char lineHeader[128];
		// Read first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break; // exit loop
		}
		else // Parse
		{
			if (strcmp(lineHeader, "v") == 0) // Vertex
			{
				Vector3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				verts.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0) // Tex Coord
			{
				Vector3 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				texCs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) // Normal
			{
				Vector3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				norms.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) // Face
			{
				unsigned int face[9];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &face[0], &face[1], &face[2],
					&face[3], &face[4], &face[5],
					&face[6], &face[7], &face[8]);
				if (matches != 9) {
					// Parser error, or not triangle faces
					return false;
				}
				for (int i = 0; i < 9; i++) {
					faces.push_back(face[i]);
				}
			}
		}
	}

#pragma region unroll_data
	float vertexPos, texturePos, normalPos;
	// loop through faces and store data for verts, textCoords and normals in their according vectors
	for (int i = 0; i < faces.size(); i += 3) {
		vertexPos = 0;
		texturePos = 0;
		normalPos = 0;
		vertexPos = faces[i];
		texturePos = faces[i + 1];
		normalPos = faces[i + 2];
		vertexPos = vertexPos - 1;
		texturePos = texturePos - 1;
		normalPos = normalPos - 1;
		vertex.push_back(verts[vertexPos]);
		texCoords.push_back(texCs[texturePos]);
		normals.push_back(norms[normalPos]);
	}
#pragma endregion unroll_data
	// clear data since it's already been stored
	verts.clear();
	norms.clear();
	texCs.clear();
	faces.clear();
	return true;
}

void Model::loadTexture(char* filename) {
	texture = SOIL_load_OGL_texture(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y // Depending on texture file type some need inverted others don't.
	);
	// check for an error during the load process
	if (texture == 0) {
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
}
