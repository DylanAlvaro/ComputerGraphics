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
#include "RenderTarget.h"
#include "ParticleEmitter.h"

//struct Transform
//{
//	Transform()
//	{
//		position = glm::vec3(0.0f);
//		rotation = glm::vec3(0.0f);
//		scale = glm::vec3(1.0f);
//	}
//	Transform(glm::vec3 _translate, glm::vec3 _scale, glm::vec3 _rotation)
//	{
//		position = _translate;
//		scale = _scale;
//		rotation = _rotation;
//	}
//	glm::vec3 scale;
//	glm::vec3 position;
//	glm::vec3 rotation;
//};

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

	bool DragonLoader();
	void DragonDraw(glm::mat4 pvm);

	bool GunLoader();
	void GunDraw(glm::mat4 pvm);
	
	bool TriangleLoader();
	void TriangleDraw(glm::mat4 pvm);


	bool QuadTextureLoader();
	void QuadTextureDraw(glm::mat4 pvm);

	void PhongDraw(glm::mat4 pvm, glm::mat4 transform);

	void ChangeParticles(unsigned int _maxParticles, unsigned int _emitRate,
		float _lifetimeMin, float _lifeTimeMax,
		float _velocityMin, float _velocityMax,
		float _startSize, float _endSize,
		const glm::vec4& _startColor, const glm::vec4& _endColor);


	int m_postProccessEffect = -1;

	void ImGUILights();
	void ImGUIPlanets();
	void ImGUIShapes();
	//void ImGUIModels();
	void ImGUICamera();


	void SetCamera(SimpleCamera* camera);
	void SetFlyCamera();
	void SetOribtalCamera();
	void SetStationaryCameraX();
	void SetStationaryCameraY();
	void SetStationaryCameraZ();
	void SetSimpleCamera();

	Scene*               m_scene;
	Scene*               m_shapeScene;

	Transform m_transform;

	// camera transforms
	glm::mat4	         m_viewMatrix;
	glm::mat4	         m_projectionMatrix;

	aie::Texture         m_gridTexture;

	aie::ShaderProgram   m_simpleShader;
	aie::ShaderProgram   m_colorShader;
	aie::ShaderProgram   m_texturedShader;
	aie::ShaderProgram   m_phongShader;
	aie::ShaderProgram   m_normalLitShader;
	aie::ShaderProgram   m_postProcessShader;
	aie::ShaderProgram   m_particleShader;

	aie::RenderTarget   m_renderTarget;

	Mesh                 m_quadMesh;
	glm::mat4            m_quadTransform;

	Mesh                 m_boxMesh;
	glm::mat4            m_boxTransform;

	aie::OBJMesh         m_bunnyMesh;
	glm::mat4            m_bunnyTransform;

	aie::OBJMesh         m_spearMesh;
	glm::mat4            m_spearTransform;
	
	aie::OBJMesh         m_dragonMesh;
	glm::mat4            m_dragonTransform;

	aie::OBJMesh         m_gunMesh;
	glm::mat4            m_gunTransform;
	
	Mesh                 m_pyramidMesh;
	glm::mat4            m_pyramidTransform;

	Mesh                 m_fullScreenQuad;

	SimpleCamera         m_simpleCamera;
	StationaryCamera     m_stationaryCam;
	FlyCamera            m_flyCam;
	OribitalCamera       m_oribtalCam;

	
	Light m_light;
	glm::vec3  m_ambientLight;

	ParticleEmitter* m_emitter;
	glm::mat4 m_particleEmitTransform;

	//cameras
	bool toggleColor;
	bool toggleStationaryCamX = false;
	bool toggleStationaryCamY = false;
	bool toggleStationaryCamZ = false;
	bool toggleOribtalCam = false;
	bool toggleFlyCam = false;
	bool toggleSimpleCam = false;

	// planets
	bool sunVisible = false;
	bool mercuryVisible = false;
	bool venusVisible = false;
	bool earthVisible = false;
	bool marsVisible = false;
	bool jupitarVisible = false;
	bool saturnVisible = false;
	bool uranusVisible = false;
	bool neptuneVisible = false;

	// shapes
	bool toggleBox = false;
	bool toggleCylinder = false;
	bool togglePyramid = false;
	bool toggleSphere = false;
	bool toggleCone = false;
	bool toggleGrid = false;
	bool toggleSpear = false;
	bool toggleDragon = false;
	bool toggleGun = false;
	bool toggleParticles = false;
	bool togglePointLightGizmos = false;

	bool toggleGizmos;

	Light light;

	glm::vec3 lightColor = { 1, 1, 1 };
	glm::vec3 lightDirection = { 1,-1,1 };

	glm::vec3 pointLightPos1 = { 5, 3, 0 };
	glm::vec3 pointLightPos2 = { -5, 3, 0 };
	glm::vec3 pointLightCol1 = { 1, 0, 0 };
	glm::vec3 pointLightCol2 = { 0, 0, 1 };
	float pointLight1Intensity = 50;
	float pointLight2Intensity = 50;

	glm::vec3 SimpleCamPos = { 0, 0, 0 };

	glm::vec3 m_spearScale = { 1,1,1 };
	glm::vec3 m_spearPosition = {0,0,0};
	glm::vec3 m_spearRotation = {0,90,0};

	glm::vec3 m_gunScale = { 5,5,5 };
	glm::vec3 m_gunPosition = { 0,0,0 };
	glm::vec3 m_gunRotation = { 0,90,0 };

	glm::vec3 m_dragonScale = { .25f,.25f,.25f };
	glm::vec3 m_dragonPosition = { 0,0,0 };
	glm::vec3 m_dragonRotation = { 0,90,0 };

	Particle* m_particles;
	unsigned int m_firstDead;
	unsigned int m_maxParticles;

	unsigned int m_vao, m_vbo, m_ibo;
	ParticleVertex* m_vertexData;

	glm::vec3 m_position;

	float m_emitTimer = 0;
	float m_emitRate = 1.0;

	float m_lifespanMin = 1;
	float m_lifespanMax = 5;

	float m_velocityMin;
	float m_velocityMax;

	float m_startSize = 5;
	float m_endSize = 0;

	glm::vec3 m_startColor = { 0, 0, 0};
	glm::vec3 m_endColor = { 0, 0, 0};

	glm::vec3 m_gravity = { 0,-1.f,0 };
	bool m_hasGravity = false;
	
};