// Scene class. Configures a basic 3D scene.
// Interfaces with the Input class to handle user input
// Calculates and outputs Frames Per Second (FPS) rendered.
// Important functions are the constructor (initialising the scene), 
// update (for process user input and updating scene objects) and render (renders scene).

#ifndef _SCENE_H
#define _SCENE_H
#include "ProjectIncludes.h"
#include "Terrain.h"
#include "Camera.h"
#include "Skybox.h"
#include "Model.h"
#include "Lighting.h"
#include "Shadow.h"
#include "Cabin.h"
#include "GeoModels.h"



class Scene {

public:
	Scene(Input* in, Camera* camera);
	void render(Camera* camera); // Main render function
	void handleInput(Camera* camera, float dt); // Handle input function that receives delta time from parent
	void update(Camera* camera, float dt); // Update function receives delta time from parent (used for frame independent updating).
	void resize(int w, int h); 	// Resizes the OpenGL output based on new window size.
	
	// textures
	GLuint myTexture;
	GLuint textureImposter;
protected:
	void initialiseOpenGL();
	void displayText(float x, float y, float r, float g, float b, char* string);
	void renderTextOutput();
	void calculateFPS();
	float speed;
	float rotation;
	float rotation2;
	void pondReflection(Model* model, Model* model2, Model* modelPlant);
	void imposter(float transparecy);

	//Classes
	Skybox* skybox;
	Camera* cam;
	Terrain* g_terrain;
	Shadow shadow;
	Lighting lights;
	Lighting lights1;
	Lighting lights2;
	Lighting lights3;
	
	//Models
	Model rock;
	Model plant;
	Model pondTerrain;
	Cabin cabin;
	Model splinterCube; 
	GeoModels planeTest;
	GeoModels cubeTest;
	GeoModels pyramid;
	Input* input;

	// For Window and frustum calculation.
	int width, height;
	float fov, nearPlane, farPlane;

	//light positions
	float lightpos1[3] = { -16, 10, -17 };
	float lightpos2[3] = { -5, 15, 5 };
	float lightpos3[3] = { 0,4,2 };
	

	// For FPS counter and mouse coordinate output.
	int frame = 0, time, timebase = 0;
	char fps[40];
	char mouseText[40];
	float shadowMatrix[16];

	// quad for planar shadow of cabin
	GLfloat floorVerts[12] = { -10, 0,-10,
							 -10, 0, 10,
							  10, 0,-10,
							  10, 0, 10 };


   // imposter quad
    GLfloat impVerts[12] = { -1, 0,-1, 
							 -1, 0, 1,
							  1, 0,-1,
							  1, 0, 1 };
};




#endif