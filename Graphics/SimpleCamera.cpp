#include "SimpleCamera.h"
#include <glm/ext.hpp>
#include "Input.h"


SimpleCamera::SimpleCamera()
{
	m_position = glm::vec3(-10, 2, 0);
	m_phi = 0;
	m_theta = 0;
}

void SimpleCamera::Update(float deltaTime)
{
	aie::Input* Input = aie::Input::getInstance();
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR),
		glm::cos(phiR) * glm::sin(thetaR));

	glm::vec3 right(-glm::sin(thetaR), 0, glm::cos(thetaR));

	glm::vec3 up(0, 1, 0);

	// We will use WASD to move and the Q & E to go up and down
	if (Input->isKeyDown(aie::INPUT_KEY_W))
		m_position += forward * deltaTime;
	
	if (Input->isKeyDown(aie::INPUT_KEY_S))
		m_position -= forward * deltaTime;

	if (Input->isKeyDown(aie::INPUT_KEY_A))
		m_position -= right * deltaTime;
	
	if (Input->isKeyDown(aie::INPUT_KEY_D))
		m_position += right * deltaTime;

	if (Input->isKeyDown(aie::INPUT_KEY_Q))
		m_position += up * deltaTime;
	if (Input->isKeyDown(aie::INPUT_KEY_E))
		m_position -= up * deltaTime;

	//get mouse coordinates
	float mx = Input->getMouseX();
	float my = Input->getMouseY();

	// if the right button is held down, increment theta and phi (rotate)
	if (Input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_theta += m_turnSpeed * (mx - m_lastMouse.x) * deltaTime;
		m_phi -= m_turnSpeed * (my - m_lastMouse.y) * deltaTime;
	}

	m_lastMouse = glm::vec2(mx, my);
}

glm::mat4 SimpleCamera::GetViewMatrix()
{
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);
	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), 
		glm::cos(phiR) * glm::sin(thetaR));

	return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 SimpleCamera::GetProjectionMatrix(float width, float height)
{
	return glm::perspective(glm::pi<float>() * .25f, width / height, .1f, 1000.f);
}

glm::mat4 SimpleCamera::GetWorldTransform(glm::vec3 camPos, glm::vec3 eularAngles, glm::vec3 scale)
{
	return glm::mat4();
}

glm::mat4 SimpleCamera::GetProjectionViewMatrix()
{
	return glm::mat4();
}

glm::mat4 SimpleCamera::GetAspectRatio()
{
	return glm::mat4(m_aspectRatio);
}

void SimpleCamera::SetPosition(glm::vec3 position)
{
}

void SimpleCamera::SetAspectRatio(float width, float height)
{
	
}

void SimpleCamera::SetViewMatrix(glm::vec3 from, glm::vec3 to, glm::vec3 up)
{
}

void SimpleCamera::SetProjectionMatrix(float fieldOfView, float aspectRatio, float, float)
{
}

void SimpleCamera::SetProjectionMatrix(float fieldOfView, float width, float height, float, float)
{
}
