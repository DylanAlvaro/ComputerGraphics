#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"
#include "SimpleCamera.h"
#include "StationaryCamera.h"
#include "FlyCamera.h"
#include "OribitalCamera.h"


class GraphicsApp : public aie::Application {
public:

	GraphicsApp();
	virtual ~GraphicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();


protected:

	bool LaunchShaders();
	bool QuadLoader();

	bool BoxLoader();

	void BoxDraw(glm::mat4 pvm);

	void QuadDraw(glm::mat4 pvm);

	bool BunnyLoader();
	void BunnyDraw(glm::mat4 pvm);

	void PhongDraw(glm::mat4 pvm, glm::mat4 transform);

	void ImGUIRefresher();

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::ShaderProgram   m_simpleShader;
	aie::ShaderProgram   m_colorShader;

	aie::ShaderProgram   m_phongShader;

	Mesh                 m_quadMesh;
	glm::mat4            m_quadTransform;

	Mesh                 m_boxMesh;
	glm::mat4            m_boxTransform;

	aie::OBJMesh         m_bunnyMesh;
	glm::mat4            m_bunnyTransform;

	SimpleCamera         m_simpleCamera;
	StationaryCamera     m_stationaryCam;

	struct Light
	{
		glm::vec3 direction;
		glm::vec3 color;
	};

	Light      m_light;
	glm::vec3  m_ambientLight;


	bool toggleColor;
};