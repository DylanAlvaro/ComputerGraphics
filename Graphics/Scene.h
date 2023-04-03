#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <list>

class SimpleCamera;
class Instance;
const int MAX_LIGHTS = 4;

struct Light {
    Light()
    {
        direction = glm::vec3(0);
        color = glm::vec3(1);
    }
    Light(glm::vec3 _position, glm::vec3 _color, float _intensity)
    {
        direction = _position;
        color = _color * _intensity;
    }
    glm::vec3 direction;
    glm::vec3 color;
};

struct Transform 
{
    Transform()
    {
        position = glm::vec3(0, 0, 0);
        rotation = glm::vec3(0, 90, 0);
        scale = glm::vec3(0, 0, 0);
    }
    Transform(glm::vec3 _translate, glm::vec3 _scale, glm::vec3 _rotation)
    {
        position = _translate;
        scale = _scale;
        rotation = _rotation;
    }
    glm::vec3 scale;
    glm::vec3 position;
    glm::vec3 rotation;
};


class Scene {
public:
    Scene(SimpleCamera* camera, glm::vec2 windowSize,
        Light& light, glm::vec3 ambientLightColor);
    ~Scene();
    void AddInstance(Instance* instance);
    void RemoveInstance();
    void Draw();
    void AddPointLights(Light light) { m_pointLights.push_back(light); }

    void AddPointLights(glm::vec3 direction, glm::vec3 color, float intensity)
    {
        m_pointLights.push_back(Light(direction, color, intensity));
    }


    void ChangeLights(int index, glm::vec3 position, glm::vec3 color, float intensity);
    void ChangeObj(int index, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    SimpleCamera* GetCamera() { return m_camera; }
    glm::vec2 GetWindowSize();
    glm::vec3 GetAmbientLightColor() { return m_ambientLightColor; }
    Light GetLight() { return m_light; }
    void SetCamera(SimpleCamera* camera) { m_camera = camera; }
    std::vector<Light>& GetPointLights() { return m_pointLights; }
    int NumberOfLights() { return m_pointLights.size(); }
    glm::vec3* GetPointLightPositions() { return &m_pointLightPositions[0]; }
    glm::vec3* GetPointLightColors() { return &m_pointLightColors[0]; }

protected:

    SimpleCamera* m_camera;
    glm::vec2 m_windowSize;
    std::vector<Light> m_pointLights;
    std::vector<Transform> m_transformObj;
    Light m_sunLight;
    Light m_light;
    glm::vec3 m_ambientLightColor;
    std::list<Instance*> m_instances;
    glm::vec3 m_pointLightPositions[MAX_LIGHTS];
    glm::vec3 m_pointLightColors[MAX_LIGHTS];
};