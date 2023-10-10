#include "Skybox.h"
Skybox::Skybox(Camera* cam) {
	camera = cam;
	//import skybox texture image
	skydome.load("models/largeSphere.obj", "gfx/stars.png");
}

void Skybox::renderSkybox() {
	glPushMatrix();
	glTranslatef(camera->get_position().x, camera->get_position().y, camera->get_position().z); // translate to camera position
	glDisable(GL_DEPTH_TEST); // disable depth test so skybox always appears behind other geometry
	glEnable(GL_TEXTURE_2D); // enable texturing
	skydome.render();
	glEnd();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D); 
	glPopMatrix();

}


















///////////render skybox code

//// back face
    //glBegin(GL_QUADS); // skybox cube
	////glColor4f(0.f, 0.f, 1.f, 1.f); ////BLUE
	//glTexCoord2f(1, 0.25);
	//glVertex3f(-0.5f, 0.5f, 0.5f);
	//glTexCoord2f(0.75, 0.25);
	//glVertex3f(0.5f, 0.5f, 0.5f);
	//glTexCoord2f(0.75, 0.5);
	//glVertex3f(0.5f, -0.5f, 0.5f);
	//glTexCoord2f(1, 0.5);
	//glVertex3f(-0.5f, -0.5f, 0.5f);

	//// right face
	////glColor4f(1.f, 0.f, 0.f, 1.f); ////RED
	//glTexCoord2f(0.75, 0.25);
	//glVertex3f(0.5f, 0.5f, 0.5f);
	//glTexCoord2f(0.5, 0.25);
	//glVertex3f(0.5f, 0.5f, -0.5f);
	//glTexCoord2f(0.5, 0.5);
	//glVertex3f(0.5f, -0.5f, -0.5f);
	//glTexCoord2f(0.75, 0.5);
	//glVertex3f(0.5f, -0.5f, 0.5f);

	//// bottom face
	////glColor4f(1.f, 0.f, 1.f, 1); ////PINK 

	//glTexCoord2f(0.25, 0.75);
	//glVertex3f(0.5f, -0.5f, 0.5f);
	//glTexCoord2f(0.25, 0.5);
	//glVertex3f(0.5f, -0.5f, -0.5f);
	//glTexCoord2f(0.5, 0.5);
	//glVertex3f(-0.5f, -0.5f, -0.5f);
	//glTexCoord2f(0.5, 0.75);
	//glVertex3f(-0.5f, -0.5f, 0.5f);

	////top face
	////glColor4f(0.9f, 1.f, 0.2f, 1.f); ////YELLOW/LIME 
	//glTexCoord2f(0.5, 0);
	//glVertex3f(0.5f, 0.5f, 0.5f);
	//glTexCoord2f(0.5, 0.25);
	//glVertex3f(0.5f, 0.5f, -0.5f);
	//glTexCoord2f(0.25, 0.25);
	//glVertex3f(-0.5f, 0.5f, -0.5f);
	//glTexCoord2f(0.25, 0);
	//glVertex3f(-0.5f, 0.5f, 0.5f);

	////left face
	////glColor4f(0.f, 1.f, 0.f, 1.f); ////GREEN
	//glTexCoord2f(0, 0.25);
	//glVertex3f(-0.5f, 0.5f, 0.5f);
	//glTexCoord2f(0.25, 0.25);
	//glVertex3f(-0.5f, 0.5f, -0.5f);
	//glTexCoord2f(0.25, 0.5);
	//glVertex3f(-0.5f, -0.5f, -0.5f);
	//glTexCoord2f(0, 0.5);
	//glVertex3f(-0.5f, -0.5f, 0.5f);

	////front face
	////glColor4f(1.f, 0.5f, 0.f, 1.f); ////ORANGE
	//glTexCoord2f(0.25, 0.25);
	//glVertex3f(-0.5f, 0.5f, -0.5f);
	//glTexCoord2f(0.5, 0.25);
	//glVertex3f(0.5f, 0.5f, -0.5f);
	//glTexCoord2f(0.5, 0.5);
	//glVertex3f(0.5f, -0.5f, -0.5f);
	//glTexCoord2f(0.25, 0.5);
	//glVertex3f(-0.5f, -0.5f, -0.5f);
