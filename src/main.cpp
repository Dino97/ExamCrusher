#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Game.h"
#include "Resources.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

Game game(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char** argv)
{
	glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ExamCrusher", 0, 0);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	game.State = GAME_MENU;

	game.Init();

	glfwSwapInterval(1);

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while(!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//std::cout << "FPS: " << 1 / deltaTime << std::endl;

		glfwPollEvents();

		game.ProcessInput(deltaTime);
		game.Update(deltaTime);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		game.Render();

		if(game.shouldClose == true)
			glfwSetWindowShouldClose(window, GL_TRUE);


		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if(key >= 0 && key < 1024)
	{
		if(action == GLFW_PRESS)
			game.Keys[key] = GL_TRUE;
		if(action == GLFW_RELEASE)
		{
			game.Keys[key] = GL_FALSE;
			game.buttonDown = GL_FALSE;
		}
	}
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	game.MouseX = xpos;
	game.MouseY = ypos;
}
