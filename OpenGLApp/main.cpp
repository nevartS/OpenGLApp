#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "GLWindow.h"

// For converting the actual angles to the radians
const float toRadians = 3.14159265f / 180.0f; // Pi / 180 degrees

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

GLWindow mainWindow;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";				

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
	unsigned int indices[] =
	{
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader *shader_01 = new Shader();
	shader_01->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader_01);
}

int main()
{
	mainWindow = GLWindow(800, 600);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;

	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeigth(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Get and Handle user input events
		glfwPollEvents();

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();

		glm::mat4 model_01 = glm::mat4(1.0f);		

		model_01 = glm::translate(model_01, glm::vec3(0.0f, 0.0f, -2.5f)); // Translate(move) the triangle							
		//model01 = glm::rotate(model01, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate the triangle		
		model_01 = glm::scale(model_01, glm::vec3(0.4f, 0.4f, 1.0f)); // Scale the triangle
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model_01));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		
		meshList[0]->RenderMesh();

		glm::mat4 model_02 = glm::mat4(1.0f);	

		model_02 = glm::translate(model_02, glm::vec3(0.0f, 1.0f, -2.5f));
		model_02 = glm::scale(model_02, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model_02));
		
		meshList[1]->RenderMesh();
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}	

	return 0;
}