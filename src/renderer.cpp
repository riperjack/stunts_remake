#include "renderer.hpp"

Renderer::Renderer() {
	glClearColor(0.f / 255.f, 252.f / 252.f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	programID = loadShaders("assets/shaders/basic.vs", "assets/shaders/flat.fs");
	matrixID = glGetUniformLocation(programID, "MVP");
	modelMatrixID = glGetUniformLocation(programID, "model_matrix");
	shaderLightPosition = glGetUniformLocation(programID, "light_position_in");
	shaderDiffuseColor = glGetUniformLocation(programID, "diffuse_color");
	shaderLightIntensity = glGetUniformLocation(programID, "light_intensity_in");

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
	glGenBuffers(1, &vertexbuffer);
}

void Renderer::render(const std::vector<Actor*> &scene, Camera* camera) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// rendering loop
	for (int i = 0; i < scene.size(); i++) {
		glUseProgram(programID);

		Actor* currentObject = scene[i];
		std::vector<tinyobj::material_t> materials = currentObject->mesh->materials;
		std::map<int, std::vector<glm::vec3>> materialVertexMap = currentObject->mesh->materialVertexMap;
		std::map<int, std::vector<glm::vec3>> materialNormalMap = currentObject->mesh->materialNormalMap;

		glm::mat4 ViewMatrix = camera->getThirdPersonViewMatrix();
		glm::mat4 ModelMatrix = currentObject->getModelMatrix();
		glm::mat4 MVP = camera->getProjectionMatrix() * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glm::vec3 lightSource = glm::vec3(-1, 1, -1);
		glUniform3f(shaderLightPosition, lightSource.x, lightSource.y, lightSource.z);
		glUniform1f(shaderLightIntensity, 2);

		// rendering materials
		for (std::map<int, std::vector<glm::vec3>>::iterator iter = materialVertexMap.begin();
			iter != materialVertexMap.end();
			++iter)
		{
			int materialIdx = iter->first;
			tinyobj::material_t material = materials[materialIdx];
			std::vector<glm::vec3> vertices = iter->second;
			std::vector<glm::vec3> normals = materialNormalMap[materialIdx];

			glUniform3f(shaderDiffuseColor, material.diffuse[0], material.diffuse[1], material.diffuse[2]);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(tinyobj::real_t) * 3 * vertices.size(), &vertices[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLES, 0, vertices.size());
			glDisableVertexAttribArray(0);
		}
	}
}

/*
Based on:
https://github.com/opengl-tutorials/ogl/
*/
GLuint Renderer::loadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
