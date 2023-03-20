#pragma once
#include <glm/glm.hpp>

class SimpleCamera
{
public:
	SimpleCamera();
	~SimpleCamera() {};

	virtual void Update(float deltaTime);
	glm::vec3 GetPosition() { return m_position; }
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float width, float height);
	glm::mat4 GetWorldTransform(glm::vec3 camPos, glm::vec3 eularAngles, glm::vec3 scale);
	glm::mat4 GetProjectionViewMatrix();
	glm::mat4 GetAspectRatio();

	void SetPosition(glm::vec3 position);
	void SetAspectRatio(float width, float height);
	void SetViewMatrix(glm::vec3 from, glm::vec3 to, glm::vec3 up);
	void SetProjectionMatrix(float fieldOfView, float aspectRatio, float near, float far);
	void SetProjectionMatrix(float fieldOfView, float width, float height, float near, float far);


protected:
	float m_theta;
	float m_phi;
	float m_aspectRatio;
	float m_turnSpeed = glm::radians(180.f);

	glm::vec3 m_position;
	glm::mat4 m_projectionViewTransform;
	glm::mat4 worldTransform;
	glm::mat4 viewTransform;


	// Last position of the mouse 
	glm::vec2 m_lastMouse;
};

