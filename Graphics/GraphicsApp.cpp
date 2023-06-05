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
	
	//float time = getTime();
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(15), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);


	Light light;

	light.color = { 1, 1, 1 };
	m_ambientLight = { 0.5f, 0.5f, 0.5f };
	light.direction = { 1,-1,1 };

	m_emitter = new ParticleEmitter();

	m_emitter->Initialise(1000, 500, .1f, 1.0f, 1, 5, 1, .1f, 
	glm::vec4(0, 0, 1, 1), glm::vec4(0, 1, 0, 1));

	m_scene = new Scene(&m_flyCam, glm::vec2(getWindowWidth(), getWindowHeight()),
		light, m_ambientLight);

	m_scene->AddPointLights(pointLightPos1, pointLightCol1, pointLight1Intensity);
	m_scene->AddPointLights(pointLightPos2, pointLightCol2, pointLight2Intensity);

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
	
	FlyCamera* FC = &m_flyCam;
	m_emitter->Update(deltaTime, FC->GetTransform(
		m_flyCam.GetPosition(), glm::vec3(0), glm::vec3(1)));

	m_flyCam.Update(deltaTime);

	mat4 t = glm::rotate(mat4(1), time, glm::normalize(vec3(0, 1, 0)));
	t[3] = vec4(0, 0, 0, 1);

	//cameras
	if (toggleFlyCam) 
	{
		SetFlyCamera();
	}
		
	if (toggleStationaryCamX)
	{
		SetStationaryCameraX();
	}

	if (toggleStationaryCamY)
	{
		SetStationaryCameraY();
	}

	if (toggleStationaryCamZ)
	{
		SetStationaryCameraZ();
	}

	if (toggleSimpleCam)
	{
		SetSimpleCamera();
	}


	m_scene->ChangeLights(0, pointLightPos1, pointLightCol1, pointLight1Intensity);
	m_scene->ChangeLights(1, pointLightPos2, pointLightCol2, pointLight2Intensity);


	ImGUILights();
	ImGUIShapes();
	ImGUICamera();
}

void GraphicsApp::draw() 
{
	//Bind the render target as the first part of our draw function 
	m_renderTarget.bind();

	// wipe the screen to the background colour
	clearScreen();

	auto pv = m_projectionMatrix * m_viewMatrix;

	m_scene->Draw();

	m_particleShader.bind();
	m_particleShader.bindUniform("ProjectionViewModel", pv * m_particleEmitTransform);

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
	

	/// <summary>
	/// if the user toggles these objects spawn in 
	/// </summary>
	if(toggleParticles)
		m_emitter->Draw();
	
	if (toggleBox)
		QuadDraw(pv * m_boxTransform);

	if (togglePyramid)
		TriangleDraw(pv * m_pyramidTransform);

	if (toggleGrid)
		QuadTextureDraw(pv * m_quadTransform);
		
	if (toggleDragon)
		PhongDraw(pv * m_dragonTransform, m_dragonTransform);

	if (toggleSpear)
		ObjDraw(pv, m_spearTransform, &m_spearMesh);
	
	if (toggleGun)
		ObjDraw(pv, m_gunTransform, &m_gunMesh);


	m_renderTarget.unbind();
	
	clearScreen();
	m_scene->Draw();

	// Bind the post process shader and the texture
	m_postProcessShader.bind();
	m_postProcessShader.bindUniform("colorTarget", 0);
	m_postProcessShader.bindUniform("postProcessTarget", m_postProccessEffect);
	m_postProcessShader.bindUniform("windowWidth", (int)getWindowWidth());
	m_postProcessShader.bindUniform("windowHeight", (int)getWindowHeight());
	m_postProcessShader.bindUniform("time", getTime());
	m_renderTarget.getTarget(0).bind(0);

	m_fullScreenQuad.Draw();
}


bool GraphicsApp::LaunchShaders()
{
	if (m_renderTarget.initialise(1, getWindowWidth(), getWindowHeight()) == false)
	{
		printf("Render Target Error!\n");
		return false;
	}

#pragma region loadingShaders

	// normal lit shader
	m_normalLitShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/normalLit.vert");
	m_normalLitShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/normalLit.frag");

	if (m_normalLitShader.link() == false)
	{
		printf("Normal Lit Phong Shader Error: %s\n ", m_normalLitShader.getLastError());
		return false;
	}

	// post process shader
	m_postProcessShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/post.vert");
	m_postProcessShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/post.frag");

	if (m_postProcessShader.link() == false)
	{
		printf("Normal Lit Phong Shader Error: %s\n ", m_postProcessShader.getLastError());
		return false;
	}

	// particle shader
	m_particleShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/particle.vert");
	m_particleShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/particle.frag");

	if (m_particleShader.link() == false)
	{
		printf("Normal Lit Phong Shader Error: %s\n ", m_particleShader.getLastError());
		return false;
	}
#pragma endregion

	m_particleEmitTransform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0,  1
	};

	// Used for loading a texure on our quad
	if (!QuadTextureLoader())
		return false;

	// used for loading in a simple quad
	if (!QuadLoader())
		return false;

	// used for loading in an OBJ bunny
	if (!BunnyLoader())
		return false;

	// used for loading an OBJ spear
	if (!SpearLoader())
		return false;

	if (!GunLoader())
		return false;

	// used for loading in a simple triangle
	if (!TriangleLoader())
		return false;

	//used for loading an obj dragon
	if (!DragonLoader())
		return false;

	// create a full screen quad
	m_fullScreenQuad.InitialiseFullscreenQuad();
	
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
	auto pvm = m_scene->GetCamera()->GetProjectionMatrix(
		m_scene->GetWindowSize().x, m_scene->GetWindowSize().y) * m_scene->GetCamera()->GetViewMatrix() * m_spearTransform;
	m_normalLitShader.bindUniform("CameraPosition", glm::vec3(glm::inverse(m_viewMatrix)[3]));
	
	m_normalLitShader.bindUniform("LightDirection", m_scene->GetLight().direction);
	m_normalLitShader.bindUniform("LightColor", m_scene->GetLight().color);
	m_normalLitShader.bindUniform("AmbientColor", m_ambientLight);
	m_normalLitShader.bindUniform("diffuseTexture", 0);
	m_normalLitShader.bindUniform("ProjectionViewModel", pv * transform);
	m_normalLitShader.bindUniform("ModelMatrix", transform);
	
	
	int numberOfLights = m_scene->NumberOfLights();
	m_normalLitShader.bindUniform("numLights", numberOfLights);
	m_normalLitShader.bindUniform("PointLightPositions", numberOfLights,
		m_scene->GetPointLightPositions());
	m_normalLitShader.bindUniform("PointLightColors", numberOfLights,
		m_scene->GetPointLightColors());


	objMesh->draw();
}

/// <summary>
/// Loads in a .obj for the dragon also changing its transform
/// </summary>
/// <returns></returns>
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
/// <summary>
/// draws the .obj
/// </summary>
/// <param name="pvm"></param>
void GraphicsApp::DragonDraw(glm::mat4 pvm)
{
	m_colorShader.bind();

	m_colorShader.bindUniform("ProjectionViewModel", pvm);

	m_colorShader.bindUniform("BaseColor", glm::vec4(1));

	m_dragonMesh.draw();
}
/// <summary>
/// loads in .obj of a gun also changing its transform
/// </summary>
/// <returns></returns>
bool GraphicsApp::GunLoader()
{
	if (m_gunMesh.load("./traveller/M1887.obj", true, true) == false)
	{
		printf("traveller Mesh Error!\n");
		return false;
	}

	m_gunTransform = {
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, 10, 0,
		0, 0, 0, 0.5f };

	return true;
}

/// <summary>
/// draws the .obj
/// </summary>
/// <param name="pvm"></param>
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


void GraphicsApp::ChangeParticles(unsigned int _maxParticles, unsigned int _emitRate, float _lifetimeMin, float _lifeTimeMax, float _velocityMin, float _velocityMax, float _startSize, float _endSize, const glm::vec4& _startColor, const glm::vec4& _endColor)
{
	m_maxParticles = _maxParticles;
	m_emitRate = 1.0 / _emitRate;
	m_emitTimer = 0;
	m_startColor = _startColor;
	m_endColor = _endColor;
	m_startSize = _startSize;
	m_endSize = _endSize;
	m_velocityMax = _velocityMax;
	m_velocityMin = _velocityMin;
	m_firstDead = 0;
}

/// <summary>
/// allows to change variables such as the lights direction and color also allows for the particle system to become active.
/// </summary>
void GraphicsApp::ImGUILights()
{
	ImGui::Begin("Lighting & Post-Processing Settings");

	if (ImGui::CollapsingHeader("Basic Lighting"))
	{
		ImGui::DragFloat3("Global Light Direction", &lightDirection[0], 0.1, -1, 1);
		ImGui::DragFloat3("Global Light Color", &lightColor[0], 0.1, 0, 1);

	}

	if (ImGui::CollapsingHeader("Point Lighting"))
	{
		ImGui::Checkbox("Gizmos", &togglePointLightGizmos);

		ImGui::DragFloat3("Change Point Light 1 Pos", &pointLightPos1[0], 3, 0);
		ImGui::DragFloat3("Change Point Light 1 Col", &pointLightCol1[0], 3, 0);
		ImGui::DragFloat("Change Point Light 1 Intensity", &pointLight1Intensity);

		ImGui::DragFloat3("Change Point Light 2 Pos", &pointLightPos2[0], 3, 0);
		ImGui::DragFloat3("Change Point Light 2 Col", &pointLightCol2[0], 3, 0);
		ImGui::DragFloat("Change Point Light 2 Intensity", &pointLight2Intensity);

	}

	if(ImGui::CollapsingHeader(""))
	
	if (ImGui::CollapsingHeader("Post Processing & Particle System"))
	{
		if (ImGui::CollapsingHeader("Post Proccess Effects"))
		{
			ImGui::InputInt("Change Post Process Effect", &m_postProccessEffect);
			//ImGui::Checkbox("Visible", &m_particleEmitTransform);
		}

		if (ImGui::CollapsingHeader("Particle System Effects"))
		{
			ImGui::Checkbox("Particles", &toggleParticles);
			ImGui::DragFloat("Start Size", &m_startSize);
			ImGui::DragFloat("End Size", &m_endSize);
			ImGui::DragFloat3("Particle Color", &m_startColor[0], 3 ,0);
		}
	}

	ImGui::End();
}

/// <summary>
/// ImGUI cameras uses the imgui system to create an interactive dropdown that allows the user to make cameras active or not active. 
/// </summary>
void GraphicsApp::ImGUICamera()
{
	ImGui::Begin("Camera Positions");
	
	if (ImGui::CollapsingHeader("Stationary Cam"))
	{
		ImGui::Checkbox("Toggle Stationary Cam X", &toggleStationaryCamX);
		ImGui::Checkbox("Toggle Stationary Cam Y", &toggleStationaryCamY);
		ImGui::Checkbox("Toggle Stationary Cam Z", &toggleStationaryCamZ);
		
	}

	if (ImGui::CollapsingHeader("Fly Cam"))
	{
		ImGui::Checkbox("Toggle Fly Cam", &toggleFlyCam);
	}

	ImGui::End();
}
/// <summary>
/// ImGUI shapes uses the imgui system to create an interactive dropdown that allows the user to make certain gameobjects active in the scene or not 
/// </summary>
void GraphicsApp::ImGUIShapes()
{
	ImGui::Begin("Shapes & Custom Models");

	ImGui::Checkbox("Box", &toggleBox);
	ImGui::Checkbox("Pyramid", &togglePyramid);
	ImGui::Checkbox("Grid", &toggleGrid);

	if (ImGui::CollapsingHeader("Custom Models"))
	{
		if (ImGui::CollapsingHeader("Change Spear"))
		{
			ImGui::Checkbox("Spear", &toggleSpear);
		}

		if (ImGui::CollapsingHeader("Change Dragon"))
		{
			ImGui::Checkbox("Dragon", &toggleDragon);
		}

		if (ImGui::CollapsingHeader("Change Gun"))
		{
			ImGui::Checkbox("Gun", &toggleGun);
		}
	}

	
	

	ImGui::End();
}

/// <summary>
/// These Set Camera functions below work by getting the cameras view matrix and changing them depending on what it is being changed to (fly cam, stationary x, y, or z)
/// </summary>
/// <param name="camera"></param>
void GraphicsApp::SetCamera(SimpleCamera* camera)
{
	m_viewMatrix = camera->GetViewMatrix();
	m_projectionMatrix = camera->GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_scene->SetCamera(camera);
}
void GraphicsApp::SetFlyCamera()
{
	m_viewMatrix = m_flyCam.GetViewMatrix();
	m_projectionMatrix = m_flyCam.GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_scene->SetCamera(&m_flyCam);
	
	toggleStationaryCamX = false;
	toggleStationaryCamY = false;
	toggleStationaryCamZ = false;
}
void GraphicsApp::SetStationaryCameraX()
{
	m_viewMatrix = m_stationaryCamX.GetViewMatrix();
	m_projectionMatrix = m_stationaryCamX.GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_scene->SetCamera(&m_stationaryCamX);

	toggleFlyCam = false;
	toggleStationaryCamY = false;
	toggleStationaryCamZ = false;
}
void GraphicsApp::SetStationaryCameraY()
{
	m_viewMatrix = m_simpleCamera.GetViewMatrix();
	m_simpleCamera.SetPosition(glm::vec3(0, 10, 0));
	m_simpleCamera.SetRotation(0, -90);
	m_scene->SetCamera(&m_stationaryCamY);

	toggleFlyCam = false;
	toggleStationaryCamX = false;
	toggleStationaryCamZ = false;
}
void GraphicsApp::SetStationaryCameraZ()
{
	m_viewMatrix = m_simpleCamera.GetViewMatrix();
	m_simpleCamera.SetPosition(glm::vec3(0, 0, -10));
	m_simpleCamera.SetRotation(90, 0);
	m_scene->SetCamera(&m_stationaryCamZ);

	toggleFlyCam = false;
	toggleStationaryCamX = false;
	toggleStationaryCamY = false;
}
void GraphicsApp::SetSimpleCamera()
{
	//m_viewMatrix = m_simpleCamera.GetViewMatrix();
	//m_projectionMatrix = m_simpleCamera.GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);
	//m_scene->SetCamera(&m_flyCam);
}



