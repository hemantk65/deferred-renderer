#include "Mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include <iostream>

Mesh::MeshEntry::MeshEntry(aiMesh *mesh, aiMaterial* material) {
	vbo[VERTEX_BUFFER] = NULL;
	vbo[TEXCOORD_BUFFER] = NULL;
	vbo[NORMAL_BUFFER] = NULL;
	vbo[INDEX_BUFFER] = NULL;
	vao = NULL;
	m_tex = NULL;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	elementCount = mesh->mNumFaces * 3;

	if (mesh->HasPositions()) {
		float *vertices = new float[mesh->mNumVertices * 3];
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			vertices[i * 3] = mesh->mVertices[i].x;
			vertices[i * 3 + 1] = mesh->mVertices[i].y;
			vertices[i * 3 + 2] = mesh->mVertices[i].z;
		}

		glGenBuffers(1, &vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		delete vertices;
	}


	if (mesh->HasTextureCoords(0)) {
		float *texCoords = new float[mesh->mNumVertices * 2];
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
			texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}

		glGenBuffers(1, &vbo[TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		delete texCoords;
	}


	if (mesh->HasNormals()) {
		float *normals = new float[mesh->mNumVertices * 3];
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			normals[i * 3] = mesh->mNormals[i].x;
			normals[i * 3 + 1] = mesh->mNormals[i].y;
			normals[i * 3 + 2] = mesh->mNormals[i].z;
		}

		glGenBuffers(1, &vbo[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		delete normals;
	}


	if (mesh->HasFaces()) {
		unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
			indices[i * 3] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		glGenBuffers(1, &vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW);

		delete indices;
	}


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		aiString Path;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
			m_tex = new Texture();
			m_tex->loadTexture(Path.data);
		}
	}

	aiColor3D ambient, diffuse, specular;
	float shininess;
	material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
	material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
	material->Get(AI_MATKEY_SHININESS, shininess);
	reflectivity = { ambient.r, diffuse.r, specular.r, shininess };
}

Mesh::MeshEntry::~MeshEntry() {
	if (vbo[VERTEX_BUFFER]) {
		glDeleteBuffers(1, &vbo[VERTEX_BUFFER]);
	}

	if (vbo[TEXCOORD_BUFFER]) {
		glDeleteBuffers(1, &vbo[TEXCOORD_BUFFER]);
	}

	if (vbo[NORMAL_BUFFER]) {
		glDeleteBuffers(1, &vbo[NORMAL_BUFFER]);
	}

	if (vbo[INDEX_BUFFER]) {
		glDeleteBuffers(1, &vbo[INDEX_BUFFER]);
	}

	glDeleteVertexArrays(1, &vao);

	delete m_tex;
}

void Mesh::MeshEntry::render() {
	GLint program;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);

	glUniform4fv(glGetUniformLocation(program, "reflectivity"), 1, reflectivity.data());

	if (m_tex)
	{

		GLuint texture = *(m_tex->getTexture());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(program, "m_tex"), 0);
	}

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

Mesh::Mesh()
{
}

Mesh::Mesh(const aiScene* scene)
{
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
		meshEntries.push_back(new Mesh::MeshEntry(scene->mMeshes[i], scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]));
	}
}


void Mesh::addScene(const aiScene* scene)
{
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
		meshEntries.push_back(new Mesh::MeshEntry(scene->mMeshes[i], scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]));
	}
}

Mesh::~Mesh(void)
{
	for (int i = 0; i < meshEntries.size(); ++i) {
		delete meshEntries.at(i);
	}
	meshEntries.clear();
}

void Mesh::render() {
	for (int i = 0; i < meshEntries.size(); ++i) {
		meshEntries.at(i)->render();
	}
}