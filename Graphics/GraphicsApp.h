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
#include "Scene.h"
#include "Instance.h"

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

	void QuadDraw(glm::mat4 pvm);

	bool BunnyLoader();
	void BunnyDraw(glm::mat4 pvm);

	bool SpearLoader();
	void ObjDraw(glm::mat4 pv, glm::mat4 transform, aie::OBJMesh* objMesh);

	bool BananaLoader();
	void BananaDraw(glm::mat4 pvm);


	bool QuadTextureLoader();
	void QuadTextureDraw(glm::mat4 pvm);

	void PhongDraw(glm::mat4 pvm, glm::mat4 transform);

	void ImGUIRefresher();
	void ImGUIPlanets();
	void ImGUIShapes();



	void SetFlyCamera();
	void SetOribtalCamera();
	void SetStationaryCamera();
	void SetSimpleCamera();

	Scene*               m_scene;


	// camera transforms
	glm::mat4	         m_viewMatrix;
	glm::mat4	         m_projectionMatrix;

	aie::Texture         m_gridTexture;

	aie::ShaderProgram   m_simpleShader;
	aie::ShaderProgram   m_colorShader;
	aie::ShaderProgram   m_texturedShader;
	aie::ShaderProgram   m_phongShader;
	aie::ShaderProgram m_normalLitShader;

	Mesh                 m_quadMesh;
	glm::mat4            m_quadTransform;

	Mesh                 m_boxMesh;
	glm::mat4            m_boxTransform;

	aie::OBJMesh         m_bunnyMesh;
	glm::mat4            m_bunnyTransform;

	aie::OBJMesh         m_spearMesh;
	glm::mat4            m_spearTransform;
	
	aie::OBJMesh         m_bananaMesh;
	glm::mat4            m_bananaTransform;

	SimpleCamera         m_simpleCamera;
	StationaryCamera     m_stationaryCam;
	FlyCamera            m_flyCam;
	OribitalCamera       m_oribtalCam;

	

	Light      m_light;
	glm::vec3  m_ambientLight;

	//cameras
	bool toggleColor;
	bool toggleStationaryCam = false;
	bool toggleOribtalCam = false;
	bool toggleFlyCam = false;
	bool toggleSimpleCam = false;

	// planets
	bool sunVisible = true;
	bool mercuryVisible = true;
	bool venusVisible = true;
	bool earthVisible = true;
	bool marsVisible = true;
	bool jupitarVisible = true;
	bool saturnVisible = true;
	bool uranusVisible = true;
	bool neptuneVisible = true;

	// shapes
	bool toggleBox = false;
	bool toggleCylinder = false;
	bool togglePyramid = false;
	bool toggleSphere = false;
	bool toggleCone = false;
	bool toggleGrid = false;
	bool toggleSpear = false;
	bool toggleBanana = false;


	glm::vec3 SimpleCamPos = { 0, 0, 0 };
};