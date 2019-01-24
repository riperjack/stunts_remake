#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <glew.h>
#include "actor.hpp"
#include "camera.hpp"

class Renderer {
public:
	Renderer();
	void render(const std::vector<Actor*> &scene, Camera* camera);

private:
	static GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);
	GLuint programID;
	GLuint vertexArrayID;
	GLuint matrixID;
	GLuint modelMatrixID;
	GLuint vertexbuffer;
	GLuint shaderLightPosition;
	GLuint shaderDiffuseColor;
	GLuint shaderLightIntensity;
};

#endif