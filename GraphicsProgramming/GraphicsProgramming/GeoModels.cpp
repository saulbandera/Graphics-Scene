#include "GeoModels.h"





GeoModels::GeoModels() {
	// load pyramid texture
	textureStone = SOIL_load_OGL_texture(
		"gfx/sandstone1.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y // Depending on texture file type some need inverted others don't.
	);

	texturePlane= SOIL_load_OGL_texture(
		"Data/Textures/sand3.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y // Depending on texture file type some need inverted others don't.
	);

}

// render a plane made up of quads
void GeoModels::plane(float x, float y, float z, float width, float length) {
	glPushMatrix();
		glTranslatef(x, y, z);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texturePlane);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBegin(GL_QUADS);
		glColor3f(0.5, 0.5, 0.5); //grey
		for (float z = 0; z <= width; z += 0.05) {
			for (float x = 0; x <= length; x += 0.05f) {
				glNormal3f(0, 1, 0);

				glTexCoord2f(0, 0);
				glVertex3f(x, -0.5, z);

				glTexCoord2f(0, 1);
				glVertex3f(x, -0.5, z - 0.05);

				glTexCoord2f(1, 0);
				glVertex3f(x - 0.05, -0.5, z - 0.05);

				glTexCoord2f(1, 1);
				glVertex3f(x - 0.05, -0.5, z);
			}
		}
		glEnd();
	glPopMatrix();
	glColor3f(1, 1, 1); //reset to white
}

//render a colored cube
void GeoModels::cube(float x, float y, float z, float rotation) {
	//glRotatef(rotation, 0, 1, 0);
	glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(rotation * 5, 0, 1, 0);
		glBegin(GL_QUADS);

		// back face
		glNormal3f(0, 0, 1);
		glColor3f(1.0f, 0.5f, 0.5f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glColor3f(1.f, 0.f, 1.1f);
		glVertex3f(0.5f, 0.5f, 0.5f);
		glColor3f(1.f, 0.7f, 0.f);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glColor3f(0.5f, 1.f, 1.1f);
		glVertex3f(-0.5f, -0.5f, 0.5f);


		// right face
		glNormal3f(1, 0, 0);
		glColor3f(1.f, 0.f, 1.1f);
		glVertex3f(0.5f, 0.5f, 0.5f);
		glColor3f(0.5f, 1.f, 1.1f);
		glVertex3f(0.5f, 0.5f, -0.5f);
		glColor3f(1.0f, 0.5f, 0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glColor3f(1.f, 0.7f, 0.f);
		glVertex3f(0.5f, -0.5f, 0.5f);

		// bottom face
		glNormal3f(0, -1, 0);
		glColor3f(1.f, 0.7f, 0.f);
		glVertex3f(0.5f, -0.5f, 0.5f);
		glColor3f(0.5f, 1.f, 1.1f);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glColor3f(1.0f, 0.5f, 0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glColor3f(1.f, 0.7f, 0.f);
		glVertex3f(-0.5f, -0.5f, 0.5f);

		//top face
		glNormal3f(0, 1, 0);
		glColor3f(1.f, 0.f, 1.1f);
		glVertex3f(0.5f, 0.5f, 0.5f);
		glColor3f(1.f, 0.7f, 0.f);
		glVertex3f(0.5f, 0.5f, -0.5f);
		glColor3f(1.f, 0.f, 1.1f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
		glColor3f(1.f, 0.7f, 0.f);
		glVertex3f(-0.5f, 0.5f, 0.5f);

		//left face
		glNormal3f(-1, 0, 0);
		glColor3f(0.5f, 1.f, 1.1f);
		glVertex3f(-0.5f, 0.5f, 0.5f);
		glColor3f(0.5f, 1.f, 1.1f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
		glColor3f(1.0f, 0.5f, 0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glColor3f(1.f, 0.f, 1.1f);
		glVertex3f(-0.5f, -0.5f, 0.5f);

		//front face
		glNormal3f(0,0,-1);
		glColor3f(0.5f, 1.f, 1.1f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
		glColor3f(1.f, 0.f, 1.1f);
		glVertex3f(0.5f, 0.5f, -0.5f);
		glColor3f(1.f, 0.f, 1.1f);
		glVertex3f(0.5f, -0.5f, -0.5f);
		glColor3f(0.5f, 1.f, 1.1f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glEnd();
	glPopMatrix();
	glColor3f(1, 1, 1);

}


// render a pyramid using vertex arrays
void GeoModels::pyramid(float x, float y, float z, float rotation) { 
	glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(rotation, 0, 1, 0);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, pyrVerts);
		glNormalPointer(GL_FLOAT, 0, pyrNormals);
		glTexCoordPointer(2, GL_FLOAT, 0, pyrTexcoords);

		glBindTexture(GL_TEXTURE_2D, textureStone);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glDrawArrays(GL_TRIANGLES, 0, 12);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
	


}



