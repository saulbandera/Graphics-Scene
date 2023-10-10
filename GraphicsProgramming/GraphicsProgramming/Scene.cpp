#include "Scene.h"


Scene::Scene(Input* in, Camera* camera) {
	input = in;
	cam = camera;
	initialiseOpenGL();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	speed = 100.f;
	skybox = new Skybox(cam);

	// load imposter texture
	textureImposter = SOIL_load_OGL_texture(
		"gfx/imposter.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y // Depending on texture file type some need inverted others don't.
	);


	// load terrain
	g_terrain = new Terrain(8, 1);
	if (!g_terrain->LoadHeightmap("Data/Terrain/terrain0-16bbp-257x257.raw", 16, 257, 257)) {
		std::cerr << "Failed to load heightmap for terrain!" << std::endl;
	}
	if (!g_terrain->LoadTexture("Data/Textures/sand3.jpg")) {
		std::cerr << "Failed to load terrain texture for texture stage 0!" << std::endl;
	}

	// loading models
    #pragma region load_models
	rock.load("models/cliff_rock/cliffrock.obj", "models/cliff_rock/Cliff_Rock_Two_BaseColor.png");
    plant.load("models/fern/tropicalPlant.obj", "models/fern/11.png");
	pondTerrain.load("models/terrainPond.obj", "Data/Textures/sand4s.png");
	splinterCube.load("models/splinterCube.obj", "gfx/glassW.png");
    #pragma endregion load_models
}

/////////     HANDLE INPUT     //////////
void Scene::handleInput(Camera* camera, float dt) {
	cam->handleInput(dt, width, height);

	
	if (input->isKeyDown('l')) { //toggle wireframe mode
		input->setKeyUp('l');
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	}
	if (input->isKeyDown('o')) {
		input->setKeyUp('o');
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}
}


/////////     UPDATE     //////////
void Scene::update(Camera* camera, float dt) {
	cam->update();
	rotation += speed *0.4* dt; // variables for animated rotation
	rotation2 += speed *0.5* dt;
	calculateFPS();
}




/////////     RENDER     //////////
void Scene::render(Camera* camera) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear the color buffer, and the depth buffer.
	glLoadIdentity();
	gluLookAt(cam->get_position().x, cam->get_position().y, cam->get_position().z,
		cam->get_lookat().x, cam->get_lookat().y, cam->get_lookat().z,
		cam->get_up().x, cam->get_up().y, cam->get_up().z);

	
	glEnable(GL_TEXTURE_2D);

	//skybox
	glPushMatrix();
		glDisable(GL_LIGHTING);
		skybox->renderSkybox();
	glPopMatrix(); 
	

	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glTranslatef(-15, -0.5, -20);
		glRotatef(180, 1, 0,0);
		glScalef(0.4, 0.4, 0.4);
		glEnable(GL_BLEND);
		glColor3f(0.2f, 0.4f, 0.6f);
		pondTerrain.render();
		glColor3f(1,1,1);
	glPopMatrix();


	
#pragma region LIGHTING
	//lights.directionalLt(1, 1, 0, "WARMWHITE"); // "sunlight"
	lights1.twinkleLight(14, 10, 19.5, "BLUE", rotation, GL_LIGHT2,0); // light on pyramid

	glPushMatrix();
		glRotatef(45, 0,0,1);
		lights2.twinkleLight(0, 10, 5, "GREEN", rotation, GL_LIGHT1, 1); // light on house
	glPopMatrix();

	lights3.twinkleLight(-16, 10, -17, "MAGENTA", rotation, GL_LIGHT4, 0); // light on pond
#pragma endregion LIGHTING
	


	//  pond
	glPushMatrix();
		glTranslatef(-15, -0.18, -20);
		pondReflection(&splinterCube, &rock, &plant);
	glPopMatrix();


	//// terrain
	glPushMatrix();
	glEnable(GL_LIGHTING);
		glTranslatef(0, -0.5, 0);
		glScalef(0.2, 0.2, 0.2);
		g_terrain->Render();
	glPopMatrix();
	

	

#pragma region MODELS
	//// pyramid
	glPushMatrix();
		glScalef(5, 5, 5);
		pyramid.pyramid(3, -0.05, 4, 0);
	glPopMatrix();



	//// cabin 
	//glPushMatrix();
	//	shadow.generateShadowMatrix(shadowMatrix, lightpos2, floorVerts);
	//	glDisable(GL_DEPTH_TEST); 
	//	glDisable(GL_LIGHTING); 
	//	glDisable(GL_TEXTURE_2D);
	//
	//	glColor3f(0.2f, 0.2f, 0.2f); // Shadow colour
	//	glPushMatrix();
	//	    // render shadow, rendering the full cabin is unnecessary as there are too many details which wouldn't change the shape of the shadow
	//		glMultMatrixf((GLfloat*)shadowMatrix);
	//		glTranslatef(0.f, 0.f, 5.f);
	//		glScalef(0.5f, 0.5f, 0.5f);
	//		cabin.cabin_roof_roof.render();
	//		cabin.cabin_stone_base.render();
	//		cabin.cabin_wood_door.render();
	//		cabin.cabin_wood_walls.render();
	//	glPopMatrix();


	//	glColor3f(1.0f, 1.0f, 1.0f); 
	//	glEnable(GL_DEPTH_TEST);
	//	glEnable(GL_LIGHTING);
	//	glEnable(GL_TEXTURE_2D);

		glPushMatrix();
		// render cabin
			glTranslatef(0, 0, 5);
			glScalef(0.5, 0.5, 0.5);
			cabin.cabin_roof_roof.render();
			cabin.cabin_roof_roofplanks.render();
			cabin.cabin_stone_base.render();
			cabin.cabin_wood_crossplanks.render();
			cabin.cabin_wood_door.render();
			cabin.cabin_wood_floor.render();
			cabin.cabin_wood_frame.render();
			cabin.cabin_wood_roofframe.render();
			cabin.cabin_wood_steps.render();
			cabin.cabin_wood_walls.render();
		glPopMatrix();
	//glPopMatrix();





	////  plants
	glPushMatrix();
	    // render imposter shadow
		glDisable(GL_LIGHTING);
		glTranslatef(-17.2, 0.2, -16.2);
		glScalef(0.7, 0.7, 0.7);
		imposter(0.3);
		glEnable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
		glEnable(GL_BLEND);
		glTranslatef(-17, 0.2, -17);
		glScalef(1, 1, 1);
		plant.render();
	glPopMatrix();




	glPushMatrix();
		glTranslatef(-12, -0.1, -20);
		glScalef(1, 1, 1);
		plant.render();
	glPopMatrix();

	
#pragma endregion MODELS 

	//// endscene
	renderTextOutput();
	glutSwapBuffers();
}


//////////    IMPOSTER SHADOW    ////////
void Scene::imposter(float transparecy) {
	//render semi transparent shadow texture on a quad
	glPushMatrix();
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureImposter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glEnable(GL_BLEND);
		glColor4f(0.5, 0.5, 0.5, transparecy);
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glTexCoord2f(0, 0);
		glVertex3f(impVerts[0], impVerts[1], impVerts[2]);
		glTexCoord2f(1, 0);
		glNormal3f(0, 1, 0);
		glVertex3f(impVerts[3], impVerts[4], impVerts[5]);
		glTexCoord2f(1, 1);
		glNormal3f(0, 1, 0);
		glVertex3f(impVerts[9], impVerts[10], impVerts[11]);
		glTexCoord2f(0, 1);
		glNormal3f(0, 1, 0);
		glVertex3f(impVerts[6], impVerts[7], impVerts[8]);
		glEnd();
	glPopMatrix();
	glColor3f(1, 1, 1);
	glEnable(GL_DEPTH_TEST);
};


//////////    POND    ////////
void Scene::pondReflection(Model*model, Model* model2, Model* modelPlant) {
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_QUADS);
	glVertex3f(4.f, 0, 4.f);
	glVertex3f(4.f, 0, -4.f);
	glVertex3f(-4.f, 0, -4.f);
	glVertex3f(-4.f, 0, 4.f);

	glEnd();
	glEnable(GL_DEPTH_TEST);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_TEXTURE_2D);

	// render reflections of models 

	// cool cube
	glPushMatrix();
		glScalef(1, -1, 1);
		glTranslatef(0, 1, 0);
		glScalef(0.6f, 0.6f, 0.6f);
		glRotatef(rotation, 0, 1, 0);
		model->render(); 
	glPopMatrix();

	// rock
	glPushMatrix();
		glScalef(1, -1, 1);
		glTranslatef(2, -0.4, -3);
		glScalef(0.015f, 0.02f, 0.015f);
		model2->render();
	glPopMatrix();


	// plant
	glPushMatrix();
		glScalef(1, -1, 1);
		glTranslatef(3, 0, 0);
		modelPlant->render();
	glPopMatrix();


	glDisable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glColor4f(0.5f, 0.7f, 9.f, 0.3f); // color of the water surface

	glBegin(GL_QUADS);
	glVertex3f(4.f, 0, 4.f);
	glVertex3f(4.f, 0, -4.f);
	glVertex3f(-4.f, 0, -4.f);
	glVertex3f(-4.f, 0, 4.f);
	glEnd();
	glColor3f(1, 1, 1);
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);

	//render models

	// cool cube
	glPushMatrix();
		glTranslatef(0, 1, 0);
		glScalef(0.6f, 0.6f, 0.6f);
		glRotatef(rotation, 0, 1, 0);
		model->render();
	glPopMatrix();


	// rock
	glPushMatrix();
		glTranslatef(2, -0.4, -3);
		glScalef(0.015f, 0.02f, 0.015f);
		model2->render();
		glPopMatrix();
	glPopMatrix();
    
	//*plant is rendered seperately and transformed because of an issue with blending

}


void Scene::initialiseOpenGL(){
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			    
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h){
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

// Calculates FPS
void Scene::calculateFPS(){
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame * 1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput(){
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width / (float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}
