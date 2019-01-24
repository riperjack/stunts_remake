#ifndef MESH_HPP
#define MESH_HPP

#include <glm.hpp>
#include <btBulletDynamicsCommon.h>
#include "tiny_obj_loader.h"

class Mesh {

public:
	Mesh(const char* filename);
	~Mesh();
	btTriangleMesh* meshInterface;
	std::map<int, std::vector<glm::vec3>> materialVertexMap;
	std::map<int, std::vector<glm::vec3>> materialNormalMap;
	std::vector<std::vector<glm::vec3>> faces;
	std::vector<tinyobj::material_t> materials;

	void Mesh::loadModel(const char* filename);
	void Mesh::generateMeshInterface();
};

#endif
