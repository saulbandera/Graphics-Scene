#include "Lighting.h"
Lighting::Lighting() {
	//load models for lights
	sphereBase1.load("models/largeSphere.obj", "gfx/spotbulb.png");
	sphere.load("models/largeSphere.obj", "gfx/glass.png");
	sphere1.load("models/largeSphere.obj", "gfx/glass.png");
	sphere2.load("models/largeSphere.obj", "gfx/glass.png");
}

Vector3 Lighting::selectColor(string color) { //color based on function input
	switch (hash<std::string>{}(color)) {
	case 2211354620: //"RED"
		retColorVector.set(1.f, 0.f, 0.f);
		return retColorVector;

	case 2875364188: //"GREEN"
		retColorVector.set(0.f, 1.f, 0.f);
		return retColorVector;

	case 750204685: //"BLUE"
		retColorVector.set(0.f, 0.f, 1.f);
		return retColorVector;

	case 2856149510: //"WHITE"
		retColorVector.set(0.7f, 0.7f, 0.7f);
		return retColorVector;

	case 2823601493: //"WARMWHITE"
		retColorVector.set(0.1f, 0.1f, 0.1f);
		return retColorVector;

	case 408039608: //"COLDWHITE"
		retColorVector.set(0.7f, 0.7f, 1.f);
		return retColorVector;

	case 1444046984: //"MAGENTA"
		retColorVector.set(1.f, 0.0f, 1.f);
		return retColorVector;

	case 1196590906: //"CYAN "
		retColorVector.set(0.f, 0.7f, 1.0f);
		return retColorVector;
	}
}

// sphere to mark the position of the camera in the scene
void Lighting::lightBulb(float x, float y, float z) { 
	glTranslatef(x, y, z);
	glDisable(GL_LIGHTING);
	glColor3f(1, 1, 1);
	gluSphere(gluNewQuadric(), 0.20, 20, 20);
	glEnable(GL_LIGHTING);
}

// spot light with cutoff
void Lighting::spotLt(float x, float y, float z, string color, GLenum lightnum, float tilt) { 
	colorVector = selectColor(color);
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glColor3f(colorVector.x, colorVector.y, colorVector.z);
		glTranslatef(x,y,z);
		glScalef(0.09, 0.09, 0.09);
		sphereBase1.render();
		glEnable(GL_LIGHTING);

		GLfloat Light_Ambient[] = { 0.7f, 0.7f, 0.7f, 1.f };
		GLfloat Light_Diffuse[] = { colorVector.x,colorVector.y,colorVector.z, 1.f };
		GLfloat Light_Position[] = { x, y, z, 1.0f };
		GLfloat spot_Direction[] = { 0, -1.0f, 0.0f };
		glLightfv(lightnum, GL_AMBIENT, Light_Ambient);
		glLightfv(lightnum, GL_DIFFUSE, Light_Diffuse);
		glLightfv(lightnum, GL_POSITION, Light_Position);
		glLightf(lightnum, GL_SPOT_CUTOFF, 50.f);
		glLightfv(lightnum, GL_SPOT_DIRECTION, spot_Direction);
		glLightf(lightnum, GL_SPOT_EXPONENT, 30.0);
		glEnable(lightnum);
	glPopMatrix();
	glColor3f(1, 1, 1);
}

// sun light 
void Lighting::directionalLt(float x, float y, float z, string color) { 
	colorVector = selectColor(color);
	glPushMatrix();
		GLfloat Light_Ambient[] = { 0.2f, 0.2f, 0.2f, 1.f };
		GLfloat Light_Diffuse[] = { colorVector.x,colorVector.y,colorVector.z, 1.0f };
		GLfloat Light_Position[] = { x, y, z, 0.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Diffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, Light_Position);
		glEnable(GL_LIGHT0);
	glPopMatrix();
}

// a light that iluminates in every direction
void Lighting::pointLt(float x, float y, float z, string color) {  
	colorVector = selectColor(color);
	glPushMatrix();
		GLfloat Light_Ambient[] = { 0.4f, 0.4f, 0.4f, 0.4f };
		GLfloat Light_Diffuse[] = { colorVector.x,colorVector.y,colorVector.z, 1.0f };
		GLfloat Light_Position[] = { x, y, z, 1.0f };
		glLightfv(GL_LIGHT1, GL_AMBIENT, Light_Ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, Light_Diffuse);
		glLightfv(GL_LIGHT1, GL_POSITION, Light_Position);
		glEnable(GL_LIGHT1);
		lightBulb(x, y, z);
	glPopMatrix();

}

void Lighting::ambientLt(float x, float y, float z, string color) { 
	colorVector = selectColor(color);
	glPushMatrix();
		GLfloat Light_Ambient[] = { colorVector.x,colorVector.y,colorVector.z, 0.4f };
		GLfloat Light_Position[] = { x, y, z, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Ambient);
		glEnable(GL_LIGHT0);
	glPopMatrix();
}

// spotlight with 3 spheres rotating around it
void Lighting::twinkleLight(float x, float y, float z, string color, float r, GLenum lightn, float tilt) { 
	colorVector = selectColor(color);

	glPushMatrix();
		spotLt(x, y, z, color, lightn, tilt);

		glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glColor3f(colorVector.x, colorVector.y, colorVector.z);
			glTranslatef(x, y, z);
			glScalef(0.09, 0.09, 0.09);
			sphereBase1.render(); // render sphere where light is
			glEnable(GL_LIGHTING);
			glColor3f(1.f, 1.f, 1.f);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(x, y, z);

			glPushMatrix(); 
				glRotatef(r*1.2, 0, 1, 0);
				glTranslatef(-1, 0, 0);
				glScalef(0.05, 0.05, 0.05);

				glPushMatrix();
					glRotatef(r*5, 0, 1, 0);
					sphere.render(); //sphere that rotates around the light sphere on x axis
				glPopMatrix();

				glPushMatrix();
					glRotatef(r * 2, 0, 1, 0);
					glTranslatef(7, 0, 0);
					glScalef(0.7, 0.7, 0.7);
					sphere1.render(); // sphere that rotates around the previous sphere on x axis

					glPushMatrix();
						glRotatef(r * 2, 1, 1, 0);
						glTranslatef(10, 0, 0);
						glScalef(0.6, 0.6, 0.6);
						sphere2.render(); //smaller sphere rotating around previous sphere on x and y axis (up and down and rotating)
					glPopMatrix(); 

				glPopMatrix();

			glPopMatrix();

		glPopMatrix(); 

	glPopMatrix();
}