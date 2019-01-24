#include "mesh.hpp"

Mesh::Mesh(const char* filename) {
	loadModel(filename);
	generateMeshInterface();
}

Mesh::~Mesh() {
	delete meshInterface;
}

void Mesh::loadModel(const char* filename) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, "assets/models/");

	if (!err.empty()) {
		printf("Error while loading obj: %s", err.c_str());
	}

	if (!ret) {
		exit(1);
	}

	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0;

		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			int materialId = shapes[s].mesh.material_ids[f];
			std::vector<glm::vec3> face;

			for (size_t v = 0; v < fv; v++) {
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				face.push_back(glm::vec3(vx, vy, vz));
				materialVertexMap[materialId].push_back({ vx, vy, vz });

				tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
				materialNormalMap[materialId].push_back({ nx, ny, nz });
			}
			faces.push_back(face);
			index_offset += fv;
		}
	}
}

void Mesh::generateMeshInterface() {
	meshInterface = new btTriangleMesh();

	for (int i = 0; i < faces.size(); i++) {
		std::vector<glm::vec3> face = faces[i];
		meshInterface->addTriangle(
			btVector3(face[0].x, face[0].y, face[0].z),
			btVector3(face[1].x, face[1].y, face[1].z),
			btVector3(face[2].x, face[2].y, face[2].z)
		);
	}
}