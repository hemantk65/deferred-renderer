#pragma once

#include "GL/glew.h"

#include "assimp/scene.h"
#include "assimp/mesh.h"

#include <vector>

class Mesh
{
public:
	struct MeshEntry {
		enum BUFFERS {
			VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER
		};
		GLuint vao;
		GLuint vbo[4];

		unsigned int elementCount;

		MeshEntry(aiMesh *mesh);
		~MeshEntry();

		void render();
	};

	std::vector<MeshEntry*> meshEntries;

public:
	Mesh();
	Mesh(const aiScene* scene);
	~Mesh(void);

	void addScene(const aiScene* scene);

	void render();
};