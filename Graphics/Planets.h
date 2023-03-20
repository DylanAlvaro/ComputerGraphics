#pragma once
#include "GraphicsApp.h"

class Planets : public GraphicsApp
{
	Planets();
	~Planets();
	virtual void Draw();

	void Update(float deltaTime);

};

