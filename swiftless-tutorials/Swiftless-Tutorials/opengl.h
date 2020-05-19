#pragma once
#include "shader.h"

constexpr float N_PI = 3.141592654f;
constexpr float N_2PI = 6.283185307f;
constexpr float N_1DIVPI = 0.318309886f;
constexpr float N_1DIV2PI = 0.159154943f;
constexpr float N_PIDIV2 = 1.570796327f;
constexpr float N_PIDIV4 = 0.785398163f;

/**
	OpenGLContext is a class designed to store all of your OpenGL functions and keep them
	out of the way of your application logic. Here we have the ability to create an OpenGL
	context on a given window and then render to that window.
*/
class OpenGLContext
{
public:
	OpenGLContext(void); // Default constructor
	OpenGLContext(HWND hwnd); // Constructor for creating our context given a hwnd
	~OpenGLContext(void); // Destructor for cleaning up our application
	bool createContext(HWND hwnd); // Creation of our OpenGL 3.x context
	void setupScene(void); // All scene information can be setup here
	void reshapeWindow(int w, int h); // Method to get our window width and height on resize
	void renderScene(void); // Render scene (display method from previous OpenGL tutorials)

private:
	void createPoint();

	GLuint vao[1];
	GLuint vbo[2];

	int windowWidth; // Store the width of our window
	int windowHeight; // Store the height of our window

	Shader *shader; // Our GLSL shader
	float angle = 0;
	glm::mat4 projectionMatrix; // Store the projection matrix
	glm::mat4 viewMatrix; // Store the view matrix
	glm::mat4 worldMatrix; // Store the model matrix

protected:
	HGLRC hrc; // Rendering context
	HDC hdc; // Device context
	HWND hwnd; // Window identifier
};