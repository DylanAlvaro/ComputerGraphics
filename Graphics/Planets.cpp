#include "Planets.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "imgui.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Planets::Planets()
{
}

Planets::~Planets()
{
}

void Planets::Update(float deltaTime)
{
	float time = getTime();

	Gizmos::addSphere(vec3(0, 0, 0), 1, 8, 8, vec4(1, 1, 0, 0.5f)); // sun

	mat4 mercury = mat4(1);
	mercury = translate(mercury, vec3(glm::sin(time * 8.23f) * 1.6f, -1, glm::cos(time * 8.23f) * 1.6f));
	Gizmos::addSphere(vec3(0), .25f, 8, 8, vec4(0.35, 0.22, 0.08, 1), &mercury); // mercury 

	mat4 Venus = mat4(1);
	Venus = translate(Venus, vec3(vec3(glm::sin(time * 1.622f) * 2.5f, -1, glm::cos(time * 1.622f) * 2.5f)));
	Gizmos::addSphere(vec3(0), .35f, 8, 8, vec4(1, 1, 1, 0.5f), &Venus); // venus

	mat4 Earth = mat4(1);
	Earth = translate(Earth, vec3(glm::sin(time * 1) * 3.75f, -1, glm::cos(time * 1) * 3.75f));
	Gizmos::addSphere(vec3(0), .65f, 8, 8, vec4(0.18, 0.46, 0.13, 1), &Earth); // Earth

	mat4 EarthMoon = mat4(1);
	EarthMoon = translate(EarthMoon, vec3(glm::sin(time * -3.f) * 1, -1, glm::cos(time * -3.f) * 1));
	Gizmos::addSphere(vec3(glm::sin(time) * 4.f, 0, glm::cos(time) * 4.f), .25f, 16, 16, vec4(0, 0, 1, 0.5f), &EarthMoon);

	mat4 Mars = mat4(1);
	Mars = translate(Mars, vec3(glm::sin(time * .531f) * 5.5f, -1, glm::cos(time * .531f) * 5.5f));
	Gizmos::addSphere(vec3(0), .55f, 8, 8, vec4(0.51, 0.13, 0.07, 1), &Mars); // Mars

	mat4 Jupitar = mat4(1);
	Jupitar = translate(Jupitar, vec3(glm::sin(time * .094f) * 6.3f, -1, glm::cos(time * .094f) * 6.3f));
	Gizmos::addSphere(vec3(0), .45f, 8, 8, vec4(0.91, 0.58, 0.23, 1), &Jupitar); // Jupitar

	mat4 Saturn = mat4(1);
	Saturn = translate(Saturn, vec3(glm::sin(time * .05f) * 7.5f, -1, glm::cos(time * .05f) * 7.5f));
	Gizmos::addSphere(vec3(0), .65f, 8, 8, vec4(1, 1, 0, 0.5f), &Saturn); // Saturn

	mat4 SaturnRing = mat4(1);
	SaturnRing = translate(SaturnRing, vec3(glm::sin(time * .05f) * 7.5f, -1, glm::cos(time * .05f) * 7.5f));
	Gizmos::addRing(vec3(0), .75f, 1.f, 8, vec4(1, 1, 0, 0.5f), &SaturnRing); // saturn ring

	mat4 Uranus = mat4(1);
	Uranus = translate(Uranus, vec3(glm::sin(time * .01189f) * 8.5f, -1, glm::cos(time * .01189f) * 8.5f));
	Gizmos::addSphere(vec3(0), .25f, 8, 8, vec4(0.23, 0.35, 0.91, 1), &Uranus); // Uranus

	mat4 UranusRing = mat4(1);
	UranusRing = translate(UranusRing, vec3(glm::sin(time * .01189f) * 8.5f, -1, glm::cos(time * .01189f) * 8.5f));
	//Gizmos::addRing(vec3(0), .25f, 1.2f, 8, vec4(0, 1, 0, 0.5f), &UranusRing);
	Gizmos::addRing(vec3(0), .35f, .55f, 8, vec4(0.23, 0.35, 0.91, 1), &UranusRing); // Uranus ring

	mat4 Neptune = mat4(1);
	Neptune = translate(Neptune, vec3(glm::sin(time * .00606f) * 9.75f, -1, glm::cos(time * .00606f) * 9.75f));
	Gizmos::addSphere(vec3(0), .25f, 8, 8, vec4(1, 1, 0, 0.5f), &Neptune); // Neptune
}

void Planets::Draw()
{

}