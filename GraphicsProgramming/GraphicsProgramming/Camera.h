#pragma once
//#include "includes.h"
#include "ProjectIncludes.h"
using namespace std;
class Camera {
public:
	Camera(Input* in);
	Input* input;

	void set_position(Vector3 vpos);
	Vector3 get_position();

	void set_lookat(Vector3 vlook);
	Vector3 get_lookat();

	void set_up(Vector3 vup);
	Vector3 get_up();

	void update();
	void handleInput(float dt, float x, float y);

private:


	Vector3 camera_position;
	Vector3 camera_lookat;
	Vector3 camera_up;
	Vector3 camera_right;
	Vector3 camera_forward;
	Vector3 camera_rotation;

	float mousePrison[2]; // to make mouse go back to center screen
	float pitch = 0; // rotation around x-axis
	float yaw = 0; // rotation around y-axis 
	float roll = 0; // rotation around z-axis
	float mouse_y;
	float mouse_x;
	float camSpeed;
	float rotSpeed;
};


