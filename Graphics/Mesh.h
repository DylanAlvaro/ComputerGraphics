#pragma once
#include <glm/glm.hpp>

class Mesh
{
public:

	Mesh() : m_triCount(0), m_vao(0), m_vbo(0), m_ibo(0) {}
	virtual ~Mesh();

	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};

	// will be used to make a simple quaed
	void InitialiseQuad();

	// will be used for post procesing
	void InitialiseFullscreenQuad(); 

	// will be used for implementing a primitive
	void Initialise();
	virtual void Draw();

protected:
	unsigned int m_triCount;
	unsigned int m_vao; // the vertex array object
	unsigned int m_vbo; // the vertex buffer object
	unsigned int m_ibo; // the index buffer object



};

