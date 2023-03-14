#include "GraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsApp::GraphicsApp() {

}

GraphicsApp::~GraphicsApp() {

}

bool GraphicsApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	return true;
}

void GraphicsApp::shutdown() {

	Gizmos::destroy();
}

void GraphicsApp::update(float deltaTime) {

	float time = getTime();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
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
	Gizmos::addTransform(mat4(1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	mat4 t = glm::rotate(mat4(1), time, glm::normalize(vec3(0, 1, 0)));
	t[3] = vec4(0, 0, 0, 1);


	mat4 mercury = mat4(1);
	mercury = translate(mercury,vec3(glm::sin(time * 8.23f) * 1.6f, -1, glm::cos(time * 8.23f) * 1.6f));

	mat4 Venus = mat4(1);
	Venus = translate(Venus, vec3(vec3(glm::sin(time * 1.622f) * 2.5f, -1, glm::cos(time * 1.622f) * 2.5f)));

	mat4 Earth = mat4(1);
	Earth = translate(Earth, vec3(glm::sin(time * 1) * 3.75f, -1, glm::cos(time * 1) * 3.75f));

	mat4 EarthMoon = mat4(1);
	EarthMoon = translate(EarthMoon, vec3(glm::sin(time * .95f) * 3.75f, -1, glm::cos(time * .95f) * 3.75f));

	mat4 Mars = mat4(1);
	Mars = translate(Mars, vec3(glm::sin(time * .531f) * 5.5f, -1, glm::cos(time * .531f) * 5.5f));

	mat4 Jupitar = mat4(1);
	Jupitar = translate(Jupitar, vec3(glm::sin(time * .094f) * 6.5f, -1, glm::cos(time * .094f) * 6.5f));

	mat4 Saturn = mat4(1);
	Saturn = translate(Saturn, vec3(glm::sin(time * .05f) * 7.5f, -1, glm::cos(time * .05f) * 7.5f));

	mat4 SaturnRing = mat4(1);
	SaturnRing = translate(SaturnRing, vec3(glm::sin(time * .05f) * 7.5f, -1, glm::cos(time * .05f) * 7.5f));

	mat4 Uranus = mat4(1);
	Uranus = translate(Uranus, vec3(glm::sin(time * .01189f) * 8.5f, -1, glm::cos(time * .01189f) * 8.5f));

	mat4 UranusRing = mat4(1);
	UranusRing = translate(UranusRing, vec3(glm::sin(time * .05f) * 7.5f, -1, glm::cos(time * .05f) * 7.5f));

	mat4 Neptune = mat4(1);
	Neptune = translate(Neptune, vec3(glm::sin(time * .00606f) * 9.75f, -1, glm::cos(time * .00606f) * 9.75f));

	Gizmos::addSphere(vec3(0, 0, 0), 1, 8, 8, vec4(1, 1, 0, 0.5f)); // sun
	Gizmos::addSphere(vec3(0), .25f, 8, 8, vec4(0, 1, 1, 0.5f), &mercury); // mercury 
	Gizmos::addSphere(vec3(0), .35f, 8, 8, vec4(1, 1, 1, 0.5f), &Venus); // venus
	Gizmos::addSphere(vec3(0), .65f, 8, 8, vec4(0, 0, 1, 0.5f), &Earth); // Earth
	
	Gizmos::addSphere(vec3(0), .25f, 16, 16, vec4(0, 0, 1, 0.5f), &EarthMoon);
	
	Gizmos::addSphere(vec3(0), .55f, 8, 8, vec4(0, 1, 1, 0.5f), &Mars); // Mars
	Gizmos::addSphere(vec3(0), .45f, 8, 8, vec4(0, 1, 1, 0.5f), &Jupitar); // Jupitar
	Gizmos::addSphere(vec3(0), .65f, 8, 8, vec4(1, 1, 0, 0.5f), &Saturn); // Saturn
	Gizmos::addRing(vec3(0), .85f, 1.5f, 8, vec4(1, 1, 0, 0.5f), &SaturnRing); // saturn ring
	
	Gizmos::addSphere(vec3(0), .25f, 8, 8, vec4(0, 1, 0, 0.5f), &Uranus); // Uranus
	Gizmos::addRing(vec3(0), .45f, 1.5f, 8, vec4(0, 1, 0, 0.5f), &UranusRing); // Uranus ring

	Gizmos::addSphere(vec3(0), .25f, 8, 8, vec4(1, 1, 0, 0.5f), &Neptune); // Neptune
	

	
}

void GraphicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}