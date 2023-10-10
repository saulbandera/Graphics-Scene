// Main entry point for application. 
// Initialises main window, captures user input and passes onto appropriate class for handling.
// Utilises FreeGLUT API for window.
// Initialises Scene and Input class.
// @author Paul Robertson


// Include glut, opengl libraries and custom classes
#include "ProjectIncludes.h"
#include "Scene.h"
#include "Input.h"
#include "Camera.h"
#include "Model.h"
#include "Skybox.h"
// Required variables; pointer to scene and input objects. Initialise variable used in delta time calculation.
Scene* scene;
Input* input;
Camera* camera;
int oldTimeSinceStart = 0;

// Called when the window detects a change in size.
// GLUT handles the window refresh, this function passes the new width and height to the
// scene object to refresh the OpenGL buffers to the new dimensions.
void changeSize(int w, int h) { 
	scene->resize(w, h);
}

// Called as part of the GLUT main loop.
// Calculates delta time (the time between frames, in seconds)
// Calls Scene update and render for the next frame.
void renderScene(void){  
	// Calculate delta time.
	// Update Scene and render next frame.
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (float)timeSinceStart - (float)oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;
	deltaTime = deltaTime / 1000.0f;
	scene->handleInput(camera, deltaTime);
	scene->update(camera, deltaTime);
	scene->render(camera);
}

// Handles keyboard input events from GLUT.
// Called whenever a "normal" key is pressed.
// Normal keys are defined as any key not including the F keys, CTRL, SHIFT, ALT, etc.
// Key press is recorded in Input class 
// Parameters include key pressed and current mouse x, y coordinates.
// Mouse coordinates are handled separately.
void processNormalKeys(unsigned char key, int x, int y)
{
	// If the ESCAPE key was pressed, exit application.
	if (key == VK_ESCAPE)	// Escape key (in non-windows you can use 27, the ASCII value for escape)
		exit(0);
	// Send key down to input class.
	input->setKeyDown(key);
}

// Handles keyboard input events from GLUT.
// Called whenever a "normal" key is released.
// Normal keys are defined as any key not including the F keys, CTRL, SHIFT, ALT, arrow keys, etc.
// Key press is recorded in Input class 
// Parameters include key pressed and current mouse x, y coordinates.
// Mouse coordinates are handled separately.
void processNormalKeysUp(unsigned char key, int x, int y)
{
	// Send key up to input class.
	input->setKeyUp(key);
}

// Handles keyboard input events from GLUT.
// Called whenever a "special" key is pressed.
// Special keys are defined as F keys, CTRL, SHIFT, ALT, arrow keys, etc
// Currently a place holder function, can be utilised if required.
// Parameters include key pressed and current mouse x, y coordinates.
// Mouse coordinates are handled separately.
void processSpecialKeys(int key, int x, int y)
{
	// TODO: Pass special key press to Input class.
}

// Handles keyboard input events from GLUT.
// Called whenever a "special" key is released.
// Special keys are defined as F keys, CTRL, SHIFT, ALT, arrow keys, etc
// Currently a place holder function, can be utilised if required.
// Parameters include key pressed and current mouse x, y coordinates.
// Mouse coordinates are handled separately.
void processSpecialKeysUp(int key, int x, int y)
{
	// TODO: Pass special key release to Input class.
}

// Handles mouse movement events from GLUT.
// Active movement is define as mouse movement while a mouse button is pressed.
// Called every loop. Parameters are the new x, y coordinates of the mouse.
void processActiveMouseMove(int x, int y)
{
	// Record mouse position in Input class.
	input->setMousePos(x, y);
}

// Handles mouse movement events from GLUT.
// Passive mouse movement is define as mouse movement without a mouse button is pressed.
// Called every loop. Parameters are the new x, y coordinates of the mouse.
void processPassiveMouseMove(int x, int y)
{
	// Record mouse position in Input class.
	input->setMousePos(x, y);
}

// Handles mouse button events from GLUT.
// Parameters include mouse button that fired the event (GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON),
// button state (up and down), and current cursor position.
void processMouseButtons(int button, int state, int x, int y)
{
	// Detect left button press/released
	if (button == GLUT_LEFT_BUTTON) {
		// when the button is released
		if (state == GLUT_DOWN) {
			input->setMouseLDown(true);
		}
		// else button state == GLUT_UP
		else  {
			input->setMouseLDown(false);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		// when the button is released
		if (state == GLUT_DOWN) {
			input->setMouseRDown(true);
		}
		// else button state == GLUT_UP
		else {
			input->setMouseRDown(false);
		}
	}
}


// Main entery point for application.
// Initialises GLUT and application window.
// Registers callback functions for handling GLUT input events
// Registers callback functions for window resizing and rendering.
// Initialises Input and Scene class, prior to starting Main Loop.
int main(int argc, char **argv)   //////////   MAIN   /////////
{
	// Init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	//glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("3D scene");
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initilalize GLEW!" << std::endl;
		exit(-1);
	}
	// Register callback functions for change in size and rendering.
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);



	glutKeyboardFunc(processNormalKeys);
	glutKeyboardUpFunc(processNormalKeysUp);
	glutSpecialFunc(NULL);	
	glutMotionFunc(processActiveMouseMove);
	glutPassiveMotionFunc(processPassiveMouseMove);
	glutMouseFunc(processMouseButtons);
	// Position mouse in centre of windows before main loop (window not resized yet)
	glutWarpPointer(400, 300);
	// Hide mouse cursor
	glutSetCursor(GLUT_CURSOR_NONE);
	// Initialise input and scene objects.
	input = new Input();
	camera = new Camera(input);
	scene = new Scene(input, camera);
	// Enter GLUT event processing cycle
	glutMainLoop();
	return 1;
}




