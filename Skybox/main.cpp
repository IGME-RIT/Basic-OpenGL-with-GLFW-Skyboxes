﻿/*
Title: Sky Boxes
File Name: main.cpp
Copyright � 2016, 2019
Author: David Erbelding, Niko Procopi
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "FreeImage.h"
#include <vector>
#include "mesh.h"
#include "fpsController.h"
#include "transform3d.h"
#include "shader.h"
#include <iostream>

Mesh* cube;

// The transform being used to draw our shape
Transform3D transform;

// These shader objects wrap the functionality of loading and compiling shaders from files.
Shader vertexShader;
Shader fragmentShader;
GLuint shaderProgram;
// Index of the world matrix in the vertex shader.
GLuint worldMatrixUniform;
// Index of the camera matrix in the vertex shader.
GLuint viewMatrixUniform;
// Texture uniform and id
GLuint textureUniform;
GLuint texture;


// Shaders used to render the skybox
Shader skyBoxVertexShader;
Shader skyBoxFragmentShader;
GLuint skyboxProgram;
// Index of skybox uniform
GLuint skyboxMatrixUniform;
// Cube Map uniform and id
GLuint cubeMapUniform;
GLuint cubeMap;


// Store the current dimensions of the viewport.
glm::vec2 viewportDimensions = glm::vec2(800, 600);
glm::vec2 mousePosition = glm::vec2();

FPSController controller = FPSController();

// Window resize callback
void resizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
    viewportDimensions = glm::vec2(width, height);
}

// This will get called when the mouse moves.
void mouseMoveCallback(GLFWwindow *window, GLdouble mouseX, GLdouble mouseY)
{
    mousePosition = glm::vec2(mouseX, mouseY);
}


int main(int argc, char **argv)
{
	// Initializes the GLFW library
	glfwInit();

	// set value for MSAA
	glfwWindowHint(GLFW_SAMPLES, 99);

	// Initialize window
	GLFWwindow* window = glfwCreateWindow(viewportDimensions.x, viewportDimensions.y, "I can see my house from here.", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	//set resize callback
	glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);

	// Initializes the glew library
	glewInit();


	// At first glance, a cube may appear simple...
    //    [C]------[G]
	// [D]------[H] |
	//	|  |     |  |
	//	|  |     |  |
	//	| [A]----|-[E]
	// [B]------[F]


	std::vector<Vertex3dUV> vertices;
    
    // Here we need to create all of the vertices of our cube.
    // Any vertices that have different texture coordinates need to be repeated with them.
    // The vertices below are organized by face.
    // The texture coordinates are using partial squares from the texture to avoid obvious repeating.

    // ABCD
	vertices.push_back(Vertex3dUV(glm::vec3(-1, -1, -1), glm::vec2(0, 0)));
    vertices.push_back(Vertex3dUV(glm::vec3(-1, -1, 1), glm::vec2(0, .5)));
    vertices.push_back(Vertex3dUV(glm::vec3(-1, 1, -1), glm::vec2(.5, 0)));
    vertices.push_back(Vertex3dUV(glm::vec3(-1, 1, 1), glm::vec2(.5, .5)));
    // EFGH
    vertices.push_back(Vertex3dUV(glm::vec3(1, -1, -1), glm::vec2(.5, 0)));
    vertices.push_back(Vertex3dUV(glm::vec3(1, 1, -1), glm::vec2(1, 0)));
    vertices.push_back(Vertex3dUV(glm::vec3(1, -1, 1), glm::vec2(.5, .5)));
    vertices.push_back(Vertex3dUV(glm::vec3(1, 1, 1), glm::vec2(1, .5)));
    // ABEF
    vertices.push_back(Vertex3dUV(glm::vec3(-1, -1, -1), glm::vec2(.5, .5)));
    vertices.push_back(Vertex3dUV(glm::vec3(1, -1, -1), glm::vec2(1, .5)));
    vertices.push_back(Vertex3dUV(glm::vec3(-1, -1, 1), glm::vec2(.5, 1)));
    vertices.push_back(Vertex3dUV(glm::vec3(1, -1, 1), glm::vec2(1, 1)));
    // CDGH
    vertices.push_back(Vertex3dUV(glm::vec3(-1, 1, -1), glm::vec2(0, .5)));
    vertices.push_back(Vertex3dUV(glm::vec3(-1, 1, 1), glm::vec2(0, 1)));
    vertices.push_back(Vertex3dUV(glm::vec3(1, 1, -1), glm::vec2(.5, .5)));
    vertices.push_back(Vertex3dUV(glm::vec3(1, 1, 1), glm::vec2(.5, 1)));
    // ACEG
    vertices.push_back(Vertex3dUV(glm::vec3(-1, -1, -1), glm::vec2(.25, .25)));
    vertices.push_back(Vertex3dUV(glm::vec3(-1, 1, -1), glm::vec2(.75, .25)));
    vertices.push_back(Vertex3dUV(glm::vec3(1, -1, -1), glm::vec2(.25, .75)));
    vertices.push_back(Vertex3dUV(glm::vec3(1, 1, -1), glm::vec2(.75, .75)));
    // BDFH
    vertices.push_back(Vertex3dUV(glm::vec3(-1, -1, 1), glm::vec2(0, 0)));
    vertices.push_back(Vertex3dUV(glm::vec3(1, -1, 1), glm::vec2(.5, 0)));
    vertices.push_back(Vertex3dUV(glm::vec3(-1, 1, 1), glm::vec2(0, .5)));
    vertices.push_back(Vertex3dUV(glm::vec3(1, 1, 1), glm::vec2(.5, .5)));


	std::vector<unsigned int> indices;
    // The vertices are ordered by faces, so we can just add indices one face at a time.
    for (int i = 0; i < 6; i++)
    {
        indices.push_back(i * 4 + 0);
        indices.push_back(i * 4 + 1);
        indices.push_back(i * 4 + 2);
        indices.push_back(i * 4 + 3);
        indices.push_back(i * 4 + 2);
        indices.push_back(i * 4 + 1);
    }



	// Create shape object
	cube = new Mesh(vertices, indices);
	

    // In OpenGL, the Z-Axis points out of the screen.
    // Put the cube 5 units away from the camera.
	transform.SetPosition(glm::vec3(0, 0, -5));
    


    // Texture Setup:

    // Load the file.
    FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType("../Assets/texture.png"), "../Assets/texture.png");
    // Convert the file to 32 bits so we can use it.
    FIBITMAP* bitmap32 = FreeImage_ConvertTo32Bits(bitmap);


    // Create an OpenGL texture.
    glGenTextures(1, &texture);

    // Bind our texture.
    glBindTexture(GL_TEXTURE_2D, texture);

    // Fill our openGL side texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, FreeImage_GetWidth(bitmap32), FreeImage_GetHeight(bitmap32),
        0, GL_BGRA, GL_UNSIGNED_BYTE, static_cast<void*>(FreeImage_GetBits(bitmap32)));

    // We can unload the images now that the texture data has been buffered with opengl
    FreeImage_Unload(bitmap);
    FreeImage_Unload(bitmap32);

    // Set texture sampling parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    // Unbind the texture.
    glBindTexture(GL_TEXTURE_2D, 0);


    //
    // Loading a Cube Map
    //

    // Create an OpenGL texture.
    glGenTextures(1, &cubeMap);

    // Bind our texture as a cube map.
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

    // A collection of each side file path (so we can loop over them)
    std::vector<const GLchar*> faceFilePaths;
    faceFilePaths.push_back("../Assets/skyboxLeft.png");
    faceFilePaths.push_back("../Assets/skyboxRight.png");
    faceFilePaths.push_back("../Assets/skyboxBottom.png");
    faceFilePaths.push_back("../Assets/skyboxTop.png");
    faceFilePaths.push_back("../Assets/skyboxBack.png");
    faceFilePaths.push_back("../Assets/skyboxFront.png");

    // Fill our openGL side texture object.
    for (GLuint i = 0; i < faceFilePaths.size(); i++)
    {
        // Load the face and convet it to 32 bit.
        bitmap = FreeImage_ConvertTo32Bits(FreeImage_Load(FreeImage_GetFileType(faceFilePaths[i]), faceFilePaths[i]));

        // Load the image into OpenGL memory.
        // GL_TEXTURE_CUBE_MAP_POSITIVE_X indicates the side of the skybox. Incrementing that value gives us the constant used by each side.
        glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap),
            0, GL_BGRA, GL_UNSIGNED_BYTE, static_cast<void*>(FreeImage_GetBits(bitmap)));

        // We can unload the image now.
        FreeImage_Unload(bitmap);
    }

    // Set sampler parameters on our cube map.
    // These make sure the texture doesn't look pixelated.
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // These prevent artifacts from appearing near the edges.
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    // Unbind
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// SHADER STUFF

	// Load and compile shaders.
	vertexShader.InitFromFile("../Assets/vertex.glsl", GL_VERTEX_SHADER);
	fragmentShader.InitFromFile("../Assets/fragment.glsl", GL_FRAGMENT_SHADER);

	// Create a shader program.
	shaderProgram = glCreateProgram();
	vertexShader.AttachTo(shaderProgram);
	fragmentShader.AttachTo(shaderProgram);
	glLinkProgram(shaderProgram);

	// After the program has been linked, we can ask it where it put our world matrix and camera matrix
	worldMatrixUniform = glGetUniformLocation(shaderProgram, "worldMatrix");
    viewMatrixUniform = glGetUniformLocation(shaderProgram, "cameraView");
    textureUniform = glGetUniformLocation(shaderProgram, "tex");


    // Create shaders and the shader program for rendering the skybox.
    skyBoxVertexShader.InitFromFile("../Assets/skyboxVertex.glsl", GL_VERTEX_SHADER);
    skyBoxFragmentShader.InitFromFile("../Assets/skyboxFragment.glsl", GL_FRAGMENT_SHADER);
    skyboxProgram = glCreateProgram();
    skyBoxVertexShader.AttachTo(skyboxProgram);
    skyBoxFragmentShader.AttachTo(skyboxProgram);
    glLinkProgram(skyboxProgram);
    // Get uniforms for camera matrix and cubemap
    skyboxMatrixUniform = glGetUniformLocation(skyboxProgram, "cameraView");
    cubeMapUniform = glGetUniformLocation(skyboxProgram, "cubeMap");


    std::cout << "Use WASD to move, and the mouse to look around." << std::endl;
    std::cout << "Press escape to exit" << std::endl;

	// enable MSAA
	glEnable(GL_MULTISAMPLE);


	// Main Loop
	while (!glfwWindowShouldClose(window))
	{

        // Exit when escape is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            break;
        }

        // Calculate delta time.
        float dt = glfwGetTime();
        // Reset the timer.
        glfwSetTime(0);
        

        // Update the player controller
        controller.Update(window, viewportDimensions, mousePosition, dt);


        // rotate square
        transform.RotateY(1.0f * dt);



        // Calculate view matrix.
        glm::mat4 viewMatrix = controller.GetTransform().GetInverseMatrix();

        // Shortcut: use glm perspective matrix.
        glm::mat4 perspectiveProjection= glm::perspective(.7f, viewportDimensions.x / viewportDimensions.y, 1.f, 100.f);

        // Compose view and projection into one matrix to send to the gpu
        glm::mat4 viewProjection = perspectiveProjection * viewMatrix;




        // Clear the screen.
        // We now clear the depth buffer too.
        // The depth buffer helps OpenGL determine which polygons should be rendered on top.
        // Try turning it off and see what happens.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0, 0.0, 0.0, 0.0);

		// Set the current shader program.
		glUseProgram(shaderProgram);


        // Send the camera matrix to the shader
        glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, &(viewProjection[0][0]));

        // Bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

        // Use the texture.
        glUniform1i(textureUniform, 0);

		// Draw using the worldMatrixUniform
        glUniformMatrix4fv(worldMatrixUniform, 1, GL_FALSE, &(transform.GetMatrix()[0][0]));
		cube->Draw();

        // Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);



        // Draw the skybox...
        glUseProgram(skyboxProgram);

        // This function tells OpenGL that we want to overwrite any pixels where our depth is less than or equal to the previous depth.
        // Since the depth of the skybox will be equal to the default max depth, it will only draw where nothing has previously been drawn.
        glDepthFunc(GL_LEQUAL);

        // Here we calculate the view projection matrix without view translation.
        // Using this matrix rotates the skybox around the camera, but keeps it centered.
        glm::mat4 specialView = perspectiveProjection * glm::mat4(glm::mat3(viewMatrix));
        glUniformMatrix4fv(skyboxMatrixUniform, 1, GL_FALSE, &(specialView[0][0]));

        // Bind the skybox texture and draw the cube.
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
        cube->Draw();

        // Set the depth test back to the default setting.
        glDepthFunc(GL_LESS);

        // Stop using the shader program.
        glUseProgram(0);


		// Swap the backbuffer to the front.
		glfwSwapBuffers(window);

		// Poll input and window events.
		glfwPollEvents();

	}

	// Free memory from shader program and individual shaders
	glDeleteProgram(shaderProgram);


	// Free memory from shape object
	delete cube;

	// Free GLFW memory.
	glfwTerminate();


	// End of Program.
	return 0;
}
