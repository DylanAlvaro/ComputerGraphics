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

	Light light;

	light.color = { 1, 1, 1 };
	m_ambientLight = { 0.5, 0.5, 0.5 };
	light.direction = { 1,-1,1 };

	//m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));
	
	m_scene = new Scene(&m_flyCam, glm::vec2(getWindowWidth(), getWindowHeight()),
		light, m_ambientLight);

	m_scene->AddPointLights(glm::vec3(5, 3, 0), glm::vec3(1, 0, 0), 50);
	m_scene->AddPointLights(glm::vec3(-5, 3, 0), glm::vec3(0, 0, 1), 50);

	return LaunchShaders();
}

void GraphicsApp::shutdown() {

	Gizmos::destroy();
	delete m_scene;
}

void GraphicsApp::update(float deltaTime) {

	float time = getTime();
	// wipe the gizmos clean for this frame
	Gizmos::clear();

	 //draw a simple grid with gizmos
	vec4 white(0);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(0));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
	//grab the time since the application has started

	// rotate the light to emulate a 'day/night' cycle
	

	m_flyCam.Update(deltaTime);
	//m_quadTransform = glm::rotate(m_quadTransform, .02f, glm::vec3(0, 1, 0));

	mat4 t = glm::rotate(mat4(1), time, glm::normalize(vec3(0, 1, 0)));
	t[3] = vec4(0, 0, 0, 1);

	if (toggleFlyCam)
		SetFlyCamera();

	if (toggleStationaryCam)
		SetStationaryCamera();

	if (toggleOribtalCam)
		SetOribtalCamera();

	if (toggleSimpleCam)
		SetSimpleCamera();


	//m_simpleCamera.Update(deltaTime);


	ImGUIRefresher();
	ImGUIPlanets();
	ImGUIShapes();
	ImGUIModels();

}

void GraphicsApp::draw() 
{
	//Bind the render target as the first part of our draw function 
	m_renderTarget.bind();

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	//glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	auto pv = m_projectionMatrix * m_viewMatrix;

	m_scene->Draw();

	m_renderTarget.unbind();
	
	clearScreen();
	//draw the quad in QuadLoader()

	if (toggleBox)
		QuadDraw(pv * m_boxTransform);

	if (togglePyramid)
		TriangleDraw(pv * m_pyramidTransform);

	//if (toggleGrid)
		//QuadDraw(pv * m_quadTransform);
		QuadTextureDraw(pv * m_quadTransform);


	// Unbind the target to return to the back buffer


	//draw the bunny setup in BunnyLoader()
	//BunnyDraw(pv * m_bunnyTransform);
	
	if (toggleSpear)
		for (int i = 0; i < 10; i++)
			m_scene->AddInstance(new Instance(glm::vec3(i * 2, 0, 0),
				glm::vec3(0, i * 30, 0), glm::vec3(1, 1, 1),
				&m_spearMesh, &m_normalLitShader));

	if (toggleDragon)
		PhongDraw(pv * m_dragonTransform, m_dragonTransform);
		//ObjDraw(pv, m_dragonTransform, &m_dragonMesh);

	if (toggleGun)
		ObjDraw(pv, m_gunTransform, &m_gunMesh);

	


	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}


bool GraphicsApp::LaunchShaders()
{

	if (m_renderTarget.initialise(1, getWindowWidth(), getWindowHeight()) == false)
	{
		printf("Render Target Error!\n");
		return false;
	}

	m_normalLitShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/normalLit.vert");
	m_normalLitShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/normalLit.frag");

	if (m_normalLitShader.link() == false)
	{
		printf("Normal Lit Phong Shader Error: %s\n ", m_normalLitShader.getLastError());
		return false;
	}

	// used for loading in a simple quad
	if (!QuadLoader())
		return false;
	// used for loading in an OBJ bunny
	if (!BunnyLoader())
		return false;

	// Used for loading a texure on our quad
	if (!QuadTextureLoader())
		return false;

	// used for loading an OBJ spear
	if (!SpearLoader())
		return false;

	if (!GunLoader())
		return false;

	if (!TriangleLoader())
		return false;

	////used for loading an obj dragon
	if (!DragonLoader())
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

	m_boxMesh.Initialise(8, vertices, 36, indices);


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
	m_boxTransform = {
		5, 0, 0, 0,
		0, 5, 0, 0,
		0, 0, 5, 0,
		0, 0, 0,  1
	};

	return true;
}

void GraphicsApp::QuadDraw(glm::mat4 pvm)
{
	//Bind the shader
	m_simpleShader.bind();

	// Bind the transform
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	//Draw the quad using Mesh's draw
	m_boxMesh.Draw();

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

bool GraphicsApp::SpearLoader()
{
	if (m_spearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soulspear Mesh Error!\n");
		return false;
	}

	m_spearTransform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0,  1
	};

	return true;
}

void GraphicsApp::ObjDraw(glm::mat4 pv, glm::mat4 transform, aie::OBJMesh* objMesh)
{
	m_normalLitShader.bind();
	// Bind the camera position    
	m_normalLitShader.bindUniform("CameraPosition",
	glm::vec3(glm::inverse(m_viewMatrix)[3]));
	m_normalLitShader.bindUniform("LightDirection", m_scene->GetLight().direction);
	m_normalLitShader.bindUniform("LightColor", m_scene->GetLight().color);
	m_normalLitShader.bindUniform("AmbientColor", m_ambientLight);
	m_normalLitShader.bindUniform("diffuseTexture", 0);
	m_normalLitShader.bindUniform("ProjectionViewModel", pv * transform);
	m_normalLitShader.bindUniform("ModelMatrix", transform);
	
	objMesh->draw();
}

bool GraphicsApp::DragonLoader()
{
	if (m_dragonMesh.load("./stanford/Dragon.obj", true, true) == false)
	{
		printf("Dragon Mesh Error!\n");
		return false;
	}

	m_dragonTransform = {
		.25f, 0, 0, 0,
		0, .25f, 0, 0,
		0, 0, .25f, 0,
		0, 0, 0,  1
	};

	return true;
}

void GraphicsApp::DragonDraw(glm::mat4 pvm)
{
	m_colorShader.bind();

	m_colorShader.bindUniform("ProjectionViewModel", pvm);

	m_colorShader.bindUniform("BaseColor", glm::vec4(1));

	m_dragonMesh.draw();
}

bool GraphicsApp::GunLoader()
{
	if (m_gunMesh.load("./traveller/M1887.obj", true, true) == false)
	{
		printf("traveller Mesh Error!\n");
		return false;
	}

	m_gunTransform = {
		5, 0, 0, 0,
		0, 5, 0, 0,
		0, 0, 5, 0,
		0, 0, 0, 0.5f };

	return true;
}

void GraphicsApp::GunDraw(glm::mat4 pvm)
{
	m_colorShader.bind();
	
	m_colorShader.bindUniform("ProjectionViewModel", pvm);
	
	m_colorShader.bindUniform("BaseColor", glm::vec4(1));
	
	m_gunMesh.draw();
}

bool GraphicsApp::TriangleLoader()
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


	Mesh::Vertex vertices[5];
	// Bottom face    
	vertices[0].position = { -0.5f, 0,  0.5f, 1 };
	vertices[1].position = { 0.5f, 0,  0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };
	vertices[3].position = { 0.5f, 0, -0.5f, 1 };
	// Top point    
	vertices[4].position = { 0, 1, 0, 1 };
	
	unsigned int indices[18] = {
		0,2,1,2,3,1,
		4,2,0,4,3,2,
		4,1,3,4,0,1 
	};

	m_pyramidMesh.Initialise(5, vertices, 18, indices);

	// this is a 10 'unit' wide quad
	m_pyramidTransform = {
		2, 0, 0, 0,
		0, 2, 0, 0,
		0, 0, 2, 0,
		0, 0, 0,  1
	};

	return true;
}

void GraphicsApp::TriangleDraw(glm::mat4 pvm)
{
	//Bind the shader
	m_simpleShader.bind();

	// Bind the transform
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	//Draw the quad using Mesh's draw
	m_pyramidMesh.Draw();
}

bool GraphicsApp::QuadTextureLoader()
{
	m_texturedShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/textured.frag");

	if (m_texturedShader.link() == false)
	{
		printf("Textured Shader has an Error: %s\n", m_texturedShader.getLastError());
		return false;
	}

	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Failed to load the grid texture correctly! \n");
		return false;
	}

	m_quadMesh.InitialiseQuad();

	m_quadTransform = {
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, 10, 0,
		0, 0, 0,  1
	};

	return true;
}

void GraphicsApp::QuadTextureDraw(glm::mat4 pvm)
{
	//Bind the shader
	m_texturedShader.bind();

	// Bind the transform
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the texture location 
	m_texturedShader.bindUniform("diffuseTexture", 0);

	// Bind the texture to a specific location
//	m_gridTexture.bind(0);

	m_renderTarget.getTarget(0).bind(0);

	//Draw the quad using Mesh's draw
	m_quadMesh.Draw();
}

void GraphicsApp::PhongDraw(glm::mat4 pvm, glm::mat4 transform)
{
	//bind the phong shader
	m_phongShader.bind();


	//bind the camera position
	m_phongShader.bindUniform("CameraPosition", 
		glm::vec3(glm::inverse(m_viewMatrix)[3]));

	//bind the directional light  we defined
	m_phongShader.bindUniform("LightDirection", m_scene->GetLight().direction);
	m_phongShader.bindUniform("LightColor", m_scene->GetLight().color);
	m_phongShader.bindUniform("AmbientColor", m_ambientLight);
	//bind the pvm using the one provided
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	//bind the transform using the one provided
	m_phongShader.bindUniform("ModelMatrix", transform);

	//draw the phong lightning
	m_dragonMesh.draw();
}

void GraphicsApp::ImGUIRefresher()
{
	ImGui::Begin("Light Settings");

	if (ImGui::CollapsingHeader("Basic Light"))
	{
		ImGui::DragFloat3("Global Light Direction", &m_light.direction[0], 0.1, -1, 1);
		ImGui::DragFloat3("Global Light Color", &m_light.color[0], 0.1, 0, 1);

	}
	
	if (ImGui::CollapsingHeader("More Lights"))
	{
		
	}

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
		ImGui::DragFloat3("Change Position", &SimpleCamPos[0]);
	}
	// Simple Cam
	if (ImGui::CollapsingHeader("Simple Cam"))
	{
	    ImGui::Checkbox("Toggle Simple Cam", &toggleSimpleCam);
		ImGui::DragFloat3("Camera Position", &SimpleCamPos[0]);
	}

	ImGui::End();


}

void GraphicsApp::ImGUIPlanets()
{
	ImGui::Begin("Solar System ");

	//ImGui::Checkbox("Toggle Planets", &)

	if (ImGui::CollapsingHeader("Sun"))
	{
		ImGui::Checkbox("Visible", &sunVisible);
	}
	if (ImGui::CollapsingHeader("Mercury"))
	{
		ImGui::Checkbox("Visible", &mercuryVisible);
	}
	if (ImGui::CollapsingHeader("Venus"))
	{
		ImGui::Checkbox("Visible", &venusVisible);
	}
	if (ImGui::CollapsingHeader("Earth"))
	{
		ImGui::Checkbox("Toggle Simple Cam", &earthVisible);
	}
	if (ImGui::CollapsingHeader("Mars"))
	{
		ImGui::Checkbox("Toggle Simple Cam", &marsVisible);
	}
	if (ImGui::CollapsingHeader("Jupitar"))
	{
		ImGui::Checkbox("Toggle Simple Cam", &jupitarVisible);
	}
	if (ImGui::CollapsingHeader("Saturn"))
	{
		ImGui::Checkbox("Toggle Simple Cam", &saturnVisible);
	}
	if (ImGui::CollapsingHeader("Uranus"))
	{
		ImGui::Checkbox("Toggle Simple Cam", &uranusVisible);
	}
	if (ImGui::CollapsingHeader("Neptune"))
	{
		ImGui::Checkbox("Toggle Simple Cam", &neptuneVisible);
	}

	ImGui::End();
}

void GraphicsApp::ImGUIShapes()
{
	ImGui::Begin("Primitive Meshes");

	ImGui::Checkbox("Box", &toggleBox);
	ImGui::Checkbox("Cylinder", &toggleCylinder);
	ImGui::Checkbox("Pyramid", &togglePyramid);
	ImGui::Checkbox("Sphere", &toggleSphere);
	ImGui::Checkbox("Cone", &toggleCone);
	ImGui::Checkbox("Grid", &toggleGrid);
	ImGui::Checkbox("Dragon", &toggleDragon);
	ImGui::Checkbox("Gun", &toggleGun);

	if(ImGui::CollapsingHeader("spear"))
	{ 
		ImGui::Checkbox("Spear", &toggleSpear);
		ImGui::DragFloat3("Change position of object", &SpearPos[0],0.1 ,-1,99);
	}
	
	ImGui::End();
}

void GraphicsApp::ImGUIModels()
{
	ImGui::Begin("Custom Models");

	if (ImGui::CollapsingHeader("Spear"))
	{
		ImGui::Checkbox("Spear", &toggleSpear);
	}

	if (ImGui::CollapsingHeader("Dragon"))
	{
	}

	if (ImGui::CollapsingHeader("Gun"))
	{
	}

	ImGui::End();
}


void GraphicsApp::SetFlyCamera()
{
	m_viewMatrix = m_flyCam.GetViewMatrix();
	m_projectionMatrix = m_flyCam.GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_scene->SetCamera(&m_flyCam);
}
void GraphicsApp::SetOribtalCamera()
{
	m_viewMatrix = m_oribtalCam.GetViewMatrix();
	m_projectionMatrix = m_oribtalCam.GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_scene->SetCamera(&m_flyCam);
}
void GraphicsApp::SetStationaryCamera()
{
	m_viewMatrix = m_stationaryCam.GetViewMatrix();
	m_projectionMatrix = m_stationaryCam.GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_scene->SetCamera(&m_flyCam);
}
void GraphicsApp::SetSimpleCamera()
{
	m_viewMatrix = glm::lookAt(vec3(15), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);
	m_scene->SetCamera(&m_flyCam);
}



