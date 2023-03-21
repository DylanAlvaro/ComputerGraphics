#include "Instance.h"
#include <glm/ext.hpp>
#include "Scene.h"
#include "OBJMesh.h"
#include <Texture.h>
#include "SimpleCamera.h"
#include "GraphicsApp.h"

Instance::Instance(glm::mat4 transform, aie::OBJMesh* mesh, aie::ShaderProgram* shader)
{
	m_transform - transform;
	m_mesh = mesh;
	m_shader = shader;
}

void Instance::Draw(Scene* scene)
{
	// Set the shader pipeline
	m_shader->bind();
	// Bind all relevant uniforms for our shaders
	auto pvm = scene->GetCamera()->GetProjectionMatrix(scene->GetWindowSize().x, 
		scene->GetWindowSize().y) * scene->GetCamera()->GetViewMatrix() * m_transform;

	m_shader->bindUniform("ProjectionViewModel", pvm);
	m_shader->bindUniform("ModelMatrix", m_transform);

	m_shader->bindUniform("CameraPosition", scene->GetCamera()->GetPosition());

	//bind the directional light  we defined
	m_shader->bindUniform("LightDirection", scene->GetLight().direction);
	m_shader->bindUniform("LightColor", scene->GetLight().color);
	m_shader->bindUniform("AmbientColor", scene->GetAmbientLightColor());

	m_mesh->draw();
}
