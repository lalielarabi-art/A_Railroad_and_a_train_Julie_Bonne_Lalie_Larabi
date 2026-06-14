#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "draw_scene.hpp"
#include "tools/shaders.hpp"
#include <json.hpp>
#include <iostream>


float camX;
float camY =-50.0f;
float camZ = 25.0f ;
float camAngle = M_PI/2.0f;


using namespace glbasimac;
using namespace STP3D;

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1200;
static const unsigned int WINDOW_HEIGHT = 800;
static const char WINDOW_TITLE[] = "TD04 Ex01";
static float aspectRatio = 1.0f;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* Error handling function */
void onError(int error, const char* description) {
	std::cout << "GLFW Error ("<<error<<") : " << description << std::endl;
}

void onWindowResized(GLFWwindow* /*window*/, int width, int height)
{
	glViewport(0, 0, width, height);
    std::cerr<<"Setting 3D projection"<<std::endl;

    myEngine.set3DProjection(90.0, aspectRatio, Z_NEAR, Z_FAR);
}

void onKey(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	int is_pressed = (action == GLFW_PRESS); 
	switch(key) {
		case GLFW_KEY_ESCAPE :
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_L:
			if (is_pressed) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			break;
		case GLFW_KEY_P:
			if (is_pressed) glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			break;
		case GLFW_KEY_I:
            if (is_pressed) {
                handleKeyboardInput('l'); 
            }
            break;
		default: std::cerr<<"Touche non gérée "<<key<<std::endl;
		case GLFW_KEY_D:
			if (is_pressed) handleKeyboardInput('d');
		break;
		case GLFW_KEY_UP :
			camX += cos(camAngle) * 0.5f;
			camY += sin(camAngle) * 0.5f;
		break;
		case GLFW_KEY_DOWN :
			camX -= cos(camAngle) * 0.5f;
			camY -= sin(camAngle) * 0.5f;
			break;
		case GLFW_KEY_LEFT :
			camAngle += 0.05f;
			break;
		case GLFW_KEY_RIGHT :
			camAngle -= 0.05f;
			break;
			
	}

}

void onMouseButton(GLFWwindow* window, int button, int action, int /*mods*/)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout<<"Pressed in "<<xpos<<" "<<ypos<<std::endl;

	}
}

int main(int argc, char** argv)
{
	if (argc < 2) {
        std::cerr << "Erreur : Vous devez specifier le fichier JSON." << std::endl;
        std::cerr << "Usage : " << argv[0] << "circuit.json" << std::endl;
        return -1;
    }

	/* GLFW initialisation */
	GLFWwindow* window;
	if (!glfwInit()) return -1;

    /* Try to uncomment this for MAC OS if it did not work */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	std::cout<<"Loading GL extension"<<std::endl;
	// Intialize glad (loads the OpenGL functions)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return -1;
	}

	glfwSetWindowSizeCallback(window,onWindowResized);
	glfwSetKeyCallback(window, onKey);
	glfwSetMouseButtonCallback(window, onMouseButton);

	std::cout<<"Engine init"<<std::endl;
	

	myEngine.mode2D = false; // Set engine to 3D mode
	
    myEngine.initGL();
    onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    CHECK_GL;

    
    loadCircuitFromJSON(argv[1]);


	initScene();
	drawFrame();
	double elapsedTime{0.0};
	double lastFrameTime = glfwGetTime();
	

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		double currentFrameTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
        lastFrameTime = currentFrameTime;

		updateScene(deltaTime);


		glClearColor(0.5f,0.7f,1.0f,1.0f);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		

	

		/* Fix camera position */
		myEngine.mvMatrixStack.loadIdentity();
		Vector3D pos_camera{camX, camY, camZ};
		Vector3D viewed_point{camX + cos(camAngle), camY + sin(camAngle), camZ};
Vector3D up_vector{0.0, 0.0, 1.0};
		Matrix4D viewMatrix = Matrix4D::lookAt(pos_camera,viewed_point,up_vector);
		myEngine.setViewMatrix(viewMatrix);
		myEngine.updateMvMatrix();
	
		drawScene();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		while(elapsedTime < FRAMERATE_IN_SECONDS)
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
			elapsedTime = glfwGetTime() - startTime;
		}
	}

	glfwTerminate();
}
