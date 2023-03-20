#include "FlyCamera.h"
#include <glm/ext.hpp>
#include "Input.h"

void FlyCamera::Update(float deltaTime)
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

void FlyCamera::SetSpeed(float speed)
{
	m_turnSpeed = glm::radians(180.f);
}
