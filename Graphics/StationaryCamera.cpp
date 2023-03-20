#include "StationaryCamera.h"

StationaryCamera::StationaryCamera()
{
}

StationaryCamera::~StationaryCamera()
{
}

void StationaryCamera::SetRotation(glm::vec3 rotation)
{
	m_turnSpeed = glm::radians(180.f);
}
