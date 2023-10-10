#include "Camera.h"

Camera::Camera(Input* in) {
	// initialising values needed
	input = in;
	camera_position.set(0, 0, 6);
	camera_rotation.set(0, 0, 0);
	camSpeed = 20.f;
	rotSpeed = 80.f;
}


////// GETTERS AND SETTERS //////
Vector3 Camera::get_position() {
	return camera_position;
}
void Camera::set_position(Vector3 vpos) {
	camera_position = vpos;
}
Vector3 Camera::get_lookat() {
	return camera_lookat;
}
void Camera::set_lookat(Vector3 vlook) {
	camera_lookat = vlook;
}

Vector3 Camera::get_up() {
	return camera_up;
}
void Camera::set_up(Vector3 vup) {
	camera_up = vup;
}


////// UPDATE //////
void Camera::update() {
	float cosR, cosP, cosY;
	float sinR, sinP, sinY;
	cosY = cosf(yaw * 3.1415 / 180);
	cosP = cosf(pitch * 3.1415 / 180);
	cosR = cosf(roll * 3.1415 / 180);
	sinY = sinf(yaw * 3.1415 / 180);
	sinP = sinf(pitch * 3.1415 / 180);
	sinR = sinf(roll * 3.1415 / 180);
	// forward vector
	camera_forward.x = sinY * cosP;
	camera_forward.y = sinP;
	camera_forward.z = cosP * -cosY;

	// look at vector
	camera_lookat = camera_position + camera_forward;

	// up vector
	camera_up.x = -cosY * sinR - sinY * sinP * cosR;
	camera_up.y = cosP * cosR;
	camera_up.z = -sinY * sinR - sinP * cosR * -cosY;

	// right vector
	camera_right = camera_forward.cross(camera_up);
}

////// HANDLE INPUT //////
void Camera::handleInput(float dt, float x, float y) {
	// forwards movement
	if (input->isKeyDown('w')) {
		camera_position.add(camera_forward, camSpeed * dt);
	}
	// backwards movement
	if (input->isKeyDown('s')) {
		camera_position.add(camera_forward, -camSpeed * dt);
	}




	// left movement
	if (input->isKeyDown('a')) {
		camera_position.add(camera_right, -camSpeed * dt);
	}
	// right movement
	if (input->isKeyDown('d')) {
		camera_position.add(camera_right, camSpeed * dt);
	}






	// up movement
	if (input->isKeyDown('r')) {
		camera_position.add(camera_up, camSpeed * dt);
	}
	// down movement
	if (input->isKeyDown('f')) {
		camera_position.add(camera_up, -camSpeed * dt);
	}
	// x-axis rotation
	if (input->isKeyDown('u')) {
		pitch += rotSpeed * dt;
	}
	if (input->isKeyDown('j')) {
		pitch -= rotSpeed * dt;
	}
	// y-axis rotation
	if (input->isKeyDown('y')) {
		yaw += rotSpeed * dt;
	}
	if (input->isKeyDown('h')) {
		yaw -= rotSpeed * dt;
	}
	// z-axis rotation
	if (input->isKeyDown('t')) {
		roll += rotSpeed * dt;
	}
	if (input->isKeyDown('g')) {
		roll -= rotSpeed * dt;
	}
	// mouse movement 

	mouse_x = input->getMouseX();
	mouse_y = input->getMouseY();
	// keep the mouse in the center of the window
	mousePrison[0] = (x / 2);
	mousePrison[1] = (y / 2);
	yaw -= -(mouse_x - mousePrison[0]) * dt * 1.f;
	pitch += -(mouse_y - mousePrison[1]) * dt * 1.f;
	glutWarpPointer(mousePrison[0], mousePrison[1]);

}