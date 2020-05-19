#include "opengl.h"

/**
	Default constructor for the OpenGLContext class. At this stage it does nothing
	but you can put anything you want here.
*/
OpenGLContext::OpenGLContext(void)
{

}
/**
	Constructor for the OpenGLContext class which will create a context given a windows HWND.
*/
OpenGLContext::OpenGLContext(HWND hwnd)
{
	// Create a context given a HWND
	createContext(hwnd);
}

/**
	Destructor for our OpenGLContext class which will clean up our rendering context
	and release the device context from the current window.
*/
OpenGLContext::~OpenGLContext(void)
{
	// Remove the rendering context from our device context
	wglMakeCurrent(hdc, 0);
	// Delete our rendering context
	wglDeleteContext(hrc);

	// Release the device context from our window
	ReleaseDC(hwnd, hdc);
}

/**
	create30Context creates an OpenGL context and attaches it to the window provided by
	the HWND. This method currently creates an OpenGL 3.2 context by default, but will default
	to an OpenGL 2.1 capable context if the OpenGL 3.2 context cannot be created.
*/
bool OpenGLContext::createContext(HWND hwnd)
{
	// Set the HWND for our window
	this->hwnd = hwnd;

	// Get the device context for our window
	hdc = GetDC(hwnd);

	// Create a new PIXELFORMATDESCRIPTOR (PFD)
	PIXELFORMATDESCRIPTOR pfd;

	// Clear our  PFD
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	// Set the size of the PFD to the size of the class
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);

	// Enable double buffering, opengl support and drawing to a window
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels
	pfd.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)
	pfd.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)
	pfd.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD

	int nPixelFormat = ChoosePixelFormat(hdc, &pfd); // Check if our PFD is valid and get a pixel format back
	if (nPixelFormat == 0) // If it fails
		return false;

	bool bResult = SetPixelFormat(hdc, nPixelFormat, &pfd); // Try and set the pixel format based on our PFD
	if (!bResult) // If it fails
		return false;
	// Create an OpenGL 2.1 context for our device context
	HGLRC tempOpenGLContext = wglCreateContext(hdc);
	// Make the OpenGL 2.1 context current and active
	wglMakeCurrent(hdc, tempOpenGLContext);

	std::cout << "Default OpenGL: " << (char*)glGetString(GL_VERSION) << std::endl;
	// Enable GLEW
	GLenum error = glewInit();
	// If GLEW fails
	if (error != GLEW_OK)
		return false;

	int attributes[] =
	{
		// Set the MAJOR version of OpenGL to 3/4
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		// Set the MINOR version of OpenGL to 2/4
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		// Set our OpenGL context to be forward compatible
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		// If the OpenGL 3.x context creation extension is available
		// Create and OpenGL 3.x context based on the given attributes
		hrc = wglCreateContextAttribsARB(hdc, NULL, attributes);
		// Remove the temporary context from being active
		wglMakeCurrent(NULL, NULL);
		// Delete the temporary OpenGL 2.1 context
		wglDeleteContext(tempOpenGLContext);
		// Make our OpenGL 3.0 context current
		wglMakeCurrent(hdc, hrc);
	}
	else
	{
		// If we didn't have support for OpenGL 3.x and up, use the OpenGL 2.1 context
		hrc = tempOpenGLContext;
	}
	// Set some default values for the version
	int glVersion[2] = { -1, -1 };
	// Get back the OpenGL MAJOR version we are using
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
	// Get back the OpenGL MAJOR version we are using
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

	// Output which version of OpenGL we are using
	std::cout << "Using OpenGL: " << glVersion[0] << "." << glVersion[1] << std::endl;
	std::cout << "Currect OpenGL: " << (char*)glGetString(GL_VERSION) << std::endl;

	return true; // We have successfully created a context, return true
}

/**
	setupScene will contain anything we need to setup before we render
*/
void OpenGLContext::setupScene(void)
{
	// Set the clear color based on Microsofts CornflowerBlue (default in XNA)
	glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
	// Create our shader by loading our vertex and fragment shader
	shader = new Shader("shader.vert", "shader.frag");

	projectionMatrix = glm::perspective(N_PIDIV4, (float)windowWidth / (float)windowHeight, 0.1f, 100.f);  // Create our perspective matrix

	createPoint();
}

/**
	reshapeWindow is called every time our window is resized, and it sets our windowWidth and windowHeight
	so that we can set our viewport size.
*/
void OpenGLContext::reshapeWindow(int w, int h)
{
	// Set the window width
	windowWidth = w;
	// Set the window height
	windowHeight = h;
}

/**
	renderScene will contain all our rendering code.

	The first thing we are going to do is set our viewport size to fill the entire window.

	Next up we are going to clear our COLOR, DEPTH and STENCIL buffers to avoid overlapping
	of renders.

	Any of your other rendering code will go here.

	Finally we are going to swap buffers.
*/
void OpenGLContext::renderScene(void)
{
	glViewport(0, 0, windowWidth, windowHeight); // Set the viewport size to fill the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers

	angle += 0.06f;

	viewMatrix = glm::lookAtRH(glm::vec3(0.0f, 0.0f, -5.f), glm::vec3(0.0f, 0.0f, 0.f), glm::vec3(0.0f, 1.0f, 0.f));
	worldMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(1.f));  // Create our model matrix


	shader->bind(); // Bind our shader

	int projectionMatrixLocation = glGetUniformLocation(shader->id(), "projectionMatrix"); // Get the location of our projection matrix in the shader
	int viewMatrixLocation = glGetUniformLocation(shader->id(), "viewMatrix"); // Get the location of our view matrix in the shader
	int modelMatrixLocation = glGetUniformLocation(shader->id(), "modelMatrix"); // Get the location of our model matrix in the shader

	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]); // Send our model matrix to the shader

	glBindVertexArray(vao[0]); // Bind our Vertex Array Object


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // Draw our square
	//glDrawArrays(GL_POINTS, 0, 4); // Draw our square
	//glDrawArrays(GL_LINES, 0, 2); // Draw our square
	//glDrawArrays(GL_TRIANGLES, 0, 6); // Draw our square

	glBindVertexArray(0); // Unbind our Vertex Array Object

	shader->unbind(); // Unbind our shader

	wglSwapIntervalEXT(1); //Enable VSync

	SwapBuffers(hdc); // Swap buffers so we can see our rendering
}

void OpenGLContext::createPoint()
{
	float *vertices = new float[12]; // Vertices for our square  
	float *colors = new float[12]; // Colors for our vertices 

	vertices[0] = -0.5; vertices[1] = -0.5; vertices[2] = 0.0; // Bottom left corner  
	colors[0] = 1.0; colors[1] = 1.0; colors[2] = 1.0; // Bottom left corner  

	vertices[3] = -0.5; vertices[4] = 0.5; vertices[5] = 0.0; // Top left corner  
	colors[3] = 1.0; colors[4] = 0.0; colors[5] = 0.0; // Top left corner  

	vertices[6] = 0.5; vertices[7] = 0.5; vertices[8] = 0.0; // Top Right corner  
	colors[6] = 0.0; colors[7] = 1.0; colors[8] = 0.0; // Top Right corner  

	vertices[9] = 0.5; vertices[10] = -0.5; vertices[11] = 0.0; // Bottom right corner  
	colors[9] = 0.0; colors[10] = 0.0; colors[11] = 1.0; // Bottom right corner  

	//vertices[12] = -0.5; vertices[13] = -0.5; vertices[14] = 0.0; // Bottom left corner  
	//colors[12] = 1.0; colors[13] = 1.0; colors[14] = 1.0; // Bottom left corner  

	//vertices[15] = 0.5; vertices[16] = 0.5; vertices[17] = 0.0; // Top Right corner  
	//colors[15] = 0.0; colors[16] = 1.0; colors[17] = 0.0; // Top Right corner  

	glGenVertexArrays(1, vao); // Create our Vertex Array Object

	glBindVertexArray(vao[0]); // Bind our Vertex Array Object so we can use it

	glGenBuffers(2, vbo); // Generate our Vertex Buffer Object

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer

	glEnableVertexAttribArray(0); // Disable our Vertex Array Object

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // Bind our second Vertex Buffer Object  

	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW  

	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer  

	glEnableVertexAttribArray(1); // Enable the second vertex attribute array 

	glBindVertexArray(0); // Disable our Vertex Buffer Object

	delete[] colors; // Delete our vertices from memory  
	delete[] vertices; // Delete our vertices from memory
}