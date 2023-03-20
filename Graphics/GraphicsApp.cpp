#include "GraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "imgui.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsApp::GraphicsApp() {

}

GraphicsApp::~GraphicsApp() {

}

bool GraphicsApp::startup() {
	
	float time = getTime();
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	//m_viewMatrix = glm::lookAt(vec3(15), vec3(0), vec3(0, 1, 0));
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	m_light.color = { 1, 1, 1 };
	m_ambientLight = { 0.5, 0.5, 0.5 };

	m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));
	return LaunchShaders();
}

void GraphicsApp::shutdown() {

	Gizmos::destroy();
}

void GraphicsApp::update(float deltaTime) {

	float time = getTime();
	// wipe the gizmos clean for this frame
	Gizmos::clear();

	 //draw a simple grid with gizmos
	//vec4 white(0);
	//vec4 black(0, 0, 0, 1);
	//for (int i = 0; i < 21; ++i) {
	//	Gizmos::addLine(vec3(-10 + i, 0, 10),
	//					vec3(-10 + i, 0, -10),
	//					i == 10 ? white : black);
	//	Gizmos::addLine(vec3(10, 0, -10 + i),
	//					vec3(-10, 0, -10 + i),
	//					i == 10 ? white : black);
	//}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(0));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
	//grab the time since the application has started

	// rotate the light to emulate a 'day/night' cycle
	

	//m_quadTransform = glm::rotate(m_quadTransform, .02f, glm::vec3(0, 1, 0));

	mat4 t = glm::rotate(mat4(1), time, glm::normalize(vec3(0, 1, 0)));
	t[3] = vec4(0, 0, 0, 1);



	//m_simpleCamera.Update(deltaTime);

	m_flyCam.Update(deltaTime);

	ImGUIRefresher();

	//Gizmos::addSphere(vec3(0, 0, 0), 1, 8, 8, vec4(1, 1, 0, 0.5f)); // sun
	//
	//mat4 mercury = mat4(1);
	//mercury = translate(mercury, vec3(glm::sin(time * 8.23f) * 1.6f, -1, glm::cos(time * 8.23f) * 1.6f));
	//Gizmos::addSphere(vec3(0), .25f, 8, 8, vec4(0.35, 0.22, 0.08, 1), &mercury); // mercury 
	//
	//mat4 Venus = mat4(1);
	//Venus = translate(Venus, vec3(vec3(glm::sin(time * 1.622f) * 2.5f, -1, glm::cos(time * 1.622f) * 2.5f)));
	//Gizmos::addSphere(vec3(0), .35f, 8, 8, vec4(1, 1, 1, 0.5f), &Venus); // venus
	//
	//mat4 Earth = mat4(1);
	//Earth = translate(Earth, vec3(glm::sin(time * 1) * 3.75f, -1, glm::cos(time * 1) * 3.75f));
	//Gizmos::addSphere(vec3(0), .65f, 8, 8, vec4(0.18, 0.46, 0.13, 1), &Earth); // Earth
	//
	//mat4 EarthMoon = mat4(1);
	//EarthMoon = translate(EarthMoon, vec3(glm::sin(time * -3.f) * 1, -1, glm::cos(time * -3.f) * 1));
	//Gizmos::addSphere(vec3(glm::sin(time) * 4.f, 0, glm::cos(time) * 4.f), .25f, 16, 16, vec4(0, 0, 1, 0.5f), &EarthMoon);
	//
	//mat4 Mars = mat4(1);
	//Mars = translate(Mars, vec3(glm::sin(time * .531f) * 5.5f, -1, glm::cos(time * .531f) * 5.5f));
	//Gizmos::addSphere(vec3(0), .55f, 8, 8, vec4(0.51, 0.13, 0.07, 1), &Mars); // Mars
	//
	//mat4 Jupitar = mat4(1);
	//Jupitar = translate(Jupitar, vec3(glm::sin(time * .094f) * 6.3f, -1, glm::cos(time * .094f) * 6.3f));
	//Gizmos::addSphere(vec3(0), .45f, 8, 8, vec4(0.91, 0.58, 0.23, 1), &Jupitar); // Jupitar
	//
	//mat4 Saturn = mat4(1);
	//Saturn = translate(Saturn, vec3(glm::sin(time * .05f) * 7.5f, -1, glm::cos(time * .05f) * 7.5f));
	//Gizmos::addSphere(vec3(0), .65f, 8, 8, vec4(1, 1, 0, 0.5f), &Saturn); // Saturn
	//
	//mat4 SaturnRing = mat4(1);
	//SaturnRing = translate(SaturnRing, vec3(glm::sin(time * .05f) * 7.5f, -1, glm::cos(time * .05f) * 7.5f));
	//Gizmos::addRing(vec3(0), .75f, 1.f, 8, vec4(1, 1, 0, 0.5f), &SaturnRing); // saturn ring
	//
	//mat4 Uranus = mat4(1);
	//Uranus = translate(Uranus, vec3(glm::sin(time * .01189f) * 8.5f, -1, glm::cos(time * .01189f) * 8.5f));
	//Gizmos::addSphere(vec3(0), .25f, 8, 8, vec4(0.23, 0.35, 0.91, 1), &Uranus); // Uranus
	//
	//mat4 UranusRing = mat4(1);
	//UranusRing = translate(UranusRing, vec3(glm::sin(time * .01189f) * 8.5f, -1, glm::cos(time * .01189f) * 8.5f));
	////Gizmos::addRing(vec3(0), .25f, 1.2f, 8, vec4(0, 1, 0, 0.5f), &UranusRing);
	//Gizmos::addRing(vec3(0), .35f, .55f, 8, vec4(0.23, 0.35, 0.91, 1), &UranusRing); // Uranus ring
	//
	//mat4 Neptune = mat4(1);
	//Neptune = translate(Neptune, vec3(glm::sin(time * .00606f) * 9.75f, -1, glm::cos(time * .00606f) * 9.75f));
	//Gizmos::addSphere(vec3(0), .25f, 8, 8, vec4(1, 1, 0, 0.5f), &Neptune); // Neptune

}

void GraphicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	
	if (toggleFlyCam)
		SetFlyCamera();
	
	if (toggleStationaryCam)
		SetStationaryCamera();
	
	if (toggleOribtalCam)
		SetOribtalCamera();
	
	if (toggleSimpleCam)
		SetSimpleCamera();

	//glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	auto pv = m_projectionMatrix * m_viewMatrix;
	//draw the quad in QuadLoader()
	QuadDraw(pv * m_quadTransform);

	BoxDraw(pv * m_boxTransform);

	//draw the bunny setup in BunnyLoader()
	//BunnyDraw(pv * m_bunnyTransform);

//	PhongDraw(pv * m_bunnyTransform, m_bunnyTransform);

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}


bool GraphicsApp::LaunchShaders()
{
	// used for loading in a simple quad
	if (!QuadLoader())
		return false;
	// used for loading in an OBJ bunny
	if (!BunnyLoader())
		return false;

	return true;
}

bool GraphicsApp::QuadLoader()
{
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/simple.frag");

	if (m_simpleShader.link() == false)
	{
		printf("Simple Shader has an Error: %s\n", m_simpleShader.getLastError());
		return false;
	}

	//// Defined as 4 vertices for the 2 triangles
	//Mesh::Vertex vertices[4];
    //vertices[0].position = { -0.5f, 0, 0.5f, 1, };
    //vertices[1].position = { 0.5,  0, 0.5f, 1, };
    //vertices[2].position = { -0.5f, 0, -0.5f, 1, };
    //vertices[3].position = { 0.5f, 0, -0.5f, 1, };
	//
    //unsigned int indices[6] = { 0, 1, 2 ,2, 1, 3 };
	//
    //m_quadMesh.Initialise(4, vertices, 6, indices);


	Mesh::Vertex vertices[8];
	vertices[0].position = { .5f, 0, .5f, 1, };
	vertices[1].position = { .5,  0, -.5f, 1, };
	vertices[2].position = { -.5f, 0, .5f, 1, };
	vertices[3].position = { -.5f, 0, -.5f, 1, };

	vertices[4].position = { .5f, 1, .5f, 1, };
	vertices[5].position = { .5f, 1, -.5f, 1, };

	vertices[6].position = { -.5f, 1, .5f, 1, };
	vertices[7].position = { -.5f, 1, -.5f, 1, };

	unsigned int indices[36] = {
		0, 1, 2, 2, 1, 3, // bottom 
		4, 0, 6, 6, 0, 2, // right
		7, 5, 6, 6, 5, 4, // top
		3, 1, 7, 7, 1, 5, // left
		4, 5, 0, 0, 5, 1, // front 
		3, 7, 2, 2, 7, 6 }; // 

	m_quadMesh.Initialise(8, vertices, 36, indices);


	//Mesh::Vertex vertices[8];
	//vertices[0].position = { .5f, 0, .5f, 1, };
	//vertices[1].position = { .5,  0, -.5f, 1, };
	//vertices[2].position = { -.5f, 0, .5f, 1, };
	//vertices[3].position = { -.5f, 0, -.5f, 1, };
	//
	//vertices[4].position = { .5f, 1, .5f, 1, };
	//vertices[5].position = { .5f, 1, -.5f, 1, };
	//
	//vertices[6].position = { -.5f, 1, .5f, 1, };
	//vertices[7].position = { -.5f, 1, -.5f, 1, };
	//
	//unsigned int indices[36] = {
	//	0, 1, 2, 
	//	2, 1, 3, // bottom 
	//	4, 0, 6, 
	//	6, 0, 2, // right
	//	7, 5, 6, 
	//	6, 5, 4, // top
	//	3, 1, 7, 
	//	7, 1, 5, // left
	//	4, 5, 0, 
	//	0, 5, 1, // front 
	//	3, 7, 2, 
	//	2, 7, 6 }; // 
	//
	//m_quadMesh.Initialise(8, vertices, 36, indices);

	//m_quadMesh.InitialiseQuad();

	// this is a 10 'unit' wide quad
	m_quadTransform = {
		5, 0, 0, 0,
		0, 5, 0, 0,
		0, 0, 5, 0,
		0, 0, 0,  1
	};

	return true;
}

bool GraphicsApp::BoxLoader()
{
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/simple.frag");

	if (m_simpleShader.link() == false)
	{
		printf("Simple Shader has an Error: %s\n", m_simpleShader.getLastError());
		return false;
	}

	Mesh::Vertex vertices[8];
	vertices[0].position = { .5f, 0, .5f, 1, };
	vertices[1].position = { .5,  0, -.5f, 1, };
	vertices[2].position = { -.5f, 0, .5f, 1, };
	vertices[3].position = { -.5f, 0, -.5f, 1, };

	vertices[4].position = { .5f, 1, .5f, 1, };
	vertices[5].position = { .5f, 1, -.5f, 1, };

	vertices[6].position = { -.5f, 1, .5f, 1, };
	vertices[7].position = { -.5f, 1, -.5f, 1, };

	unsigned int indices[36] = {
		0, 1, 2, 2, 1, 3, // bottom 
		4, 0, 6, 6, 0, 2, // right
		7, 5, 6, 6, 5, 4, // top
		3, 1, 7, 7, 1, 5, // left
		4, 5, 0, 0, 5, 1, // front 
		3, 7, 2, 2, 7, 6 }; // 

	m_boxMesh.Initialise(8, vertices, 36, indices);

	//m_quadMesh.InitialiseQuad();

	// this is a 10 'unit' wide quad
	m_boxTransform = {
		5, 0, 0, 0,
		0, 5, 0, 0,
		0, 0, 5, 0,
		0, 0, 0,  1
	};


	return true;
}

void GraphicsApp::BoxDraw(glm::mat4 pvm)
{
	//Bind the shader
	m_simpleShader.bind();

	// Bind the transform
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	//Draw the box using Mesh's draw
	m_boxMesh.Draw();

}

void GraphicsApp::QuadDraw(glm::mat4 pvm)
{
	//Bind the shader
	m_simpleShader.bind();

	// Bind the transform
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	//Draw the quad using Mesh's draw
	m_quadMesh.Draw();

}

bool GraphicsApp::BunnyLoader()
{
	m_phongShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/phong.frag");


	if (m_phongShader.link() == false)
	{
		printf("Color Shader Error: %s\n ", m_phongShader.getLastError());
		return false;
	}

	if (m_bunnyMesh.load("./stanford/Bunny.obj") == false)
	{
		printf("Bunny Mesh Error!\n");
		return false;
	}

	m_bunnyTransform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0,  1
	};

	return true;
}

void GraphicsApp::BunnyDraw(glm::mat4 pvm)
{
	//bind the shader
	m_colorShader.bind();

	//bind the transform
	m_colorShader.bindUniform("ProjectionViewModel", pvm);

	//bind the color
	m_colorShader.bindUniform("BaseColor", glm::vec4(1));

	//draw the quad using Mesh's draw
	m_bunnyMesh.draw();
}

void GraphicsApp::PhongDraw(glm::mat4 pvm, glm::mat4 transform)
{
	//bind the phong shader
	m_phongShader.bind();


	//bind the camera position
	m_phongShader.bindUniform("CameraPosition", 
		glm::vec3(glm::inverse(m_viewMatrix)[3]));

	//bind the directional light  we defined
	m_phongShader.bindUniform("LightDirection", m_light.direction);
	m_phongShader.bindUniform("LightColor", m_light.color);
	m_phongShader.bindUniform("AmbientColor", m_ambientLight);
	//bind the pvm using the one provided
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	//bind the transform using the one provided
	m_phongShader.bindUniform("ModelMatrix", transform);

	//draw the phong lightning
	m_bunnyMesh.draw();
}

void GraphicsApp::ImGUIRefresher()
{
	ImGui::Begin("Light Settings");

	ImGui::ColorEdit4("Change Color", &m_light.color[0]);

	ImGui::End();

	ImGui::Begin("Camera Positions");
	//Stationary Cam
	ImGui::Checkbox("Toggle Stationary Cam", &toggleStationaryCam);
	// Oribtal Cam
	ImGui::Checkbox("Toggle Oribtal Cam", &toggleOribtalCam);
	// Fly Cam
	if (ImGui::CollapsingHeader("Fly Cam"))
	{
		ImGui::Checkbox("Toggle Fly Cam", &toggleFlyCam);

	}
	// Simple Cam
	if (ImGui::CollapsingHeader("Simple Cam"))
	{
	    ImGui::Checkbox("Toggle Simple Cam", &toggleSimpleCam);
		ImGui::DragFloat3("Camera Position", &SimpleCamPos[0]);
		ImGui::DragFloat3("Camera Position", &SimpleCamPos[0]);
	}

	ImGui::End();

	ImGui::Begin("Solar System ");

	if (ImGui::CollapsingHeader("Sun"))
	{

	}

	ImGui::End();

}

void GraphicsApp::SetFlyCamera()
{
	m_viewMatrix = m_flyCam.GetViewMatrix();
	m_projectionMatrix = m_flyCam.GetProjectionMatrix(getWindowWidth(), getWindowHeight());
}

void GraphicsApp::SetOribtalCamera()
{
	m_viewMatrix = m_oribtalCam.GetViewMatrix();
	m_projectionMatrix = m_oribtalCam.GetProjectionMatrix(getWindowWidth(), getWindowHeight());
}

void GraphicsApp::SetStationaryCamera()
{
	m_viewMatrix = m_stationaryCam.GetViewMatrix();
	m_projectionMatrix = m_stationaryCam.GetProjectionMatrix(getWindowWidth(), getWindowHeight());
}

void GraphicsApp::SetSimpleCamera()
{
	m_viewMatrix = glm::lookAt(vec3(15), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);
}



