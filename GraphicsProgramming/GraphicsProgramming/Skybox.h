#pragma once
//#include "includes.h"
#include "ProjectIncludes.h"
#include "Model.h"
#include "Camera.h"
class Skybox {
public:
	Skybox(Camera* cam); //include pointer to camera so we can get its position
	void renderSkybox(); // render sphere with sky texture around camera
	Camera* camera;
	Model skydome;
	
};

